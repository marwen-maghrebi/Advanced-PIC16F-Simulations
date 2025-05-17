# PIC16F877A Ultrasonic Distance Measurement Using SRF04 and UART Output

This embedded project demonstrates interfacing the **SRF04 ultrasonic distance sensor** with the **PIC16F877A microcontroller** to measure distance accurately using pulse timing and display the result through **UART serial communication**. The system uses **Timer1** to capture the echo return time and converts it into a readable distance value (in centimeters), which is transmitted to a serial terminal in real time.

---

## Hardware Requirements
![PIC16F877A SRF04 UART Circuit](circuit.png)

- **PIC16F877A Microcontroller**  
- **SRF04 Ultrasonic Sensor**  
- **UART Virtual Terminal** (e.g., Proteus Serial Monitor or RealTerm)  
- **20 MHz Crystal Oscillator**  
- **Power Supply (5V DC)**  
- **100nF Decoupling Capacitor**  
- **Breadboard & Jumper Wires**

---

## Circuit Overview

- **SRF04 Sensor**  
  - **TRIG** connected to **RB0** (digital output pin)  
  - **ECHO** connected to **RB1** (digital input for timing)  
  - Powered by **5V** supply  
  - 100nF capacitor across VCC-GND for stability

- **PIC16F877A**  
  - **Timer1** used to measure the ECHO pulse width  
  - **UART TX (RC6)** connected to a **Virtual Terminal** for serial output  
  - **20 MHz Crystal Oscillator** ensures accurate timing for ultrasonic measurement  
  - Optional: MCLR connected to pull-up and capacitor for reset

---

## Sensor Specifications

### SRF04 – Ultrasonic Distance Sensor

- **Measurement Range:** 2 cm to 400 cm  
- **Trigger Pulse:** Minimum 10 µs HIGH  
- **Echo Output:** Pulse width proportional to distance  
- **Distance Formula:**  
  \[
  \text{Distance (cm)} = \frac{\text{Time (µs)}}{58}
  \]  
- **Operating Voltage:** 5V  
- **Interface Pins:** TRIG (input), ECHO (output)

---

## Functional Overview

- **Trigger Generation:**  
  PIC sends a 10 µs pulse to the TRIG pin of the SRF04 sensor to initiate a measurement.

- **Echo Measurement:**  
  When the ECHO pin goes HIGH, Timer1 starts. It stops when ECHO goes LOW. This measures the round-trip time of the ultrasonic signal in microseconds.

- **Distance Conversion:**  
  The measured time is divided by 58 to convert microseconds to distance in centimeters.

- **UART Transmission:**  
  The computed distance is formatted as a string and transmitted over UART every 500 milliseconds for monitoring on a serial terminal.

---

## Software Highlights

- **Timer1** configured in 16-bit mode with precise timing resolution  
- **Polling or interrupt** used to detect ECHO pulse edges  
- **UART module** initialized at standard baud rate (e.g., 9600 bps)  
- **Formatted data string** sent over UART:  
  Example: `Distance: 123 cm\r\n`  
- **Delay loop** manages measurement refresh rate (e.g., every 500 ms)

---

## Proteus Simulation Configuration

### Components to Use

- `PIC16F877A`  
- `SRF04 Ultrasonic Sensor`  
- `Virtual Terminal`  
- `20 MHz Crystal Oscillator`  
- `100nF Capacitor`  
- `Power Supply (5V DC)`

### Setup Steps

1. Create a new Proteus project.  
2. Add the **PIC16F877A**, **SRF04 sensor**, and **Virtual Terminal**.  
3. Connect TRIG to **RB0**, ECHO to **RB1**.  
4. Connect **TX (RC6)** to the **Virtual Terminal RX** pin.  
5. Add the **20 MHz crystal** and capacitors for accurate timing.  
6. Load the compiled `.hex` file into the PIC.  
7. Run the simulation. Measured distances will appear in the terminal window.

---

## Applications

- Proximity sensing for robots  
- Automated door control  
- Smart parking systems  
- Liquid level detection  
- DIY sonar rangefinders

---

## Troubleshooting

| Problem                   | Possible Cause                          | Suggested Solution                              |
|---------------------------|------------------------------------------|--------------------------------------------------|
| No UART output            | UART misconfigured or TX not connected   | Check UART baud rate and wiring (RC6 to RX)     |
| Always reads zero         | TRIG pulse too short or ECHO not timed  | Verify TRIG pulse width and ECHO timing logic   |
| Inconsistent readings     | Noisy echo signal or incorrect Timer1   | Add filtering capacitor; confirm Timer config   |
| Terminal shows garbage    | Baud rate mismatch                      | Match UART baud rate in code and terminal (e.g., 9600) |
| Simulation won’t start    | Missing hex file or clock misconfigured | Reload hex and confirm 20 MHz crystal wiring     |

---

## License

**MIT License** – Free to use, modify, and distribute for personal, academic, or commercial use with attribution.



