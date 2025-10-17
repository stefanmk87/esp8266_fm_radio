# 🎉 ESP-01 FM Radio Project - Quick Start

**Your new FM Radio project is ready, brother!** 📻

## ✅ What's Been Created

```
C:\Users\Stefan\Documents\PlatformIO\Projects\
├── esp8266_weather_oled\          ← Your weather station (existing)
│   ├── platformio.ini
│   └── src\main.cpp
│
├── esp8266_fm_radio\               ← NEW FM radio project!
│   ├── platformio.ini             ← PlatformIO config
│   ├── src\main.cpp               ← Complete FM radio code (ready!)
│   ├── README.md                  ← Full documentation
│   └── WIRING.md                  ← Detailed wiring guide
│
└── stefan_esp01_projects.code-workspace  ← Multi-project workspace
```

## 🚀 How to Use the Workspace

### **The workspace file lets you manage BOTH projects in one VS Code window!**

You'll see in VS Code sidebar:
```
EXPLORER
  ├─ 📁 Weather Station (ESP-01 + OLED)
  │   ├─ platformio.ini
  │   └─ src/main.cpp
  │
  └─ 📁 FM Radio (ESP-01 + Si4703)
      ├─ platformio.ini
      ├─ src/main.cpp
      ├─ README.md
      └─ WIRING.md
```

### **Switching Between Projects:**

Just click on the file you want to edit! PlatformIO will automatically know which project you're working on.

## 📋 Next Steps to Build FM Radio

### 1️⃣ **Hardware Setup** (15-30 minutes)

Wire up your Si4703 to ESP-01:
- Open `WIRING.md` for detailed diagrams
- Key connections:
  ```
  Si4703 SDIO → ESP-01 GPIO0 (Pin 3)
  Si4703 SCLK → ESP-01 GPIO2 (Pin 2)
  Si4703 SEN  → 3.3V (important!)
  Si4703 RST  → 3.3V via 10kΩ
  ```

### 2️⃣ **Install Si4703 Library** (1 minute)

Open terminal in VS Code and run:
```bash
cd C:\Users\Stefan\Documents\PlatformIO\Projects\esp8266_fm_radio
pio lib install
```

This will download the SparkFun Si4703 library automatically!

### 3️⃣ **Build the Code** (30 seconds)

```bash
pio run
```

Or use PlatformIO toolbar: Click "Build" button (✓)

### 4️⃣ **Upload to ESP-01** (1 minute)

Connect your FT232R programmer and enter FLASH mode:
1. Hold FLASH button
2. Press RESET button
3. Release both
4. Upload:
```bash
pio run --target upload
```

Or use PlatformIO toolbar: Click "Upload" button (→)

### 5️⃣ **Open Web Interface** (Immediate!)

1. Open Serial Monitor: `pio device monitor`
2. Find the IP address (e.g., 192.168.1.100)
3. Open browser: `http://192.168.1.100`
4. Control your FM radio! 🎵

## 🎯 Project Features

Your FM radio has:

✅ **Seek Up/Down** - Auto-scan for stations
✅ **Manual Tuning** - Enter frequency directly (87.5-108.0 MHz)
✅ **Volume Control** - Web slider (0-15)
✅ **6 Station Presets** - Save favorites to EEPROM
✅ **RDS Display** - Station name & song info
✅ **Signal Strength** - Visual RSSI indicator
✅ **Stereo Detection** - Shows mono/stereo
✅ **Beautiful Web UI** - Responsive design
✅ **Auto-Save Settings** - Remembers last station

## 📱 Web Interface Preview

```
┌──────────────────────────────────┐
│      📻 FM Radio Tuner           │
├──────────────────────────────────┤
│                                  │
│        95.3 MHz                  │
│        Rock FM                   │
│   ♫ Now Playing: Best Hits...   │
│                                  │
│  [◄◄ SEEK]    [SEEK ►►]        │
│                                  │
│  Volume: ━━━━━●━━━ 8/15        │
│                                  │
│  Presets:                       │
│  [1: 88.5] [2: 95.3] [3: 101.9]│
│                                  │
│  Signal: ████░░░░ RSSI: 42/75  │
│  Status: STEREO                 │
└──────────────────────────────────┘
```

## 🔧 Working on Both Projects

### **To work on Weather Station:**
```bash
cd C:\Users\Stefan\Documents\PlatformIO\Projects\esp8266_weather_oled
```
Then open `src/main.cpp` in VS Code

### **To work on FM Radio:**
```bash
cd C:\Users\Stefan\Documents\PlatformIO\Projects\esp8266_fm_radio
```
Then open `src/main.cpp` in VS Code

### **Or just use the workspace!**
The workspace file already has both projects loaded. Just click the file you want to edit in the Explorer sidebar!

## 📚 Documentation Files

| File | Purpose |
|------|---------|
| `README.md` | Complete project documentation, features, troubleshooting |
| `WIRING.md` | Detailed wiring diagrams, component connections, testing |
| `src/main.cpp` | Main source code with comments |
| `platformio.ini` | PlatformIO configuration |

## 💡 Quick Tips

1. **Same WiFi credentials** - Both projects use your MIMA network
2. **Same I2C pins** - GPIO0 (SDA) and GPIO2 (SCL) just like OLED
3. **Same power setup** - Use your MP2307 buck converter
4. **Same programming** - FT232R method with FLASH+RESET buttons
5. **Different IP** - FM radio will get its own IP on network

## 🆘 If You Need Help

**Si4703 not detected?**
→ Check `WIRING.md` - Section "Troubleshooting Guide"

**Want to modify features?**
→ Edit `src/main.cpp` - Code is heavily commented!

**Need hardware advice?**
→ Check `README.md` - Section "Hardware Requirements"

**WiFi not connecting?**
→ Same as weather station - check credentials in `src/main.cpp`

## 🎵 Audio Output Options

Your Si4703 has **built-in audio outputs** (no DAC needed!):

**Option 1: Headphones** (easiest)
```
Si4703 LOUT → 3.5mm jack TIP
Si4703 ROUT → 3.5mm jack RING
Si4703 GND  → 3.5mm jack SLEEVE
```

**Option 2: Amplifier Module** (for speakers)
```
Si4703 → PAM8403 amplifier → Speakers
```

## ⚡ Power Requirements

```
Component      Current
─────────────────────────
ESP-01         80-170mA
Si4703         60-80mA
─────────────────────────
Total          140-250mA @ 3.3V
```

Your MP2307 can handle this easily! Just add capacitors:
- 100µF at ESP-01 VCC
- 100µF at Si4703 VCC
- 100nF at both (close to pins)

## 🎉 Ready to Rock!

You now have TWO awesome ESP-01 projects:

1. **Weather Station** - Weather, clock, animations, web control
2. **FM Radio** - Tuner, RDS, presets, web control

Both managed in one workspace, both using the same hardware skills you've learned!

---

**Have fun building your FM radio, brother!** 📻🎵🚀

Any questions? Just ask in this chat - I know both projects now!
