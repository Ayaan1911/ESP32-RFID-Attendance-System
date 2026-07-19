#ifndef ATTENDANCE_MANAGER_H
#define ATTENDANCE_MANAGER_H

#include <Arduino.h>

#include "application_controller.h"
#include "user_manager.h"

void markAttendance(User *users, int registeredUsers, const String &uid);

#endif
