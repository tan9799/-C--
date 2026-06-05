#ifndef UTILS_H
#define UTILS_H

#include "common.h"

void clearInputBuffer();
int getValidInt(const char* prompt, int min, int max);
float getValidFloat(const char* prompt, float min);
int isDateValid(const char* date);
int daysBetween(const char* date1, const char* date2);
void pressAnyKey();

#endif