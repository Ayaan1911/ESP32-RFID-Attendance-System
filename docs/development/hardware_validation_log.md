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

### Entry: 2026-07-16 - Application Controller Integration (Milestone v0.5.0) [SUCCESS]
- **Summary**: Integrated the Application Controller layer to decouple RFID service scanning from high-level state decisions. Introduced a FSM-like system mode state (`SystemMode`) with initial support for `ATTENDANCE_MODE` and `REGISTRATION_MODE`.
- **Results & Verification**:
  - Successfully compiled the firmware and uploaded it to the physical ESP32 target hardware.
  - Verified boot sequence behaves as expected with SSD1306 displaying the ready state.
  - RFID scanning functions normally, extracting UIDs and delegating to the `processCard()` application controller.
  - Attendance mode successfully prints scans and shows card UID details on the OLED screen.
  - Registration mode routing acts as a placeholder screen showing "Coming Soon".
  - Verified no regressions in communication stability over SPI and I2C buses.
- **Status**: Completed firmware foundation and prepared the project for the upcoming User Management phase.

### Entry: 2026-07-16 - User Management & RFID Authentication (Milestone v0.6.0) [SUCCESS]
- **Summary**: Implemented an in-memory user database, user data model (`User` struct), and lookup system (`findUserByUID()`). Integrated authentication checks in the attendance processing loop.
- **Results & Verification**:
  - Successfully compiled the firmware and uploaded it to the physical ESP32 target hardware.
  - Verified that scanning a registered RFID card (`DB 1D 29 07`) successfully prints `[SUCCESS] Attendance Marked: Ayaan` on Serial and updates the SSD1306 OLED screen with a personalized welcome message: `Welcome`, `Ayaan`, `Attendance Marked`.
  - Verified that scanning an unregistered RFID card displays `Unknown Card` and `Access Denied` on the OLED screen and outputs `[ERROR] Unknown Card` via Serial.
  - Confirmed lookups are performant and error-free with the in-memory array database.
- **Status**: The hardware successfully authenticates users against the internal database. Prepared the project for the upcoming Duplicate Attendance Prevention features.

### Entry: 2026-07-16 - Duplicate Attendance Prevention (Milestone v0.7.0) [SUCCESS]
- **Summary**: Extended the `User` struct with an `attendanceMarked` boolean flag. Updated `markAttendance()` to check the flag before processing a scan. Attendance state is held in RAM and resets on device power loss or restart, which is the expected session-scoped behavior for this milestone.
- **Results & Verification**:
  - Successfully compiled the firmware and uploaded it to the physical ESP32 target hardware.
  - **First scan**: Scanning a registered card produces `[SUCCESS] Attendance Marked: Ayaan` on Serial and displays `Welcome`, `Ayaan`, `Attendance Marked` on the OLED.
  - **Duplicate scan**: Scanning the same card a second time produces `[INFO] Attendance already marked for Ayaan` on Serial and displays `Welcome Back`, `Ayaan`, `Already Marked` on the OLED.
  - **Power cycle**: Restarting the ESP32 correctly resets all `attendanceMarked` flags, allowing fresh attendance marking for the new session as expected.
  - Confirmed no regressions in unknown card rejection or boot sequence behavior.
- **Note**: Attendance state is volatile (stored in RAM only). Data does not persist across power cycles. Persistent storage is planned for the next milestone.
- **Status**: Business rule enforcement is operational. Prepared the project for the upcoming Persistent Storage phase.

