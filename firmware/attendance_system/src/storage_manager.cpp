#include "storage_manager.h"

#include <Preferences.h>

static Preferences preferences;

void initStorage(User *users, int &registeredUsers)
{
    preferences.begin("attendance", false);

    Serial.println("[OK] Preferences Storage Initialized");

    loadUsers(users, registeredUsers);
}

void saveUsers(const User *users, int registeredUsers)
{
    preferences.putInt("userCount", registeredUsers);

    for (int i = 0; i < registeredUsers; i++)
    {
        preferences.putString(("uid" + String(i)).c_str(), users[i].uid);
        preferences.putString(("name" + String(i)).c_str(), users[i].name);
        preferences.putBool(("attendance" + String(i)).c_str(), users[i].attendanceMarked);
        preferences.putBool(("registered" + String(i)).c_str(), users[i].registered);
    }

    Serial.println("[OK] Users saved to flash.");
}

void loadUsers(User *users, int &registeredUsers)
{
    registeredUsers = preferences.getInt("userCount", 2);

    for (int i = 0; i < registeredUsers; i++)
    {
        users[i].uid = preferences.getString(("uid" + String(i)).c_str(), "");
        users[i].name = preferences.getString(("name" + String(i)).c_str(), "");
        users[i].attendanceMarked = preferences.getBool(("attendance" + String(i)).c_str(), false);
        users[i].registered = preferences.getBool(("registered" + String(i)).c_str(), false);
    }

    Serial.print("[OK] Loaded ");
    Serial.print(registeredUsers);
    Serial.println(" users from flash.");
}
