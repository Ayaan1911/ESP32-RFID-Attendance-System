#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <Arduino.h>

#include "user_manager.h"

void initStorage(User *users, int &registeredUsers);
void saveUsers(const User *users, int registeredUsers);
void loadUsers(User *users, int &registeredUsers);

#endif
