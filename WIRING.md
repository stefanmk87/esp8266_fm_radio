# ESP-01 + Si4703 FM Radio - Complete Wiring Guide

## 🚀 Quick Reference - Critical Connections

**ESP-01 to Si4703:**
- Pin 2 (GPIO2) → Si4703 SCLK + 1kΩ pull-up to 3.3V
- Pin 3 (GPIO0) → Si4703 SDIO + 1kΩ pull-up to 3.3V  
- Pin 4 (GPIO3/RX) → Si4703 RST + 1kΩ pull-up to 3.3V ⚡ **CRITICAL FOR STABLE BOOT!**

**Power:**
- Both VCC pins → 3.3V regulated (MP2307) + 100µF capacitors
- Si4703 SEN → 3.3V (enables I2C mode)

**Why GPIO3 for RST?**
- Arduino Uno uses dedicated reset pin → **works reliably** ✅
- ESP-01 without reset control → **random crashes** ❌
- ESP-01 with GPIO3 reset → **stable like Arduino!** ✅

---

## Visual Wiring Diagram

```
                        ┌─────────────────────────────────┐
                        │   SparkFun Si4703 FM Tuner      │
                        ├─────────────────────────────────┤
                        │                                 │
    ┌──────────────────►│ GND                             │
    │                   │ 3.3V   ◄────────────────────┐   │
    │   ┌──────────────►│ SDIO   (I2C Data)           │   │
    │   │   ┌──────────►│ SCLK   (I2C Clock)          │   │
    │   │   │   ┌──────►│ RST    (Reset control)      │   │
    │   │   │   │       │ SEN    ◄────────────────────┤   │
    │   │   │   │       │ GPIO2  (not connected)      │   │
    │   │   │   │       │ GPIO1  (not connected)      │   │
    │   │   │           │                             │   │
    │   │   │           │ LOUT   ──► Left Audio       │   │
    │   │   │           │ ROUT   ──► Right Audio      │   │
    │   │   │           │ GND    ──► Audio Ground     │   │
    │   │   │           └─────────────────────────────┘   │
    │   │   │                                              │
    │   │   │   │       ┌─────────────┐                   │
    │   │   │   │       │   ESP-01    │                   │
    │   │   │   │       ├─────────────┤                   │
    │   │   │   │ ┌─────┤ GND  (1)    │                   │
    │   │   └───┼─┼─────┤ GPIO2 (2)   │ I2C Clock         │
    │   └───────┼─┼─────┤ GPIO0 (3)   │ I2C Data          │
    │   ┌───────┼─┼─────┤ RX/G3 (4)   │ Si4703 RST + 1kΩ  │
    │   │       │ │  ┌──┤ TX    (5)   │ FT232R RX         │
    │   │       │ │  │  │ CH_PD (6)   ├◄───── 3.3V        │
    │   │       │ │  │  │ RESET (7)   ├◄───── 1kΩ ───┐   │
    │   │       │ │  │ └─ VCC  (8)   ├◄──────────────┤   │
    │   │       │ │  │    └─────────────┘                 │
    │   │       │ │  │                                    │
    │   │       │ │  └────────────────────────────────────┤
    │   │       │ │                                       │
    │   │       │ └───────────────────────────────────────┤
    │   │       │                                         │
    └───┴───────┴─────────────────────────────────────────┘
                 │
                 │
                 ▼
               GND Rail


    3.3V Rail (MP2307 Output)
    ═══════════════════════════════════════════════════════
         │         │         │         │         │
         │         │         │         │         │
         ▼         ▼         ▼         ▼         ▼
    ESP-01    ESP-01    Si4703    Si4703    1kΩ pull-up
     VCC      CH_PD     VCC       SEN       to Si4703 RST
    (Pin 8)   (Pin 6)                       (GPIO3 controlled)


    GND Rail
    ═══════════════════════════════════════════════════════
         │         │         │
         ▼         ▼         ▼
    ESP-01    Si4703    Audio
     GND       GND       GND
    (Pin 1)
```

## Component Connections Table

| Component | Pin | Connect To | Notes |
|-----------|-----|------------|-------|
| **ESP-01** | Pin 1 (GND) | GND rail | Common ground |
| | Pin 2 (GPIO2) | Si4703 SCLK | I2C Clock + 1kΩ pull-up |
| | Pin 3 (GPIO0) | Si4703 SDIO | I2C Data + 1kΩ pull-up |
| | Pin 4 (RX/GPIO3) | Si4703 RST + FT232R TX | Reset control + 1kΩ pull-up to 3.3V ⚡ |
| | Pin 5 (TX) | FT232R RX | Programming only |
| | Pin 6 (CH_PD) | 3.3V | Enable chip |
| | Pin 7 (RESET) | 3.3V via 1kΩ | + Reset button to GND |
| | Pin 8 (VCC) | 3.3V rail | Power input |
| **Si4703** | GND | GND rail | Ground |
| | 3.3V | 3.3V rail | Power (add 100µF cap!) |
| | SDIO | ESP-01 GPIO0 | I2C Data |
| | SCLK | ESP-01 GPIO2 | I2C Clock |
| | SEN | 3.3V rail | Tie HIGH for I2C mode |
| | RST | ESP-01 GPIO3 + 1kΩ to 3.3V | Reset pulse control ⚡ CRITICAL! |
| | GPIO1 | Not connected | Optional seek button |
| | GPIO2 | Not connected | Optional seek button |
| | LOUT | Audio left | To headphones/amp |
| | ROUT | Audio right | To headphones/amp |
| **Power** | MP2307 OUT+ | 3.3V rail | Regulated 3.3V |
| | MP2307 OUT- | GND rail | Ground |
| **FT232R** | TX | ESP-01 RX (Pin 4) | Shares with Si4703 RST ⚡ |
| | RX | ESP-01 TX (Pin 5) | Programming only |
| | GND | GND rail | Common ground |
| | VCC | Not connected | ESP-01 powered by battery |

