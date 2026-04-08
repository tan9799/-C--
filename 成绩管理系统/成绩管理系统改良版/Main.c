#include "student.h"
#include <direct.h>   // for _mkdir
#include <errno.h>    // for errno

STU* head = NULL;   // 链表头指针
USER* user_head = NULL;
USER* current_user = NULL;
int n = 0;          // 学生人数
int m = 0;          // 课程门数

// 释放链表内存
void FreeList(void) {
    STU* p = head;
    while (p) {
        STU* tmp = p;
        p = p->next;
        free(tmp);
    }
    head = NULL;
    n = 0;
}
// ========== 用户管理函数 ==========
void InitUsers(void) {
    // 教师账号
    USER* t = (USER*)malloc(sizeof(USER));
    if (!t) return;
    strcpy(t->username, "teacher");
    strcpy(t->password, "123");
    t->role = ROLE_TEACHER;
    t->student_id = 0;
    t->next = user_head;
    user_head = t;

    // 学生账号（学号需要与实际录入的学生匹配，此处示例）
    USER* s1 = (USER*)malloc(sizeof(USER));
    if (s1) {
        strcpy(s1->username, "zhangsan");
        strcpy(s1->password, "123");
        s1->role = ROLE_STUDENT;
        s1->student_id = 2021001;   // 与张三学号一致
        s1->next = user_head;
        user_head = s1;
    }

    USER* s2 = (USER*)malloc(sizeof(USER));
    if (s2) {
        strcpy(s2->username, "lisi");
        strcpy(s2->password, "123");
        s2->role = ROLE_STUDENT;
        s2->student_id = 2021002;
        s2->next = user_head;
        user_head = s2;
    }
}

void AutoCreateUserForStudent(long student_id, const char* name) {
    // 检查是否已存在该学号的账号
    USER* u = user_head;
    while (u) {
        if (u->student_id == student_id) return; // 已存在，不重复创建
        u = u->next;
    }

    USER* newuser = (USER*)malloc(sizeof(USER));
    if (!newuser) return;
    // 用户名使用学号（字符串形式）
    sprintf(newuser->username, "%ld", student_id);
    // 密码默认123
    strcpy(newuser->password, "123");
    newuser->role = ROLE_STUDENT;
    newuser->student_id = student_id;
    newuser->next = user_head;
    user_head = newuser;
}

void Login(void) {
    char name[20], pwd[20];
    system("cls");
    SetPosition(40, 10);
    printf("===== 学生成绩管理系统登录 =====\n");
    SetPosition(40, 12);
    printf("用户名: ");
    scanf("%s", name);
    SetPosition(40, 13);
    printf("密码: ");
    scanf("%s", pwd);

    USER* u = user_head;
    while (u) {
        if (strcmp(u->username, name) == 0 && strcmp(u->password, pwd) == 0) {
            current_user = u;
            SetPosition(40, 15);
            printf("登录成功！角色：%s\n", u->role == ROLE_TEACHER ? "教师" : "学生");
            system("pause");
            return;
        }
        u = u->next;
    }
    SetPosition(40, 15);
    printf("用户名或密码错误！程序退出。\n");
    system("pause");
    exit(0);
}

int CheckPermission(int required_role, long target_id) {
    if (current_user == NULL) return 0;
    if (current_user->role == ROLE_TEACHER) return 1;
    if (required_role == ROLE_STUDENT && current_user->student_id == target_id) return 1;
    return 0;
}

// 以下两个函数为可选（持久化用户信息）
void WriteUsersToFile(void) {
    FILE* fp = fopen("users.txt", "w");
    if (!fp) return;
    USER* u = user_head;
    while (u) {
        fprintf(fp, "%s %s %d %ld\n", u->username, u->password, u->role, u->student_id);
        u = u->next;
    }
    fclose(fp);
}

