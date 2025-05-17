# PIC16F877A Temperature & Humidity Monitoring using TMP36 & HIH-5030 Sensors

This embedded project demonstrates environmental sensing using a **PIC16F877A microcontroller** connected to a **TMP36 temperature sensor**, an **HIH-5030 humidity sensor**, and a **16x2 LCD** for real-time data display. The sensors are read through the microcontroller’s ADC channels, and the resulting temperature and humidity values are processed and printed on the LCD screen.

---

## Hardware Requirements  

- **PIC16F877A Microcontroller**  
- **TMP36 Analog Temperature Sensor**  
- **HIH-5030 Analog Humidity Sensor**  
- **16x2 Alphanumeric LCD**  
- **10k Potentiometer** (for LCD contrast)  
- **Resistors (4.7kΩ)**  
- **Power Supply (5V DC)**  
- **Crystal Oscillator (typically 4–10 MHz)**  
- **Breadboard & Jumper Wires**

---

## Circuit Overview  

- **TMP36** → Connected to **AN1** (RA1) of PIC16F877A  
- **HIH-5030** → Connected to **AN0** (RA0) of PIC16F877A  
- **LCD Data Pins** → Connected to PORTD (D4–D7)  
- **LCD Control Pins** → RS, RW, EN on PORTC  
- **Power Supply**: 5V connected to all active components  
- **Contrast**: Adjustable using a potentiometer on the LCD’s VEE pin  

---

## Sensor Specifications  

### HIH-5030 – Analog Humidity Sensor
- Operating Voltage: 2.7V – 5.5V  
- Output: Near-linear voltage (typical: 0.8–3.9V for 0–100% RH)  
- Accuracy: ±3% RH  
- Current: ~200 μA  
- Excellent stability and resistance to condensation

### TMP36 – Analog Temperature Sensor  
- Output: 10 mV/°C  
- Accuracy: ±2°C (typical)  
- Voltage at 25°C = 750 mV  
- Operating Range: −40°C to +125°C  
- Low power & minimal self-heating  

---

## Functional Overview  

- **Analog Readings via ADC**  
  - AN0 → Humidity sensor (HIH-5030)  
  - AN1 → Temperature sensor (TMP36)  

- **Data Processing**  
  - ADC values are averaged over 100 samples  
  - TMP36 formula: `Temperature (°C) = (Vout - 0.5) × 100`  
  - HIH-5030 formula: `Humidity (%) = (((Vout / Vcc) - 0.1515) / 0.00636)`  

- **Display**  
  - Line 1: `Temp : XX.XX C`  
  - Line 2: `Hum  : XX.XX %`  
  - Updated continuously every loop cycle

---

## Proteus Simulation Configuration  

### Components to Select:
- `PIC16F877A`  
- `LCD 16x2`  
- `TMP36` (or equivalent voltage source)  
- `HIH-5030` (or custom analog voltage input)  
- `VCC`, `GND`, `Terminal (for voltage input)`  

### Setup Steps:
1. Open Proteus and create a new project  
2. Place the required components  
3. Connect:
   - TMP36 to AN1 (RA1)
   - HIH-5030 to AN0 (RA0)
   - LCD pins (D4–D7, RS, EN) to PORTD & PORTC
4. Power all components properly  
5. Compile and load the HEX file into PIC16F877A  
6. Run the simulation

---

## Applications  

- **Environmental Monitoring**  
- **Greenhouse Control Systems**  
- **Weather Stations**  
- **Industrial Sensing**  
- **HVAC Control Units**  

---

## Troubleshooting  

| Problem                   | Possible Cause                  | Solution                         |
|---------------------------|----------------------------------|----------------------------------|
| Incorrect temperature     | Sensor not calibrated            | Check TMP36 wiring and formula   |
| Unstable humidity reading | No averaging or ADC noise        | Increase sample size or filtering|
| LCD not displaying data   | Incorrect pin connections        | Verify PORT configuration        |
| Wrong RH values           | Vcc mismatch in formula          | Ensure VSUPPLY matches actual V+ |

---

## License  
**MIT License** – Free to use with attribution  
Created by **Marwen Maghrebi**
