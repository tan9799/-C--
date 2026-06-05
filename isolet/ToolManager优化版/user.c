#include "user.h"

User* userHead = NULL;   // 全局变量定义（仅此一处）

void initDefaultUsers() {
    // 清空已有链表
    while (userHead) {
        User* tmp = userHead;
        userHead = userHead->next;
        free(tmp);
    }
    // 管理员
    User* admin = (User*)malloc(sizeof(User));
    strcpy(admin->username, "admin");
    strcpy(admin->password, "123456");
    admin->isAdmin = 1;
    admin->next = NULL;
    userHead = admin;
    // 普通用户
    User* user1 = (User*)malloc(sizeof(User));
    strcpy(user1->username, "zhangsan");
    strcpy(user1->password, "111111");
    user1->isAdmin = 0;
    user1->next = userHead;
    userHead = user1;
}

void saveUsersToFile() {
    FILE* fp = fopen("data/users.dat", "wb");
    if (!fp) return;
    User* p = userHead;
    while (p) {
        fwrite(p, sizeof(User), 1, fp);
        p = p->next;
    }
    fclose(fp);
}

void loadUsersFromFile() {
    // 先释放旧链表，避免重复
    while (userHead) {
        User* tmp = userHead;
        userHead = userHead->next;
        free(tmp);
    }
    FILE* fp = fopen("data/users.dat", "rb");
    if (!fp) {
        initDefaultUsers();
        saveUsersToFile();
        return;
    }
    User temp;
    while (fread(&temp, sizeof(User), 1, fp) == 1) {
        User* newNode = (User*)malloc(sizeof(User));
        *newNode = temp;
        newNode->next = userHead;
        userHead = newNode;
    }
    fclose(fp);
}

User* findUserByUsername(const char* username) {
    User* p = userHead;
    while (p) {
        if (strcmp(p->username, username) == 0) return p;
        p = p->next;
    }
    return NULL;
}

void listUsers() {
    if (!userHead) { printf("无用户数据。\n"); return; }
    printf("\n===== 用户列表 =====\n");
    User* p = userHead;
    while (p) {
        printf("用户名: %s, 角色: %s\n", p->username, p->isAdmin ? "管理员" : "普通用户");
        p = p->next;
    }
}

void addUser() {
    char username[MAX_USERNAME], password[MAX_PASSWORD];
    int role;
    printf("请输入新用户名: ");
    scanf("%s", username);
    clearInputBuffer();
    if (findUserByUsername(username) != NULL) {
        printf("用户名已存在！\n");
        return;
    }
    printf("请输入密码: ");
    scanf("%s", password);
    printf("请选择角色: 1-管理员 2-普通用户: ");
    role = getValidInt("", 1, 2);
    User* newUser = (User*)malloc(sizeof(User));
    strcpy(newUser->username, username);
    strcpy(newUser->password, password);
    newUser->isAdmin = (role == 1) ? 1 : 0;
    newUser->next = userHead;
    userHead = newUser;
    printf("用户添加成功！\n");
    saveUsersToFile();
}