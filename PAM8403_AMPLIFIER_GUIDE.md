# 🔊 PAM8403 AMPLIFIER - Test Setup Guide

## 🎯 WHY ADD PAM8403?

**Your Problem:**
- Distorted audio on BOTH ESP-01 AND Arduino Uno
- Same Si4703 module on both = hardware issue!

**PAM8403 Solution:**
- **Buffers Si4703 audio output** (reduces load)
- **Adds proper amplification** (Class-D, very clean!)
- **Isolates headphones** from Si4703 internal amp
- **Cost:** $1-2 on AliExpress/eBay
- **Power:** 2.5V-5V (can use your 3.3V or 5V!)

**Result:** If PAM8403 sounds clean, Si4703 output stage is the problem! ✅

---

## 🛒 WHAT TO BUY

**PAM8403 Module:**
- Search: "PAM8403 amplifier module"
- Red/blue PCB with volume potentiometer
- **Cost:** $1-2
- **Where:** AliExpress, eBay, Amazon

**Module looks like:**
```
┌─────────────────────────┐
│  PAM8403 Mini Amp       │
│                         │
│    [Volume Knob]        │
│                         │
│  VCC GND L R L+ L- R+ R-│
│   ↓   ↓  ↓ ↓  ↓  ↓  ↓ ↓│
└───┴───┴──┴─┴──┴──┴──┴──┘
```

---

## 📐 WIRING DIAGRAM

### **Option A: PAM8403 + Headphones (TESTING)**

**Use this to test if distortion is from Si4703!**

```
┌──────────────┐         ┌──────────────┐         ┌─────────────┐
│   Si4703     │         │   PAM8403    │         │ Headphones  │
│   Module     │         │  Amplifier   │         │  32Ω        │
└──────────────┘         └──────────────┘         └─────────────┘

3.5mm Jack:              Inputs:                  Outputs:
                         
TIP (L) ────────────────→ L (Left In)             
                         
RING (R) ───────────────→ R (Right In)            
                                                   
SLEEVE (GND) ───────────→ GND                     
                         
                         VCC ← 3.3V or 5V          
                                                   
                         L+ ────────────────────→ Left Ear
                         L- ────────────────────→ GND
                         
                         R+ ────────────────────→ Right Ear
                         R- ────────────────────→ GND
```

**Complete Connections:**

| Si4703 | Wire | PAM8403 | Wire | Headphones |
|--------|------|---------|------|------------|
| TIP (Left) | → | L (Left Input) | | |
| RING (Right) | → | R (Right Input) | | |
| SLEEVE | → | GND | | |
| | | VCC | ← 3.3V or 5V | From power supply |
| | | L+ | → | Left channel |
| | | L- | → | GND |
| | | R+ | → | Right channel |
| | | R- | → | GND |

---

### **Option B: PAM8403 + Speaker (BETTER TEST!)**

**Instead of headphones, use 4Ω-8Ω speakers!**

```
┌──────────────┐         ┌──────────────┐         ┌─────────────┐
│   Si4703     │         │   PAM8403    │         │ Speakers    │
│   Module     │         │  Amplifier   │         │ 4Ω or 8Ω    │
└──────────────┘         └──────────────┘         └─────────────┘

TIP ────────────────────→ L                        
                                                   
RING ───────────────────→ R                        
                                                   
SLEEVE ─────────────────→ GND                      
                         
                         VCC ← 5V (NOT 3.3V!)     
                                                   
                         L+ ──┐                    
                              ├─────────────────→ Left Speaker +
                         L- ──┤                    
                              │                    
                         R+ ──┤                    
                              ├─────────────────→ Right Speaker +
                         R- ──┘                    
                              │
                              └─────────────────→ Speakers GND -
```

**Notes:**
- For speakers, use **5V power** (louder, better quality)
- For headphones, use **3.3V power** (safer, less loud)

---

## 🔌 DETAILED WIRING STEPS

### **Materials Needed:**
- ✅ PAM8403 module ($1-2)
- ✅ 5-6 jumper wires (female-to-female)
- ✅ Optional: 3.5mm male plug OR cut headphone cable
- ✅ Power source: Your existing 3.3V (MP2307) or 5V (LX-2BUPS)

### **Step 1: Prepare Audio Input Wires**