void ReadUsersFromFile(void) {
    FILE* fp = fopen("users.txt", "r");
    if (!fp) {
        InitUsers();
        return;
    }
    // 释放原有用户链表
    while (user_head) {
        USER* tmp = user_head;
        user_head = user_head->next;
        free(tmp);
    }
    char name[20], pwd[20];
    int role;
    long sid;
    while (fscanf(fp, "%s %s %d %ld", name, pwd, &role, &sid) == 4) {
        USER* u = (USER*)malloc(sizeof(USER));
        if (!u) continue;
        strcpy(u->username, name);
        strcpy(u->password, pwd);
        u->role = role;
        u->student_id = sid;
        u->next = user_head;
        user_head = u;
    }
    fclose(fp);
}
// ========== 用户管理函数结束 ==========

// 创建新节点
STU* CreateNode(void) {
    STU* p = (STU*)malloc(sizeof(STU));
    if (!p) {
        printf("内存分配失败！\n");
        exit(1);
    }
    p->next = NULL;
    return p;
}

// 插入节点（末尾）
void InsertNode(STU* node) {
    if (!head) {
        head = node;
    }
    else {
        STU* p = head;
        while (p->next) p = p->next;
        p->next = node;
    }
    n++;
}

// 删除节点（按学号）
int DeleteNode(long id) {
    STU* p = head, * prev = NULL;
    while (p) {
        if (p->num == id) {
            if (prev)
                prev->next = p->next;
            else
                head = p->next;
            free(p);
            n--;
            return 1;
        }
        prev = p;
        p = p->next;
    }
    return 0;
}

// 按学号查找节点
STU* FindNodeByNum(long id) {
    STU* p = head;
    while (p) {
        if (p->num == id) return p;
        p = p->next;
    }
    return NULL;
}

// 按姓名查找（返回第一个匹配的节点，用于修改等；查询全部使用 SearchByName）
STU* FindNodeByName(const char* name) {
    STU* p = head;
    while (p) {
        if (strcmp(p->name, name) == 0) return p;
        p = p->next;
    }
    return NULL;
}

// 计算所有学生的总分和平均分
void CalculateAllScores(void) {
    STU* p = head;
    while (p) {
        p->sum = 0;
        for (int j = 0; j < m; j++)
            p->sum += p->score[j];
        p->aver = p->sum / m;
        p = p->next;
    }
}

// ---------- 以下为原有功能函数，改用链表实现 ----------

int Menu(void) {
    int posy = 5;
    int option;
    SetPosition(POS_X3, posy);
    printf("学生成绩管理系统\n");
    for (int i = 0; i < 2; i++) {
        SetPosition(POS_X1, ++posy);
        for (int j = 0; j < 55; j++) printf("-");
    }
    SetPosition(POS_X1, ++posy);
    printf("1.输入学生信息");
    SetPosition(POS_X4, posy);
    printf("2.增加学生成绩");
    SetPosition(POS_X1, posy += 2);
    printf("3.删除学生信息");
    SetPosition(POS_X4, posy);
    printf("4.按学号查找记录");
    SetPosition(POS_X1, posy += 2);
    printf("5.按姓名查找记录");
    SetPosition(POS_X4, posy);
    printf("6.修改学生信息");
    SetPosition(POS_X1, posy += 2);
    printf("7.计算学生成绩");
    SetPosition(POS_X4, posy);
    printf("8.计算课程成绩");
    SetPosition(POS_X1, posy += 2);
    printf("9.按学号排序");
    SetPosition(POS_X4, posy);
    printf("10.按姓名排序");
    SetPosition(POS_X1, posy += 2);
    printf("11.按总成绩降序排序");
    SetPosition(POS_X4, posy);
    printf("12.按总成绩升序排序");
    SetPosition(POS_X1, posy += 2);
    printf("13.学生成绩统计");
    SetPosition(POS_X4, posy);
    printf("14.打印学生记录");
    SetPosition(POS_X1, posy += 2);
    printf("15.学生记录存盘");
    SetPosition(POS_X4, posy);
    printf("16.从磁盘读取学生记录");
    SetPosition(POS_X1, posy += 2);
    // 在原有0退出之后，添加：
    SetPosition(POS_X1, ++posy);
    printf("17. 切换账号");
    // 记得调整循环和选项范围（0~17）
    printf("0.退出");
    for (int i = 0; i < 2; i++) {
        SetPosition(POS_X1, ++posy);
        for (int j = 0; j < 55; j++) printf("-");
    }
    SetPosition(POS_X1, ++posy);
    printf("请选择你想要进行的操作[0~17]: [  ]\b\b\b");
    scanf("%d", &option);
    return option;
}

