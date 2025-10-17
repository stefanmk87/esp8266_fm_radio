# ESP-01 + Si4703 FM Radio - SIMPLE WIRING GUIDE

## 🔌 EXACT CONNECTIONS - Follow This!

### **Step 1: Power Connections**

```
MP2307 3.3V Output → Connect to these 5 points:
  1. ESP-01 Pin 8 (VCC)
  2. ESP-01 Pin 6 (CH_PD) 
  3. Si4703 VCC pin
  4. Si4703 SEN pin
  5. Top of all pull-up resistors

MP2307 GND → Connect to these 2 points:
  1. ESP-01 Pin 1 (GND)
  2. Si4703 GND pin
```

---

### **Step 2: I2C Connections (Data Lines)**

```
ESP-01 Pin 3 (GPIO0) ──────► Si4703 SDIO pin
                      │
                      └──── 1kΩ resistor to 3.3V

ESP-01 Pin 2 (GPIO2) ──────► Si4703 SCLK pin
                      │
                      └──── 1kΩ resistor to 3.3V
```

---

### **Step 3: Reset Connection (CRITICAL!)**

```
ESP-01 Pin 4 (RX) ──────► Si4703 RST pin
                    │
                    └──── 1kΩ resistor to 3.3V
```

**⚠️ IMPORTANT:** Si4703 RST goes to **ESP-01 Pin 4**, NOT to 3.3V directly!

---

### **Step 4: ESP-01 Pull-ups**

```
3.3V ──── 1kΩ resistor ──── ESP-01 Pin 7 (RESET)
                            (Also add button from Pin 7 to GND)
```

---

### **Step 5: Programming Connections (FT232R)**

```
FT232R TX ──────► ESP-01 Pin 4 (RX) - shares with Si4703 RST!
FT232R RX ──────► ESP-01 Pin 5 (TX)
FT232R GND ─────► GND rail

⚠️ Do NOT connect FT232R VCC - use battery power only!
```

---

## 📋 COMPONENT CHECKLIST

**You Need:**
- [ ] 4x 1kΩ resistors (pull-ups)
- [ ] 2x 100µF capacitors (power filtering)
- [ ] Breadboard or perfboard
- [ ] Jumper wires

---

## 🎯 PIN-BY-PIN TABLE

| ESP-01 Pin | Pin Name | Connect To | Notes |
|------------|----------|------------|-------|
| **Pin 1** | GND | GND rail | Ground |
| **Pin 2** | GPIO2 | Si4703 SCLK | I2C Clock + 1kΩ to 3.3V |
| **Pin 3** | GPIO0 | Si4703 SDIO | I2C Data + 1kΩ to 3.3V |
| **Pin 4** | RX (GPIO3) | Si4703 RST + FT232R TX | Reset + Programming + 1kΩ to 3.3V |
| **Pin 5** | TX | FT232R RX | Programming only |
| **Pin 6** | CH_PD | 3.3V | Enable chip |
| **Pin 7** | RESET | 3.3V via 1kΩ | Pull-up + button to GND |
| **Pin 8** | VCC | 3.3V | Power + 100µF cap to GND |

| Si4703 Pin | Connect To | Notes |
|------------|------------|-------|
| **VCC** | 3.3V | Power + 100µF cap to GND |
| **GND** | GND rail | Ground |
| **SDIO** | ESP-01 Pin 3 | I2C Data |
| **SCLK** | ESP-01 Pin 2 | I2C Clock |
| **RST** | ESP-01 Pin 4 | ⚡ CRITICAL! + 1kΩ to 3.3V |
| **SEN** | 3.3V | I2C mode enable |
| **GPIO1** | Not connected | Optional button |
| **GPIO2** | Not connected | Optional button |

---

## 🔧 BREADBOARD LAYOUT (Top View)

```
                    ┌─── 3.3V Rail ───┐
                    │                 │
        ┌───────────┴─────────────────┴───────────┐
        │                                         │
    ┌───┴───┐                           ┌─────────┴──┐
    │ ESP-01│                           │  Si4703    │
    │       │                           │  Breakout  │
    │  1  2 │                           │            │
    │  3  4 │───── RST ──────────────────► RST      │
    │  5  6 │                           │            │
    │  7  8 │                           │  SDIO SCLK │
    └───┬───┘                           └────┬───┬───┘
        │                                    │   │
        │  Pin 3 ─────────────────────────────┘   │
        │  Pin 2 ─────────────────────────────────┘
        │
    ┌───┴─────────────────────────────────────────┐
    │               GND Rail                      │
    └─────────────────────────────────────────────┘

Pull-up Resistors (1kΩ each):
  3.3V ─┬─ R1 ─ ESP-01 Pin 2 (GPIO2)
        ├─ R2 ─ ESP-01 Pin 3 (GPIO0)
        ├─ R3 ─ ESP-01 Pin 4 (RX/GPIO3)
        └─ R4 ─ ESP-01 Pin 7 (RESET)

Capacitors (100µF each):
  3.3V ─┬─ C1(+) ─ GND  (near ESP-01)
        └─ C2(+) ─ GND  (near Si4703)
```

---

## ⚡ POWER FILTERING (VERY IMPORTANT!)