**⚠️ CRITICAL NOTE:** Pin 4 (RX/GPIO3) serves **dual purpose**:
- **During boot:** Controls Si4703 RST pulse (makes boot stable)
- **After boot:** Can receive serial data from FT232R TX
- **Must have 1kΩ pull-up** to 3.3V to keep Si4703 out of reset!

## Pull-up Resistors Required

```
3.3V ────┬─── 1kΩ ─── ESP-01 GPIO0 (Pin 3) ─── Si4703 SDIO
         │
         ├─── 1kΩ ─── ESP-01 GPIO2 (Pin 2) ─── Si4703 SCLK
         │
         ├─── 1kΩ ─── ESP-01 CH_PD (Pin 6)
         │
         ├─── 1kΩ ─── Si4703 RST ◄────────────── ESP-01 GPIO3 (Pin 4)
         │
         └─── 1kΩ ─── ESP-01 RESET (Pin 7) + button to GND
```

**Why pull-ups are needed:**
- GPIO0 & GPIO2: Required for ESP-01 boot mode selection + I2C pull-ups
- CH_PD: Enables ESP-01 chip
- Si4703 RST: Pull-up keeps HIGH, GPIO3 can pulse LOW for reset
- ESP-01 RESET: Normal operation HIGH, button pulls LOW to reset

**Note:** Using 1kΩ resistors (instead of standard 10kΩ) provides:
- Faster I2C rise times
- Better noise immunity
- More reliable communication on longer wires

## Decoupling Capacitors (EMI Protection)

```
ESP-01 Power Filtering:
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
3.3V ───┬─── 100µF ─── GND  (bulk storage)
        │
        ├─── 10µF ──── GND  (fast transients)
        │
        └─── 100nF ─── GND  (high frequency noise)
            │
            └─── ESP-01 VCC (Pin 8)


Si4703 Power Filtering:
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
3.3V ───┬─── 100µF ─── GND  (bulk storage)
        │
        └─── 100nF ─── GND  (high frequency noise)
            │
            └─── Si4703 VCC
```

**Capacitor Placement:**
- Place 100nF ceramic caps as close as possible to IC VCC/GND pins (<5mm)
- Place 100µF electrolytics near power input
- Use low-ESR capacitors for best performance

## Audio Output Options

### Option 1: Headphones (Direct Connection)
```
Si4703 LOUT ───► 3.5mm Jack TIP (Left)
Si4703 ROUT ───► 3.5mm Jack RING (Right)
Si4703 GND  ───► 3.5mm Jack SLEEVE (Ground)
```

### Option 2: Amplifier Module (PAM8403)
```
Si4703 LOUT ───► PAM8403 L-IN
Si4703 ROUT ───► PAM8403 R-IN
Si4703 GND  ───► PAM8403 GND

PAM8403 L-OUT ───► Speaker Left +
PAM8403 R-OUT ───► Speaker Right +
PAM8403 GND   ───► Speaker GND (both)
```

### Option 3: Audio Jack with Volume Control
```
Si4703 LOUT ───► 10kΩ Pot Pin 1
                 10kΩ Pot Pin 2 ───► Jack TIP
                 10kΩ Pot Pin 3 ───► GND

Si4703 ROUT ───► 10kΩ Pot Pin 1
                 10kΩ Pot Pin 2 ───► Jack RING
                 10kΩ Pot Pin 3 ───► GND
```

## FM Antenna Options

### Option 1: Wire Antenna (Simplest)
```
Si4703 ANT pin ───► 70-80cm copper wire
                    (FM wavelength ≈ 3m, so λ/4 ≈ 75cm)
```

### Option 2: 75Ω FM Antenna (Best)
```
Si4703 ANT pin ───► 75Ω FM antenna
                    (car antenna or telescopic)
```

### Option 3: Car Antenna Adapter
```
Si4703 ANT pin ───► Car antenna adapter cable
                    (with built-in amplifier if needed)
```

**Antenna Tips:**
- Keep antenna away from ESP-01 WiFi antenna (5mm+ clearance)
- Extend wire antenna fully for best reception
- Try different orientations (FM is directional)
- Avoid metal enclosures (blocks FM signals)

