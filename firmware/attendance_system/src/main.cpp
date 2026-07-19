#include <Arduino.h>

#include "display_manager.h"
#include "rfid_service.h"
#include "storage_manager.h"

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

enum SystemMode
{
    ATTENDANCE_MODE,
    ADMIN_MODE,
    REGISTRATION_MODE
};

SystemMode currentMode = ATTENDANCE_MODE;
const String ADMIN_UID = "3D 87 D0 06";

struct AttendanceSession
{
    bool active;
};

AttendanceSession session =
{
    false
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

// Persistent Storage
// Application Controller
void processCard(const String &uid);
void registerCard(const String &uid);
void markAttendance(const String &uid);
String readNameFromSerial();
User* findUserByUID(const String &uid);
bool isAdminCard(const String &uid);

void setup() {
  // 1. Initialize Serial
  initSerial();

  initStorage(users, registeredUsers);
  
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


void processCard(const String &uid)
{
    // Admin card must never fall through to normal user lookup/registration.
    if (isAdminCard(uid))
    {
        if (currentMode == ATTENDANCE_MODE)
        {
            currentMode = REGISTRATION_MODE;

            updateDisplay("REGISTER USER", "", "Scan New Card");

            Serial.println("[MODE] Registration Mode");
        }

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

    saveUsers(users, registeredUsers);

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
