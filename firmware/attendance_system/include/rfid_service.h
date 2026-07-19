#ifndef RFID_SERVICE_H
#define RFID_SERVICE_H

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

extern MFRC522 mfrc522;

void initRFID();
void processRFID();
bool isCardPresent();
String getCardUID();
void handleCard(const String &uid);

#endif
