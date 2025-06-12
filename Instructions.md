# Wiring Instructions for OptoNCDT Analog Sensor

### Sensor Output

- Sensor outputs analog voltage from **0–10V**
- Use a **voltage divider** to step this down to **0–3.3V**

### Voltage Divider Example:

| Sensor Output (0–10V) | Resistor R1 | Resistor R2 | ADC Input Voltage |
|------------------------|-------------|-------------|-------------------|
|     0 – 10 V          |   15kΩ      |    4.7kΩ    |     ~3.2 V max    |

### Connections:

- **Sensor GND** → Board GND  
- **Sensor Analog Out** → Voltage divider → **ADCINA3** (J3.26)  
- **Voltage divider GND** → Board GND  

# Build Instructions for TMS320F28379D

### Requirements

- Code Composer Studio (CCS) v12+
- C2000Ware installed
- Target: TMS320F28379D LaunchPad

### Steps

1. Open CCS and import this project.
2. Ensure you link `C2000Ware` in your project's properties:

![Screenshot 2025-06-12 154853](https://github.com/user-attachments/assets/b9639c03-d400-4df1-a79c-0fc89abe9439)

![image](https://github.com/user-attachments/assets/6acafe46-d9ae-4a99-880c-cf2291f071c6)