```
MP2307 3.3V Output
      │
      ├─── 100µF capacitor (+) ──┐
      │                          │
      └─── 100µF capacitor (+) ──┤
                                 │
                               GND Rail

Place capacitors:
  - One CLOSE to ESP-01 VCC pin (< 2cm)
  - One CLOSE to Si4703 VCC pin (< 2cm)
```

---

## 🎬 STEP-BY-STEP BUILD SEQUENCE

### **Build Order (Important!):**

1. **Connect GND rail** everywhere first
2. **Connect 3.3V rail** everywhere (but don't power on yet!)
3. **Add 100µF capacitors** (positive to 3.3V, negative to GND)
4. **Add 1kΩ pull-up resistors:**
   - R1: 3.3V to ESP-01 Pin 2
   - R2: 3.3V to ESP-01 Pin 3
   - R3: 3.3V to ESP-01 Pin 4
   - R4: 3.3V to ESP-01 Pin 7

5. **Connect I2C lines:**
   - ESP-01 Pin 3 → Si4703 SDIO
   - ESP-01 Pin 2 → Si4703 SCLK

6. **Connect RESET line:**
   - ESP-01 Pin 4 → Si4703 RST

7. **Connect Si4703 SEN:**
   - Si4703 SEN → 3.3V

8. **Double-check everything!**

9. **Power on and test!**

---

## ✅ PRE-POWER CHECKLIST

Before turning on power:

- [ ] GND rail connected to ESP-01 Pin 1 and Si4703 GND
- [ ] 3.3V rail connected to ESP-01 Pin 8, Pin 6, Si4703 VCC, Si4703 SEN
- [ ] 100µF capacitor near ESP-01 (positive to 3.3V, negative to GND)
- [ ] 100µF capacitor near Si4703 (positive to 3.3V, negative to GND)
- [ ] 1kΩ resistor from 3.3V to ESP-01 Pin 2
- [ ] 1kΩ resistor from 3.3V to ESP-01 Pin 3
- [ ] 1kΩ resistor from 3.3V to ESP-01 Pin 4 ⚡
- [ ] 1kΩ resistor from 3.3V to ESP-01 Pin 7
- [ ] Wire from ESP-01 Pin 3 to Si4703 SDIO
- [ ] Wire from ESP-01 Pin 2 to Si4703 SCLK
- [ ] Wire from ESP-01 Pin 4 to Si4703 RST ⚡ CRITICAL!
- [ ] Wire from Si4703 SEN to 3.3V
- [ ] Multimeter shows 3.3V on power rail (±0.1V)
- [ ] No shorts between 3.3V and GND (ohmmeter = infinite)

---

## 🚨 COMMON MISTAKES TO AVOID

❌ **WRONG:** Si4703 RST → 3.3V directly
✅ **RIGHT:** Si4703 RST → ESP-01 Pin 4 (with 1kΩ pull-up to 3.3V)

❌ **WRONG:** Using FT232R for power
✅ **RIGHT:** Use MP2307 for power, FT232R only for programming

❌ **WRONG:** No capacitors
✅ **RIGHT:** 100µF near each chip

❌ **WRONG:** 10kΩ pull-ups or no pull-ups
✅ **RIGHT:** 1kΩ pull-ups on GPIO0, GPIO2, GPIO3

---

## 🧪 TESTING

After wiring:

1. **Voltage test:**
   ```
   Multimeter DC voltage mode:
   - ESP-01 Pin 8 to GND = 3.3V ✅
   - Si4703 VCC to GND = 3.3V ✅
   - ESP-01 Pin 4 to GND = 3.3V ✅ (pulled high by resistor)
   ```

2. **Continuity test:**
   ```
   Multimeter continuity mode (power OFF!):
   - ESP-01 Pin 3 to Si4703 SDIO = beep ✅
   - ESP-01 Pin 2 to Si4703 SCLK = beep ✅
   - ESP-01 Pin 4 to Si4703 RST = beep ✅
   ```

3. **Upload firmware:**
   ```
   pio run --target upload
   ```

4. **Test boot 10 times** - should work every time!

---

## 📸 VISUAL SUMMARY

```
ESP-01 Pinout (Looking at chip from top, antenna at bottom):
     ┌──────────┐
     │  ESP-01  │
     │ ┌──────┐ │
   1 │●│      │ │ 8  ← VCC (3.3V)
   2 │●│      │ │ 7  ← RESET (3.3V + 1kΩ)
   3 │●│      │ │ 6  ← CH_PD (3.3V)
   4 │●│      │ │ 5  ← TX (FT232R RX)
     │ └──────┘ │
     │ ~antenna~│
     └──────────┘
     │ │ │ │
     │ │ │ └─── To Si4703 RST ⚡
     │ │ └───── To Si4703 SDIO
     │ └─────── To Si4703 SCLK
     └───────── To GND

All pins 2, 3, 4, 7 have 1kΩ pull-ups to 3.3V!
```

---

**NOW IT'S CLEAR, RIGHT?** 👍

The KEY thing: **ESP-01 Pin 4 goes to Si4703 RST** (not directly to 3.3V!)

This is what makes it work like your Arduino Uno! 🎯
