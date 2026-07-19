#include "attendance_manager.h"

#include "display_manager.h"

void markAttendance(User *users, int registeredUsers, const String &uid)
{
  if (currentMode == ADMIN_MODE){
    updateDisplay("ADMIN MODE", "", "Coming Soon");

    Serial.println("[ADMIN] Registration module not implemented yet.");

    delay(2000);

    showReadyScreen();

    currentMode = ATTENDANCE_MODE;

    return;
    }

    User *user = findUserByUID(users, registeredUsers, uid);

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
