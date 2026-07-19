#include "application_controller.h"

#include "attendance_manager.h"
#include "display_manager.h"
#include "rfid_service.h"
#include "storage_manager.h"
#include "user_manager.h"

SystemMode currentMode = ATTENDANCE_MODE;
static const String ADMIN_UID = "3D 87 D0 06";

struct AttendanceSession
{
    bool active;
};

static AttendanceSession session =
{
    false
};

static const int MAX_USERS = 10;

static User users[MAX_USERS] =
{
    {"DB 1D 29 07", "Ayaan", false, true},
    {"A1 B2 C3 D4", "Demo User", false, true}
};

static int registeredUsers = 2;

static void initSerial();
static bool isAdminCard(const String &uid);

void appSetup()
{
    initSerial();

    initStorage(users, registeredUsers);
  
    initOLED();

    showBootScreen();
    delay(2000);

    initRFID();

    showReadyScreen();
}

void appLoop()
{
    processRFID();
}

void processCard(const String &uid)
{
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
        registerCard(users, registeredUsers, MAX_USERS, uid);

        currentMode = ATTENDANCE_MODE;
    }
    else
    {
        markAttendance(users, registeredUsers, uid);
    }
}

static void initSerial()
{
    Serial.begin(115200);
    while (!Serial);
    Serial.println("========================================");
    Serial.println("ESP32 RFID Attendance System");
    Serial.println("Boot Sequence");
    Serial.println("========================================");
}

static bool isAdminCard(const String &uid)
{
    return uid == ADMIN_UID;
}
