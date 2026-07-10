/**
 * @file attendance_system.ino
 * @brief ESP32 RFID Attendance System - Hardware Validation Sketch
 * 
 * This sketch validates the SPI communication with the MFRC522 RFID reader
 * and the I2C communication with the SSD1306 OLED display. When a card is scanned,
 * it retrieves the UID, prints it to the Serial Monitor, and displays it on the OLED screen.
 * 
 * Hardware Connections:
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
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D or 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// MFRC522 Configuration
#define RST_PIN         4   // Configurable, typical: GPIO 4 to avoid I2C SCL conflicts
#define SS_PIN          5   // VSPI Chip Select (SS)

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

// Function prototypes
void updateDisplay(const String& line1, const String& line2, const String& line3 = "");
String getUIDString(MFRC522::Uid* uid);

void setup() {
  // Initialize serial communications with the PC
  Serial.begin(115200);
  while (!Serial); // Wait for serial port to connect. Needed for native USB port only
  
  Serial.println("\n=================================================");
  Serial.println("   ESP32 RFID Attendance System - Validation");
  Serial.println("=================================================");

  // Initialize OLED Display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("[ERROR] SSD1306 allocation failed. Check I2C wiring."));
    while (true); // Don't proceed, loop forever
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  updateDisplay("System Init...", "Checking peripherals");
  delay(1000);

  // Initialize SPI bus
  SPI.begin();
  
  // Initialize MFRC522 reader
  mfrc522.PCD_Init();
  delay(4); // Optional delay according to some board requirements
  
  // Read and print MFRC522 details to Serial
  Serial.print(F("[INFO] MFRC522 Software Version: "));
  mfrc522.PCD_DumpVersionToSerial();
  
  // Validate reader version to verify communication
  byte version = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
  if (version == 0x00 || version == 0xFF) {
    Serial.println(F("[ERROR] Communication failure with MFRC522. Verify SPI wiring and power."));
    updateDisplay("Hardware Error", "RC522 Not Found", "Check SPI wiring");
    while (true); // Halt execution on hardware check failure
  }

  Serial.println(F("[SUCCESS] MFRC522 communication established."));
  updateDisplay("System Status", "RFID: OK", "Awaiting card scan...");
}

void loop() {
  // Reset the loop if no new card is present on the sensor/reader. This saves power when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Retrieve UID and format
  String cardUID = getUIDString(&(mfrc522.uid));
  
  // Print details to Serial Monitor
  Serial.print(F("[EVENT] Card Detected! UID: "));
  Serial.println(cardUID);
  
  // Display details on OLED
  updateDisplay("Card Detected!", "UID:", cardUID);

  // Halt PICC (Physical IC Card) and encryption
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  // Keep display message visible for a short duration
  delay(2000);
  
  // Reset view to standby
  updateDisplay("System Status", "RFID: OK", "Awaiting card scan...");
}

/**
 * Helper function to output standard layout text to the SSD1306 display.
 */
void updateDisplay(const String& line1, const String& line2, const String& line3) {
  display.clearDisplay();
  display.setCursor(0, 4);
  display.setTextSize(1);
  display.println(line1);
  
  display.setCursor(0, 24);
  display.setTextSize(1);
  display.println(line2);
  
  if (line3.length() > 0) {
    display.setCursor(0, 44);
    display.setTextSize(1);
    display.println(line3);
  }
  
  display.display();
}

/**
 * Converts MFRC522 UID bytes into a standard space-separated hex String.
 */
String getUIDString(MFRC522::Uid* uid) {
  String uidStr = "";
  for (byte i = 0; i < uid->size; i++) {
    if (uid->uidByte[i] < 0x10) {
      uidStr += "0";
    }
    uidStr += String(uid->uidByte[i], HEX);
    if (i < uid->size - 1) {
      uidStr += " ";
    }
  }
  uidStr.toUpperCase();
  return uidStr;
}