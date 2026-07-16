# ESP32 RFID Attendance System

[![Platform: ESP32](https://img.shields.io/badge/Platform-ESP32-blue?style=for-the-badge&logo=espressif)](https://www.espressif.com/)
[![Language: C++](https://img.shields.io/badge/Language-C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B)](https://isocpp.org/)
[![IDE: Arduino](https://img.shields.io/badge/IDE-Arduino-00979D?style=for-the-badge&logo=arduino)](https://www.arduino.cc/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=for-the-badge)](LICENSE)
[![Status: Firmware Development](https://img.shields.io/badge/Status-Firmware_Development-blue?style=for-the-badge)](docs/development/hardware_validation_log.md)

A professional, modular RFID-based attendance logging system developed on the ESP32 platform. This project is built incrementally, prioritizing clean software engineering principles, clear hardware decoupling, documentation-first validation, and a clear path toward full network synchronization and dashboard analytics.

---

## 📖 Project Overview

The ESP32 RFID Attendance System aims to deliver a resume-quality, robust IoT system. The initial hardware setup features an ESP32 microcontroller, an SPI-based MFRC522 RFID reader, and an I2C-based SSD1306 OLED display. Starting from hardware bring-up and physical verification, this repository will evolve to encompass local caching, encrypted credentials database storage, Wi-Fi networking, a REST API client, and a remote management dashboard.

### ✨ Key Features (Firmware)
- **Registered User Authentication**: Validates scanned card UIDs against an internal user database.
- **Unknown Card Rejection**: Instantly rejects unregistered cards to prevent unauthorized logging.
- **OLED User Feedback**: Displays personalized greeting messages and attendance status.

---

## 💡 Motivation

Embedded systems design in academic and hobbyist settings often relies on copy-paste code and monolithic, unmaintainable Arduino sketches. This project treats the ESP32 platform like a professional IoT node:
- **Clean separation of concerns** between drivers, logic, and networking.
- **Strict state management** using Finite State Machines.
- **Traceability** via hardware logs and changelogs.
- **Scalability** to support enterprise dashboard management in later phases.

---

## 📈 Development Roadmap

- ✅ Repository Scaffold
- ✅ Hardware Validation
- ✅ System Boot Sequence
- ✅ RFID Service
- ✅ Application Controller
- ✅ User Management
- ✅ Duplicate Attendance Prevention
- ✅ Admin Mode
- ✅ Dynamic User Registration
- ✅ Duplicate User Registration Prevention
- ✅ Persistent User Storage
- ⬜ Attendance Session Manager
- ⬜ Attendance Logs
- ⬜ Wi-Fi Synchronization
- ⬜ Backend API
- ⬜ Web Dashboard

---

## 🏛️ Current Firmware Architecture

```mermaid
graph TD
    A[Hardware Layer] --> B[Initialization Layer]
    B --> C[RFID Service]
    C --> D[Application Controller]
    D --> E[User Manager]
    E --> F[Persistent Storage Preferences/NVS]
    F --> G[Future Attendance Log Manager]
    G --> H[Future Network Layer]

    style A fill:#4CAF50,stroke:#388E3C,stroke-width:2px,color:#fff
    style B fill:#4CAF50,stroke:#388E3C,stroke-width:2px,color:#fff
    style C fill:#4CAF50,stroke:#388E3C,stroke-width:2px,color:#fff
    style D fill:#4CAF50,stroke:#388E3C,stroke-width:2px,color:#fff
    style E fill:#4CAF50,stroke:#388E3C,stroke-width:2px,color:#fff
    style F fill:#4CAF50,stroke:#388E3C,stroke-width:2px,color:#fff
    style G fill:#9E9E9E,stroke:#757575,stroke-width:2px,color:#fff
    style H fill:#9E9E9E,stroke:#757575,stroke-width:2px,color:#fff
```

This modular pipeline establishes that:
1.  **Hardware Layer** represents the physical peripherals (ESP32, MFRC522, SSD1306).
2.  **Initialization Layer** configures system interfaces (Serial, I2C, SPI, Preferences) and displays booting screens, loading saved users from NVS.
3.  **RFID Service** encapsulates card detection, scanning, and raw UID extraction.
4.  **Application Controller** dispatches to the correct operating mode based on system state and card identity.
5.  **User Manager** maintains the user database, coordinates user retrieval/registration, and interfaces with storage.
6.  **Persistent Storage (Preferences/NVS)** handles saving and loading the user database to ESP32 non-volatile flash memory.
7.  **Future Attendance Log Manager** will handle caching attendance logs locally.
8.  **Future Network Layer** will handle remote server/cloud synchronization.



---

## 🛠️ System Specifications

### Hardware Inventory
- **Microcontroller**: ESP32 Dev Module (WROOM-32 Core)
- **RFID Reader**: MFRC522 (13.56 MHz RFID Transceiver)
- **Display**: SSD1306 OLED (0.96" 128x64 display, I2C interface)
- **Access Credentials**: Mifare Classic 1K RFID cards & key fobs

### Software Stack
- **IDE**: Arduino IDE (bringing up hardware), migrating to **VS Code & PlatformIO** in Phase 2.
- **Core Library Dependencies**:
  - `Adafruit SSD1306` (Display output driver)
  - `Adafruit GFX Library` (Display text rendering engine)
  - `MFRC522` (SPI RFID controller library)

---

## 🏛️ High-Level Architecture

The system utilizes an event-driven loop that separates driver execution from high-level state decisions. Detail specifications are documented in the [System Architecture and Design](file:///d:/Projects/ESP32-RFID-Attendance-System/docs/architecture_and_design.md).

```text
+---------------------------------------------------------+
|                  Application Loop                       |
+---------------------------------------------------------+
                          |
                          v
+---------------------------------------------------------+
|                Finite State Machine                     |
|  (BOOTING -> IDLE -> SCANNING -> VALIDATING -> ALERTS)  |
+---------------------------------------------------------+
        |                                       |
        v                                       v
+-----------------------+               +-----------------+
|   MFRC522 RFID SPI    |               | SSD1306 OLED I2C|
|   Driver Interface    |               |  UI Render Box  |
+-----------------------+               +-----------------+
```

---

## 📂 Directory Layout

```text
ESP32-RFID-Attendance-System/
├── .gitignore                    # Arduino, PlatformIO, VS Code, OS temp files
├── LICENSE                       # MIT License file
├── README.md                     # Main repository documentation entry
├── CHANGELOG.md                  # Project versioning and history tracking
├── firmware/                     # Embedded C/C++ firmware
│   └── attendance_system/
│       └── attendance_system.ino # Main Arduino sketch
├── hardware/                     # Wiring maps and layout schematics
│   └── wiring.md
└── docs/                         # Extended specifications and architectural plans
    ├── architecture_and_design.md
    ├── development/
    │   └── hardware_validation_log.md # Log book detailing active bring-up phase
    └── images/                   # Block diagrams and schematics assets
```

---

## ⚙️ Development Philosophy

- **Modular Design**: Driver specific functions are encapsulated rather than scattered through raw loops.
- **Defensive Programming**: Validate serial bounds, memory buffers, and connection integrity explicitly.
- **Explicit Pin Mapping**: Always map pins inside a unified file ([wiring.md](file:///d:/Projects/ESP32-RFID-Attendance-System/hardware/wiring.md)) to avoid hardcoding pins in code.

---

## 🤝 Contributing

Contributions are welcome! Since this repository is a structured engineering portfolio, please open an Issue to discuss hardware mapping changes, alternative FSM implementations, or custom PCB layout reviews before submitting Pull Requests.

---

## 📜 License

This project is licensed under the [MIT License](LICENSE).
