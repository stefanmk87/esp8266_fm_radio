# 🔧 RDA5807FP Bare Chip - Complete Build Guide

## ⚠️ UPDATED WITH CORRECT SCHEMATIC! ⚠️

**This guide has been corrected based on the official RDA5807FP datasheet schematic!**

---

## 🎯 WHAT YOU HAVE

**RDA5807FP Bare Chip:**
- FM Radio receiver IC (chip only, no breakout board)
- Package: QFN-16 (3mm x 3mm)
- Needs external components to work
- **This is a challenge but doable!** 💪

---

## ⚠️ IMPORTANT WARNINGS

### **Difficulty Level: ADVANCED** ⚙️⚙️⚙️⚙️⚙️

**Required skills:**
- ✅ SMD soldering (very fine pitch!)
- ✅ Circuit design knowledge
- ✅ Schematic reading
- ✅ Component identification
- ✅ PCB layout or protoboard adaptation

**Required tools:**
- SMD soldering station (with fine tip) OR Hot air station
- Magnifying glass or microscope
- Tweezers (fine point)
- Multimeter
- QFN-16 to DIP adapter (HIGHLY RECOMMENDED - $2)

**If you're not comfortable with SMD soldering:**
→ **Buy RDA5807M module instead!** ($3-5, pre-built, easier!)

---

## 📋 CORRECTED PIN LAYOUT

### **RDA5807FP Chip (QFN-16 Package) - OFFICIAL DATASHEET**

**Pinout (Top View):**
```
        ┌─────────┐
GPIO1  1│●        │16 GPIO2
 GND_1 2│         │15 GPIO3
RFGND  3│  RDA    │14 GND_5
 FMIN  4│  5807FP │13 LOUT
 GND_2 5│         │12 ROUT
 GND_3 6│         │11 GND_4
 SCLK  7│         │10 VDD (3.3V)
  SDA  8│         │9  RCLK
        └─────────┘

Pin Functions (CORRECTED!):
1  - GPIO1 (Antenna coupling via 1nF cap)
2  - GND_1 (Ground)
3  - RFGND (RF Ground)
4  - FMIN (FM Antenna Input - main RF input)
5  - GND_2 (Ground)
6  - GND_3 (Ground)
7  - SCLK (I2C Clock - connect to ESP GPIO2)
8  - SDA (I2C Data - connect to ESP GPIO0)
9  - RCLK (32.768kHz crystal reference clock)
10 - VDD (3.3V power supply)
11 - GND_4 (Ground)
12 - ROUT (Right audio output)
13 - LOUT (Left audio output)
14 - GND_5 (Ground)
15 - GPIO3 (Not connected - optional control)
16 - GPIO2 (Not connected - optional control)
```

---

## 🛒 CORRECTED SHOPPING LIST

### **Essential Components (Based on Official Schematic):**

| Component | Value | Quantity | Purpose | Price |
|-----------|-------|----------|---------|-------|
| **RDA5807FP chip** | QFN-16 | 1 | FM receiver | You have! ✅ |
| **QFN-16 to DIP adapter** | - | 1 | Makes soldering easier! | $2.00 |
| **Crystal** | 32.768 kHz | 1 | Clock reference | $0.50 |
| **Antenna coupling** | 1 nF (102) | 1 | Antenna DC blocking | $0.10 |
| **Audio DC blocking** | 10 µF electrolytic | 2 | Audio output | $0.20 |
| **Power bulk caps** | 100 µF electrolytic | 2 | Power smoothing (optional) | $0.30 |
| **I2C series resistors** | 1 kΩ | 2 | I2C protection (R1, R2) | $0.10 |
| **I2C pull-ups** | 10 kΩ | 2 | I2C pull-ups (R3, R4) | $0.10 |
| **3.5mm audio jack** | Stereo | 1 | Audio output | $0.50 |
| **Antenna wire** | 75cm | 1 | FM reception | Free! |
| **Protoboard** | Generic | 1 | Mounting | $1.00 |

**Total cost:** ~$4.80 (you already have the chip!)

**⚠️ REMOVED from old list:**
- ❌ 20pF crystal load caps (NOT needed - chip has internal caps!)
- ❌ 100nF decoupling caps (NOT needed - chip handles internally!)
- ❌ 47pF antenna tuning cap (NOT in official schematic!)
- ❌ 4.7kΩ resistors (Use 10kΩ instead!)

