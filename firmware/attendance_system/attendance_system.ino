/**
 * @file attendance_system.ino
 * @brief ESP32 RFID Attendance System - System Boot Sequence (v0.3.0)
 * 
 * Implements a structured initialization flow and boot sequence. Separates
 * startup procedures into clean, modular functions and displays professional 
 * status logs over Serial and the OLED screen.
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

// Function Prototypes
void initSerial();
void initOLED();
void initRFID();
void showBootScreen();
void showReadyScreen();
void updateDisplay(const String& line1, const String& line2, const String& line3 = "");

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
  // Awaiting card scan in loop - placeholder for Phase 2 card registration / authorization logic
  
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Basic card scan feedback for verification
    String cardUID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      cardUID += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "") + String(mfrc522.uid.uidByte[i], HEX) + " ";
    }
    cardUID.trim();
    cardUID.toUpperCase();

    Serial.print("[EVENT] Card Detected: ");
    Serial.println(cardUID);

    updateDisplay("Card Detected", "UID:", cardUID);
    delay(2000);
    showReadyScreen();

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
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