# Hardware Wiring Specifications

This document outlines the current electrical wiring, pin assignments, and physical interfaces for the ESP32 RFID Attendance System.

---

## 1. Pin Mapping Table

| Component | Pin Name | ESP32 GPIO | Description | Interface |
| :--- | :--- | :--- | :--- | :--- |
| **SSD1306 OLED** | VCC | 3V3 | 3.3V Power Supply | Power |
| **SSD1306 OLED** | GND | GND | Ground | Power |
| **SSD1306 OLED** | SCL | GPIO 22 | I2C Serial Clock | I2C |
| **SSD1306 OLED** | SDA | GPIO 21 | I2C Serial Data | I2C |
| **MFRC522 RFID** | 3.3V | 3V3 | 3.3V Power (Do NOT connect to 5V!) | Power |
| **MFRC522 RFID** | GND | GND | Ground | Power |
| **MFRC522 RFID** | MISO | GPIO 19 | VSPI Master In Slave Out | SPI |
| **MFRC522 RFID** | MOSI | GPIO 23 | VSPI Master Out Slave In | SPI |
| **MFRC522 RFID** | SCK | GPIO 18 | VSPI Serial Clock | SPI |
| **MFRC522 RFID** | SDA (SS) | GPIO 5 | VSPI Slave Select (CS) | SPI |
| **MFRC522 RFID** | RST | GPIO 4 | Device Reset | Control |
| **MFRC522 RFID** | IRQ | N/C | Interrupt Request (Not Connected) | Interrupt |

---

## 2. Interface Wiring Details

### I2C Display Connection (SSD1306)
- The OLED screen is connected via the ESP32 default hardware I2C port (GPIO 21 for SDA, GPIO 22 for SCL).
- Pull-up resistors (typically 4.7kΩ) are built into standard breakout modules. If using raw ICs, external pull-ups are required.

### SPI RFID Connection (MFRC522)
- Connected via the VSPI bus of the ESP32.
- **Reset (RST)** pin is connected to **GPIO 4** to avoid conflicts with default I2C pins or strapping pins.
- **Slave Select (SS)** pin maps to standard VSPI hardware chip select **GPIO 5**.

---

## 3. Future Hardware Notes & Improvements

### Circuit Diagrams (Placeholder)
> [!NOTE]
> Schematic files and Fritzing/KiCad breadboard diagrams will be uploaded to `docs/images/` and linked here once hardware validation is fully cleared.

### Custom PCB Revisions (Placeholder)
> [!NOTE]
> Future plans include transitioning from a breadboard layout to a custom-designed double-layer printed circuit board (PCB) to reduce noise on SPI lines and package the device in a compact, 3D-printed enclosure. Notes on trace widths, decoupling capacitors, and power regulation will be compiled here.