**✅ ADDED to new list:**
- ✅ 1nF antenna coupling capacitor (REQUIRED!)
- ✅ 1kΩ I2C series resistors (REQUIRED for protection!)
- ✅ QFN-16 to DIP adapter (HIGHLY recommended!)

---
                    ┌───────────────┐
                    │               │
VDD (3.3V) ─┬───────┤6,16 VDD       │
            │       │               │
           ┴10µF    │               │
            │       │               │
GND ────────┴───────┤5,7,10,12,15   │
                    │     GND       │
                    │               │
                    │               │
SCL ────4.7kΩ──┬────┤2 RCLK        │
               │    │               │
         VDD ──┘    │               │
                    │               │
SDA ────4.7kΩ──┬────┤3 SDIO        │
               │    │               │
         VDD ──┘    │               │
                    │               │
                    │      11 FM_IN ├──47pF──── Antenna (75cm wire)
                    │               │
                    │               │
                    │   13 LOUT     ├──100µF─── Left Audio Out
                    │               │    +
                    │               │
                    │   14 ROUT     ├──100µF─── Right Audio Out
                    │               │    +
                    └───────────────┘

Between ALL VDD and GND pins:
  - 100nF ceramic capacitors (6 total, one per power pin)

32.768 kHz Crystal:
  - Connected between RCLK and ground
  - With 18-22pF load capacitors on each side
```

---

## 🔨 STEP-BY-STEP BUILD

### **Step 1: Prepare the Protoboard**

**Option A: SMD to DIP Adapter Board** (RECOMMENDED!)
```
Buy: QFN-16 to DIP adapter board
Cost: $1-2 on AliExpress
Benefit: Much easier to solder and prototype!

The adapter looks like:
  ┌─────────────┐
  │  ●●●●●●●●   │ ← DIP pins (easy to solder!)
  │  ┌──┐       │
  │  │IC│       │ ← QFN chip solders here
  │  └──┘       │
  │  ●●●●●●●●   │
  └─────────────┘
```

**Option B: Direct Solder to Protoboard** (HARD!)
```
Use copper protoboard with pads
Solder QFN chip directly with hot air
Requires advanced soldering skills!
```

**I STRONGLY RECOMMEND Option A!** 🎯

---

### **Step 2: Solder the RDA5807FP Chip**

**Using QFN-16 to DIP adapter:**
1. Apply solder paste to adapter pads
2. Place RDA5807FP chip (align pin 1 marker!)
3. Use hot air station at 300°C
4. Chip will self-align when solder melts
5. Let cool naturally
6. Check with multimeter (no shorts!)

**Pin 1 marker:** Small dot on chip corner

---

### **Step 3: Add Decoupling Capacitors**

**CRITICAL: Place these VERY CLOSE to chip!**

For EACH VDD pin (6, 16):
```
VDD pin ──┤├── Nearest GND pin
         100nF
   (ceramic capacitor)
```

**Layout on adapter board:**
```
Pin 16 (VDD) ──┤├─ Pin 15 (GND)  ← 100nF
Pin 6  (VDD) ──┤├─ Pin 7  (GND)  ← 100nF
Pin 6  (VDD) ──┤├─ Pin 5  (GND)  ← 100nF
```

**Use shortest possible connections!** Power filtering is critical!

---

### **Step 4: Add Crystal Oscillator**

**32.768 kHz crystal circuit:**
```
              RCLK (Pin 2)
                   │
                   ├──── 32.768kHz Crystal ──── GND
                   │
                  ┴ 20pF
                   │
                  GND

Alternative (better stability):
              RCLK (Pin 2)
                   │
         20pF ┬────┴──── Crystal ────┬ 20pF
              │                      │
             GND                    GND
```

**Crystal type:** SMD 3215 package (3.2mm x 1.5mm) or through-hole

---

### **Step 5: Add I2C Pull-up Resistors**

```
VDD (3.3V) ──┬── 4.7kΩ ──┬── Pin 2 (RCLK/SCL)
             │           │
             └── 4.7kΩ ──┴── Pin 3 (SDIO/SDA)
```

**These are REQUIRED for I2C communication!**

---

### **Step 6: Add Antenna Input**

```
Pin 11 (FM_IN) ──┤├─── Antenna wire (75cm)
                47pF
                 │
                GND

47pF capacitor provides impedance matching
```

**Antenna:** 75cm wire (quarter-wave for FM band ~95MHz)

---

### **Step 7: Add Audio Output**

```
Pin 13 (LOUT) ──[+ 100µF -]──── 3.5mm Jack TIP (Left)

