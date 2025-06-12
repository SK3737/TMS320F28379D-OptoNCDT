# Wiring Instructions

### Thermistor Setup (Creality hotbed NTC 100k):

- Connect one side of thermistor to **3.3V**
- Other side to a voltage divider node (10k resistor to GND)
- Tap the center to **ADCINA0** (pin J3.30 on LaunchPad)

### SCI Communication

- SCITXDA: GPIO43 (J1.9)
- SCIRXDA: GPIO42 (J1.10)
- Connect TX to RX of USB-UART module and vice versa

# Build Instructions for TMS320F28379D Thermistor Project

### Requirements

- Code Composer Studio (CCS) v12+
- C2000Ware installed
- Target: TMS320F28379D LaunchPad

### Steps

1. Open CCS and import this project.
2. Ensure you link `C2000Ware` in your project's properties:

