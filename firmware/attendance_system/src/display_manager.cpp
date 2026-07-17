#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "display_manager.h"

// OLED Display Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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
