#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

#define STU_NUM 50  
#define COURSE_NUM 10
#define NAME_LEN 10
#define POS_X1 35
#define POS_X2 40
#define POS_X3 50
#define POS_X4 65
#define POS_Y 3
// 用户角色枚举
#define ROLE_TEACHER 0
#define ROLE_STUDENT 1

// 用户节点
typedef struct user {
    char username[20];
    char password[20];
    int role;               // 0:教师, 1:学生
    long student_id;        // 学生关联的学号（教师为0）
    struct user* next;
} USER;

// 全局变量声明（实际定义在 student.c）
extern USER* user_head;     // 用户链表头
extern USER* current_user;  // 当前登录用户

typedef struct student {
    long num;                 // 学号
    char name[NAME_LEN];      // 姓名
    float score[COURSE_NUM];  // 各科成绩
    float sum;                // 总分
    float aver;               // 平均分
    struct student* next;     // 链表指针
} STU;

// 全局变量声明（实际定义在 main.c）
extern STU* head;             // 链表头指针
extern int n;                 // 学生总数
extern int m;                 // 课程门数

// 函数声明
int Menu(void);
void SetPosition(int x, int y);
void InputRecord(void);
void AppendRecord(void);
void DeleteRecord(void);
void SearchByNumber(void);
void SearchByName(void);
void ModifyRecord(void);
void CalculateScoreOfStudent(void);
void CalculateScoreOfCourse(void);
void SortbyNum(void);
void SortbyName(void);
void SortbyScore(int (*compare)(float a, float b));
int Ascending(float a, float b);
int Descending(float a, float b);
void StatisticAnalysis(void);
void PrintRecord(void);
void WritetoFile(void);
int ReadfromFile(int* first);
void InitUsers(void);
void Login(void);
int CheckPermission(int required_role, long target_id);
void WriteUsersToFile(void);   // 可选
void ReadUsersFromFile(void);  // 可选
// 用户管理函数
void InitUsers(void);
void Login(void);
int CheckPermission(int required_role, long target_id);
void WriteUsersToFile(void);
void ReadUsersFromFile(void);
void AutoCreateUserForStudent(long student_id, const char* name);   // 新增
#endif