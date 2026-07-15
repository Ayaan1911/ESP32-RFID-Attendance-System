/**
 * @file attendance_system.ino
 * @brief ESP32 RFID Attendance System - RFID Service Refactoring (v0.4.0)
 * 
 * Refactors the RFID card processing logic into a service-oriented structure.
 * Breaks down card detection, UID extraction, and UI/Serial handling into
 * modular service functions following the Single Responsibility Principle.
 * 
 * Hardware Pin Connections:
 * - SSD1306 OLED: SDA -> GPIO 21, SCL -> GPIO 22 (I2C)
 * - MFRC522 RFID: SCK -> GPIO 18, MISO -> GPIO 19, MOSI -> GPIO 23, SS/SDA -> GPIO 5, RST -> GPIO 4 (SPI)
 * 
 * @author Ayaan
 * @date 2026-07-11
 */

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED Display Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// MFRC522 Configuration
#define RST_PIN         4
#define SS_PIN          5
MFRC522 mfrc522(SS_PIN, RST_PIN);

enum SystemMode
{
    ATTENDANCE_MODE,
    ADMIN_MODE,
    REGISTRATION_MODE
};

SystemMode currentMode = ATTENDANCE_MODE;
const String ADMIN_UID = "3D 87 D0 06";

struct User
{
    String uid;
    String name;
    bool attendanceMarked;
    bool registered;
};

const int MAX_USERS = 10;

User users[MAX_USERS] =
{
    {"DB 1D 29 07", "Ayaan", false, true},
    {"A1 B2 C3 D4", "Demo User", false, true}
};

int registeredUsers = 2;

// Function Prototypes - System Boot
void initSerial();
void initOLED();
void initRFID();
void showBootScreen();
void showReadyScreen();
void updateDisplay(const String& line1, const String& line2, const String& line3 = "");

// Function Prototypes - RFID Service
void processRFID();
bool isCardPresent();
String getCardUID();
void handleCard(const String& cardUID);

// Application Controller
void processCard(const String &uid);
void registerCard(const String &uid);
void markAttendance(const String &uid);
String readNameFromSerial();

void setup() {
  // 1. Initialize Serial
  initSerial();

  // 2. Initialize OLED Display
  initOLED();

  // 3. Display Boot Screen
  showBootScreen();
  delay(2000); // Allow user to read boot screen and simulate initialization delay

  // 4. Initialize RFID Reader
  initRFID();

  // 5. Display Ready Screen
  showReadyScreen();
}

void loop() {
  // Run the RFID processing service on every loop iteration
  processRFID();
}

/**
 * Controls the RFID processing workflow.
 */
void processRFID() {
  if (isCardPresent()) {
    String cardUID = getCardUID();
    if (cardUID.length() > 0) {
      handleCard(cardUID);
    }
  }
}

/**
 * Detects whether a new RFID card has been scanned.
 * @return True if a card is present and read successfully, false otherwise.
 */
bool isCardPresent() {
  // Check if a new card is physically present on the reader
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }
  // Read the card serial number details
  if (!mfrc522.PICC_ReadCardSerial()) {
    return false;
  }
  return true;
}

/**
 * Extracts and formats the UID from the scanned card.
 * @return Formatted space-separated uppercase hex String.
 */
String getCardUID() {
  String uidStr = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) {
      uidStr += "0";
    }
    uidStr += String(mfrc522.uid.uidByte[i], HEX);
    if (i < mfrc522.uid.size - 1) {
      uidStr += " ";
    }
  }
  uidStr.toUpperCase();
  return uidStr;
}

/**
 * Handles presentation and callback operations for a validated card UID.
 * Prints logs to Serial, updates OLED output, and halts card communications.
 * @param cardUID The formatted card UID string to handle.
 */
void handleCard(const String &uid)
{
    processCard(uid);
}

/**
 * Initializes the Serial interface and prints the boot sequence header.
 */
void initSerial() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("========================================");
  Serial.println("ESP32 RFID Attendance System");
  Serial.println("Boot Sequence");
  Serial.println("========================================");
}

/**
 * Initializes the SSD1306 OLED display via I2C.
 */
void initOLED() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("[ERROR] OLED Display Initialization Failed.");
    while (true); // Loop forever on initialization failure
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  Serial.println("[OK] OLED Display Initialized");
}

/**
 * Renders the initial boot screen on the OLED.
 */
void showBootScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 4);
  display.println("ESP32 RFID");
  display.println("Attendance System");
  display.setCursor(0, 40);
  display.println("Initializing...");
  display.display();
  Serial.println("[OK] Boot Screen Displayed");
}

/**
 * Initializes the MFRC522 reader via SPI.
 */
void initRFID() {
  SPI.begin();
  mfrc522.PCD_Init();
  
  // Verify physical communication with the chip
  byte version = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
  if (version == 0x00 || version == 0xFF) {
    Serial.println("[ERROR] MFRC522 RFID Reader Not Found. Halt.");
    updateDisplay("System Error", "RFID Reader", "Initialization Failed");
    while (true); // Halt loop
  }
  Serial.println("[OK] RFID Reader Initialized");
}

/**
 * Renders the ready screen on the OLED and prints the ready status over Serial.
 */
void showReadyScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 4);
  display.println("RFID Ready");
  display.setCursor(0, 30);
  display.println("Scan your card");
  display.display();

  Serial.println("\n========================================");
  Serial.println("System Ready");
  Serial.println("Waiting for RFID card...");
  Serial.println("========================================");
}

/**
 * Helper utility to render simple multi-line layouts to the display.
 */
void updateDisplay(const String& line1, const String& line2, const String& line3) {
  display.clearDisplay();
  display.setCursor(0, 4);
  display.setTextSize(1);
  display.println(line1);
  
  display.setCursor(0, 24);
  display.println(line2);
  
  if (line3.length() > 0) {
    display.setCursor(0, 44);
    display.println(line3);
  }
  display.display();
}

void processCard(const String &uid)
{
    // Admin card scanned while in Attendance Mode
    if (currentMode == ATTENDANCE_MODE && isAdminCard(uid))
    {
        currentMode = REGISTRATION_MODE;

        updateDisplay("REGISTER USER", "", "Scan New Card");

        Serial.println("[MODE] Registration Mode");

        return;
    }

    if (currentMode == REGISTRATION_MODE)
    {
        registerCard(uid);
    }
    else
    {
        markAttendance(uid);
    }
}

void registerCard(const String &uid)
{
    if (registeredUsers >= MAX_USERS)
    {
        updateDisplay("Database Full", "Full", "");

        Serial.println("[ERROR] User database is full.");

        delay(2000);

        showReadyScreen();

        currentMode = ATTENDANCE_MODE;

        return;
    }

    // Check if this RFID card is already registered
    User *existingUser = findUserByUID(uid);

    if (existingUser != nullptr)
    {
    updateDisplay("Already Registered", existingUser->name);

    Serial.print("[ERROR] Card already belongs to: ");
    Serial.println(existingUser->name);

    delay(2000);

    showReadyScreen();

    currentMode = ATTENDANCE_MODE;

    return;
  }

    String name = readNameFromSerial();

    users[registeredUsers].uid = uid;
    users[registeredUsers].name = name;
    users[registeredUsers].attendanceMarked = false;
    users[registeredUsers].registered = true;

    registeredUsers++;

    updateDisplay("User Saved", name);

    Serial.print("[SUCCESS] Registered: ");
    Serial.println(name);

    delay(2000);

    showReadyScreen();

    currentMode = ATTENDANCE_MODE;
}

void markAttendance(const String &uid)
{
  if (currentMode == ADMIN_MODE){
    updateDisplay("ADMIN MODE", "", "Coming Soon");

    Serial.println("[ADMIN] Registration module not implemented yet.");

    delay(2000);

    showReadyScreen();

    currentMode = ATTENDANCE_MODE;

    return;
    }

    User *user = findUserByUID(uid);

    if (user->attendanceMarked){

    updateDisplay("Welcome Back", user->name, "Already Marked");

    Serial.print("[INFO] Attendance already marked for ");
    Serial.println(user->name);

    delay(2000);

    showReadyScreen();

    return;
}

// First attendance
    user->attendanceMarked = true;

    updateDisplay("Welcome", user->name, "Attendance Marked");

    Serial.print("[SUCCESS] Attendance Marked: ");
    Serial.println(user->name);

    delay(2000);

    showReadyScreen();
}

User* findUserByUID(const String &uid)
{
    for (int i = 0; i < registeredUsers; i++)
    {
        if (users[i].uid == uid)
        {
            return &users[i];
        }
    }

    return nullptr;
}

bool isAdminCard(const String &uid)
{
    return uid == ADMIN_UID;
}

String readNameFromSerial()
{
    Serial.println();
    Serial.println("=================================");
    Serial.println("Enter User Name:");
    Serial.println("=================================");

    while (!Serial.available())
    {
        delay(10);
    }

    String name = Serial.readStringUntil('\n');
    name.trim();

    return name;
}