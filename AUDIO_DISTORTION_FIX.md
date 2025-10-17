# 🔊 AUDIO DISTORTION FIX GUIDE

## 🎯 YOUR PROBLEM

**Symptoms:**
- ✅ Radio works on BOTH ESP-01 AND Arduino Uno  
- ✅ RSSI is GOOD (36-44/75 is decent signal!)
- ❌ **DISTORTED sound on BOTH devices**

**Diagnosis:** Si4703 **HARDWARE PROBLEM** - NOT code!

---

## 🔴 ROOT CAUSE

The Si4703 chip has problems with its built-in headphone amplifier:

1. **Volume too high** → Clipping distortion
2. **DC offset on audio** → Missing/weak capacitors (MOST COMMON!)
3. **Wrong headphone impedance** → Overload
4. **Faulty chip** → Hardware damage

---

## ✅ FIX #1: LOWER VOLUME (DO THIS FIRST!)

**I changed the code for you:**
```cpp
int currentVolume = 4;  // Was 8, now 4 (safer!)
```

**Upload and test:**
```powershell
pio run --target upload
```

**Then test:**
1. Go to `http://192.168.1.31`
2. Set volume to **2**
3. Listen - is it clean?
4. Slowly increase: 3, 4, 5, 6...
5. **At what volume does distortion start?**

**Results:**
- Clean at 1-5, distorts at 6+: **NORMAL** - just keep volume low!
- Distorts even at 1-2: **Need Fix #2** (capacitors)

---

## ✅ FIX #2: ADD 100µF CAPACITORS (PERMANENT!)

The SparkFun Si4703 has **TINY capacitors** (0.1µF) on audio output. They're too small!

### **What You Need:**
- **2x 100µF electrolytic capacitors**  
- 10V or 16V rating
- Cost: $0.50 total

### **How to Install:**

**EASY METHOD: Make External Adapter (No soldering on Si4703!)**

```
Materials:
- 3.5mm extension cable ($2)
- 2x 100µF capacitors
- Solder, heat shrink

Steps:
1. Cut extension cable in half
2. Strip wires on Si4703 side:
   - Red = TIP (Left)
   - White/Green = RING (Right)  
   - Copper shield = SLEEVE (GND)

3. Solder capacitors:

   Si4703 (Male Plug)           Headphones (Female Jack)
   ──────────────────           ────────────────────────
   
   TIP (Red) ─────[+ 100µF -]────── TIP
                   ↑
            Long leg = +
            (toward Si4703)
   
   RING (White) ──[+ 100µF -]────── RING
                   ↑
            Long leg = +
   
   SLEEVE ──────────────────────── SLEEVE
            (no capacitor!)

4. Cover with heat shrink
5. Plug between Si4703 and headphones
```

**Capacitor Polarity:**
```
    ┌──┐
    │  │ ← LONG leg = POSITIVE (+) → toward Si4703
────┤  │
    │  │ ← SHORT leg = NEGATIVE (-) → toward headphones
    │  │ ← White stripe on side = negative
    └──┘
```

**Result:** Blocks DC voltage, **clean audio!** ✅

---

## ✅ FIX #3: TRY DIFFERENT HEADPHONES

Si4703 works best with **32Ω headphones**.

**Test:**
- Try 3-4 different headphones/earbuds
- Most earbuds are 32Ω (perfect!)
- Small speakers (4-8Ω) will distort badly!

---

## 🧪 TESTING PROCEDURE

### **After uploading volume=4 firmware:**

1. Set volume to **1** in web interface
2. Listen - clean or distorted?
3. Increase to 2, 3, 4, 5, 6...
4. **Tell me at what volume it distorts!**

### **Results mean:**
- Distorts at volume **6-8**: Normal - keep it at 4-5 ✅
- Distorts at volume **1-2**: You NEED capacitors!  
- Distorts at **ALL volumes**: Broken Si4703 or wrong headphones

---

## 📊 QUICK DIAGNOSIS

| What You Hear | Cause | Fix |
|---------------|-------|-----|
| Clean at low volume, distorts at high | Normal clipping | Keep volume at 4-5 ✅ |
| Distorted at ALL volumes | DC offset | Add 100µF capacitors |
| One headphone works, others distort | Wrong impedance | Use 32Ω headphones |

---

## 🛒 SHOPPING LIST (if you need capacitors)

**Option A: Buy ready-made**
- Search: "3.5mm audio DC blocker"
- Cost: $3-5
- Plug and play!

**Option B: DIY**
- 2x 100µF electrolytic capacitors (10V or 16V)
- 1x 3.5mm extension cable
- Cost: $2.50 total

**Where to buy:**
- Electronics store
- Amazon: "100µF 16V radial capacitor"
- eBay: "audio coupling capacitor"

---

## 💡 WHY THIS HAPPENS

The Si4703 chip outputs **DC voltage + audio signal** on the 3.5mm jack.

**Normal commercial radios:**
- Use 100-470µF output capacitors
- Block DC, pass audio only
- Clean sound! ✅

**SparkFun Si4703 board:**
- Uses tiny 0.1µF capacitors (to save space/cost)
- DC leaks through
- Distortion! ❌

**The 100µF capacitor fix is PROPER engineering!** This is how it should be.

---

## 🚀 NEXT STEPS

1. **Upload firmware** with volume=4:
   ```powershell
   pio run --target upload
   ```

2. **Test and report:**
   - Volume level where it's clean: ___
   - Volume level where distortion starts: ___
   - Clean at volume 1-2? YES / NO

3. **If distorted even at volume 1:**
   - Buy 100µF capacitors
   - Make adapter cable
   - Problem solved! ✅

---

**Created:** October 17, 2025  
**Status:** Firmware ready - upload and test!  
**Most likely fix:** Add 100µF capacitors between Si4703 and headphones

Let me know what volume level sounds clean! 🎧✨
