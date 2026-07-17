#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>

void initOLED();
void showBootScreen();
void showReadyScreen();
void updateDisplay(const String& line1, const String& line2, const String& line3 = "");

#endif