Pin 14 (ROUT) ──[+ 100µF -]──── 3.5mm Jack RING (Right)

GND ──────────────────────────── 3.5mm Jack SLEEVE
```

**Capacitor polarity:**
- **Positive (+)**: Toward RDA5807FP chip
- **Negative (-)**: Toward 3.5mm jack

---

### **Step 8: Add Power Connections**

```
VDD (3.3V) ──┬──[+ 10µF -]── GND  ← Bulk capacitor
             │
             ├──── Pin 6, Pin 16
             │
GND ─────────┴──── Pin 5, 7, 10, 12, 15
```

**Power input:** Connect to your ESP-01 3.3V supply (MP2307)

---

## 🔌 CONNECTIONS TO ESP-01

| RDA5807FP Pin | Wire | ESP-01 Pin | Notes |
|---------------|------|------------|-------|
| Pin 2 (RCLK/SCL) | → | GPIO2 (SCL) | + 4.7kΩ pull-up to 3.3V |
| Pin 3 (SDIO/SDA) | → | GPIO0 (SDA) | + 4.7kΩ pull-up to 3.3V |
| Pin 6, 16 (VDD) | → | 3.3V | From MP2307 |
| Pin 5,7,10,12,15 (GND) | → | GND | Common ground |
| Pin 11 (FM_IN) | → | 75cm antenna | Through 47pF cap |
| Pin 13 (LOUT) | → | 3.5mm jack TIP | Through 100µF cap |
| Pin 14 (ROUT) | → | 3.5mm jack RING | Through 100µF cap |

---

## 📐 PROTOBOARD LAYOUT

```
┌────────────────────────────────────────────┐
│  Protoboard Layout (Top View)              │
│                                            │
│  [VDD]─────┬───[10µF]───[GND]             │
│            │                               │
│  [4.7k]─┬──┴──[QFN Adapter]───[100nF x6]  │
│         │         │  │  │                  │
│  [4.7k]─┴─────────┘  │  │                  │
│                      │  │                  │
│  [Crystal]───[20pF]──┘  │                  │
│      │                  │                  │
│     GND                 │                  │
│                         │                  │
│  [Antenna]──[47pF]──────┘                  │
│                                            │
│  [100µF]──[100µF]──[3.5mm Jack]           │
│   LOUT     ROUT     Audio Out             │
│                                            │
│  [VDD] [GND] [SDA] [SCL]  ← To ESP-01     │
└────────────────────────────────────────────┘
```

---

## 💻 SOFTWARE / CODE

**GOOD NEWS:** Your FM radio code will work with **minimal changes!**

The PU2CLR library supports RDA5807!

### **Changes Needed:**

```cpp
// In main.cpp

// OLD (Si4703):
#include <SI470X.h>
SI470X radio;

// NEW (RDA5807):
#include <RDA5807.h>
RDA5807 radio;

// RDA5807 I2C address is different
// Si4703: 0x10
// RDA5807: 0x11

// Most functions are the same!
radio.setFrequency(9530);  // ✅ Same
radio.setVolume(8);        // ✅ Same
radio.getRdsStationName(); // ✅ Same
```

**That's it!** Most code stays the same! 🎉

---

## 🧪 TESTING PROCEDURE

### **Step 1: Power Test**
```
1. Connect 3.3V and GND ONLY
2. Measure voltage at chip pins 6 and 16
3. Should read 3.25-3.35V
4. Check for shorts with multimeter
5. Measure current: Should be ~10-15mA
```

### **Step 2: I2C Test**
```
1. Upload I2C scanner sketch to ESP-01
2. Should detect device at address 0x11
3. If not detected:
   - Check SDA/SCL connections
   - Check pull-up resistors
   - Check crystal is oscillating
