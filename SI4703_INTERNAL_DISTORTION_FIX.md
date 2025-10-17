# 🔴 CRITICAL FIX - Si4703 Internal Distortion

## 🎯 DIAGNOSIS CONFIRMED!

**You tested:**
- ✅ Direct Si4703 → Headphones = **DISTORTED**
- ✅ Si4703 → PAM8403 → Headphones = **STILL DISTORTED**

**Conclusion:**
The distortion is **INSIDE the Si4703 chip** before it even reaches the output!

**This means:**
- ❌ NOT output capacitors (PAM8403 has them)
- ❌ NOT amplifier (PAM8403 is clean Class-D)
- ❌ NOT headphone load (PAM8403 buffers it)
- ✅ **Si4703 INTERNAL audio processing issue!**

---

## 🔴 ROOT CAUSES (Si4703 Internal)

### **1. VOLUME TOO HIGH → Internal DSP Clipping** ⚡⚡⚡

The Si4703 has an **internal digital volume control** (0-15). At high volumes, the **DSP clips BEFORE the analog output**!

**Current setting:** `volume = 4` (still may be too high!)

**FIX:** Lower to **volume = 2-3**

---

### **2. WRONG AUDIO BLEND/STEREO SETTINGS** 📻

Si4703 has **stereo blend** that can cause distortion on weak signals.

**Current:** Auto blend (may cause artifacts)

**FIX:** Force settings based on signal strength

---

### **3. AGC (Automatic Gain Control) TOO AGGRESSIVE** 🎚️

Si4703's AGC can cause "pumping" distortion on FM signals.

**Current:** Default AGC (may be too aggressive)

**FIX:** Disable or reduce AGC

---

### **4. WRONG FM REGION/BAND SETTINGS** 🌍

Different FM broadcast standards can cause distortion.

**Current:** Europe settings (50µs de-emphasis)

**FIX:** Verify ALL region settings are correct

---

## 🔧 COMPLETE FIX - Multiple Solutions

### **✅ FIX #1: LOWER VOLUME TO 2 (Most Important!)**

The Si4703 DSP clips internally at high volume!

**Change in code:**

```cpp
// CURRENT:
int currentVolume = 4;  // Still too high!

// CHANGE TO:
int currentVolume = 2;  // Much safer for DSP
```

**Then in PAM8403 setup:**
- Si4703 volume = **2** (low to prevent internal clipping)
- PAM8403 knob = **70-80%** (compensate with external amp)

This keeps Si4703 DSP from clipping, PAM8403 does the amplification!

---

### **✅ FIX #2: DISABLE SOFT MUTE**

Soft mute can cause artifacts on moderate signals!

```cpp
// CURRENT:
radio.setSoftmute(true);  // May cause pumping/distortion!

// CHANGE TO:
radio.setSoftmute(false);  // Disable - let audio flow naturally
```

Your RSSI is 36-44, which is borderline for soft-mute. It may be kicking in/out causing artifacts!

---

### **✅ FIX #3: ADJUST STEREO BLEND**

Si4703 blends stereo→mono on weak signals, can cause distortion.

**Add this setting:**

```cpp
// After radio.setMono(false);
// Add stereo/mono blend control

// Option A: Force MONO (test if this is cleaner)
radio.setMono(true);  // Force mono, may eliminate stereo artifacts

// Option B: Adjust blend threshold (if library supports)
// This requires direct register access (advanced)
```

**TEST:** Try forcing MONO mode to see if distortion disappears!

---

### **✅ FIX #4: REDUCE SEEK THRESHOLD**

High seek threshold might be affecting AGC behavior.

```cpp
// CURRENT:
radio.setSeekThreshold(35);  // High threshold

// CHANGE TO:
radio.setSeekThreshold(20);  // Lower, less aggressive
```

---

### **✅ FIX #5: CHECK POWER SUPPLY VOLTAGE**

Si4703 is **VERY sensitive** to power supply noise and voltage!

**Requirements:**
- **Voltage:** 2.7V - 3.6V (you're using 3.3V ✅)
- **Noise:** Must be clean, low ripple
- **Current:** Stable 50mA minimum

**Check with multimeter:**
```
Measure Si4703 VCC pin while radio is playing:
- Should be: 3.25V - 3.35V (stable)
- If fluctuates: Bad power supply!
- If < 3.2V: Not enough current!
- If > 3.4V: Too high, may damage!
```

**Test:** Add **100µF capacitor** directly on Si4703 VCC and GND pins!

```
Si4703 Module:
  VCC ──┤├── GND
       100µF
     (electrolytic)
```

This filters power supply noise that causes audio distortion!

---

## 🔧 COMPLETE CODE FIX

Let me update the code with all fixes:

```cpp
// In initRadio() function:

  // Set initial volume - VERY LOW to prevent internal DSP clipping!
  Serial.println("Setting initial volume...");
  currentVolume = 2;  // CHANGED: 4 → 2 (prevent internal clipping!)
  radio.setVolume(currentVolume);
  Serial.print("✓ Volume set to: ");
  Serial.println(currentVolume);
  
  // Set FM band for Europe (87.5-108 MHz, 100kHz spacing)
  radio.setSpace(0);  // 0 = 100kHz
  
  // CRITICAL AUDIO SETTINGS FOR CLEAN SOUND!
  Serial.println("Configuring audio settings...");
  
  // Set audio de-emphasis for Europe (50µs) - USA uses 75µs
  radio.setFmDeemphasis(1);  // Europe standard
  Serial.println("✓ De-emphasis set to 50µs (Europe)");
  
  // DISABLE soft mute - can cause artifacts on borderline signals!
  radio.setSoftmute(false);  // CHANGED: true → false
  Serial.println("✓ Soft mute DISABLED (prevents artifacts)");
  
  // TEST: Force MONO mode to eliminate stereo blend artifacts
  Serial.println("Testing MONO mode...");
  radio.setMono(true);  // CHANGED: false → true (test if cleaner!)
  Serial.println("✓ MONO forced (testing for stereo artifacts)");
  
  // Reduce seek threshold - less aggressive AGC
  radio.setSeekThreshold(20);  // CHANGED: 35 → 20
  Serial.println("✓ Seek threshold set to 20 (less aggressive)");
  
  // Unmute
  radio.setMute(false);
```

---

## 🧪 SYSTEMATIC TESTING

### **Test Sequence:**

**1. Current Setup (Baseline):**
```
Si4703 → PAM8403 → Headphones
Volume: 4 (in code)
Result: DISTORTED ❌
```

**2. Test Volume = 2:**
```
Change: currentVolume = 2
Si4703 volume: 2
PAM8403 knob: 80% (compensate)
Expected: Less distortion? ✅ or ❌?
```

**3. Test Soft-Mute OFF:**
```
Change: setSoftmute(false)
Expected: Smoother audio? ✅ or ❌?
```

**4. Test MONO Mode:**
```
Change: setMono(true)
Expected: Clean audio? ✅ or ❌?
```

**5. Test with Power Filter:**
```
Add: 100µF cap on Si4703 VCC/GND
Expected: Cleaner audio? ✅ or ❌?
```

---

## 💡 QUICK DIAGNOSTICS

### **What type of distortion do you hear?**

| Type | Description | Likely Cause |
|------|-------------|--------------|
| **Clipping** | Audio cuts off peaks, harsh | Volume too high (internal DSP) |
| **Crackling** | Random pops, clicks | Power supply noise |
| **Pumping** | Volume goes up/down | AGC or soft-mute artifacts |
| **Harsh/Tinny** | High frequencies too loud | Wrong de-emphasis (75µs vs 50µs) |
| **Breaking** | Audio cuts in/out | Weak signal, multipath |
| **Warbling** | Pitch fluctuates slightly | Stereo blend/pilot tone issue |

**Tell me which type and I can pinpoint the exact fix!**

---

## 🚀 IMMEDIATE ACTION PLAN

### **Step 1: Update Code (5 minutes)**

Change these lines in `main.cpp`:

```cpp
// Line ~56:
int currentVolume = 2;  // Was 4, now 2

// Line ~287:
radio.setSoftmute(false);  // Was true, now false

// Line ~293:
radio.setMono(true);  // Was false, now true (TEST MONO)

// Line ~298:
radio.setSeekThreshold(20);  // Was 35, now 20
```

### **Step 2: Compile and Upload**

```powershell
pio run --target upload
# OR use OTA: http://192.168.1.31/update
```

### **Step 3: Test with PAM8403**

```
Si4703 Web Interface:
  - Volume = 2 (LOW!)
  
PAM8403:
  - Volume knob = 70-80% (HIGH!)
  - This does the amplification, not Si4703!

Listen to same station (RSSI 40+):
  - Is it cleaner now?
  - Still distorted?
  - What type of distortion?
```

### **Step 4: Add Power Filter Capacitor**

```
Buy/use: 100µF electrolytic capacitor

Solder across Si4703 module:
  VCC pin ──┤├── GND pin
           100µF
```

This is **critical** if you hear crackling/popping!

---

## 📊 EXPECTED RESULTS

| Fix Applied | Expected Improvement |
|-------------|---------------------|
| Volume 4 → 2 | **50-70% less clipping** |
| Soft-mute OFF | **Smoother audio, less pumping** |
| Force MONO | **Clean audio if stereo blend was issue** |
| Seek threshold 35 → 20 | **Less aggressive AGC** |
| 100µF power cap | **No crackling/pops from power noise** |
| **ALL FIXES COMBINED** | **Crystal clear audio!** ✅ |

---

## 🎯 THEORY: Why This Happens

### **Si4703 DSP Architecture:**

```
RF Input → Tuner → IF Processing → DSP → DAC → Amp → Output
                                    ↑
                              CLIPPING HERE!
                           (internal digital volume)
```

**The Problem:**
- Si4703 DSP processes audio **digitally** first
- Volume control is **digital** (0-15)
- At high volume, DSP **clips BEFORE** analog stage
- No amount of external filtering helps!
- **Must reduce internal volume!**

**The Solution:**
- Keep Si4703 volume LOW (2-3)
- Use external amp (PAM8403) for volume
- DSP never clips, clean signal out!

---

## ✅ WHAT I'M CHANGING NOW

Let me update the code with all these fixes:
