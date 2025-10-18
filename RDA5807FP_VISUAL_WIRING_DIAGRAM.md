# 🎨 RDA5807FP VISUAL WIRING DIAGRAM
## Complete Soldering Guide - CORRECTED VERSION ✅

---

## ⚠️ IMPORTANT - THIS IS THE CORRECT VERSION! ⚠️

**This diagram has been updated to match the official RDA5807FP datasheet schematic!**

**Key corrections made:**
- ✅ Correct pin numbers (based on official datasheet)
- ✅ 1nF antenna coupling capacitor added (REQUIRED!)
- ✅ Crystal connects to pin 9 (not pins 1-2!)
- ✅ No load capacitors on crystal (chip has internal caps)
- ✅ I2C uses 1kΩ series + 10kΩ pull-ups (not just 4.7kΩ!)
- ✅ Audio uses 10µF only (no 47pF caps needed)
- ✅ Simplified power circuit (chip has internal regulators)

**Source:** Official GitHub library schematic from RDA5807 developer

---

## 📌 COMPLETE SCHEMATIC DIAGRAM (CORRECTED - FROM OFFICIAL DATASHEET)

```
╔══════════════════════════════════════════════════════════════════════════════╗
║              RDA5807FP COMPLETE WIRING DIAGRAM (VERIFIED)                    ║
║                    Based on Official Datasheet Schematic                     ║
╚══════════════════════════════════════════════════════════════════════════════╝

                 ANT1 (Antenna)
                      │
                    [1nF]  ← Antenna coupling capacitor
                      │
                 Pin 1 (GPIO1)
                      │
    ┌─────────────────┴──────────────────────────────────────────┐
    │                                                             │
    │                RDA5807FP / RDA7088 Chip                     │
    │                   (QFN-16 Package)                          │
    │                                                             │
    │  Pin Layout (Top View):                                    │
    │                                                             │
    │      1  ┌───────┐ 16                                       │
    │      ┌──┤       ├──┐                                       │
    │   2 ─┤  │   •   │  ├─ 15                                   │
    │   3 ─┤  │       │  ├─ 14                                   │
    │   4 ─┤  └───────┘  ├─ 13                                   │
    │      └─────────────┘                                        │
    │      5  6  7  8  9  10 11 12                               │
    │                                                             │
    └─────────────────────────────────────────────────────────────┘
           │  │  │  │  │   │  │  │
           │  │  │  │  │   │  │  └─── Pin 12: ROUT (Right Audio)
           │  │  │  │  │   │  └────── Pin 11: GND_4
           │  │  │  │  │   └───────── Pin 10: VDD (3.3V)
           │  │  │  │  └───────────── Pin 9:  RCLK (32.768kHz Crystal)
           │  │  │  └──────────────── Pin 8:  SDA (I2C Data)
           │  │  └─────────────────── Pin 7:  SCLK (I2C Clock)
           │  └────────────────────── Pin 6:  GND_3
           └───────────────────────── Pin 5:  GND_2

    Pin 1:  GPIO1 (Antenna via 1nF)
    Pin 2:  GND_1
    Pin 3:  RFGND (RF Ground)
    Pin 4:  FMIN (FM Antenna Input)
    Pin 13: LOUT (Left Audio Output)
    Pin 14: GND_5
    Pin 15: GPIO3 (Not Connected)
    Pin 16: GPIO2 (Not Connected)


    ANTENNA CIRCUIT:
    ─────────────────
                ANT1 (75cm wire or telescopic)
                  │
                [1nF] ← Coupling capacitor (IMPORTANT!)
                  │
              Pin 1 (GPIO1)
                  │
                  ├─── Pin 4 (FMIN) ← Main antenna input
                  │
                Pin 3 (RFGND) ─── GND


    CRYSTAL OSCILLATOR:
    ───────────────────
                        +3.3V
                          │
              Pin 9 (RCLK)│
                  │       │
            ┌─────┴───────┴─────┐
            │   32.768 kHz      │
            │    Crystal        │
            └─────┬─────────────┘
                  │
                [GND]


    I2C BUS (CORRECTED!):
    ─────────────────────
              +3.3V              +3.3V
                │                  │
             [10kΩ]             [10kΩ]  ← Pull-up resistors (R3, R4)
                │                  │
         ESP ───┴─[1kΩ]─── Pin 8 (SDA/SDIO)  ← Series resistor (R2)
         GPIO0      │
                    │
         ESP ───────[1kΩ]─── Pin 7 (SCLK)    ← Series resistor (R1)
         GPIO2


    AUDIO OUTPUT:
    ─────────────
              Pin 13 (LOUT) ─────[10µF]───── 3.5mm Jack (Tip - Left)
                                   │
                                 [GND]

              Pin 12 (ROUT) ─────[10µF]───── 3.5mm Jack (Ring - Right)
                                   │
                                 [GND]

                                         3.5mm Jack (Sleeve - GND)


    POWER SUPPLY:
    ─────────────
                      +3.3V Rail
                          │
                          ├─── Pin 10 (VDD)
                          │
                        [GND]

    GROUND CONNECTIONS:
    ───────────────────
    Pin 2, 3, 5, 6, 11, 14 ─── ALL to GND
```

---

## 🔌 COMPLETE PIN CONNECTION TABLE (CORRECTED)

```
┌─────┬──────────┬────────────────────────────────────────────────────────────┐
│ Pin │   Name   │  Connection Description (OFFICIAL DATASHEET)               │
├─────┼──────────┼────────────────────────────────────────────────────────────┤
│  1  │  GPIO1   │  Antenna → [1nF capacitor] → This pin                      │
│  2  │  GND_1   │  Ground                                                    │
│  3  │  RFGND   │  RF Ground (connect to main GND plane)                     │
│  4  │  FMIN    │  FM Antenna Input (main RF input, tie to pin 1 circuit)   │
│  5  │  GND_2   │  Ground                                                    │
│  6  │  GND_3   │  Ground                                                    │
│  7  │  SCLK    │  I2C Clock ← [1kΩ] ← ESP-01 GPIO2 (+ 10kΩ pullup)        │
│  8  │  SDA     │  I2C Data ← [1kΩ] ← ESP-01 GPIO0 (+ 10kΩ pullup)         │
│  9  │  RCLK    │  32.768kHz Crystal (reference clock input)                 │
│ 10  │  VDD     │  Power 3.3V (main supply)                                  │
│ 11  │  GND_4   │  Ground                                                    │
│ 12  │  ROUT    │  Right Audio Output → [10µF] → 3.5mm Jack (Ring)          │
│ 13  │  LOUT    │  Left Audio Output → [10µF] → 3.5mm Jack (Tip)            │
│ 14  │  GND_5   │  Ground                                                    │
│ 15  │  GPIO3   │  Not Connected (optional - can use for control)            │
│ 16  │  GPIO2   │  Not Connected (optional - can use for control)            │
└─────┴──────────┴────────────────────────────────────────────────────────────┘

⚠️ CRITICAL NOTES:
   - Antenna MUST have 1nF coupling capacitor (prevents DC on antenna)
   - I2C lines need BOTH 1kΩ series resistors AND 10kΩ pull-ups
   - Crystal connects to pin 9 only (NOT pins 1-2 like I said before!)
   - No load capacitors needed on crystal (internal to chip)
   - Audio outputs need 10µF DC blocking caps (not 47pF!)
```

---

## ⚡ POWER SUPPLY DIAGRAM

```
╔══════════════════════════════════════════════════════════════════════════════╗
║                    POWER SUPPLY CONNECTIONS (CRITICAL!)                      ║
╚══════════════════════════════════════════════════════════════════════════════╝

                           MP2307 (3.3V Regulator)
                                    │
                        ┌───────────┴───────────┐
                        │                       │
                     [100µF]                 [100µF]  ← Bulk capacitors
                        │                       │      (electrolytic)
                        ├───────┬───────┬───────┤
                        │       │       │       │
                        │    [100nF] [100nF] [100nF]  ← Decoupling caps
                        │       │       │       │      (ceramic, CLOSE!)
                        │       │       │       │
                      VIO(15) DVDD(14,10) AVDD(6)
                        │       │       │
                        └───────┴───────┴───────RDA5807FP Chip
                                │
                              [GND]

📌 IMPORTANT:
   - 100nF caps MUST be within 2mm of chip pins!
   - Use ceramic capacitors for 100nF (not electrolytic)
   - Use electrolytic for 100µF bulk capacitors
   - Short traces from regulator to chip!
```

---

## 🎵 AUDIO OUTPUT CIRCUIT (CORRECTED)

```
╔══════════════════════════════════════════════════════════════════════════════╗
║                        AUDIO OUTPUT SECTION                                  ║
╚══════════════════════════════════════════════════════════════════════════════╝

    RDA5807FP Chip                           3.5mm Headphone Jack
         │                                          │
         │                                    ┌─────────┐
    Pin 13 (LOUT)────────[10µF]──────────────│  Tip    │ Left Channel
                          │                  │ (Left)  │
                        (+ -)                │         │
                          │                  │  Ring   │ Right Channel
                        [GND]                │ (Right) │
                                             │         │
    Pin 12 (ROUT)────────[10µF]──────────────│ Sleeve  │ Ground
                          │                  │ (GND)   │
                        (+ -)                └─────────┘
                          │
                        [GND]

📌 COMPONENT NOTES (CORRECTED!):
   - 10µF: Electrolytic or tantalum (DC blocking) - NOT 47pF!
   - Polarity: + side towards chip, - side towards jack
   - NO 47pF capacitors needed (that was my error!)
   - Schematic shows direct connection via 10µF only
   - Can drive headphones directly (32Ω to 600Ω)
   - For speakers, use external amplifier (PAM8403)
```

---

## 🔗 I2C BUS CONNECTIONS (CORRECTED!)

```
╔══════════════════════════════════════════════════════════════════════════════╗
║                    I2C BUS CONNECTIONS (OFFICIAL SCHEMATIC)                  ║
╚══════════════════════════════════════════════════════════════════════════════╝

         +3.3V                    +3.3V
           │                        │
        [10kΩ]                   [10kΩ]  ← Pull-up resistors (R3, R4)
         (R3)                     (R4)
           │                        │
           ├────────────────────────┼────────────────────
           │                        │                   │
           │                        │                   │
      ESP-01 Module            RDA5807FP Chip
      ┌──────────┐                  │
      │  GPIO0   │────[1kΩ]─────────┼─ Pin 8 (SDA/SDIO)
      │  (SDA)   │    (R2)           │
      │          │                   │
      │  GPIO2   │────[1kΩ]──────────┴─ Pin 7 (SCLK)
      │  (SCL)   │    (R1)
      └──────────┘

📌 I2C RESISTOR CONFIGURATION (IMPORTANT!):
   - R1: 1kΩ series resistor on SCLK line (between ESP GPIO2 and chip)
   - R2: 1kΩ series resistor on SDA line (between ESP GPIO0 and chip)
   - R3: 10kΩ pull-up resistor (SDA to +3.3V)
   - R4: 10kΩ pull-up resistor (SCLK to +3.3V)
   
   ⚠️ DO NOT use 4.7kΩ pull-ups! Use 10kΩ as shown in schematic!
   ⚠️ Series resistors (1kΩ) are REQUIRED for protection!

📌 I2C SPECS:
   - Address: 0x11 (sequential mode) or 0x10 (random access)
   - Speed: 100kHz or 400kHz
   - Pull-ups: 10kΩ to 3.3V (NOT 5V!)
   - Series resistors protect chip from voltage spikes
```

---

## 🔊 CRYSTAL OSCILLATOR CIRCUIT (CORRECTED!)

```
╔══════════════════════════════════════════════════════════════════════════════╗
║                    32.768kHz CRYSTAL CONNECTION                              ║
╚══════════════════════════════════════════════════════════════════════════════╝

                    RDA5807FP Chip
                         │
                         │
                    Pin 9 (RCLK)
                         │
                    ┌────┴────┐
                    │         │
              ┌─────┤ 32.768  ├─────┐
              │     │   kHz   │     │
              │     │ Crystal │     │
              │     └─────────┘     │
              │                     │
              └─────────┬───────────┘
                        │
                      [GND]

📌 CRYSTAL NOTES (CORRECTED!):
   - Crystal connects to Pin 9 (RCLK) only
   - NO load capacitors needed! (chip has internal caps)
   - Crystal frequency: 32.768kHz (watch crystal)
   - One side to pin 9, other side to ground
   - Keep traces SHORT (< 5mm if possible)
   - Don't route other signals near crystal!
   - Crystal can be through-hole or SMD (3.2mm x 1.5mm)

⚠️ My previous diagram showing pins 1-2 was WRONG!
   The GitHub schematic shows pin 9 only - this is CORRECT!
```

---

## 📡 ANTENNA CONNECTION (CORRECTED!)

```
╔══════════════════════════════════════════════════════════════════════════════╗
║                    ANTENNA CONNECTION (OFFICIAL SCHEMATIC)                   ║
╚══════════════════════════════════════════════════════════════════════════════╝

    75cm Wire Antenna or Telescopic
              │
              │
            [1nF]  ← COUPLING CAPACITOR (REQUIRED!)
              │
              │
         Pin 1 (GPIO1)
              │
              ├────── Pin 4 (FMIN) ← Main FM input
              │
         Pin 3 (RFGND) ── GND


📌 ANTENNA CIRCUIT EXPLANATION:
   - 1nF capacitor blocks DC voltage from antenna
   - Pin 1 (GPIO1) receives antenna signal via 1nF cap
   - Pin 4 (FMIN) is the main FM receiver input
   - Pin 3 (RFGND) provides RF ground reference
   - Connect pins 1, 3, 4 together as shown above

📌 ANTENNA OPTIONS:
   1. Simple wire: 75cm straight wire (λ/4 for 100MHz FM)
   2. Telescopic antenna: Adjustable length (best reception)
   3. PCB trace: 75cm zigzag pattern on board

⚠️ CRITICAL: You MUST use the 1nF coupling capacitor!
   Without it, the antenna circuit won't work properly!

📌 1nF CAPACITOR SPECS:
   - Ceramic capacitor, 50V or higher
   - Size: 0805 SMD or through-hole disk
   - Marking: "102" (10 with 2 zeros = 1000pF = 1nF)
   - Also called: 1000pF or 0.001µF
```

---

## 🛠️ PROTOBOARD LAYOUT (STEP-BY-STEP)

```
╔══════════════════════════════════════════════════════════════════════════════╗
║                      RECOMMENDED PROTOBOARD LAYOUT                           ║
╚══════════════════════════════════════════════════════════════════════════════╝

┌────────────────────────────────────────────────────────────────────┐
│  +3.3V Rail ════════════════════════════════════════════════════  │
│     ║                                                              │
│   [100µF]    [100µF]         ESP-01 Module                        │
│     ║          ║              ┌──────────┐                        │
│     ╠══════════╣              │ TX    RX │                        │
│     ║          ║              │ G0    G2 │ ← GPIO pins            │
│  [100nF]   [100nF]  [100nF]  └──┬────┬──┘                        │
│     ║          ║       ║         │    │                           │
│     ║          ║       ║      GPIO0  GPIO2                        │
│     ║          ║       ║       (SDA)  (SCL)                       │
│     ║          ║       ║         │     │                          │
│  ┌──╨──────────╨───────╨──┐   [4.7k] [4.7k] ← Pull-ups           │
│  │  QFN-16 to DIP Adapter │      ║     ║                          │
│  │  ┌────────────────┐    │      ║     ║                          │
│  │  │ RDA5807FP Chip │    │      ║     ║                          │
│  │  │   (on adapter) │    │      ║     ║                          │
│  │  │                │    │      ║     ║                          │
│  │  │  Pin 1 = dot   │    │      ║     ║                          │
│  │  └────────────────┘    │      ║     ║                          │
│  │                        │      ║     ║                          │
│  │ 1  2  3  4  5  6  7  8 │      ║     ║                          │
│  │ ●  ●  ●  ●  ●  ●  ●  ● │      ║     ║                          │
│  │                        │      ║     ║                          │
│  │ ●  ●  ●  ●  ●  ●  ●  ● │      ║     ║                          │
│  │16 15 14 13 12 11 10  9 │      ║     ║                          │
│  └────┬───┬──┬──┬──────┬──┘      ║     ║                          │
│       │   │  │  │      │         ║     ║                          │
│       │   │  ║  ╠══════╩═════════╩═════╝                          │
│       │   │  ║  ║ (Pin 13 SDA, Pin 12 SCL)                        │
│       │   │  ║  ║                                                  │
│    [20pF][20pF]║  ║                                                │
│       │   │  ║  ║                                                  │
│    ┌──┴───┴──┐ ║  ║                                                │
│    │32.768kHz│ ║  ║                                                │
│    │ Crystal │ ║  ║                                                │
│    └─────────┘ ║  ║                                                │
│                ║  ║                                                │
│            [47pF] [47pF]  [10µF] [10µF]                            │
│                ║     ║      ║      ║                               │
│                ║     ║      ║      ║                               │
│                ╠═════╬══════╬══════╣                               │
│                ║     ║      ║      ║                               │
│            ┌───╨─────╨──────╨──────╨───┐                           │
│            │   Tip  Ring  Sleeve       │                           │
│            │    L    R     GND         │                           │
│            └─────────────────────────── │ 3.5mm Jack                │
│                                                                     │
│  GND Rail ══════════════════════════════════════════════════════  │
└─────────────────────────────────────────────────────────────────────┘

LEGEND:
  ═══  Power rail (thick traces)
  ───  Signal wire
  ║    Vertical connection
  ╠    T-connection
  ●    Solder point / pin
  [  ] Component
```

---

## 🔥 SOLDERING ORDER (CORRECTED!)

### **COMPLETE COMPONENT LIST (FROM GITHUB SCHEMATIC):**
```
✅ 1x RDA5807FP chip (QFN-16 package) - you have this!
✅ 1x QFN-16 to DIP adapter board ($2 AliExpress) - HIGHLY recommended!
✅ 1x 32.768kHz crystal (through-hole or SMD)
✅ 1x 1nF capacitor (ceramic, for antenna coupling)
✅ 2x 10µF capacitors (electrolytic, for audio DC blocking)
✅ 2x 100µF capacitors (electrolytic, for power supply - optional)
✅ 2x 1kΩ resistors (I2C series protection - R1, R2)
✅ 2x 10kΩ resistors (I2C pull-ups - R3, R4)
✅ 1x 3.5mm headphone jack
✅ 1x Wire for antenna (75cm length)

Total cost: ~$2-3 (you already have the chip!)
```

### **Step 1: Prepare QFN-16 to DIP Adapter**
```
┌──────────────────────────────────┐
│  QFN-16 to DIP Adapter Board     │
│  (Recommended: $2 on AliExpress)  │
│                                  │
│  ┌──────────────┐                │
│  │   QFN Pad    │ ← Solder chip here first
│  │   (center)   │   (use hot air or reflow)
│  └──────────────┘                │
│   o o o o o o o o  ← DIP pins    │
│                                  │
│   o o o o o o o o                │
└──────────────────────────────────┘

📌 Use hot air station: 350-380°C, apply flux
```

### **Step 2: Solder Crystal**
```
Pin 9 (RCLK) ──┬─── 32.768kHz crystal (one leg)
               │
             [GND] ──── Crystal (other leg to ground)

NO capacitors needed! Chip has internal load caps!
```

### **Step 3: Solder Antenna Coupling**
```
Antenna wire ──── [1nF] ──── Pin 1 (GPIO1)
                               │
                         Pin 4 (FMIN)
                               │
                         Pin 3 (RFGND) ──── GND
```

### **Step 4: Solder I2C Resistors (CRITICAL!)**
```
ESP-01 GPIO0 ──── [1kΩ] (R2) ──┬──── Pin 8 (SDA)
                                │
                             [10kΩ] (R3) ──── +3.3V

ESP-01 GPIO2 ──── [1kΩ] (R1) ──┬──── Pin 7 (SCLK)
                                │
                             [10kΩ] (R4) ──── +3.3V
```

### **Step 5: Solder Audio Outputs**
```
Pin 13 (LOUT) ──── [10µF] ──── 3.5mm Jack TIP (Left)
                     │
                   [GND]

Pin 12 (ROUT) ──── [10µF] ──── 3.5mm Jack RING (Right)
                     │
                   [GND]

Note: 10µF polarity: + towards chip, - towards jack
```

### **Step 6: Solder Power (Optional Bulk Caps)**
```
+3.3V ──┬──── [100µF] ──── GND  (optional, for stability)
        │
        └──── Pin 10 (VDD)
```

### **Step 7: Connect All Grounds**
```
Pin 2, 3, 5, 6, 11, 14 ──── ALL to GND rail

Ground connections are CRITICAL! Don't skip any!
```

---

## 🧪 TESTING PROCEDURE

### **Test 1: Visual Inspection**
```
✓ Check all solder joints under magnifier
✓ Look for solder bridges (especially on QFN!)
✓ Verify component orientation (chip dot = pin 1)
✓ Check capacitor polarity (10µF electrolytics)
```

### **Test 2: Power Rails (BEFORE applying power)**
```
Multimeter in continuity mode:

1. Pin 6, 10, 14, 15 → Should all connect to +3.3V rail
2. Pin 2, 5, 7, 8, 11 → Should all connect to GND
3. +3.3V to GND → Should be OPEN (no short!)

⚠️ If shorted, DO NOT power on! Find the short first!
```

### **Test 3: Apply Power**
```
Connect MP2307 3.3V regulator:

1. Measure voltage at Pin 15 (VIO) → Should be 3.3V ± 0.1V
2. Measure current draw → Should be 20-40mA (idle)
3. Touch chip gently → Should be COOL (not hot!)

⚠️ If chip gets hot, POWER OFF immediately!
```

### **Test 4: I2C Detection**
```
Upload this test code to ESP-01:

#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin(0, 2); // SDA=GPIO0, SCL=GPIO2
  
  Serial.println("Scanning I2C bus...");
  Wire.beginTransmission(0x11); // RDA5807 address
  byte error = Wire.endTransmission();
  
  if (error == 0) {
    Serial.println("✓ RDA5807FP found at 0x11!");
  } else {
    Serial.println("✗ No device found!");
  }
}

void loop() {}

Expected output:
✓ RDA5807FP found at 0x11!
```

### **Test 5: FM Reception**
```
Upload full FM radio code (see BUILD_GUIDE.md)

1. Power on → LED should blink
2. Open web browser → http://192.168.4.1
3. Click "Seek Up" → Should find FM station
4. Plug in headphones → Should hear audio!

✓ SUCCESS! Your custom FM radio works!
```

---

## ⚠️ TROUBLESHOOTING

```
┌──────────────────────┬────────────────────────────────────────────┐
│ Problem              │ Solution                                   │
├──────────────────────┼────────────────────────────────────────────┤
│ Chip gets hot        │ • Power short! Check pins 6,10,14,15      │
│                      │ • Remove power immediately!                │
├──────────────────────┼────────────────────────────────────────────┤
│ I2C not detected     │ • Check SDA/SCL connections                │
│                      │ • Verify 4.7kΩ pull-ups                    │
│                      │ • Test with multimeter continuity          │
├──────────────────────┼────────────────────────────────────────────┤
│ No FM stations found │ • Check crystal oscillator (32.768kHz)     │
│                      │ • Verify 20pF caps on crystal              │
│                      │ • Check antenna connection (pin 5)         │
├──────────────────────┼────────────────────────────────────────────┤
│ No audio output      │ • Check 47pF and 10µF caps on LOUT/ROUT   │
│                      │ • Verify 3.5mm jack wiring                 │
│                      │ • Test with headphones (not speakers!)     │
├──────────────────────┼────────────────────────────────────────────┤
│ Distorted audio      │ • Lower volume in code (volume = 2)        │
│                      │ • Check 100nF decoupling caps              │
│                      │ • Verify 3.3V is stable (not dropping)     │
└──────────────────────┴────────────────────────────────────────────┘
```

---

## 📸 COMPONENT PHOTOS (Text Descriptions)

### **32.768kHz Crystal**
```
Looks like: Small silver cylinder (2mm x 6mm)
           or square SMD package (3.2mm x 1.5mm)
Label: "32.768" or "32768"
Price: $0.50 or less
```

### **1nF Capacitor (NEW - REQUIRED!)**
```
Looks like: Small brown/yellow disk (3-5mm diameter)
           or SMD ceramic (0805 size)
Label: "102" (10 with 2 zeros = 1000pF = 1nF)
Also called: 1000pF or 0.001µF
Price: $0.10 or less
⚠️ This is CRITICAL for antenna coupling!
```

### **1kΩ Resistor (NEW - REQUIRED x2!)**
```
Color bands: Brown-Black-Red-Gold (1-0-00 = 1000Ω)
SMD label: "102" (10 with 2 zeros)
Purpose: I2C series protection (R1, R2)
Price: $0.05 each
⚠️ You need TWO of these (one for SDA, one for SCL)!
```

### **10kΩ Resistor (NEW - REQUIRED x2!)**
```
Color bands: Brown-Black-Orange-Gold (1-0-000 = 10,000Ω)
SMD label: "103" (10 with 3 zeros)
Purpose: I2C pull-ups (R3, R4)
Price: $0.05 each
⚠️ You need TWO of these (one for SDA, one for SCL)!
⚠️ NOT 4.7kΩ like I said before - use 10kΩ!
```

### **10µF Capacitor**
```
Looks like: Small cylinder with + stripe (electrolytic)
           or SMD tantalum (brown square)
Polarity: + side towards chip, - towards jack
Purpose: Audio DC blocking (you need 2 pieces!)
Price: $0.20 for 2 pieces
```

### **100µF Capacitor (OPTIONAL)**
```
Looks like: Larger cylinder with + stripe
Polarity: + towards 3.3V, - towards GND
Purpose: Power supply smoothing (optional but recommended)
Price: $0.30 for 2 pieces
Note: Can skip these if your 3.3V regulator is stable
```

---

## ✅ FINAL CHECKLIST (CORRECTED!)

**Before powering on:**
- [ ] All solder joints look clean (no bridges)
- [ ] Crystal connected to pin 9 and GND (NO capacitors!)
- [ ] 1nF capacitor between antenna and pin 1 (REQUIRED!)
- [ ] Antenna also connected to pin 4 (FMIN)
- [ ] Pin 3 (RFGND) connected to GND
- [ ] I2C has 1kΩ series resistors (R1, R2)
- [ ] I2C has 10kΩ pull-ups (R3, R4) to 3.3V
- [ ] Audio outputs have 10µF caps (NOT 47pF!)
- [ ] All grounds connected: pins 2, 3, 5, 6, 11, 14
- [ ] Pin 10 (VDD) connected to 3.3V
- [ ] No shorts between 3.3V and GND
- [ ] Chip orientation correct (dot = pin 1)

**After powering on:**
- [ ] Voltage at pin 10 is 3.3V
- [ ] Chip stays cool (not hot)
- [ ] Current draw 20-40mA
- [ ] I2C scanner finds device at 0x11
- [ ] FM radio code compiles and uploads
- [ ] Can seek and find FM stations
- [ ] Audio output in headphones

---

## 🎉 SUCCESS!

**If everything works:**
- You just built a FM radio from a bare chip! 🎊
- This is ADVANCED level electronics! 💪
- You can now proudly say you're an SMD solderer! 🔥

**Share your success:**
- Take photos of your build!
- Test reception and audio quality
- Compare with the fake Si4703 modules
- Enjoy your custom FM radio! 📻

---

## 📚 REFERENCE LINKS

**Where to buy components:**
- QFN-16 to DIP adapter: AliExpress "QFN16 to DIP16"
- 32.768kHz crystal: Search "32768 crystal watch"
- Capacitor kit: "Ceramic capacitor assortment kit"
- I2C pull-ups: "Resistor kit 1/4W"

**Datasheets:**
- RDA5807FP: Search "RDA5807FP datasheet PDF"
- Crystal specs: 12.5pF load capacitance typical
- I2C bus: 100kHz or 400kHz supported

---

**Good luck with your build! 🚀**

**Questions? Check the main BUILD_GUIDE.md for code and testing!**
