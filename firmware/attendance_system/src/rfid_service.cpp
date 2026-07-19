#include "rfid_service.h"

#include "application_controller.h"
#include "display_manager.h"

// MFRC522 Configuration
#define RST_PIN         4
#define SS_PIN          5
MFRC522 mfrc522(SS_PIN, RST_PIN);

void processRFID() {
  if (isCardPresent()) {
    String cardUID = getCardUID();
    if (cardUID.length() > 0) {
      handleCard(cardUID);
    }
  }
}

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

void handleCard(const String &uid)
{
    processCard(uid);
}

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
