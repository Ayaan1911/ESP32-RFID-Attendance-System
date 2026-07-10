# System Architecture and Design

This document details the system design, hardware constraints, and firmware execution structures of the ESP32 RFID Attendance System.

---

## 1. Project Overview
The ESP32 RFID Attendance System is an IoT-enabled attendance logger. By checking student/employee RFID credentials against a local secure database or cloud API, it logs entries and displays immediate status alerts on an I2C OLED display.

---

## 2. System Architecture (Placeholder)
> [!NOTE]
> High-level system interaction diagram block (linking the ESP32 node to the network gateway and cloud services) will be added here once Phase 3 (Networking) implementation begins.

---

## 3. Hardware Layer
- **MCU**: ESP32 Dev Module (Dual-core, built-in flash, high-speed interfaces).
- **Sensors/Actuators**: MFRC522 (RFID reader) and SSD1306 (OLED screen).
- **Communication Buses**: Shared I2C for the screen, SPI for the high-frequency RFID transceivers.

---

## 4. Firmware Layer (Placeholder)
> [!NOTE]
> Detailed finite state machine (FSM) state transitions (BOOTING, IDLE, READ, VALIDATING, RESPONSE, OFFLINE_BUFFER) and task separation policies (Core 0 vs. Core 1) will be documented here during Phase 2.

---

## 5. Networking Layer (Placeholder)
> [!NOTE]
> Details regarding Wi-Fi reconnection handling, captive portal usage for local setup, and cryptographic methods for HTTPS payload delivery will be logged here in Phase 3.

---

## 6. Backend (Placeholder)
> [!NOTE]
> API endpoints definitions, REST schema parameters, database entity-relationship (ER) diagrams, and token-based client verification details will be added in Phase 4.

---

## 7. Web Dashboard (Placeholder)
> [!NOTE]
> User management interfaces, administration features, and statistics metrics charts design will be defined here in Phase 5.

---

## 8. Data Flow (Placeholder)
> [!NOTE]
> Step-by-step transaction logs from RFID tap to database insertion (and caching processes during network timeouts) will be visually outlined here.

---

## 9. Future Improvements
- Migration from Arduino IDE to PlatformIO toolchains.
- Dynamic credential provisioning over Bluetooth Low Energy (BLE).
- Integration of hardware security modules (HSM) or hardware-encrypted storage (EEPROM) for high-integrity operations.
