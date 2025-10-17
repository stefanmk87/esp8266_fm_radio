# Si4703 FM Antenna Guide - 3.5mm Jack Connection

## 🎯 THE PROBLEM

Your Si4703 has a **3.5mm headphone jack** that serves **DUAL PURPOSE**:
1. **Audio Output** (LOUT/ROUT signals)
2. **FM Antenna Input** (headphone cable acts as antenna!)

**Why audio is breaking:**
- ❌ **Weak FM signal** from poor antenna
- ❌ **Incorrect de-emphasis** (75µs USA vs 50µs Europe)
- ❌ **Soft-mute not configured** (noise breakthrough)
- ❌ **Headphone cable too short** or coiled up

---

## 🔌 3.5mm Jack Pinout (Si4703 Breakout)

```
3.5mm Stereo Jack (Looking at plug insertion point):

     ┌─────────────────────┐
     │  ┌─┐  ┌─┐  ┌──────┐ │
     │  │T│  │R│  │ S    │ │
     │  └─┘  └─┘  └──────┘ │
     └─────────────────────┘
        │    │       │
        │    │       └──── SLEEVE = GND + FM Antenna GND
        │    └──────────── RING = ROUT (Right Audio)
        └───────────────── TIP = LOUT (Left Audio)

CRITICAL: Antenna is the SLEEVE (longest part)!
```

---

## 📡 WHERE TO SOLDER EXTERNAL ANTENNA

### **Option 1: Replace Headphone Jack with External Antenna**

If you don't need the jack for audio output:

```
1. UNSOLDER the 3.5mm jack from Si4703 board
   (or cut the traces on PCB)

2. Solder 75cm wire directly to LOUT or ROUT pad
   (FM signal couples through audio output circuit)

3. Extend wire as straight dipole antenna
```

**Best for:** Fixed installation, no headphone playback needed

---

### **Option 2: Keep Jack + Add External Antenna**

Solder wire to **SLEEVE** connection:

```
SparkFun Si4703 Board (Red PCB):
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Look at bottom of board near 3.5mm jack:
  - Find trace going to jack SLEEVE pin
  - Solder 75cm wire to this trace or pin
  - Keep headphone jack functional!

Soldering Point:
  ┌────────────────────┐
  │   Si4703 PCB       │
  │                    │
  │  [3.5mm Jack]      │
  │      │  │  │       │
  │      │  │  └─ SLEEVE (GND + Antenna)
  │      │  └──── RING
  │      └─────── TIP
  │         ↑
  │    Solder wire HERE (to SLEEVE trace)
  │                    │
  └────────────────────┘
```

**Best for:** Want both headphone output AND better antenna

---

### **Option 3: Use Y-Splitter Cable (NO SOLDERING!)**

```
Si4703 Jack
    │
    ▼
┌───────────────┐
│  3.5mm Male   │
│  Y-Splitter   │
└───┬───────┬───┘
    │       │
    │       └──► Headphones (for audio)
    │
    └──────────► Wire antenna (75cm plugged into second jack)
```

Buy a **3.5mm male to dual 3.5mm female Y-splitter**:
1. Plug into Si4703 jack
2. One output → headphones
3. Other output → 75cm wire soldered to 3.5mm plug

**Best for:** No soldering, easy to test

---

## 📏 ANTENNA LENGTH CALCULATION

### **FM Broadcast Band:**
```
Frequency Range: 87.5 - 108 MHz
Wavelength at 95 MHz: λ = c/f = 300/95 = 3.16 meters

Quarter-wave (λ/4): 3.16m / 4 = 79cm ≈ 75-80cm
Half-wave (λ/2):    3.16m / 2 = 158cm ≈ 160cm
```

### **Recommended Antenna Lengths:**

| Type | Length | Performance | Notes |
|------|--------|-------------|-------|
| **Quarter-wave** | **75cm** | ⭐⭐⭐⭐⭐ Best! | Vertical orientation |
| Half-wave | 160cm | ⭐⭐⭐⭐ Good | Needs more space |
| Random wire | 50-100cm | ⭐⭐⭐ OK | Better than nothing |
| Headphone cable | 120cm | ⭐⭐⭐ OK | Depends on routing |

---

## 🛠️ DIY EXTERNAL ANTENNA OPTIONS

### **Option A: Simple Wire Dipole (Best Performance)**

```
Materials:
  - 75cm solid copper wire (1mm diameter)
  - 3.5mm male plug (optional)
  - Heat shrink tubing

Construction:
  1. Cut wire to exactly 75cm
  2. Strip 5mm insulation from one end
  3. Solder to Si4703 SLEEVE (or 3.5mm plug sleeve)
  4. Mount vertically for best reception
  5. Keep away from metal objects (>10cm clearance)

Orientation:
     │
     │ 75cm wire
     │ (vertical)
     │
     ▼
  [Si4703]
```

---

### **Option B: Telescopic Antenna (Adjustable)**

```
Buy: AM/FM telescopic antenna (car antenna style)
     - Usually 60-100cm extended
     - 3.5mm plug or bare wire

Connection:
  3.5mm plug → Si4703 jack SLEEVE
  OR
  Solder wire → Si4703 SLEEVE trace

Advantage:
  - Collapsible for storage
  - Adjustable length
  - Professional look
```

---

### **Option C: F-Connector + Coax Cable**

```
For permanent installation:

Materials:
  - RG6 coaxial cable (any length)
  - F-connector or BNC connector
  - 75cm wire antenna element
  - Mounting bracket

Wiring:
  Antenna ──► Coax center ──► Si4703 SLEEVE
              Coax shield ──► GND

Benefits:
  - Run antenna to roof/window
  - Shielded cable = less noise
  - Professional FM reception
```

---

## 🎧 HEADPHONE CABLE AS ANTENNA

If using headphones plugged into Si4703 jack:

### **Why It Works:**
The headphone cable acts as a random-wire antenna!

### **How to Optimize:**

```
✅ DO:
  - Fully extend cable (don't coil!)
  - Route cable vertically if possible
  - Keep away from metal objects
  - Use longer headphone cable (>1 meter)
  - Try different orientations

❌ DON'T:
  - Coil excess cable (kills reception!)
  - Run near power cables
  - Put inside metal enclosure
  - Use short cables (<50cm)
```

---

## 🧪 TESTING DIFFERENT ANTENNAS

### **Test 1: Baseline (Headphone Cable)**
```
1. Plug headphones into Si4703 jack
2. Extend cable fully
3. Tune to strong FM station (RSSI > 40)
4. Note audio quality
```

### **Test 2: Added Wire Element**
```
1. Solder 75cm wire to SLEEVE
2. Mount vertically
3. Compare RSSI to baseline
4. Should see +10-20 RSSI improvement
```

### **Test 3: Telescopic Antenna**
```
1. Connect telescopic antenna
2. Extend fully
3. Rotate for best signal
4. Should see +15-30 RSSI improvement
```

---

## 📊 RSSI Signal Strength Guide

```
RSSI Value | Reception Quality | Audio Quality
-----------|-------------------|---------------
0-15       | ❌ No signal      | None
16-25      | ⚠️ Very weak      | Lots of noise
26-35      | ⚠️ Weak           | Some noise ← YOU ARE HERE!
36-45      | ✅ Good           | Clear audio
46-55      | ✅ Strong         | Excellent
56-75      | ✅ Very strong    | Perfect
```

**Your current RSSI: 26-36** → Weak/borderline signal!
**Target RSSI: >40** → Need better antenna!

---

## 🔧 IMMEDIATE FIX - WHAT TO DO NOW

### **Quick Test (5 minutes):**

1. **Extend headphone cable fully** (no coils!)

2. **Move cable vertically:**
   ```
   Try this:     NOT this:
      │            ___
      │           /   \___
      │          (coiled)
      ▼           
   [Radio]      [Radio]
   ```

3. **Try different positions:**
   - Near window (better!)
   - Away from metal
   - Higher elevation

4. **Check web interface RSSI:**
   - Open http://192.168.1.31
   - Watch signal strength bar
   - Should see 40+ for clear audio

---

### **Permanent Fix (30 minutes):**

1. **Solder 75cm wire to Si4703:**
   ```
   Location: SLEEVE pin of 3.5mm jack on PCB
   Wire: 1mm solid copper, 75cm length
   Position: Vertical, away from metal
   ```

