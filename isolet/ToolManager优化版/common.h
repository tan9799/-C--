#ifndef COMMON_H
#define COMMON_H

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <direct.h>

#define MAX_NAME 50
#define MAX_TYPE 30
#define MAX_LOCATION 50
#define MAX_PERSON 30
#define DATE_LEN 11
#define MAX_HISTORY 2000
#define MAX_USERNAME 20
#define MAX_PASSWORD 20

#define TOOL_NORMAL 0
#define TOOL_DAMAGED 1

#define BORROW_STATUS_BORROWED 0
#define BORROW_STATUS_RETURNED 1

typedef struct Tool {
    int id;
    char name[MAX_NAME];
    char type[MAX_TYPE];
    int stock;
    int status;
    int borrowedCount;
    char location[MAX_LOCATION];
    float price;
    char history[MAX_HISTORY];
    struct Tool* next;
} Tool;

typedef struct BorrowRecord {
    int recordId;
    int toolId;
    char borrower[MAX_PERSON];
    char borrowDate[DATE_LEN];
    char dueDate[DATE_LEN];
    int quantity;
    int status;
    float fine;
    struct BorrowRecord* next;
} BorrowRecord;

typedef struct User {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    int isAdmin;
    struct User* next;
} User;

extern Tool* toolHead;
extern BorrowRecord* recordHead;
extern User* userHead;
extern int nextRecordId;
extern User* currentUser;

#endif