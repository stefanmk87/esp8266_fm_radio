# ESP-01 FM Radio Tuner with Si4703

Web-controlled FM radio tuner with RDS display using ESP-01 and SparkFun Si4703 breakout board.

## Features

✅ **Web Interface** - Control from any device on your network
✅ **FM Tuning** - 87.5 to 108 MHz coverage
✅ **Seek Function** - Auto-scan for stations (up/down)
✅ **RDS Decoding** - Display station name and radio text
✅ **Volume Control** - 0-15 levels via web slider
✅ **6 Station Presets** - Save favorite stations to EEPROM
✅ **Manual Tuning** - Direct frequency entry
✅ **Signal Strength** - RSSI indicator with visual bar
✅ **Stereo Detection** - Shows mono/stereo status
✅ **Settings Persistence** - Remembers frequency, volume, presets

## Hardware Requirements

- ESP-01 (ESP8266 module with 1MB flash)
- SparkFun Si4703 FM Tuner Breakout Board
- 3.3V power supply (MP2307 buck converter recommended)
- FM antenna (75Ω or simple wire, 70-80cm length)
- Audio output: Headphones or amplifier module

## Wiring Diagram

```
Si4703 Module → ESP-01
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
GND    → Pin 1 (GND)
3.3V   → Pin 8 (VCC)
SDIO   → Pin 3 (GPIO0) - I2C Data
SCLK   → Pin 2 (GPIO2) - I2C Clock
SEN    → 3.3V (tie HIGH for I2C mode)
RST    → 3.3V via 10kΩ resistor
GPIO2  → Not connected
GPIO1  → Not connected

Power:
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
3.3V Rail: ESP-01 VCC, Si4703 VCC, Si4703 SEN
GND Rail:  ESP-01 GND, Si4703 GND

Pull-up Resistors (for reliable boot):
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
GPIO0 → 10kΩ → 3.3V
GPIO2 → 10kΩ → 3.3V
CH_PD → 10kΩ → 3.3V (or direct to 3.3V)

Audio Output:
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Si4703 LOUT → Left channel
Si4703 ROUT → Right channel
Si4703 GND  → Audio ground

Connect to:
- 3.5mm headphone jack
- Or amplifier module (PAM8403, MAX98357A, etc.)
```

## Pin Configuration

```
ESP-01 Pinout:
┌─────────────┐
│  • • • •   │  ← Antenna side
│  1 2 3 4   │
│            │
│  8 7 6 5   │
│  • • • •   │
└─────────────┘

Pin 1 (GND):    Ground
Pin 2 (GPIO2):  I2C SCL → Si4703 SCLK
Pin 3 (GPIO0):  I2C SDA → Si4703 SDIO
Pin 4 (RX):     Not used (Serial programming)
Pin 5 (TX):     Not used (Serial programming)
Pin 6 (CH_PD):  Pull-up to 3.3V
Pin 7 (RESET):  Pull-up to 3.3V + Reset button
Pin 8 (VCC):    3.3V power
```

## Installation

### 1. Install PlatformIO
If not already installed, follow: https://platformio.org/install

### 2. Clone or Open Project
```bash
cd C:\Users\Stefan\Documents\PlatformIO\Projects\esp8266_fm_radio
```

### 3. Install Dependencies
```bash
pio lib install
```

### 4. Configure WiFi
Edit `src/main.cpp` and update WiFi credentials:
```cpp
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
```

### 5. Build Project
```bash
pio run
```

### 6. Upload to ESP-01
Connect FT232R programmer:
- TX → ESP-01 RX (Pin 4)
- RX → ESP-01 TX (Pin 5)
- GND → ESP-01 GND (Pin 1)

Enter programming mode:
1. Hold FLASH button (GPIO0 to GND)
2. Press RESET button
3. Release RESET, then FLASH
4. Upload:
```bash
pio run --target upload
```

### 7. Monitor Serial Output
```bash
pio device monitor
```

## Usage

### Web Interface

1. Power on ESP-01 with Si4703 connected
2. Wait for WiFi connection (watch Serial monitor)
3. Note IP address shown in Serial output
4. Open web browser and navigate to: `http://[ESP_IP_ADDRESS]`
5. Control radio from web interface:
   - **Seek buttons**: Scan for next station
   - **Volume slider**: Adjust audio level (0-15)
   - **Manual tune**: Enter frequency directly
   - **Presets**: Save/load favorite stations
   - **Mute button**: Toggle audio on/off

### Web Interface Features

- **Real-time RDS**: Updates station name and song info every 500ms
- **Signal strength**: Visual bar showing RSSI (0-75)
- **Stereo indicator**: Shows mono/stereo reception
- **Station presets**: 6 slots with save/load functionality
- **Frequency display**: Large, easy-to-read current frequency
- **Auto-refresh**: Status updates every second

