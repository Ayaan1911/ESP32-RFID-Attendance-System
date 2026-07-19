#include "user_manager.h"

#include "display_manager.h"
#include "storage_manager.h"

void registerCard(User *users, int &registeredUsers, int maxUsers, const String &uid)
{
    if (registeredUsers >= maxUsers)
    {
        updateDisplay("Database Full", "Full", "");

        Serial.println("[ERROR] User database is full.");

        delay(2000);

        showReadyScreen();

        return;
    }

    // Check if this RFID card is already registered
    User *existingUser = findUserByUID(users, registeredUsers, uid);

    if (existingUser != nullptr)
    {
    updateDisplay("Already Registered", existingUser->name);

    Serial.print("[ERROR] Card already belongs to: ");
    Serial.println(existingUser->name);

    delay(2000);

    showReadyScreen();

    return;
  }

    String name = readNameFromSerial();

    users[registeredUsers].uid = uid;
    users[registeredUsers].name = name;
    users[registeredUsers].attendanceMarked = false;
    users[registeredUsers].registered = true;

    registeredUsers++;

    saveUsers(users, registeredUsers);

    updateDisplay("User Saved", name);

    Serial.print("[SUCCESS] Registered: ");
    Serial.println(name);

    delay(2000);

    showReadyScreen();
}

User* findUserByUID(User *users, int registeredUsers, const String &uid)
{
    for (int i = 0; i < registeredUsers; i++)
    {
        if (users[i].uid == uid)
        {
            return &users[i];
        }
    }

    return nullptr;
}

String readNameFromSerial()
{
    Serial.println();
    Serial.println("=================================");
    Serial.println("Enter User Name:");
    Serial.println("=================================");

    while (!Serial.available())
    {
        delay(10);
    }

    String name = Serial.readStringUntil('\n');
    name.trim();

    return name;
}
