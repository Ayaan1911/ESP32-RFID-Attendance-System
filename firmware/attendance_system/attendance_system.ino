#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN   5
#define RST_PIN 22

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);

  while (!Serial);

  Serial.println();
  Serial.println("===== RC522 SPI Diagnostic =====");

  SPI.begin(18, 19, 23, 5);

  mfrc522.PCD_Init();

  delay(100);

  byte version = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);

  Serial.print("Version Register = 0x");
  Serial.println(version, HEX);

  if (version == 0x91)
    Serial.println("MFRC522 detected");
  else if (version == 0x92)
    Serial.println("MFRC522 v2 detected");
  else if (version == 0x88)
    Serial.println("Clone detected");
  else if (version == 0x00 || version == 0xFF)
    Serial.println("SPI communication FAILED");
  else
    Serial.println("Unknown response");
}

void loop() {
}