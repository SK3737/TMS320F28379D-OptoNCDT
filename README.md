# TMS320F28379D-OptoNCDT
To read and interpret the analog output of the OptoNCDT ILD1220-50 displacement sensor using the ADC of the TMS320F28379D, and transmit the resulting distance in millimeters via SCIA serial communication for real-time observation and calibration.

### Sensor Overview: ILD1220-50:
•	Type: Analog laser triangulation sensor

•	Output Range: 0.624 V to 3.12 V

•	Measurement Range: 35 mm to 85 mm

•	Output Type: Voltage output proportional to distance

•	Output Impedance: ~< 1 Ω (buffered analog output)

---

![image](https://github.com/user-attachments/assets/2c822e80-4824-4ad9-b478-0cf25a196ce8)

---

![image](https://github.com/user-attachments/assets/3ce820f3-e19e-4dbe-a580-5e5a836f8d2d)

---

### 3) Wiring and Circuit Setup

| Sensor Pin   | Connected To                    | Notes                                |
|--------------|----------------------------------|--------------------------------------|
| +Vs          | 12V regulated supply             | Sensor rated for 10–30V              |
| GND          | Common ground with MCU          | Shared with F28379D GND              |
| Analog Out   | Voltage divider → ADCIN3 (J3.30) | Scaled to 3.3V using 156Ω–100Ω       |

**Voltage Divider Calculation** (Used for Scaling 5V analog output to 3.3V ADC input):
- R1 (sensor side) = 156 Ω  
- R2 (GND side) = 100 Ω  
- Output voltage scaled from ~5V down to ~3.12V max (safe for ADC)

---

### 4) ADC and SCIA Configuration

- **ADC Pin Used:** ADCIN3  
- **ADC Range:** 0 to 3.3V → 0 to 4095 (12-bit resolution)  
- **Data Format:** `Distance: XX mm, ADC: XXXX`  

---

### 5) Voltage to Distance Mapping

The sensor provides 0.624 V at 0 mm and 3.12 V at 50 mm. To convert voltage to distance:

```c
if (voltage_mV > 624)
    distance_mm = ((voltage_mV - 624) * 50) / (3120 - 624);
else
    distance_mm = 0;