**Option A: Use 3.5mm Male Plug (CLEAN!)**
```
Buy: 3.5mm male plug with screw terminals ($1)

Connect:
- TIP terminal → wire to PAM8403 L input
- RING terminal → wire to PAM8403 R input  
- SLEEVE terminal → wire to PAM8403 GND

Plug into Si4703 3.5mm jack
```

**Option B: Cut and Strip Headphone Cable**
```
1. Cut old/cheap headphone cable near plug
2. Strip wires (~1cm):
   - Red wire = TIP (Left)
   - White/Green wire = RING (Right)
   - Copper shield = SLEEVE (GND)
3. Tin wire ends with solder
4. Connect to PAM8403
```

### **Step 2: Connect to PAM8403**

```
PAM8403 Terminal    Connection              Notes
────────────────    ──────────────────────  ─────────────────
VCC                 → 5V (from LX-2BUPS)   For speakers
                    OR 3.3V (from MP2307)   For headphones

GND                 → GND (common ground)   Same GND as Si4703

L (Left In)         → Si4703 TIP (red)      Left audio input

R (Right In)        → Si4703 RING (white)   Right audio input

L+ (Left Out+)      → Headphone Left        Positive output
L- (Left Out-)      → Headphone GND         Negative output

R+ (Right Out+)     → Headphone Right       Positive output  
R- (Right Out-)     → Headphone GND         Negative output
```

### **Step 3: Power Connection**

**For 3.3V (Headphones):**
```
MP2307 3.3V Out ────→ PAM8403 VCC
MP2307 GND ─────────→ PAM8403 GND
                     (same GND as Si4703 and ESP-01)
```

**For 5V (Speakers):**
```
LX-2BUPS 5V Out ────→ PAM8403 VCC  
LX-2BUPS GND ───────→ PAM8403 GND
                     (same GND as everything)
```

**⚠️ IMPORTANT:** PAM8403 can handle 2.5V-5.5V, so either voltage works!

---

## 🧪 TESTING PROCEDURE

### **Test #1: Baseline (Current Setup)**

Before adding PAM8403, confirm distortion:

1. **Current setup:** Si4703 → Headphones (direct)
2. Tune to station with RSSI 40+
3. Set volume = 4 in web interface
4. **Listen:** Distorted? YES / NO
5. **Note distortion type:** Clipping, buzzing, harsh, breaking

---

### **Test #2: With PAM8403 (New Setup)**

Add PAM8403 between Si4703 and headphones:

1. **Wire PAM8403** per diagram above
2. **Power PAM8403** from 3.3V or 5V
3. **Plug Si4703** audio into PAM8403 inputs (L, R, GND)
4. **Connect headphones** to PAM8403 outputs (L+, L-, R+, R-)
5. **Turn PAM8403 volume knob** to 50% (middle position)
6. **Set Si4703 volume** = 8-10 in web interface (higher than before!)
7. **Tune to same station**
8. **Listen:** Distorted? YES / NO

---

### **Expected Results:**

| Scenario | Result | Meaning |
|----------|--------|---------|
| **Clean with PAM8403!** ✅ | No distortion, sounds great! | Si4703 output stage is weak - PAM8403 buffers it |
| **Still distorted** ❌ | Same distortion as before | Problem is earlier (Si4703 internal, or signal issue) |
| **Different distortion** ⚠️ | New type of distortion | PAM8403 wiring issue or power problem |

---

## 🎚️ VOLUME ADJUSTMENT GUIDE

With PAM8403, you have **TWO volume controls**:

### **Si4703 Volume (Web Interface):**
- **Set to 8-12** (higher than without PAM8403!)
- This is the **signal level** going into PAM8403
- Higher = less noise, better SNR

### **PAM8403 Volume (Potentiometer Knob):**
- **Start at 50%** (middle position)
- This is the **final output** volume
- Adjust to comfortable listening level

**Optimal Setup:**
```
Si4703 volume = 10 (strong signal to PAM8403)
         ↓
    PAM8403 input (clean signal, good level)
         ↓
PAM8403 volume knob = 40-60% (comfortable listening)
         ↓
    Clean, amplified output to headphones! ✅
```

---

## 🔧 TROUBLESHOOTING

