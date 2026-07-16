# Changelog

## v1.0.0

### Added

- ESP32 Preferences (NVS) integration
- Persistent user database
- Automatic user loading on boot
- Automatic user saving after registration

### Improved

- Firmware persistence
- Registration workflow
- Boot initialization

### Notes

- User database now survives power cycles.
- Attendance session state remains runtime-only.

## v0.10.0

### Added

- Duplicate registration validation
- UID uniqueness enforcement
- Existing user verification

### Improved

- Registration workflow
- User database integrity
- Registration error handling

## v0.9.0

### Added

- Runtime user registration
- Registration mode workflow
- User name input through Serial Monitor
- Dynamic user creation
- Runtime updates to in-memory user database

### Notes

- Duplicate registration prevention is scheduled for v0.10.

## v0.8.0

### Added

- Admin operating mode
- Administrator RFID card detection
- Admin UID configuration
- Admin mode switching

### Improved

- Multi-mode firmware architecture
- Separation between attendance workflow and administrative workflow

## v0.7.0

### Added

- Duplicate attendance prevention
- Attendance state tracking
- Attendance session management

### Improved

- User attendance workflow
- OLED attendance feedback
- Serial attendance logs

## v0.6.0

### Added

- User data model
- User database
- User lookup service
- User search by RFID UID
- Registered user authentication
- Unknown card detection
- Personalized OLED messages
- User-aware attendance workflow

## v0.5.0

### Added

- Application Controller layer
- System mode architecture
- Registration mode placeholder
- Attendance mode placeholder

### Refactored

- handleCard() now delegates to processCard().
- RFID Service is now independent of application logic.
- Improved firmware layering.
- Improved scalability for future features.

## v0.4.0

### Refactored

- Refactored RFID processing into dedicated service functions.
- Added `processRFID()`.
- Added `isCardPresent()`.
- Added `getCardUID()`.
- Added `handleCard()`.
- Improved firmware readability.
- Improved maintainability.
- Applied the Single Responsibility Principle.
- No functional behavior changed.

## v0.3.0

- Implemented structured firmware boot architecture
- Added OLED boot screen
- Added OLED ready screen
- Added RFID initialization
- Added professional serial boot log
- Established firmware initialization sequence

## v0.2.0

- Implemented hardware validation code in `attendance_system.ino`
- Validated SPI communication register validation logic for MFRC522 RFID reader
- Integrated SSD1306 OLED display status reports and card UID rendering

## v0.1.0

- Repository initialized
- Project scaffold created
- Documentation added
- Hardware bring-up phase started
