# ⚠️ RDA5807FP SCHEMATIC CORRECTIONS

## 🔍 What Happened?

I initially created the build guide from generic RDA5807 information, but you found the **OFFICIAL schematic** from the GitHub library developer! Thank you for catching this! 🙏

---

## ✅ CORRECTED SCHEMATIC (Official Datasheet)

```
     ANT (75cm wire)
          │
        [1nF] ← Antenna coupling cap (REQUIRED!)
          │
     Pin 1 (GPIO1)
          │
          ├──── Pin 4 (FMIN) ← Main antenna input
          │
     Pin 3 (RFGND) ── GND


     Pin 9 (RCLK) ──┬── 32.768kHz Crystal
                    │
                  [GND]
     
     (NO load capacitors needed!)


     ESP GPIO0 ──[1kΩ]──┬──── Pin 8 (SDA)
                         │
                      [10kΩ] to +3.3V

     ESP GPIO2 ──[1kΩ]──┬──── Pin 7 (SCLK)
                         │
                      [10kΩ] to +3.3V


     Pin 13 (LOUT) ──[10µF]── 3.5mm Jack (Left)
                       │
                     [GND]

     Pin 12 (ROUT) ──[10µF]── 3.5mm Jack (Right)
                       │
                     [GND]


     Pin 10 (VDD) ── +3.3V

     Pins 2,3,5,6,11,14 ── GND (all grounds!)
```

---

## ❌ vs ✅ COMPARISON TABLE

| Item | My Original (WRONG) | Official Schematic (CORRECT) |
|------|---------------------|------------------------------|
| **Antenna coupling** | ❌ Direct connection | ✅ 1nF capacitor required |
| **Crystal connection** | ❌ Pins 1-2 with 20pF caps | ✅ Pin 9 only, no caps |
| **Crystal load caps** | ❌ 2x 20pF to ground | ✅ None (internal to chip) |
| **I2C resistors** | ❌ Only 4.7kΩ pull-ups | ✅ 1kΩ series + 10kΩ pull-ups |
| **Audio output** | ❌ 47pF + 10µF caps | ✅ 10µF only |
| **Power decoupling** | ❌ 100nF on multiple pins | ✅ Not needed (internal) |
| **Pin 7** | ❌ I called it RFGND | ✅ It's SCLK (I2C clock) |
| **Pin 8** | ❌ I called it GND | ✅ It's SDA (I2C data) |
| **Pin 9** | ❌ I said tie to pin 1 | ✅ It's RCLK (crystal) |
| **Pin numbering** | ❌ Completely wrong! | ✅ Based on datasheet |

---

## 🛒 CORRECTED COMPONENT LIST

### **What you ACTUALLY need:**

✅ **REQUIRED components:**
- 1x RDA5807FP chip (QFN-16) - you have this!
- 1x QFN-16 to DIP adapter board ($2) - HIGHLY recommended
- 1x 32.768kHz crystal (through-hole or SMD)
- 1x 1nF capacitor (ceramic, "102" marking)
- 2x 10µF capacitors (electrolytic, for audio)
- 2x 1kΩ resistors (I2C series protection)
- 2x 10kΩ resistors (I2C pull-ups)
- 1x 3.5mm headphone jack
- 1x 75cm wire (antenna)
- 2x 100µF capacitors (optional, for power smoothing)

**Total: ~$4-5** (you have the chip!)

---

### **What you DON'T need (my errors):**

❌ **REMOVE from shopping list:**
- ❌ 2x 20pF capacitors (for crystal load) - NOT needed!
- ❌ 6x 100nF capacitors (for power decoupling) - NOT needed!
- ❌ 1x 47pF capacitor (antenna tuning) - NOT in schematic!
- ❌ 2x 47pF capacitors (audio filtering) - NOT needed!
- ❌ 4.7kΩ resistors (use 10kΩ instead!)

---

## 📌 CRITICAL CORRECTIONS

### **1. Antenna Circuit (MOST IMPORTANT!)**

**❌ WRONG (what I said):**
```
Antenna ──── Pin 5 (FMIN)
```

**✅ CORRECT (official schematic):**
```
Antenna ──[1nF]── Pin 1 (GPIO1) ──┬── Pin 4 (FMIN)
                                  │
                             Pin 3 (RFGND) ── GND
```

**Why it matters:** Without the 1nF coupling cap, the antenna circuit won't work properly! DC voltage would interfere with RF reception.

---

### **2. Crystal Connection (CRITICAL!)**

