#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <Arduino.h>

struct User
{
    String uid;
    String name;
    bool attendanceMarked;
    bool registered;
};

void registerCard(User *users, int &registeredUsers, int maxUsers, const String &uid);
User* findUserByUID(User *users, int registeredUsers, const String &uid);
String readNameFromSerial();

#endif