### **No Sound:**
- Check PAM8403 power (VCC = 3.3V or 5V?)
- Check GND connections (all connected?)
- Check audio input wires (L, R connected?)
- Turn PAM8403 volume knob up
- Check Si4703 volume in web interface

### **Distortion Still Present:**
- Try 5V instead of 3.3V for PAM8403
- Check if Si4703 volume is too high (try 6-8)
- Check PAM8403 volume knob (try 30-50%)
- Verify all GND connections share same ground

### **Quiet/Weak Sound:**
- Increase Si4703 volume (try 10-12)
- Turn PAM8403 volume knob to 70-80%
- Use 5V power instead of 3.3V
- Check speaker/headphone impedance

### **Buzzing/Humming:**
- Ground loop! Make sure all GND connect to same point
- Try different power source for PAM8403
- Add 100µF capacitor between PAM8403 VCC and GND

---

## 💡 ADVANCED: PERMANENT INSTALLATION

If PAM8403 sounds clean, you can make it permanent!

### **Clean Installation:**

```
┌─────────────────────────────────────────────────────────┐
│                     FINAL SETUP                          │
└─────────────────────────────────────────────────────────┘

Power Supply (LX-2BUPS + MP2307):
  5V ──┬──→ ESP-01 (via FT232R for programming only)
       │
       └──→ PAM8403 VCC (for best audio quality!)
       
  3.3V ───→ Si4703 VCC
           → ESP-01 VCC  
           → I2C Pull-ups
           
  GND ────→ ALL GND (common ground)


Audio Path:
  Si4703 3.5mm Jack → PAM8403 Input → Headphones/Speakers
  
  Volume = 10       → PAM8403 Knob  → Comfortable level
  (in web interface)   (40-60%)
```

### **Shopping List for Permanent Setup:**
- ✅ PAM8403 module ($1-2)
- ✅ 3.5mm male plug with terminals ($1)
- ✅ 3.5mm female jack (for output) ($1)
- ✅ Small project box to mount everything
- ✅ Knob for PAM8403 potentiometer (optional, looks nice!)

**Total cost: ~$5** to fix audio permanently! 🎉

---

## 📊 COMPARISON: Before vs After

| Setup | Pros | Cons | Sound Quality |
|-------|------|------|---------------|
| **Si4703 → Headphones** | Simple, cheap | Distorted, weak output | ⭐⭐ Poor |
| **Si4703 → PAM8403 → Headphones** | Clean, buffered | Need extra module | ⭐⭐⭐⭐⭐ Excellent! |
| **Si4703 → 100µF Caps → Headphones** | Clean DC blocking | Need soldering | ⭐⭐⭐⭐ Very Good |
| **Si4703 → PAM8403 → Speakers** | Loud, clean, powerful! | Need speakers | ⭐⭐⭐⭐⭐ Best! |

---

## 🚀 QUICK START - DO THIS NOW!

1. **Order PAM8403 module** ($1-2, arrives in 1-2 weeks)

2. **Meanwhile, test lower volume:**
   - Upload firmware with volume=4 via OTA
   - See if volume 1-3 is cleaner than 8

3. **When PAM8403 arrives:**
   - Wire per diagram above (15 minutes)
   - Test with headphones
   - Report if it's clean!

4. **If clean with PAM8403:**
   - **Root cause confirmed:** Si4703 output stage too weak
   - **Solution:** Keep PAM8403 permanently OR add 100µF capacitors
   - **Your choice!** PAM8403 is easier and sounds better!

---

## 🎯 EXPECTED OUTCOME

**With PAM8403, you should get:**
- ✅ **Crystal clear audio** (no distortion!)
- ✅ **Louder output** (can use speakers!)
- ✅ **Better control** (two volume controls)
- ✅ **Buffered output** (protects Si4703)
- ✅ **Same RSSI, RDS, features** (PAM8403 is just audio path)

**This is a proper engineering solution!** Many commercial FM radios use external amplifiers for exactly this reason. The Si4703 is a great **FM receiver chip** but has a mediocre **headphone amplifier**. PAM8403 fixes that! 🎧✨

---

**Created:** October 17, 2025  
**Cost:** $1-2 for PAM8403 module  
**Time:** 15 minutes to wire and test  
**Result:** Professional-quality FM radio audio! 🎉

Good luck, brother! This will sound AMAZING! 📻🔊