void SetPosition(int x, int y) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { x, y };
    SetConsoleCursorPosition(hOut, pos);
}

void InputRecord(void) {
    if (current_user->role != ROLE_TEACHER) {
        printf("权限不足！此功能仅限教师使用。\n");
        return;
    }
    int i, j;
    int stu_count;               // 用局部变量接收输入的人数
    int posy = 6;

    // 清空缓冲区（可选，但推荐）
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    SetPosition(POS_X2, posy);
    printf("输入学生人数(1~%d):", STU_NUM);
    scanf("%d", &stu_count);
    while (stu_count < 1 || stu_count > STU_NUM) {
        while ((c = getchar()) != '\n' && c != EOF);
        SetPosition(POS_X2, posy);
        printf("输入错误！请重新输入学生人数(1~%d):", STU_NUM);
        scanf("%d", &stu_count);
    }

    SetPosition(POS_X2, posy += 2);
    printf("输入课程数(1~%d):", COURSE_NUM);
    scanf("%d", &m);
    while (m < 1 || m > COURSE_NUM) {
        while ((c = getchar()) != '\n' && c != EOF);
        SetPosition(POS_X2, posy);
        printf("输入错误！请重新输入课程数(1~%d):", COURSE_NUM);
        scanf("%d", &m);
    }

    // 清空缓冲区
    while ((c = getchar()) != '\n' && c != EOF);

    // 输出分隔线（原代码中的两行横线）
    for (i = 0; i < 2; i++) {
        SetPosition(POS_X1, ++posy);
        for (j = 0; j < 55; j++) printf("-");
    }

    SetPosition(POS_X2, ++posy);
    printf("请依次输入学号、姓名和 %d 门课的成绩（每行一个学生）：", m);

    n = 0;   // 🔧 关键：先清零，后面 InsertNode 会正确增加
    for (i = 0; i < stu_count; i++) {
        STU* p = CreateNode();
        SetPosition(POS_X2, ++posy);
        printf("第 %d 个学生: ", i + 1);
        scanf("%ld%s", &p->num, p->name);
        for (j = 0; j < m; j++)
            scanf("%f", &p->score[j]);
        InsertNode(p);   // 内部会执行 n++
        AutoCreateUserForStudent(p->num, p->name);
    }
    CalculateAllScores();
    WritetoFile();
    printf("\n输入完成！按任意键返回菜单...");
    getch();
}

void AppendRecord(void) {
    int i, j, num_record;
    printf("请输入需要增加的学生记录条数：");
    scanf("%d", &num_record);
    while (n + num_record > STU_NUM) {
        printf("要增加的记录条数太多，请重新输入：");
        scanf("%d", &num_record);
    }
    int base = n;  // 记录增加前的学生数
    for (i = 0; i < num_record; i++) {
        STU* p = CreateNode();
        printf("输入第%d个学生信息:\t", base + i + 1);  // 使用 base
        scanf("%ld%s", &p->num, p->name);
        for (j = 0; j < m; j++)
            scanf("%f", &p->score[j]);
        InsertNode(p);  // 这里 n++ 会正确增加总人数
    }
    CalculateAllScores();
    WritetoFile();
    printf("添加完毕！\n");
}

