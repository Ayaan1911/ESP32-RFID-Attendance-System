#ifndef APPLICATION_CONTROLLER_H
#define APPLICATION_CONTROLLER_H

#include <Arduino.h>

enum SystemMode
{
    ATTENDANCE_MODE,
    ADMIN_MODE,
    REGISTRATION_MODE
};

extern SystemMode currentMode;

void appSetup();
void appLoop();
void processCard(const String &uid);

#endif