2. **Upload improved firmware:**
   ```
   pio run --target upload
   ```
   (Includes audio quality fixes!)

3. **Test and adjust:**
   - Check RSSI improved to 40+
   - Audio should be clean now
   - Save best antenna position

---

## 🎯 EXPECTED RESULTS

### **Before (Headphone Cable Only):**
```
RSSI: 26-36 (weak/borderline)
Audio: Breaking up, noise, distortion
Stations: Only strongest ones work
```

### **After (75cm Wire Antenna):**
```
RSSI: 40-55 (good/strong)
Audio: Clean, clear, no breaks
Stations: Many more receivable
RDS: Works reliably
```

---

## 🚨 TROUBLESHOOTING AUDIO PROBLEMS

### **Problem: Audio Breaking Up**

**Causes:**
1. RSSI < 35 (weak signal) → **Add external antenna!**
2. Wrong de-emphasis (75µs vs 50µs) → **Fixed in new firmware**
3. Soft-mute not enabled → **Fixed in new firmware**
4. Antenna too close to ESP-01 WiFi → **Move apart 10cm+**

**Solutions:**
- Upload latest firmware (includes audio fixes)
- Add 75cm wire antenna
- Position antenna away from ESP-01
- Try different FM stations (stronger signal)

---

### **Problem: Noise/Static**

**Causes:**
1. Power supply noise → **Add more capacitors**
2. WiFi interference → **Position antenna away**
3. Weak signal + no soft-mute → **Upload new firmware**

**Solutions:**
- Add 100µF capacitor close to Si4703 VCC
- Keep antenna >10cm from ESP-01
- Enable soft-mute (new firmware has this)

---

### **Problem: Mono Audio (Not Stereo)**

**Causes:**
1. setMono(false) not called → **Fixed in firmware**
2. Weak signal auto-switches to mono → **Improve antenna**

**Solutions:**
- Upload latest firmware
- Check web interface shows "STEREO"
- If shows "MONO", signal too weak → add antenna

---

## 📋 COMPLETE CHECKLIST

Before blaming antenna:
- [ ] Upload latest firmware (has audio fixes!)
- [ ] Check power supply is stable 3.3V
- [ ] Verify 100µF capacitor on Si4703 VCC
- [ ] ESP-01 and Si4703 at least 5cm apart
- [ ] Headphone cable fully extended (not coiled)
- [ ] Try strongest FM station in your area

After adding external antenna:
- [ ] Wire is 75cm ±5cm
- [ ] Soldered to SLEEVE or audio output
- [ ] Mounted vertically
- [ ] >10cm from ESP-01 WiFi
- [ ] >10cm from metal objects
- [ ] RSSI improved to >40 on web interface
- [ ] Audio is clean and clear

---

## 🎓 ADVANCED: ACTIVE ANTENNA

For super weak signal areas:

```
Buy: LNA (Low Noise Amplifier) module
     - Example: NooElec LaNA
     - Or: Mini-Whip active antenna

Connection:
  Antenna → LNA → Si4703 SLEEVE
            │
            └─ Powered by 3.3V

Gain: +20dB improvement!
Cost: $10-20
```

---

## 📸 VISUAL GUIDE

```
CORRECT Antenna Position:
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

        │
        │ 75cm wire
        │ (vertical)
        │
        ▼
    ┌───────┐        ┌─────────┐
    │Si4703 │◄─10cm─►│ ESP-01  │
    └───────┘        └─────────┘
        │
        ▼
    Headphones
    (for audio)


WRONG Antenna Position:
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

 ___________
(  coiled  )───┐
 ~~~~~~~~~~~   │
    ┌──────────┴─┐   ┌─────────┐
    │   Si4703   │◄─►│ ESP-01  │ ← Too close!
    └────────────┘   └─────────┘
         │
         ▼
    Inside metal box ← Shielded!
```

---

**NEXT STEP:** Upload the new firmware with audio quality fixes, then add 75cm wire antenna to SLEEVE pin! 🚀📻

Your RSSI will jump from 26-36 to 40-55, and audio will be crystal clear! ✨