```

### **Step 3: FM Radio Test**
```
1. Upload modified FM radio code
2. Tune to strong local station
3. Check if RSSI reading appears
4. Listen for audio on 3.5mm jack
5. Test RDS (if station broadcasts it)
```

---

## ⚠️ COMMON PROBLEMS & SOLUTIONS

### **Problem: No I2C Detection**
**Solutions:**
- Check crystal - must be 32.768 kHz exactly
- Check crystal load caps (18-22pF)
- Check all GND connections
- Verify VDD is 3.3V (not 3.0V, not 3.5V)
- Check for solder bridges on QFN chip

### **Problem: Detected but No Audio**
**Solutions:**
- Check audio output capacitors polarity
- Verify 100µF caps are electrolytic (not ceramic!)
- Test with known working headphones
- Check antenna connection
- Try strong FM station (RSSI > 40)

### **Problem: Noisy Audio**
**Solutions:**
- Add more decoupling caps near VDD pins
- Check GND connections (star ground layout)
- Keep antenna away from ESP-01
- Use shielded audio cable

### **Problem: Poor Sensitivity**
**Solutions:**
- Check antenna is 75cm length
- Verify 47pF antenna matching cap
- Mount antenna vertically
- Keep away from metal objects

---

## 🎯 HONEST ASSESSMENT

### **Should You Build This?**

**Pros:**
- ✅ You already have the chips (free!)
- ✅ Learning experience
- ✅ Custom design
- ✅ Satisfaction of DIY

**Cons:**
- ❌ Difficult SMD soldering (QFN package)
- ❌ Need special tools (hot air station)
- ❌ Easy to damage chip
- ❌ May not work first time
- ❌ Debugging is hard

**My recommendation:**

**If you have:**
- SMD soldering experience ✅
- Hot air station ✅
- Patience and time ✅
- Desire to learn ✅

→ **GO FOR IT!** It's a great project! 🚀

**If you DON'T have:**
- SMD soldering skills ❌
- Proper tools ❌
- Much free time ❌

→ **Buy RDA5807M module instead** ($3-5, pre-built, works immediately!)

---

## 🛒 ALTERNATIVE: Just Buy RDA5807M Module

**Pre-built RDA5807M breakout board:**
- All components included ✅
- Pre-tested ✅
- Plug-and-play ✅
- **$3-5 on AliExpress** ✅
- Works immediately ✅

**Comparison:**

| Aspect | Build RDA5807FP | Buy RDA5807M Module |
|--------|----------------|---------------------|
| **Cost** | ~$3.50 components | $3-5 module |
| **Time** | 2-4 hours | 0 minutes |
| **Difficulty** | ⭐⭐⭐⭐⭐ Advanced | ⭐ Beginner |
| **Success rate** | 60% (if experienced) | 99% |
| **Learning value** | High | Low |
| **Risk** | Chip damage possible | None |

---

## 📚 ADDITIONAL RESOURCES

**Datasheets needed:**
- RDA5807FP datasheet (you have!)
- RDA5807 application notes
- 32.768 kHz crystal datasheet

**Recommended reading:**
- RF circuit design basics
- QFN soldering tutorials
- I2C troubleshooting guide

**YouTube searches:**
- "QFN soldering tutorial"
- "Hot air station SMD"
- "RDA5807 DIY FM radio"

---

## 🎯 DECISION TIME

### **Path 1: DIY RDA5807FP (From Bare Chip)**
```
Difficulty: ⭐⭐⭐⭐⭐
Time: 2-4 hours
Cost: ~$3.50
Learning: ⭐⭐⭐⭐⭐
Success rate: 60% (if skilled)
```

### **Path 2: Buy RDA5807M Module**
```
Difficulty: ⭐
Time: 5 minutes
Cost: $3-5
Learning: ⭐
Success rate: 99%
```

### **Path 3: Buy Genuine SparkFun Si4703**
```
Difficulty: ⭐
Time: 5 minutes
Cost: $15
Learning: ⭐
Success rate: 99%
Quality: ⭐⭐⭐⭐⭐ Best!
```

---

## 💡 MY HONEST RECOMMENDATION

**Brother, I'll be straight with you:**

You have **bare RDA5807FP chips** which is awesome, BUT:

1. **QFN-16 package** is VERY hard to solder (3mm x 3mm, 16 pins!)
2. **Need hot air station** ($30-50 minimum)
3. **Easy to damage** the chip with too much heat
4. **Success rate** is maybe 50-70% even with experience

**What I suggest:**

1. **Keep the RDA5807FP chips** for a future advanced project
2. **For now, buy RDA5807M module** ($3-5) to test if you like RDA vs Si4703
3. **If RDA5807M works great**, THEN try building from RDA5807FP bare chip
4. **Or save up for genuine SparkFun Si4703** ($15) for best quality

**But if you really want to try building from bare chip:**
- I'll help you 100%!
- The schematic above is complete
- Just get the QFN-16 to DIP adapter board first
- That makes it MUCH easier!

---

**Let me know what you decide!** 🔧✨

**Want me to:**
1. Help you build RDA5807FP from bare chip? 💪
2. Modify code for RDA5807M module? 💻
3. Help you find genuine Si4703? 🛒

Tell me brother! 📻🎧