## API Endpoints

For automation or custom interfaces:

```
GET  /              - Main web interface (HTML)
GET  /status        - JSON status (frequency, RDS, signal, etc.)
GET  /tune?freq=X   - Tune to frequency X MHz (87.5-108.0)
GET  /seek?dir=up   - Seek next station upward
GET  /seek?dir=down - Seek next station downward
GET  /volume?vol=X  - Set volume (0-15)
GET  /mute          - Toggle mute on/off
GET  /preset?load=X - Load preset slot X (0-5)
GET  /savepreset?slot=X - Save current freq to slot X
```

Example API usage:
```bash
# Tune to 95.3 MHz
curl http://192.168.1.100/tune?freq=95.3

# Seek up
curl http://192.168.1.100/seek?dir=up

# Set volume to 10
curl http://192.168.1.100/volume?vol=10

# Get status JSON
curl http://192.168.1.100/status
```

## EEPROM Storage

Settings are automatically saved to EEPROM and restored on boot:
- Last tuned frequency
- Volume level
- 6 station presets

## Troubleshooting

### Si4703 Not Detected
```
✗ Si4703 initialization FAILED!
```
**Solutions:**
- Check I2C connections (SDIO→GPIO0, SCLK→GPIO2)
- Verify SEN pin is tied to 3.3V (enables I2C mode)
- Check 3.3V power is stable (use capacitors!)
- Ensure RST pin has pull-up to 3.3V
- Try adding 4.7kΩ pull-ups on SDA/SCL lines

### No Audio Output
**Solutions:**
- Check audio connections (LOUT/ROUT)
- Verify volume is not set to 0 or muted
- Test with headphones first (easier than amplifier)
- Check Si4703 power (needs clean 3.3V)

### Poor FM Reception
**Solutions:**
- Use proper 75Ω FM antenna
- Or use wire antenna 70-80cm length
- Keep antenna away from metal objects
- Keep antenna clear of ESP-01 WiFi antenna area
- Try different locations (FM is directional)

### No RDS Data
**Solutions:**
- Wait 10-30 seconds (RDS takes time to decode)
- Not all stations broadcast RDS
- Weak signal = no RDS (check RSSI)
- Move antenna for better reception

### WiFi Connection Issues
**Solutions:**
- Check WiFi credentials in code
- Verify 2.4GHz network (ESP8266 doesn't support 5GHz)
- Check router settings (some block unknown devices)
- Monitor Serial output for connection status

## Power Consumption

```
Component         Current Draw
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
ESP-01 idle       ~80mA
ESP-01 WiFi TX    ~170mA peak
Si4703 normal     ~60mA
Si4703 seek       ~80mA peak
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Total typical     ~140mA @ 3.3V
Total peak        ~250mA @ 3.3V
```

**Power Supply Recommendations:**
- Use MP2307 buck converter (90% efficient)
- Add 100µF + 100nF capacitors at ESP-01 VCC
- Add 100µF + 100nF capacitors at Si4703 VCC
- Use stable 3.3V supply (voltage drops cause resets!)

## Technical Specifications

- **FM Band**: 87.5 - 108.0 MHz
- **Frequency Step**: 0.1 MHz (100 kHz)
- **Volume Range**: 0-15 (16 levels)
- **RDS Support**: Yes (PS, RT)
- **Stereo/Mono**: Auto-detect
- **Seek Sensitivity**: Adjustable (default medium)
- **RSSI Range**: 0-75
- **I2C Address**: 0x10 (Si4703 default)

## Project Structure

```
esp8266_fm_radio/
├── platformio.ini       # PlatformIO configuration
├── src/
│   └── main.cpp        # Main source code
├── include/            # Header files (if needed)
├── lib/                # Custom libraries (if needed)
└── README.md           # This file
```

## Credits

- **ESP8266 Core**: ESP8266 Community
- **Si4703 Library**: SparkFun Electronics
- **Web Interface**: Custom responsive design
- **Project Creator**: Stefan

## License

Free for personal and educational use.

## Support

For issues or questions:
1. Check wiring connections
2. Monitor Serial output for errors
3. Verify Si4703 initialization
4. Test with known-good FM stations

## Future Enhancements

Possible additions:
- [ ] Physical button control via PCF8574
- [ ] OLED display for standalone operation
- [ ] Alarm/timer functionality
- [ ] Bluetooth audio streaming
- [ ] Recording/streaming capabilities
- [ ] Custom EQ settings
- [ ] More preset slots
- [ ] Station scanning and storage

---

**Enjoy your ESP-01 FM Radio!** 📻🎵