## Programming Setup (FT232R)

```
┌──────────────────────────────────────────────────────┐
│  Programming Mode Entry (Upload Firmware)            │
├──────────────────────────────────────────────────────┤
│                                                      │
│  1. Hold FLASH button (GPIO0 to GND)                │
│  2. Press and release RESET button                  │
│  3. Release FLASH button                            │
│  4. Click Upload in PlatformIO                      │
│                                                      │
│  FT232R Connections:                                │
│    TX → ESP-01 RX (Pin 4)                          │
│    RX → ESP-01 TX (Pin 5)                          │
│   GND → ESP-01 GND (Pin 1)                         │
│   VCC → NOT CONNECTED (battery powered)            │
│                                                      │
└──────────────────────────────────────────────────────┘
```

## Physical Layout Recommendations

```
Top View Layout:
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

    [ESP-01]        [Si4703]        [MP2307]
       │               │                │
       └───────────────┴────────────────┘
                       │
                   [Battery]

Keep Short:
  - I2C wires (SDA/SCL): < 15cm
  - Power wires: < 20cm
  - Twist power wires together
  - Route away from antenna

Keep Clear:
  - 5mm around ESP-01 antenna
  - 10mm around FM antenna
  - Separate audio wires from I2C
```

## Power Supply Recommendations

```
Battery Configuration:
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
4x 18650 (parallel) ───► LX-2BUPS ───► MP2307 ───► 3.3V
   4000mAh total          5V boost      3.3V buck

Expected Runtime:
  - 4000mAh battery
  - 150mA average current
  - ≈ 26 hours continuous operation
  - (250mA peak during WiFi TX or FM seek)
```

## Testing Procedure

### Step 1: Visual Inspection
- [ ] Check all solder joints
- [ ] Verify no shorts between pins
- [ ] Confirm capacitor polarity (electrolytic)
- [ ] Check pull-up resistors installed

### Step 2: Power Test (No ESP-01 yet)
- [ ] Measure 3.3V at power rails
- [ ] Verify voltage stable (±0.1V)
- [ ] Check current draw < 10mA (idle)

### Step 3: ESP-01 Test (No Si4703 yet)
- [ ] Install ESP-01
- [ ] Power on
- [ ] Check WiFi connection via Serial
- [ ] Measure 3.3V still stable

### Step 4: Si4703 Test (Full system)
- [ ] Connect Si4703
- [ ] Power on
- [ ] Check Serial: "Si4703 initialized successfully"
- [ ] Tune to known FM station
- [ ] Verify audio output

### Step 5: Web Interface Test
- [ ] Open web browser to ESP-01 IP
- [ ] Test seek up/down
- [ ] Test manual tuning
- [ ] Test volume control
- [ ] Save and load presets
- [ ] Verify RDS updates

## Troubleshooting Guide

| Problem | Possible Cause | Solution |
|---------|---------------|----------|
| Si4703 not detected | I2C wiring wrong | Check SDIO→GPIO0, SCLK→GPIO2 |
| | SEN not HIGH | Tie SEN to 3.3V |
| | RST not HIGH | Add 10kΩ pull-up to RST |
| | Power issue | Check 3.3V at Si4703 VCC |
| No audio | Volume = 0 | Increase volume via web |
| | Muted | Click mute button |
| | Bad audio connection | Check LOUT/ROUT wiring |
| Poor FM reception | No antenna | Add 70cm wire antenna |
| | Antenna too short | Extend to full length |
| | Metal enclosure | Remove or use external antenna |
| No RDS data | Weak signal | Improve antenna |
| | Wait longer | RDS needs 10-30 seconds |
| | Station has no RDS | Try different station |
| ESP won't boot | GPIO0 pulled LOW | Check FLASH button not stuck |
| | No pull-ups | Add 10kΩ on GPIO0/GPIO2 |
| | Power dip | Add 100µF cap at VCC |
| Random resets | Power noise | Add decoupling caps |
| | WiFi current spike | Add 100µF + 100nF caps |

## Safety Notes

⚠️ **Important:**
- Never exceed 3.6V on ESP-01 (will damage!)
- Check polarity before connecting power
- Disconnect power when making wiring changes
- Use proper antenna (don't exceed 1W RF power)
- Keep clear of ESP-01 WiFi antenna area

## Final Checklist

Before powering on for first time:
- [ ] All connections double-checked
- [ ] Power supply set to 3.3V
- [ ] Capacitors installed (100µF + 100nF minimum)
- [ ] Pull-up resistors on GPIO0, GPIO2, CH_PD
- [ ] Si4703 SEN tied to 3.3V
- [ ] Si4703 RST tied to 3.3V via 10kΩ
- [ ] No shorts between VCC and GND
- [ ] FM antenna connected
- [ ] Audio output connected (headphones or amp)
- [ ] FT232R ready for programming

---

**You're ready to build your ESP-01 FM Radio!** 📻🎵

Good luck with your project, brother! 🚀