void DeleteRecord(void) {
    if (current_user->role != ROLE_TEACHER) {
        printf("权限不足！此功能仅限教师使用。\n");
        return;
    }
    long id;
    char ch;
    printf("请输入你要删除学生信息对应的学号：");
    scanf("%ld", &id);
    STU* p = FindNodeByNum(id);
    if (!p) {
        printf("没有找到该生记录!\n");
        return;
    }
    printf("找到了该学生的记录，信息为：\n");
    printf("%10ld%15s", p->num, p->name);
    for (int j = 0; j < m; j++)
        printf("%10.2f", p->score[j]);
    printf("%10.2f%10.2f\n", p->sum, p->aver);
    printf("请确认是否要删除这条记录？(Y/y, N/n):");
    getchar();
    scanf("%c", &ch);
    if (ch == 'Y' || ch == 'y') {
        if (DeleteNode(id))
            printf("删除完毕\n");
        else
            printf("删除失败\n");
    }
    else if (ch == 'N' || ch == 'n') {
        printf("找到了这个学生的记录，但不删除\n");
    }
    else {
        printf("输入出错\n");
    }
}

void SearchByNumber(void) {
    long id;
    printf("请输入你要查找的学生的学号：");
    scanf("%ld", &id);
    if (!CheckPermission(ROLE_STUDENT, id)) {
        printf("权限不足！您只能查看自己的成绩。\n");
        return;
    }
    STU* p = FindNodeByNum(id);
    if (p) {
        printf("找到了，该学号对应的学生信息为：\n");
        printf("%10ld%15s", p->num, p->name);
        for (int j = 0; j < m; j++)
            printf("%10.2f", p->score[j]);
        printf("%10.2f%10.2f\n", p->sum, p->aver);
    }
    else {
        printf("没有找到这个学号对应的学生记录\n");
    }
}

void SearchByName(void) {
    int flag = 1;
    char name[NAME_LEN];
    printf("请输入你要查找的学生的姓名：");
    scanf("%s", name);

    // 先找出该姓名对应的学号（取第一个匹配）
    STU* tmp = head;
    long target_id = -1;
    while (tmp) {
        if (strcmp(tmp->name, name) == 0) {
            target_id = tmp->num;
            break;
        }
        tmp = tmp->next;
    }
    if (target_id == -1) {
        printf("没有找到这个姓名的学生记录\n");
        return;
    }
    if (!CheckPermission(ROLE_STUDENT, target_id)) {
        printf("权限不足！您只能查找自己的信息。\n");
        return;
    }

    // 正式查找并打印（仅打印与 target_id 匹配的记录）
    STU* p = head;
    while (p) {
        if (strcmp(p->name, name) == 0 && p->num == target_id) {
            printf("找到了，学生信息为：\n");
            printf("%10ld%15s", p->num, p->name);
            for (int j = 0; j < m; j++)
                printf("%10.2f", p->score[j]);
            printf("%10.2f%10.2f\n", p->sum, p->aver);
            flag = 0;
            break;
        }
        p = p->next;
    }
    if (flag)
        printf("没有找到这个姓名对应的学生记录（或无权查看）\n");
}

void ModifyRecord(void) {
    if (current_user->role != ROLE_TEACHER) {
        printf("权限不足！此功能仅限教师使用。\n");
        return;
    }
    long id;
    char ch;
    printf("请输入需要修改信息对应的学号:");
    scanf("%ld", &id);
    STU* p = FindNodeByNum(id);
    if (!p) {
        printf("没有找到这个学生的记录!\n");
        return;
    }
    printf("找到了该学生的记录，信息为:\n");
    printf("%10ld%15s", p->num, p->name);
    for (int j = 0; j < m; j++)
        printf("%10.2f", p->score[j]);
    printf("%10.2f%10.2f\n", p->sum, p->aver);
    printf("请确认是否需要修改？(Y/N或y/n):");
    getchar();
    scanf("%c", &ch);
    if (ch == 'Y' || ch == 'y') {
        printf("请输入要修改的学生信息:");
        scanf("%ld%s", &p->num, p->name);
        p->sum = 0;
        for (int j = 0; j < m; j++) {
            scanf("%f", &p->score[j]);
            p->sum += p->score[j];
        }
        p->aver = p->sum / m;
        printf("修改完毕\n");
    }
    else if (ch == 'N' || ch == 'n') {
        printf("找到了该学生记录，但不修改\n");
    }
    else {
        printf("输入出错！\n");
    }
}

