# Hardware Validation Log

This document serves as the official bring-up and verification logbook for validating the physical peripheral connections of the ESP32 RFID Attendance System.

---

## Bring-up Status Checklist

- [x] **ESP32 Core Verification**: Dev module boots, runs blink test sketch, communicates at 115200 bps via serial.
- [x] **OLED Display Verification**: SSD1306 initialized, I2C address validated at `0x3C`, and graphics render reliably.
- [x] **RC522 Physical Wiring**: Physical pins connected via SPI bus according to wiring specifications.
- [x] **RC522 Communication Verification**: SPI interface successfully validated, reader software version read registers validated, and card UID reading confirmed.

---

## Log History

### Entry: 2026-07-11 - SPI Verification & Card Detection [SUCCESS]
- **Summary**: Implemented active register polling and display feedback validation.
- **Results**:
  - The MFRC522 firmware version register successfully query-reads version identifiers (`0x92` typical for v2.0).
  - Serial monitor prints exact card UIDs (e.g. `4 bytes hex: AA BB CC DD`).
  - OLED display successfully shows immediate card detection feedback and details UIDs.

### Entry: 2026-07-11 - Transition to Active Firmware Development
- **Summary**: Hardware verification successfully completed. All core peripherals (OLED display on I2C and RFID reader on SPI) are operational and communicating with the ESP32.
- **Status**: Milestone v0.3.0 boot sequence has been implemented. The project has successfully transitioned from the initial hardware bring-up and validation phase into active application firmware development.

### Entry: 2026-07-11 - RFID Service Refactoring (Milestone v0.4.0)
- **Summary**: Refactored card detection, read, and display logic out of the global Arduino `loop()` into decoupled service modules (`processRFID()`, `isCardPresent()`, `getCardUID()`, and `handleCard()`).
- **Verification**: Verified compilation and execution on target hardware. System bootup and card scan behavior remains identical, but code is now cleanly structured to support future state transitions and database check policies.

