#ifndef USER_H
#define USER_H

#include "common.h"

void initDefaultUsers();
void saveUsersToFile();
void loadUsersFromFile();
User* findUserByUsername(const char* username);
void listUsers();
void addUser();

#endif