void CalculateScoreOfStudent(void) {
    if (current_user->role != ROLE_TEACHER) {
        printf("权限不足！此功能仅限教师使用。\n");
        return;
    }
    printf("每个学生各门课程的总分和平均分为：\n");
    STU* p = head;
    int i = 1;
    while (p) {
        printf("第%d个学生：总分=%.2f, 平均分=%.2f\n", i++, p->sum, p->aver);
        p = p->next;
    }
}

void CalculateScoreOfCourse(void) {
    if (current_user->role != ROLE_TEACHER) {
        printf("权限不足！此功能仅限教师使用。\n");
        return;
    }
    float sum[COURSE_NUM] = { 0 }, aver[COURSE_NUM];
    int posy = 7;
    SetPosition(POS_X1, posy);
    printf("各门课程总分和平均分的计算结果为：");
    STU* p = head;
    while (p) {
        for (int j = 0; j < m; j++)
            sum[j] += p->score[j];
        p = p->next;
    }
    for (int j = 0; j < m; j++) {
        aver[j] = sum[j] / n;
        SetPosition(POS_X1, ++posy);
        printf("第%d门课程: 总分 = %.2f, 平均分 = %.2f\n", j + 1, sum[j], aver[j]);
    }
}

// 辅助函数：将链表转换为数组（用于排序）
STU* ListToArray(void) {
    STU* arr = (STU*)malloc(n * sizeof(STU));
    if (!arr) return NULL;
    STU* p = head;
    int i = 0;
    while (p) {
        arr[i] = *p;   // 浅拷贝，注意指针成员（next）会被复制，但后续会丢弃
        arr[i].next = NULL;  // 避免残留指针
        p = p->next;
        i++;
    }
    return arr;
}

// 辅助函数：用排序后的数组重建链表
void RebuildFromArray(STU* arr, int len) {
    FreeList();  // 释放原链表
    for (int i = 0; i < len; i++) {
        STU* node = CreateNode();
        *node = arr[i];   // 拷贝内容
        node->next = NULL;
        InsertNode(node);
    }
    free(arr);
}

void SortbyNum(void) {
    if (current_user->role != ROLE_TEACHER) {
        printf("权限不足！此功能仅限教师使用。\n");
        return;
    }
    if (head == NULL || n == 0) {
        printf("没有学生记录，无法排序！\n");
        system("pause");
        return;
    }
    STU* arr = ListToArray();
    if (!arr) return;
    // 选择排序（升序）
    for (int i = 0; i < n - 1; i++) {
        int k = i;
        for (int j = i + 1; j < n; j++)
            if (arr[j].num < arr[k].num) k = j;
        if (k != i) {
            STU tmp = arr[i];
            arr[i] = arr[k];
            arr[k] = tmp;
        }
    }
    RebuildFromArray(arr, n);
    printf("按学号从小到大对学生记录排序完毕！\n");
    printf("排序后的学生记录如下：\n");
    PrintRecord();   // 新增：显示排序结果
    // 注意：PrintRecord 内部没有暂停，需要在此处暂停让用户查看
    system("pause");
}

