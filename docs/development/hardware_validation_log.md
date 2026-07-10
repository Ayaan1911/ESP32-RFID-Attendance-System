# Hardware Validation Log

This document serves as the official bring-up and verification logbook for validating the physical peripheral connections of the ESP32 RFID Attendance System.

---

## Bring-up Status Checklist

- [x] **ESP32 Core Verification**: Dev module boots, runs blink test sketch, communicates at 115200 bps via serial.
- [x] **OLED Display Verification**: SSD1306 initialized, I2C address validated at `0x3C`, and graphics render reliably.
- [x] **RC522 Physical Wiring**: Physical pins connected via SPI bus according to wiring specifications.
- [ ] **RC522 Communication Verification**: SPI interface is currently failing loopback/read register checks and is under active debugging.

---

## Log History

### Entry: 2026-07-11 - SPI Interface Debugging
- **Observed Behavior**: The RC522 reader chip version query returns `0x00` or `0xFF` instead of the expected version register (such as `0x92` for MFRC522).
- **Hypotheses**:
  1. *Pin Conflict*: Using GPIO pins that overlap with internal flash or default SPI pins configured incorrectly.
  2. *Signal Integrity*: High capacitance on standard breadboard jumpers causing SPI clock skew at high frequency.
  3. *Power Draw*: MFRC522 peak power consumption during initialization might be causing minor voltage drops on the 3.3V line.
- **Actions Planned**:
  - Probe SPI lines (SCK, MOSI, MISO) using a logic analyzer or oscilloscope to verify clock cycles and output waveforms.
  - Test lower SPI clock frequencies in software (down to 1 MHz or less).
  - Inspect solder joints on the MFRC522 header pins.
