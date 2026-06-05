#include "auth.h"
#include "user.h"
#include "utils.h"

int login() {
    char username[MAX_USERNAME], password[MAX_PASSWORD];
    int attempts = 0;
    while (attempts < 3) {
        printf("用户名: ");
        scanf("%s", username);
        printf("密码: ");
        scanf("%s", password);
        clearInputBuffer();

        User* u = findUserByUsername(username);
        if (u && strcmp(u->password, password) == 0) {
            currentUser = u;
            printf("登录成功！欢迎 %s (%s)\n", u->username, u->isAdmin ? "管理员" : "普通用户");
            return 1;
        }
        else {
            attempts++;
            printf("用户名或密码错误，剩余尝试次数: %d\n", 3 - attempts);
        }
    }
    printf("登录失败次数过多，程序退出。\n");
    return 0;
}