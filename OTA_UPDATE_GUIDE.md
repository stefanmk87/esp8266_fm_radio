# OTA (Over-The-Air) Firmware Update Guide

## 🚀 NO MORE CABLES! Update ESP-01 Wirelessly!

Your FM Radio now supports **OTA updates** - upload new firmware through the web interface!

---

## 📋 How to Use OTA Updates

### **Method 1: Web Browser (Easiest)**

1. **Open your web browser**
2. **Go to:** `http://192.168.1.31/update`
3. **You'll see the OTA Update page** with:
   - Current firmware info
   - File upload button
   - Update progress bar

4. **Click "Choose File"** and select:
   ```
   C:\Users\Stefan\Documents\PlatformIO\Projects\esp8266_fm_radio\.pio\build\esp01_1m\firmware.bin
   ```

5. **Click "Update"** button

6. **Wait** - progress bar shows upload (takes ~30 seconds)

7. **ESP-01 automatically reboots** with new firmware!

8. **Done!** Radio comes back online in 10-15 seconds

---

### **Method 2: From Main Radio Page**

1. **Open radio interface:** `http://192.168.1.31`
2. **Scroll to bottom**
3. **Click "🔄 Firmware Update (OTA)"** link
4. **Follow steps from Method 1**

---

## 🛠️ Building New Firmware

### **1. Make Code Changes**

Edit `main.cpp` with your changes

### **2. Compile (No Upload!)**

```powershell
cd C:\Users\Stefan\Documents\PlatformIO\Projects\esp8266_fm_radio
pio run
```

This creates: `.pio\build\esp01_1m\firmware.bin`

### **3. Upload via OTA**

- Go to `http://192.168.1.31/update`
- Select `firmware.bin` from `.pio\build\esp01_1m\` folder
- Click Update
- Wait for reboot

**No cables needed!** 🎉

---

## ⚡ OTA Update Process

```
┌──────────────────────────────────────────────────┐
│  1. You: Make code changes in main.cpp           │
│  2. You: Run "pio run" to compile                │
│  3. PlatformIO: Creates firmware.bin file        │
│  4. You: Open http://192.168.1.31/update         │
│  5. You: Select firmware.bin and click Update    │
│  6. ESP-01: Downloads new firmware (30 sec)      │
│  7. ESP-01: Verifies checksum                    │
│  8. ESP-01: Writes to flash memory               │
│  9. ESP-01: Reboots automatically                │
│ 10. Radio: Back online with new firmware! ✅     │
└──────────────────────────────────────────────────┘
```

---

## 🎯 When to Use OTA vs Cable

### **Use OTA When:**
✅ Making software changes only
✅ Radio is already working
✅ ESP-01 is installed in enclosure
✅ You don't want to disconnect wires
✅ Testing new features
✅ Fixing bugs

### **Use FT232R Cable When:**
❌ First-time programming (no WiFi configured yet)
❌ WiFi password changed (can't connect)
❌ OTA upload failed and radio won't boot
❌ Need to recover from bad firmware
❌ Need to access serial debug during boot

---

## 🔒 Security Notes

**No Password Protection:**
- OTA update page has no password (for simplicity)
- Anyone on your WiFi network can update firmware
- Okay for home network
- **NOT recommended for public WiFi!**

**To Add Password (Advanced):**
```cpp
// In setup(), before httpUpdater.setup():
httpUpdater.setup(&server, "/update", "admin", "yourpassword");
```

---

## 🐛 Troubleshooting

### **Problem: "Update Failed!"**

**Causes:**
1. Wrong file selected (not firmware.bin)
2. File too large for ESP-01 flash (1MB limit)
3. Power supply voltage dip during write
4. WiFi disconnected during upload

**Solution:**
- Use cable upload to recover
- Check power supply is stable 3.3V
- Ensure WiFi signal is strong
- Use correct firmware.bin file

---

### **Problem: Radio Won't Boot After OTA**

**Causes:**
1. Bad firmware uploaded
2. Flash corruption
3. Power issue during write

**Solution:**
```powershell
# Recover with cable upload:
pio run --target upload
```

---

### **Problem: Can't Find /update Page**

**Causes:**
1. Old firmware without OTA support
2. Wrong IP address
3. ESP-01 not connected to WiFi

**Solution:**
- Check serial monitor for IP address
- Upload firmware with cable first (this version has OTA)
- Verify WiFi connection

---

## 📊 Firmware Size Monitoring

**ESP-01 has 1MB flash:**
```
Current firmware: 334,263 bytes (43.9% of 761,840 bytes)
Available space: 427,577 bytes (56.1%)
```

**Always check after compile:**
```
RAM:   [=====     ]  48.5% (used 39732 bytes from 81920 bytes)
Flash: [====      ]  43.9% (used 334263 bytes from 761840 bytes)
```

If Flash > 95%, you'll have problems!

---

## 🎓 Pro Tips

### **Tip 1: Keep Backup Firmware**
```powershell
# After successful build, backup the .bin file:
copy .pio\build\esp01_1m\firmware.bin firmware_backup_v1.bin
```

### **Tip 2: Test Before OTA**
```powershell
# Always test new firmware with cable first:
pio run --target upload

# If it works, THEN use OTA for next updates
```

### **Tip 3: Version Numbers**
Add version to your code:
```cpp
const char* FW_VERSION = "v1.0.3";
Serial.println("Firmware: " + String(FW_VERSION));
```

### **Tip 4: Serial Monitor During OTA**
Keep serial monitor open during OTA to see:
- Upload progress
- Verification status
- Reboot messages
- Any errors

---

## 🚀 Quick Reference

**OTA Update URL:**
```
http://192.168.1.31/update
```

**Firmware File Location:**
```
C:\Users\Stefan\Documents\PlatformIO\Projects\esp8266_fm_radio\.pio\build\esp01_1m\firmware.bin
```

**Compile Command:**
```powershell
pio run
```

**Cable Recovery:**
```powershell
pio run --target upload
```

---

## ✅ First OTA Update Checklist

This is your FIRST OTA update. Follow these steps:

1. **Upload this firmware via cable** (has OTA support)
   ```
   pio run --target upload
   ```

2. **Verify radio works** at http://192.168.1.31

3. **Test OTA page loads** at http://192.168.1.31/update

4. **Make a small test change** (e.g., change a button label)

5. **Compile:** `pio run`

6. **Upload via OTA** to test it works

7. **If successful:** From now on, use OTA! 🎉

8. **If failed:** Use cable as backup

---

**Congratulations!** You now have wireless firmware updates! No more disconnecting cables every time you want to test code changes! 🚀📻

**Next code change?** Just compile and OTA upload - takes 30 seconds total! ⚡
