/*
  ESP-01 FM Radio Tuner with Si4703
  Web interface for FM control and RDS display
  
  Hardware:
  - ESP-01 (ESP8266)
  - SparkFun Si4703 FM Tuner Breakout
  - Optional: PCF8574 for physical buttons
  
  Connections:
  Si4703 → ESP-01
  ----------------
  VCC    → 3.3V
  GND    → GND
  SDIO   → GPIO0 (SDA)
  SCLK   → GPIO2 (SCL)
  SEN    → 3.3V (I2C mode)
  RST    → 3.3V via 10kΩ (or GPIO3 if needed)
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>  // OTA update via web interface
#include <Wire.h>
#include <SI470X.h>
#include <EEPROM.h>

// I2C pins for ESP-01
#define SDA_PIN 0  // GPIO0
#define SCL_PIN 2  // GPIO2

// Si4703 reset pin - use GPIO3 (RX pin) for reset pulse
// Keep 10kΩ pull-up to 3.3V on Si4703 RST pin
#define RESET_PIN 3  // GPIO3 (RX pin) - can control Si4703 reset

// WiFi credentials (same as weather station)
const char* ssid = "MIMA";
const char* password = "nikola18092015";

// Create objects
SI470X radio;  // PU2CLR SI470X library
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;  // OTA update handler

// EEPROM Settings
#define EEPROM_SIZE 64
#define EEPROM_MAGIC 0xAF  // Magic byte for FM radio (0xAF = Audio/FM)
#define ADDR_MAGIC 0
#define ADDR_FREQUENCY 1      // 2 bytes for frequency (float * 10)
#define ADDR_VOLUME 3
#define ADDR_PRESET_START 4   // 6 presets * 2 bytes = 12 bytes

// Global variables
bool radioInitialized = false;
float currentFrequency = 95.0;  // MHz
int currentVolume = 2;          // 0-15, VERY LOW! Si4703 DSP clips internally above 3!
bool isMuted = false;
unsigned long startTime = 0;
unsigned long totalRequests = 0;

// Station presets (6 stations)
float presets[6] = {88.5, 95.3, 101.9, 0.0, 0.0, 0.0};

// RDS data
char rdsStationName[9] = "--------";
char rdsRadioText[65] = "No RDS data";
int rdsSignalStrength = 0;
bool rdsStereo = false;

// Update intervals
unsigned long lastRdsUpdate = 0;
const unsigned long rdsUpdateInterval = 500;  // Update RDS every 500ms

// Forward declarations
void initRadio();
void updateRDS();
void tuneToFrequency(float frequency);
void seekUp();
void seekDown();
void setVolume(int volume);
void handleRoot();
void handleStatus();
void handleTune();
void handleSeek();
void handleVolume();
void handleMute();
void handlePreset();
void handleSavePreset();
void saveSettings();
void loadSettings();
String getUptime();

void setup() {
  // CRITICAL: Stabilize ESP-01 boot
  delay(100);  // Let power stabilize before anything else
  
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n=================================");
  Serial.println("ESP-01 FM Radio Tuner Starting...");
  Serial.println("=================================");
  Serial.println("NOTE: GPIO3 (RX) used for Si4703 RESET");
  Serial.println("Serial debug will work after radio init");
  
  // Print boot reason (helps diagnose crashes)
  Serial.print("Boot reason: ");
  Serial.println(ESP.getResetReason());
  
  startTime = millis();
  
  // Initialize EEPROM
  EEPROM.begin(EEPROM_SIZE);
  Serial.println("EEPROM initialized");
  
  // Load saved settings
  loadSettings();
  
  // Initialize I2C
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.println("I2C initialized on GPIO0 (SDA) and GPIO2 (SCL)");
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Signal strength: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    Serial.println("\nWiFi connection failed!");
  }
  
  // Initialize Si4703 FM Radio
  Serial.println("\nInitializing Si4703 FM Radio...");
  initRadio();
  
  // Setup web server routes
  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  server.on("/tune", handleTune);
  server.on("/seek", handleSeek);
  server.on("/volume", handleVolume);
  server.on("/mute", handleMute);
  server.on("/preset", handlePreset);
  server.on("/savepreset", handleSavePreset);
  
  // Setup OTA firmware update at /update
  // Username: admin, Password: (blank for now - add if needed)
  httpUpdater.setup(&server, "/update");
  Serial.println("OTA update enabled at /update");
  
  server.onNotFound([]() {
    server.send(404, "text/plain", "404 - Not Found");
  });
  
  // Start the server
  server.begin();
  Serial.println("HTTP server started!");
  Serial.print("Access web interface at: http://");
  Serial.println(WiFi.localIP());
  Serial.print("OTA firmware update at: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/update");
  Serial.println("\n=================================");
  Serial.println("System ready!");
  Serial.println("=================================\n");
}

void loop() {
  // Handle web server requests
  server.handleClient();
  yield();  // Feed watchdog after web handling
  
  // Update RDS data periodically (only if radio working)
  if (radioInitialized && millis() - lastRdsUpdate > rdsUpdateInterval) {
    updateRDS();
    lastRdsUpdate = millis();
    yield();  // Feed watchdog after RDS update
  }
  
  // Check WiFi connection every 30 seconds
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck > 30000) {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi disconnected, reconnecting...");
      WiFi.begin(ssid, password);
    }
    lastCheck = millis();
    yield();  // Feed watchdog after WiFi check
  }
  
  delay(10);  // Small delay to prevent tight loop
}

void initRadio() {
  // Initialize Si4703 with PU2CLR library - NO RESET PIN (ESP-01 limitation)
  Serial.println("=== Starting Si4703 Initialization ===");
  Serial.println("NOTE: Si4703 RST must be tied to 3.3V via 10kΩ resistor!");
  
  Serial.println("Initializing I2C...");
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(100000);  // 100kHz I2C clock (safer for ESP-01)
  delay(200);
  Serial.println("✓ I2C initialized on GPIO0 (SDA) and GPIO2 (SCL)");
  
  // Scan I2C bus to verify Si4703 is present
  Serial.println("Scanning I2C bus...");
  Wire.beginTransmission(0x10);  // Si4703 I2C address
  byte i2cError = Wire.endTransmission();
  if (i2cError == 0) {
    Serial.println("✓ Si4703 found at address 0x10!");
  } else {
    Serial.println("✗ ERROR: Si4703 NOT found on I2C bus!");
    Serial.println("  Check wiring: SDIO→GPIO0, SCLK→GPIO2, SEN→3.3V");
    // Continue anyway - setup() will handle it
  }
  
  Serial.println("Calling radio.setup() with RESET on GPIO3...");
  yield();  // Feed watchdog
  
  // Use RESET pin like Arduino Uno (that works!)
  // setup(resetPin, sdaPin) - Library will pulse RESET LOW→HIGH
  // This properly initializes Si4703 internal state
  radio.setup(RESET_PIN, SDA_PIN);  // GPIO3 (RX) controls Si4703 reset pulse
  
  delay(500);  // Wait for radio to stabilize
  yield();  // Feed watchdog
  
  // Verify Si4703 is responding after setup
  Serial.println("Verifying Si4703 communication...");
  Wire.beginTransmission(0x10);
  byte error = Wire.endTransmission();
  if (error != 0) {
    Serial.println("✗ FATAL: Si4703 still not responding after setup!");
    Serial.println("✗ Radio will NOT work - hardware problem!");
    Serial.println("✗ Web interface will load but controls won't function");
    radioInitialized = false;
    return;  // Abort initialization - prevent crash
  }
  
  radioInitialized = true;
  Serial.println("✓ Si4703 communication verified!");
  Serial.println("✓ Si4703 initialized!");
  
  // Set initial volume (0-15) - EXACTLY like working Arduino example
  Serial.println("Setting initial volume...");
  yield();
  radio.setVolume(currentVolume);
  Serial.print("✓ Volume set to: ");
  Serial.println(currentVolume);
  delay(500);  // Let it settle
  
  // Enable RDS - EXACTLY like working Arduino example
  Serial.println("Enabling RDS...");
  yield();
  radio.setRDS(true);  // Capital RDS first!
  delay(100);
  radio.setRds(true);   // Lowercase rds second!
  radio.setRdsMode(1);  // Standard RDS
  Serial.println("✓ RDS enabled");
  
  // Set FM band for Europe (87.5-108 MHz, 100kHz spacing)
  radio.setSpace(0);  // 0 = 100kHz, 1 = 200kHz, 2 = 50kHz
  
  // CRITICAL AUDIO SETTINGS FOR CLEAN SOUND!
  Serial.println("Configuring audio settings...");
  
  // Set audio de-emphasis for Europe (50µs) - USA uses 75µs
  // 0 = 75µs (USA), 1 = 50µs (Europe/Australia)
  radio.setFmDeemphasis(1);  // Europe standard
  Serial.println("✓ De-emphasis set to 50µs (Europe)");
  
  // DISABLE soft mute - can cause artifacts/pumping on borderline signals (RSSI 30-40)!
  radio.setSoftmute(false);  // DISABLED - your RSSI 36-44 is borderline, soft-mute causes artifacts
  Serial.println("✓ Soft mute DISABLED (prevents pumping artifacts)");
  
  // TEST: Force MONO mode - stereo blend can cause distortion on weak signals
  Serial.println("Testing MONO mode...");
  radio.setMono(true);  // TRUE = Force MONO (eliminates stereo blend artifacts)
  Serial.println("✓ MONO forced (testing - stereo blend may cause distortion)");
  
  // Set seek threshold - LOWER for less aggressive AGC
  // Valid range: 0-127, default is usually 25
  radio.setSeekThreshold(20);  // Lower threshold = less aggressive AGC
  Serial.println("✓ Seek threshold set to 20 (reduced from 35)");;
  
  // Unmute
  radio.setMute(false);
  
  // Tune to last frequency - wrap in safety check
  Serial.println("Tuning to initial frequency...");
  yield();
  tuneToFrequency(currentFrequency);
  yield();
  delay(500);  // Wait for tuning to stabilize
  
  Serial.println("=== Si4703 Initialization Complete! ===");
}

void updateRDS() {
  // Safety check - don't try to read if radio not initialized
  if (!radioInitialized) {
    return;
  }
  
  yield();  // Feed watchdog
  
  // Get signal strength (RSSI) - always safe
  rdsSignalStrength = radio.getRssi();
  
  // Check stereo indicator - always safe
  rdsStereo = radio.isStereo();
  
  // Read RDS data ONLY if ready - prevents crashes
  if (radio.getRdsReady()) {
    yield();  // Feed watchdog before RDS read
    
    // Get station name (Program Service - PS) - SAFE version
    char *ps = radio.getRdsStationName();
    if (ps != NULL) {
      // Validate pointer before reading
      size_t len = strnlen(ps, 8);  // Limit length check
      if (len > 0 && len <= 8) {
        memcpy(rdsStationName, ps, len);
        rdsStationName[len] = '\0';
      }
    }
    
    yield();  // Feed watchdog between RDS reads
    
    // Get radio text (RT) - SAFE version
    char *rt = radio.getRdsProgramInformation();
    if (rt != NULL) {
      // Validate pointer before reading
      size_t len = strnlen(rt, 64);  // Limit length check
      if (len > 0 && len <= 64) {
        memcpy(rdsRadioText, rt, len);
        rdsRadioText[len] = '\0';
      }
    }
  }
  
  // Debug output occasionally
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug > 5000) {  // Every 5 seconds
    Serial.print("FM: ");
    Serial.print(currentFrequency, 1);
    Serial.print(" MHz | RDS: ");
    Serial.print(rdsStationName);
    Serial.print(" | RSSI: ");
    Serial.print(rdsSignalStrength);
    Serial.print(" | ");
    Serial.println(rdsStereo ? "STEREO" : "MONO");
    lastDebug = millis();
  }
}

void tuneToFrequency(float frequency) {
  if (!radioInitialized) return;
  
  // Constrain to FM band (87.5 - 108 MHz)
  if (frequency < 87.5) frequency = 87.5;
  if (frequency > 108.0) frequency = 108.0;
  
  currentFrequency = frequency;
  
  // PU2CLR library uses integer frequency (e.g., 9530 for 95.3 MHz)
  uint16_t freq = (uint16_t)(frequency * 100);
  radio.setFrequency(freq);
  
  Serial.print("Tuned to: ");
  Serial.print(frequency, 1);
  Serial.println(" MHz");
  
  // Clear RDS data when changing station
  strcpy(rdsStationName, "--------");
  strcpy(rdsRadioText, "Tuning...");
  
  // Save to EEPROM
  saveSettings();
}

void seekUp() {
  if (!radioInitialized) return;
  
  Serial.println("Seeking UP...");
  
  // Clear RDS data before seek
  strcpy(rdsStationName, "--------");
  strcpy(rdsRadioText, "Seeking...");
  
  yield();  // Feed watchdog before long operation
  radio.seek(SI470X_SEEK_WRAP, SI470X_SEEK_UP);
  
  // Wait for seek with watchdog feeding
  for (int i = 0; i < 50; i++) {
    delay(10);
    yield();  // Feed watchdog every 10ms
  }
  
  yield();  // Feed watchdog before I2C read
  
  // Get new frequency
  currentFrequency = radio.getFrequency() / 100.0;
  
  Serial.print("Found station at: ");
  Serial.print(currentFrequency, 1);
  Serial.println(" MHz");
  
  yield();  // Feed watchdog before EEPROM write
  saveSettings();
}

void seekDown() {
  if (!radioInitialized) return;
  
  Serial.println("Seeking DOWN...");
  
  // Clear RDS data before seek
  strcpy(rdsStationName, "--------");
  strcpy(rdsRadioText, "Seeking...");
  
  yield();  // Feed watchdog before long operation
  radio.seek(SI470X_SEEK_WRAP, SI470X_SEEK_DOWN);
  
  // Wait for seek with watchdog feeding
  for (int i = 0; i < 50; i++) {
    delay(10);
    yield();  // Feed watchdog every 10ms
  }
  
  yield();  // Feed watchdog before I2C read
  
  // Get new frequency
  currentFrequency = radio.getFrequency() / 100.0;
  
  Serial.print("Found station at: ");
  Serial.print(currentFrequency, 1);
  Serial.println(" MHz");
  
  yield();  // Feed watchdog before EEPROM write
  saveSettings();
}

void setVolume(int volume) {
  if (!radioInitialized) return;
  
  // Constrain volume (0-15)
  if (volume < 0) volume = 0;
  if (volume > 15) volume = 15;
  
  currentVolume = volume;
  radio.setVolume(volume);
  
  Serial.print("Volume set to: ");
  Serial.println(volume);
  
  saveSettings();
}

// ============================================================================
// WEB SERVER HANDLERS
// ============================================================================

void handleRoot() {
  totalRequests++;
  
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset='UTF-8'>
  <title>ESP-01 FM Radio</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    body {
      font-family: 'Segoe UI', Arial, sans-serif;
      background: linear-gradient(135deg, #667eea, #764ba2);
      margin: 0;
      padding: 20px;
      color: #fff;
      text-align: center;
    }
    .container {
      max-width: 600px;
      margin: 30px auto;
      background: rgba(255,255,255,0.1);
      border-radius: 20px;
      padding: 30px;
      box-shadow: 0 20px 60px rgba(0,0,0,0.3);
      backdrop-filter: blur(10px);
    }
    h1 {
      margin: 0 0 10px 0;
      font-size: 2.2em;
      text-shadow: 2px 2px 10px rgba(0,0,0,0.3);
    }
    .frequency {
      font-size: 4em;
      font-weight: bold;
      margin: 15px 0;
      text-shadow: 2px 2px 10px rgba(0,0,0,0.3);
    }
    .rds-station {
      font-size: 1.5em;
      margin: 10px 0;
      opacity: 0.9;
    }
    .rds-text {
      font-size: 1em;
      margin: 5px 0;
      opacity: 0.8;
      font-style: italic;
    }
    .controls {
      background: rgba(255,255,255,0.15);
      padding: 20px;
      border-radius: 15px;
      margin-top: 20px;
    }
    .btn-group {
      display: flex;
      gap: 10px;
      margin: 15px 0;
      justify-content: center;
    }
    .btn {
      background: rgba(255,255,255,0.2);
      border: 2px solid rgba(255,255,255,0.3);
      color: #fff;
      padding: 12px 20px;
      border-radius: 10px;
      cursor: pointer;
      font-size: 1em;
      font-weight: 600;
      transition: all 0.3s;
      flex: 1;
    }
    .btn:hover {
      background: rgba(255,255,255,0.35);
      transform: translateY(-2px);
      box-shadow: 0 5px 15px rgba(0,0,0,0.3);
    }
    .btn:active {
      transform: translateY(0);
    }
    .btn-large {
      font-size: 1.5em;
      padding: 15px;
    }
    .volume-control {
      margin: 20px 0;
    }
    .volume-slider {
      width: 100%;
      height: 8px;
      border-radius: 5px;
      background: rgba(255,255,255,0.2);
      outline: none;
      -webkit-appearance: none;
    }
    .volume-slider::-webkit-slider-thumb {
      -webkit-appearance: none;
      width: 20px;
      height: 20px;
      border-radius: 50%;
      background: #fff;
      cursor: pointer;
      box-shadow: 0 2px 5px rgba(0,0,0,0.3);
    }
    .presets {
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      gap: 10px;
      margin: 15px 0;
    }
    .preset-btn {
      padding: 10px;
      font-size: 0.9em;
    }
    .status {
      margin-top: 15px;
      padding: 10px;
      background: rgba(255,255,255,0.1);
      border-radius: 8px;
      font-size: 0.9em;
    }
    .signal-bar {
      height: 20px;
      background: rgba(255,255,255,0.2);
      border-radius: 10px;
      overflow: hidden;
      margin: 10px 0;
    }
    .signal-fill {
      height: 100%;
      background: linear-gradient(90deg, #f00, #ff0, #0f0);
      transition: width 0.3s;
    }
    .manual-tune {
      display: flex;
      gap: 10px;
      margin: 15px 0;
    }
    .manual-tune input {
      flex: 1;
      padding: 12px;
      border: 2px solid rgba(255,255,255,0.3);
      border-radius: 8px;
      background: rgba(255,255,255,0.2);
      color: #fff;
      font-size: 1em;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>📻 FM Radio Tuner</h1>
    <div class="frequency" id="frequency">95.0 MHz</div>
    <div class="rds-station" id="rdsStation">--------</div>
    <div class="rds-text" id="rdsText">No RDS data</div>
    
    <div class="controls">
      <div class="btn-group">
        <button class="btn btn-large" onclick="seek('down')">◄◄ SEEK</button>
        <button class="btn btn-large" onclick="seek('up')">SEEK ►►</button>
      </div>
      
      <div class="volume-control">
        <label>Volume: <span id="volumeValue">8</span>/15</label>
        <input type="range" class="volume-slider" id="volumeSlider" min="0" max="15" value="8" oninput="setVolume(this.value)">
      </div>
      
      <div class="btn-group">
        <button class="btn" onclick="toggleMute()">🔇 MUTE</button>
      </div>
      
      <h3>Presets</h3>
      <div class="presets" id="presets">
        <!-- Preset buttons will be populated by JavaScript -->
      </div>
      
      <h3>Manual Tune</h3>
      <div class="manual-tune">
        <input type="number" id="manualFreq" placeholder="95.0" step="0.1" min="87.5" max="108.0">
        <button class="btn" onclick="manualTune()">GO</button>
      </div>
      
      <div class="status">
        <div>Signal Strength</div>
        <div class="signal-bar">
          <div class="signal-fill" id="signalFill" style="width: 50%"></div>
        </div>
        <div id="statusText">RSSI: 32/75 | STEREO</div>
      </div>
      
      <div style="margin-top: 20px; padding: 15px; background: rgba(255,255,255,0.1); border-radius: 10px;">
        <a href="/update" style="color: #fff; text-decoration: none; font-size: 0.9em;">
          🔄 Firmware Update (OTA)
        </a>
      </div>
    </div>
  </div>
  
  <script>
    function updateStatus() {
      fetch('/status')
        .then(r => r.json())
        .then(data => {
          document.getElementById('frequency').innerText = data.frequency.toFixed(1) + ' MHz';
          document.getElementById('rdsStation').innerText = data.rdsStation || '--------';
          document.getElementById('rdsText').innerText = data.rdsText || 'No RDS data';
          document.getElementById('volumeValue').innerText = data.volume;
          document.getElementById('volumeSlider').value = data.volume;
          
          let signalPercent = (data.rssi / 75) * 100;
          document.getElementById('signalFill').style.width = signalPercent + '%';
          document.getElementById('statusText').innerText = 
            'RSSI: ' + data.rssi + '/75 | ' + (data.stereo ? 'STEREO' : 'MONO');
          
          updatePresets(data.presets);
        })
        .catch(err => console.error('Error:', err));
    }
    
    function updatePresets(presets) {
      let html = '';
      for(let i = 0; i < 6; i++) {
        let freq = presets[i];
        if(freq > 0) {
          html += `<button class="btn preset-btn" onclick="loadPreset(${i})">${i+1}: ${freq.toFixed(1)}</button>`;
        } else {
          html += `<button class="btn preset-btn" onclick="savePreset(${i})">${i+1}: Empty</button>`;
        }
      }
      document.getElementById('presets').innerHTML = html;
    }
    
    function seek(direction) {
      fetch('/seek?dir=' + direction)
        .then(() => setTimeout(updateStatus, 1000));
    }
    
    function setVolume(vol) {
      document.getElementById('volumeValue').innerText = vol;
      fetch('/volume?vol=' + vol);
    }
    
    function toggleMute() {
      fetch('/mute')
        .then(() => setTimeout(updateStatus, 100));
    }
    
    function manualTune() {
      let freq = document.getElementById('manualFreq').value;
      if(freq >= 87.5 && freq <= 108.0) {
        fetch('/tune?freq=' + freq)
          .then(() => setTimeout(updateStatus, 500));
      } else {
        alert('Frequency must be between 87.5 and 108.0 MHz');
      }
    }
    
    function loadPreset(index) {
      fetch('/preset?load=' + index)
        .then(() => setTimeout(updateStatus, 500));
    }
    
    function savePreset(index) {
      if(confirm('Save current station to preset ' + (index+1) + '?')) {
        fetch('/savepreset?slot=' + index)
          .then(() => setTimeout(updateStatus, 100));
      }
    }
    
    // Update status every 1 second
    setInterval(updateStatus, 1000);
    updateStatus();
  </script>
</body>
</html>
)rawliteral";
  
  server.send(200, "text/html", html);
}

void handleStatus() {
  totalRequests++;
  
  String json = "{";
  json += "\"frequency\":" + String(currentFrequency, 1) + ",";
  json += "\"volume\":" + String(currentVolume) + ",";
  json += "\"muted\":" + String(isMuted ? "true" : "false") + ",";
  json += "\"rdsStation\":\"" + String(rdsStationName) + "\",";
  json += "\"rdsText\":\"" + String(rdsRadioText) + "\",";
  json += "\"rssi\":" + String(rdsSignalStrength) + ",";
  json += "\"stereo\":" + String(rdsStereo ? "true" : "false") + ",";
  json += "\"presets\":[";
  for (int i = 0; i < 6; i++) {
    json += String(presets[i], 1);
    if (i < 5) json += ",";
  }
  json += "]";
  json += "}";
  
  server.send(200, "application/json", json);
}

void handleTune() {
  if (server.hasArg("freq")) {
    float frequency = server.arg("freq").toFloat();
    tuneToFrequency(frequency);
    server.send(200, "text/plain", "Tuned to " + String(frequency, 1) + " MHz");
  } else {
    server.send(400, "text/plain", "Missing freq parameter");
  }
}

void handleSeek() {
  if (server.hasArg("dir")) {
    String direction = server.arg("dir");
    if (direction == "up") {
      seekUp();
      server.send(200, "text/plain", "Seeking up");
    } else if (direction == "down") {
      seekDown();
      server.send(200, "text/plain", "Seeking down");
    } else {
      server.send(400, "text/plain", "Invalid direction. Use 'up' or 'down'");
    }
  } else {
    server.send(400, "text/plain", "Missing dir parameter");
  }
}

void handleVolume() {
  if (server.hasArg("vol")) {
    int volume = server.arg("vol").toInt();
    setVolume(volume);
    server.send(200, "text/plain", "Volume set to " + String(volume));
  } else {
    server.send(400, "text/plain", "Missing vol parameter");
  }
}

void handleMute() {
  if (!radioInitialized) {
    server.send(500, "text/plain", "Radio not initialized");
    return;
  }
  
  isMuted = !isMuted;
  
  if (isMuted) {
    radio.setVolume(0);
    Serial.println("Muted");
  } else {
    radio.setVolume(currentVolume);
    Serial.println("Unmuted");
  }
  
  server.send(200, "text/plain", isMuted ? "Muted" : "Unmuted");
}

void handlePreset() {
  if (server.hasArg("load")) {
    int slot = server.arg("load").toInt();
    if (slot >= 0 && slot < 6 && presets[slot] > 0) {
      tuneToFrequency(presets[slot]);
      server.send(200, "text/plain", "Loaded preset " + String(slot + 1));
    } else {
      server.send(400, "text/plain", "Invalid or empty preset slot");
    }
  } else {
    server.send(400, "text/plain", "Missing load parameter");
  }
}

void handleSavePreset() {
  if (server.hasArg("slot")) {
    int slot = server.arg("slot").toInt();
    if (slot >= 0 && slot < 6) {
      presets[slot] = currentFrequency;
      saveSettings();
      Serial.print("Saved preset ");
      Serial.print(slot + 1);
      Serial.print(": ");
      Serial.print(currentFrequency, 1);
      Serial.println(" MHz");
      server.send(200, "text/plain", "Saved to preset " + String(slot + 1));
    } else {
      server.send(400, "text/plain", "Invalid preset slot (0-5)");
    }
  } else {
    server.send(400, "text/plain", "Missing slot parameter");
  }
}

// ============================================================================
// EEPROM SETTINGS FUNCTIONS
// ============================================================================

void saveSettings() {
  yield();  // Feed watchdog before EEPROM writes
  
  EEPROM.write(ADDR_MAGIC, EEPROM_MAGIC);
  
  // Save frequency as integer (frequency * 10)
  int freqInt = (int)(currentFrequency * 10);
  EEPROM.write(ADDR_FREQUENCY, freqInt >> 8);      // High byte
  EEPROM.write(ADDR_FREQUENCY + 1, freqInt & 0xFF); // Low byte
  
  // Save volume
  EEPROM.write(ADDR_VOLUME, currentVolume);
  
  yield();  // Feed watchdog mid-save
  
  // Save presets
  for (int i = 0; i < 6; i++) {
    int presetInt = (int)(presets[i] * 10);
    int addr = ADDR_PRESET_START + (i * 2);
    EEPROM.write(addr, presetInt >> 8);
    EEPROM.write(addr + 1, presetInt & 0xFF);
  }
  
  yield();  // Feed watchdog before commit
  EEPROM.commit();
  
  Serial.println("Settings saved to EEPROM");
}

void loadSettings() {
  byte magic = EEPROM.read(ADDR_MAGIC);
  
  if (magic == EEPROM_MAGIC) {
    // Load frequency
    int freqInt = (EEPROM.read(ADDR_FREQUENCY) << 8) | EEPROM.read(ADDR_FREQUENCY + 1);
    currentFrequency = freqInt / 10.0;
    
    // Validate frequency
    if (currentFrequency < 87.5 || currentFrequency > 108.0) {
      currentFrequency = 95.0;
    }
    
    // Load volume
    currentVolume = EEPROM.read(ADDR_VOLUME);
    if (currentVolume < 0 || currentVolume > 15) {
      currentVolume = 8;
    }
    
    // Load presets
    for (int i = 0; i < 6; i++) {
      int addr = ADDR_PRESET_START + (i * 2);
      int presetInt = (EEPROM.read(addr) << 8) | EEPROM.read(addr + 1);
      presets[i] = presetInt / 10.0;
      
      // Validate preset
      if (presets[i] < 87.5 || presets[i] > 108.0) {
        presets[i] = 0.0;  // Mark as empty
      }
    }
    
    Serial.println("Settings loaded from EEPROM:");
    Serial.print("  Frequency: ");
    Serial.print(currentFrequency, 1);
    Serial.println(" MHz");
    Serial.print("  Volume: ");
    Serial.println(currentVolume);
    Serial.println("  Presets:");
    for (int i = 0; i < 6; i++) {
      if (presets[i] > 0) {
        Serial.print("    ");
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(presets[i], 1);
        Serial.println(" MHz");
      }
    }
  } else {
    Serial.println("No valid EEPROM data found, using defaults");
    Serial.println("Settings will be saved on first change");
  }
}

String getUptime() {
  unsigned long uptime = (millis() - startTime) / 1000;
  unsigned long seconds = uptime % 60;
  unsigned long minutes = (uptime / 60) % 60;
  unsigned long hours = (uptime / 3600) % 24;
  unsigned long days = uptime / 86400;
  
  String result = "";
  if (days > 0) result += String(days) + "d ";
  if (hours > 0) result += String(hours) + "h ";
  if (minutes > 0) result += String(minutes) + "m ";
  result += String(seconds) + "s";
  
  return result;
}