void SortbyName(void) {
    if (current_user->role != ROLE_TEACHER) {
        printf("权限不足！此功能仅限教师使用。\n");
        return;
    }
    if (head == NULL || n == 0) {
        printf("没有学生记录，无法排序！\n");
        system("pause");
        return;
    }
    STU* arr = ListToArray();
    if (!arr) return;
    // 冒泡排序（按姓名升序）
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (strcmp(arr[j].name, arr[j + 1].name) > 0) {
                STU tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
    RebuildFromArray(arr, n);
    printf("按姓名字典序对学生记录排序完毕！\n");
    printf("排序后的学生记录如下：\n");
    PrintRecord();
    system("pause");
}

void SortbyScore(int (*compare)(float a, float b)) {
    if (current_user->role != ROLE_TEACHER) {
        printf("权限不足！此功能仅限教师使用。\n");
        return;
    }
    if (head == NULL || n == 0) {
        printf("没有学生记录，无法排序！\n");
        system("pause");
        return;
    }
    STU* arr = ListToArray();
    if (!arr) return;
    for (int i = 0; i < n - 1; i++) {
        int k = i;
        for (int j = i + 1; j < n; j++) {
            if ((*compare)(arr[j].sum, arr[k].sum))
                k = j;
        }
        if (k != i) {
            STU tmp = arr[i];
            arr[i] = arr[k];
            arr[k] = tmp;
        }
    }
    RebuildFromArray(arr, n);
    printf("按学生总成绩排序完毕！\n");
    printf("排序后的学生记录如下：\n");
    PrintRecord();
    system("pause");
}

int Descending(float a, float b) { return a > b; }
int Ascending(float a, float b) { return a < b; }

void StatisticAnalysis(void) {
    if (current_user->role != ROLE_TEACHER) {
        printf("权限不足！此功能仅限教师使用。\n");
        return;
    }
    int i, j, t[6];
    STU* p = head;

    for (j = 0; j < m; j++) {
        printf("\n课程%d成绩统计结果为：\n", j + 1);
        printf("分数段\t人数\t占比\n");
        memset(t, 0, sizeof(t));
        p = head;
        while (p) {
            float s = p->score[j];
            if (s >= 0 && s < 60) t[0]++;
            else if (s < 70) t[1]++;
            else if (s < 80) t[2]++;
            else if (s < 90) t[3]++;
            else if (s < 100) t[4]++;
            else if (s == 100) t[5]++;
            p = p->next;
        }
        for (i = 0; i < 6; i++) {
            if (i == 0)
                printf("<60\t%d\t%.2f%%\n", t[i], (float)t[i] / n * 100);
            else if (i == 5)
                printf("100\t%d\t%.2f%%\n", t[i], (float)t[i] / n * 100);
            else
                printf("%d-%d\t%d\t%.2f%%\n", (i + 5) * 10, (i + 5) * 10 + 9, t[i], (float)t[i] / n * 100);
        }
    }
    printf("\n学生成绩平均分统计结果为：\n");
    printf("分数段\t人数\t占比\n");
    memset(t, 0, sizeof(t));
    p = head;
    while (p) {
        float a = p->aver;
        if (a >= 0 && a < 60) t[0]++;
        else if (a < 70) t[1]++;
        else if (a < 80) t[2]++;
        else if (a < 90) t[3]++;
        else if (a < 100) t[4]++;
        else if (a == 100) t[5]++;
        p = p->next;
    }
    for (i = 0; i < 6; i++) {
        if (i == 0)
            printf("<60\t%d\t%.2f%%\n", t[i], (float)t[i] / n * 100);
        else if (i == 5)
            printf("100\t%d\t%.2f%%\n", t[i], (float)t[i] / n * 100);
        else
            printf("%d-%d\t%d\t%.2f%%\n", (i + 5) * 10, (i + 5) * 10 + 9, t[i], (float)t[i] / n * 100);
    }
}

void PrintRecord(void) {
    if (current_user->role != ROLE_TEACHER) {
        printf("权限不足！此功能仅限教师使用。\n");
        return;
    }
    printf("学号\t\t姓名\t\t");
    for (int j = 0; j < m; j++)
        printf("课程%d\t\t", j + 1);
    printf("总分\t\t平均分\n");
    STU* p = head;
    while (p) {
        printf("%-16ld%-16s", p->num, p->name);
        for (int j = 0; j < m; j++)
            printf("%-16.1lf", p->score[j]);
        printf("%-16.1lf%-16.1lf\n", p->sum, p->aver);
        p = p->next;
    }
}

void WritetoFile(void) {
    if (current_user->role != ROLE_TEACHER) {
        printf("权限不足！此功能仅限教师使用。\n");
        return;
    }
    FILE* fp;
    // 使用相对路径，在当前程序所在目录下创建 data 文件夹
    char dir[] = ".\\data";
    char filepath[] = ".\\data\\student.txt";

    // 尝试创建目录（Windows 下使用 _mkdir）
#include <direct.h>   // 确保文件顶部已包含此头文件
    if (_mkdir(dir) != 0 && errno != EEXIST) {
        printf("无法创建目录 %s，请检查权限！\n", dir);
        return;   // 不再 exit(0)，而是返回
    }

    if ((fp = fopen(filepath, "w")) == NULL) {
        printf("文件 %s 无法打开！存盘失败。\n", filepath);
        return;   // 不再 exit(0)
    }

    fprintf(fp, "%10d%10d\n", n, m);
    STU* p = head;
    while (p) {
        fprintf(fp, "%10ld%10s\n", p->num, p->name);
        for (int j = 0; j < m; j++)
            fprintf(fp, "%10.1f", p->score[j]);
        fprintf(fp, "%10.1f%10.1f\n", p->sum, p->aver);
        p = p->next;
    }
    fclose(fp);
    printf("存盘完毕！数据已保存至 %s\n", filepath);
}

int ReadfromFile(int* first) {
    FILE* fp;
    int i, j;
    char filepath[] = ".\\data\\student.txt";
    if ((fp = fopen(filepath, "r")) == NULL) {
        printf("磁盘文件 %s 无法打开\n", filepath);
        return 1;
    }
    fscanf(fp, "%10d%10d", &n, &m);
    FreeList();  // 清空当前链表
    for (i = 0; i < n; i++) {
        STU* p = CreateNode();
        fscanf(fp, "%10ld", &p->num);
        fscanf(fp, "%10s", p->name);
        for (j = 0; j < m; j++)
            fscanf(fp, "%10f", &p->score[j]);
        fscanf(fp, "%10f%10f", &p->sum, &p->aver);
        InsertNode(p);
        AutoCreateUserForStudent(p->num, p->name);
    }
    *first = 0;
    fclose(fp);
    printf("数据从磁盘读取完毕！");
    return 0;
}

// ---------- 主函数 ----------
int main() {
    int first = 1;  // 1表示尚未录入数据
    int ch;
    system("mode con cols=130 lines=60");
    system("color 0E");
    ReadUsersFromFile();   // 尝试从文件读取，若无则 InitUsers
    Login();
    if (ReadfromFile(&first) != 0) {
        // 文件不存在或读取失败，first保持为1
        first = 1;
    }
    while (1) {
        system("cls");
        ch = Menu();
        switch (ch) {
        case 1:
            system("cls");
            if (current_user->role != ROLE_TEACHER) {
                printf("权限不足！\n");
                system("pause");
                break;
            }
            InputRecord();
            first = 0;
            break;
        case 2:
            system("cls");
            if (first) {
                printf("系统中尚无学生成绩信息，请先输入！\n");
                system("pause");
                break;
            }
            if (current_user->role != ROLE_TEACHER) {
                printf("权限不足！\n");
                system("pause");
                break;
            }
            AppendRecord();
            system("pause");
            break;
        case 3:
            system("cls");
            if (first) {
                printf("系统中尚无学生成绩信息，请先输入！\n");
                system("pause");
                break;
            }
            if (current_user->role != ROLE_TEACHER) {
                printf("权限不足！只有教师可以删除学生信息。\n");
                system("pause");
                break;
            }
            DeleteRecord();
            system("pause");
            break;
        case 4:
            system("cls");
            if (first) {
                printf("系统中尚无学生成绩信息，请先输入！\n");
                system("pause");
                break;
            }
            SearchByNumber();
            system("pause");
            break;
        case 5:
            system("cls");
            if (first) {
                printf("系统中尚无学生成绩信息，请先输入！\n");
                system("pause");
                break;
            }
            SearchByName();
            system("pause");
            break;
        case 6:
            system("cls");
            if (first) {
                printf("系统中尚无学生成绩信息，请先输入！\n");
                system("pause");
                break;
            }
            if (current_user->role != ROLE_TEACHER) {
                printf("权限不足！只有教师可以修改学生信息。\n");
                system("pause");
                break;
            }
            ModifyRecord();
            system("pause");
            break;
        case 7:
            system("cls");
            if (first) {
                printf("系统中尚无学生成绩信息，请先输入！\n");
                system("pause");
                break;
            }
            if (current_user->role != ROLE_TEACHER) {
                printf("权限不足！\n");
                system("pause");
                break;
            }
            CalculateScoreOfStudent();
            system("pause");
            break;
        case 8:
            system("cls");
            if (first) {
                printf("系统中尚无学生成绩信息，请先输入！\n");
                system("pause");
                break;
            }
            if (current_user->role != ROLE_TEACHER) {
                printf("权限不足！\n");
                system("pause");
                break;
            }
            CalculateScoreOfCourse();
            system("pause");
            break;
        case 9:
            system("cls");
            if (first) {
                printf("系统中尚无学生成绩信息，请先输入！\n");
                system("pause");
                break;
            }
            if (current_user->role != ROLE_TEACHER) {
                printf("权限不足！\n");
                system("pause");
                break;
            }
            SortbyNum();
            break;
        case 10:
            system("cls");
            if (first) {
                printf("系统中尚无学生成绩信息，请先输入！\n");
                system("pause");
                break;
            }
            if (current_user->role != ROLE_TEACHER) {
                printf("权限不足！\n");
                system("pause");
                break;
            }
            SortbyName();
            system("pause");
            break;
        case 11:
            system("cls");
            if (first) {
                printf("系统中尚无学生成绩信息，请先输入！\n");
                system("pause");
                break;
            }
            if (current_user->role != ROLE_TEACHER) {
                printf("权限不足！\n");
                system("pause");
                break;
            }
            SortbyScore(Descending);
            system("pause");
            break;
        case 12:
            system("cls");
            if (first) {
                printf("系统中尚无学生成绩信息，请先输入！\n");
                system("pause");
                break;
            }
            if (current_user->role != ROLE_TEACHER) {
                printf("权限不足！\n");
                system("pause");
                break;
            }
            SortbyScore(Ascending);
            system("pause");
            break;
        case 13:
            system("cls");
            if (first) {
                printf("系统中尚无学生成绩信息，请先输入！\n");
                system("pause");
                break;
            }
            if (current_user->role != ROLE_TEACHER) {
                printf("权限不足！\n");
                system("pause");
                break;
            }
            StatisticAnalysis();
            system("pause");
            break;
        case 14:
            system("cls");
            if (first) {
                printf("系统中尚无学生成绩信息，请先输入！\n");
                system("pause");
                break;
            }
            if (current_user->role != ROLE_TEACHER) {
                printf("权限不足！\n");
                system("pause");
                break;
            }
            PrintRecord();
            system("pause");
            break;
        case 15:
            system("cls");
            if (first) {
                printf("系统中尚无学生成绩信息，请先输入！\n");
                system("pause");
                break;
            }
            if (current_user->role != ROLE_TEACHER) {
                printf("权限不足！\n");
                system("pause");
                break;
            }
            WritetoFile();
            system("pause");
            break;
        case 16:
            system("cls");
            if (ReadfromFile(&first) || first) {
                printf("尚未输入学生信息或文件打开失败,请先检查!\n");
                system("pause");
                break;
            }
            if (current_user->role != ROLE_TEACHER) {
                printf("权限不足！\n");
                system("pause");
                break;
            }
            system("pause");
            break;
        case 17:
            system("cls");
            // 保存当前用户数据（可选）
            WriteUsersToFile();
            // 重新登录
            Login();
            // 重新加载学生数据（因为可能换了学生身份）
            if (ReadfromFile(&first) != 0) first = 1;
            break;
        case 0:
            system("cls");
            SetPosition(0, 10);
            printf("退出系统！\n");
            WriteUsersToFile();
            FreeList();  // 释放内存
            exit(0);
        default:
            system("cls");
            SetPosition(0, 10);
            printf("输入出错，请重新选择操作！\n");
            system("pause");
        }
    }
    return 0;
}