### Entry: 2026-07-16 - Admin Mode & RFID-Based Mode Switching (Milestone v0.8.0) [SUCCESS]
- **Summary**: Introduced `ADMIN_MODE` to the `SystemMode` enum and a configurable `ADMIN_UID` constant. Added `isAdminCard()` helper. Updated `processCard()` to detect the administrator card and transition the system into Admin Mode. After the placeholder admin operation, the firmware automatically returns to `ATTENDANCE_MODE`.
- **Results & Verification**:
  - Successfully compiled the firmware and uploaded it to the physical ESP32 target hardware.
  - **Student card scan**: Registered student cards continue to mark attendance normally. OLED displays `Welcome`, `<Name>`, `Attendance Marked`. No regression observed.
  - **Admin card — first scan**: Scanning the designated admin card (`DB 1D 29 07`) triggers `[MODE] Switched to ADMIN MODE` on Serial and shows `ADMIN MODE` / `Scan New Card` on the OLED.
  - **Admin card — operation**: Any subsequent scan while in Admin Mode displays `ADMIN MODE` / `Coming Soon` on the OLED and prints `[ADMIN] Registration module not implemented yet.` on Serial, then automatically returns to Attendance Mode.
  - **Unknown card**: Unknown cards continue to be rejected with `Unknown Card` / `Access Denied` on the OLED.
  - Confirmed no regressions in duplicate-prevention, boot sequence, or OLED rendering.
- **Status**: Multi-mode firmware architecture is operational. Prepared the project for the upcoming Dynamic User Registration phase.

### Entry: 2026-07-16 - Dynamic User Registration (Milestone v0.9.0) [SUCCESS]
- **Summary**: Implemented a full runtime user registration workflow. Scanning the admin card transitions the system into `REGISTRATION_MODE`. The next RFID scan captures the new card's UID, then `readNameFromSerial()` prompts the operator for a name via the Serial Monitor. The new user is dynamically inserted into the `users[]` array and immediately available for attendance marking — no firmware recompilation required.
- **Results & Verification**:
  - Successfully compiled the firmware and uploaded it to the physical ESP32 target hardware.
  - **Registration flow**: Admin card scan → OLED shows `REGISTER USER` / `Scan New Card` → new card presented → Serial Monitor prompts `Enter User Name:` → name entered → OLED shows `User Saved` / `<Name>` → system returns to `ATTENDANCE_MODE`.
  - **Serial output**: `[MODE] Registration Mode` on admin card; `[SUCCESS] Registered: <Name>` after name entry.
  - **Immediate attendance**: Newly registered card was scanned immediately and successfully marked attendance, confirming the live database update works correctly.
  - **Database full guard**: Confirmed `[ERROR] User database is full.` fires and gracefully returns to Attendance Mode when `registeredUsers >= MAX_USERS`.
  - **Regression check**: Pre-loaded users, duplicate-attendance prevention, unknown card rejection, and boot sequence all continue to behave correctly.
- **Known Limitation**: The current implementation allows re-registering the same RFID UID multiple times, creating duplicate entries. Duplicate registration prevention is scheduled for v0.10.
- **Status**: Runtime user management is fully operational. Prepared the project for the upcoming Duplicate User Registration Prevention phase.

### Entry: 2026-07-16 - Registration Validation & UID Uniqueness (Milestone v0.10.0) [SUCCESS]
- **Summary**: Added a pre-registration uniqueness check inside `registerCard()`. Before accepting a new card, the firmware calls `findUserByUID()` against the existing database. If the UID is already assigned to a user, registration is rejected with an informative OLED message and Serial log entry. The `ADMIN_UID` was also updated to the correct physical card UID (`3D 87 D0 06`).
- **Results & Verification**:
  - Successfully compiled the firmware and uploaded it to the physical ESP32 target hardware.
  - **New card registration**: Unregistered cards continue to register normally — OLED shows `User Saved` / `<Name>`, Serial prints `[SUCCESS] Registered: <Name>`.
  - **Duplicate registration attempt**: Re-scanning an already-registered card during Registration Mode displays `Already Registered` / `<Name>` on the OLED and prints `[ERROR] Card already belongs to: <Name>` on Serial, then returns to Attendance Mode.
  - **Admin card routing**: Updated `ADMIN_UID` (`3D 87 D0 06`) correctly triggers Registration Mode on the physical card.
  - **Attendance mode**: All attendance flows (first scan, duplicate scan, unknown card) continue to work correctly with no regression.
  - **Database integrity**: After multiple registration attempts including duplicates, the `users[]` array remains consistent with only unique entries.
- **Status**: User database integrity is enforced at runtime. The in-memory registration system is complete. Prepared the project for the upcoming Persistent Storage (Preferences/NVS) phase.





