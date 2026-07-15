# Changelog

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