**❌ WRONG (what I said):**
```
Pin 1 (RCLK) ──┬── Crystal
               │
            [20pF] to GND

Pin 2 (GND) ───┴── Crystal
               │
            [20pF] to GND
```

**✅ CORRECT (official schematic):**
```
Pin 9 (RCLK) ──┬── 32.768kHz Crystal
               │
             [GND]

NO load capacitors! Chip has internal caps!
```

**Why it matters:** Wrong pins = crystal won't oscillate = chip won't work at all!

---

### **3. I2C Bus (PROTECTION!)**

**❌ WRONG (what I said):**
```
ESP GPIO0 ──┬── Pin 13 (SDA)
            │
         [4.7kΩ] to 3.3V
```

**✅ CORRECT (official schematic):**
```
ESP GPIO0 ──[1kΩ]──┬── Pin 8 (SDA)
                   │
                [10kΩ] to 3.3V
```

**Why it matters:** 
- Series resistors (1kΩ) protect chip from voltage spikes
- Wrong pull-up value affects signal integrity
- Wrong pins = I2C won't communicate!

---

### **4. Audio Output (SIMPLIFIED!)**

**❌ WRONG (what I said):**
```
Pin 3 (LOUT) ──[47pF]──[10µF]── Jack
```

**✅ CORRECT (official schematic):**
```
Pin 13 (LOUT) ──[10µF]── Jack (Left)
Pin 12 (ROUT) ──[10µF]── Jack (Right)
```

**Why it matters:** 
- No 47pF capacitors needed (simpler!)
- Correct pin numbers (13/12, not 3/4)

---

### **5. Power Supply (SIMPLER!)**

**❌ WRONG (what I said):**
```
Need 100nF caps on pins 6, 10, 14, 15 (close to chip!)
```

**✅ CORRECT (official schematic):**
```
Pin 10 (VDD) ── +3.3V
Optional: 100µF bulk caps for stability

Chip has INTERNAL voltage regulation!
No decoupling caps needed!
```

**Why it matters:** 
- Saves money (fewer components!)
- Easier assembly
- Chip is designed with internal regulators

---

## 🎯 PIN NUMBERING CORRECTION

**❌ MY WRONG PINOUT:**
```
Pin 1: RCLK (crystal)
Pin 2: GND
Pin 3: LOUT
Pin 4: ROUT
Pin 5: FMIN
...
```

**✅ CORRECT PINOUT (from datasheet):**
```
Pin 1:  GPIO1 (antenna coupling)
Pin 2:  GND_1
Pin 3:  RFGND
Pin 4:  FMIN (main antenna input)
Pin 5:  GND_2
Pin 6:  GND_3
Pin 7:  SCLK (I2C clock)
Pin 8:  SDA (I2C data)
Pin 9:  RCLK (32.768kHz crystal)
Pin 10: VDD (3.3V power)
Pin 11: GND_4
Pin 12: ROUT (right audio)
Pin 13: LOUT (left audio)
Pin 14: GND_5
Pin 15: GPIO3 (not connected)
Pin 16: GPIO2 (not connected)
```

---

## 📝 UPDATED FILES

I've corrected both build guides:

✅ **RDA5807FP_BUILD_GUIDE.md** - Main build guide (corrected)
✅ **RDA5807FP_VISUAL_WIRING_DIAGRAM.md** - Visual diagrams (corrected)
✅ **SCHEMATIC_CORRECTIONS.md** - This file (summary of changes)

---

## 🙏 THANK YOU!

**You saved the project by finding the official schematic!** 

Building from my old (wrong) diagram would have resulted in:
- ❌ Non-functional I2C (wrong pins)
- ❌ Dead crystal oscillator (wrong pins)
- ❌ Poor antenna reception (missing 1nF cap)
- ❌ Wasted money on unnecessary components

**Now you have the CORRECT schematic from the actual developer!** 🎉

---

## 📚 SOURCE

**Official schematic:** GitHub library by RDA5807 developer
**Your file:** `C:/Users/Stefan/Desktop/Datasheet.hk_rda5807fp_7668828.pdf`

**This is the AUTHORITATIVE source - use this!** ✅

---

## 🚀 NEXT STEPS

1. ✅ Use the **CORRECTED** shopping list above
2. ✅ Follow the **CORRECTED** wiring diagrams
3. ✅ Verify pinout matches the official schematic you found
4. ✅ Start soldering with confidence! 🔥

**Good luck with your build!** 🎶📻
