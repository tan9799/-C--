#define _CRT_SECURE_NO_WARNINGS
#include "snake.h"
#include "snake.h"

/* 全局变量定义 */
Snake snake;                    // 蛇
Food foods[30];                 // 食物数组（扩大容量到30）
Obstacle obstacles[MAX_OBSTACLE_COUNT]; // 障碍物数组
int obstacleCount = 0;          // 当前障碍物数量

int CURRENT_FOOD_COUNT = 0;     // 当前实际食物数量
int TARGET_FOOD_COUNT = 0;      // 目标食物数量（会随机变化）
int foodChangeTimer = 0;        // 食物数量变化计时器
int isPaused = 0;               // 暂停状态（0=运行，1=暂停）

Difficulty currentDifficulty = DIFFICULTY_MEDIUM;  // 默认中等难度
DifficultyParams diffParams;    // 当前难度参数

char now_Dir = RIGHT;           // 当前蛇头方向
char direction = RIGHT;         // 预期蛇头方向
int foodEatenCount = 0;         // 已吃食物计数
int speedBoostActive = 0;       // 加速是否激活
int speedBoostCounter = 0;      // 加速计数器

/* 排行榜相关全局变量 */
ScoreRecord records[MAX_RECORDS];
int recordCount = 0;

Prop props[MAX_PROP_COUNT];     // 道具数组
int propCount = 0;               // 当前道具数量
int propGenerateTimer = 0;       // 道具生成计时器
PlayerStatus playerStatus;       // 玩家状态

/* ========== 获取当前日期时间 ========== */
void GetCurrentDateTime(char* buffer) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    strftime(buffer, 20, "%Y-%m-%d %H:%M", tm_info);
}

/* ========== 从文件加载记录 ========== */
void LoadRecords() {
    FILE* fp = fopen("snake_ranking.dat", "rb");
    if (fp == NULL) {
        // 文件不存在，初始化空记录
        recordCount = 0;
        return;
    }

    fread(&recordCount, sizeof(int), 1, fp);
    if (recordCount > MAX_RECORDS) recordCount = MAX_RECORDS;

    fread(records, sizeof(ScoreRecord), recordCount, fp);
    fclose(fp);
}

/* ========== 保存记录到文件 ========== */
void SaveRecords() {
    FILE* fp = fopen("snake_ranking.dat", "wb");
    if (fp == NULL) return;

    fwrite(&recordCount, sizeof(int), 1, fp);
    fwrite(records, sizeof(ScoreRecord), recordCount, fp);
    fclose(fp);
}

/* ========== 对记录排序（高分在前） ========== */
void SortRecords() {
    for (int i = 0; i < recordCount - 1; i++) {
        for (int j = 0; j < recordCount - i - 1; j++) {
            if (records[j].score < records[j + 1].score) {
                ScoreRecord temp = records[j];
                records[j] = records[j + 1];
                records[j + 1] = temp;
            }
        }
    }
}

/* ========== 输入玩家姓名 ========== */
void InputPlayerName(char* name) {
    int pos = 0;
    char ch;

    GotoXY(45, 20);
    printf("请输入你的大名: ");

    while (1) {
        ch = _getch();
        if (ch == '\r') {  // 回车键结束
            break;
        }
        else if (ch == '\b' && pos > 0) {  // 退格键
            pos--;
            printf("\b \b");
        }
        else if (pos < NAME_LENGTH - 1 && ch >= 32 && ch <= 126) {
            name[pos++] = ch;
            printf("%c", ch);
        }
    }
    name[pos] = '\0';

    // 如果名字为空，用默认名
    if (strlen(name) == 0) {
        strcpy(name, "匿名玩家");
    }
}

/* ========== 添加新记录 ========== */
void AddRecord(int score) {
    // 如果记录已满且新分数比最低分还低，则不添加
    if (recordCount >= MAX_RECORDS) {
        SortRecords();
        if (score <= records[MAX_RECORDS - 1].score) {
            return;
        }
    }

    // 创建新记录
    ScoreRecord newRecord;

    // 输入玩家姓名
    InputPlayerName(newRecord.name);

    newRecord.score = score;
    strcpy(newRecord.difficulty, diffParams.name);
    GetCurrentDateTime(newRecord.date);

    // 添加到数组
    if (recordCount < MAX_RECORDS) {
        records[recordCount++] = newRecord;
    }
    else {
        // 替换最低分
        records[MAX_RECORDS - 1] = newRecord;
    }

    // 排序
    SortRecords();

    // 保存到文件
    SaveRecords();
}

/* ========== 显示排行榜 ========== */
void ShowRanking() {
    system("cls");

    // 加载最新记录
    LoadRecords();

    // 绘制边框
    SetColor(11);
    GotoXY(35, 3);
    printf("╔════════════════════════════════════════════════╗");
    GotoXY(35, 4);
    printf("║                 🏆 积分排行榜 🏆              ║");
    GotoXY(35, 5);
    printf("╠════════════════════════════════════════════════╣");

    if (recordCount == 0) {
        SetColor(14);
        GotoXY(40, 8);
        printf("暂无记录");
    }
    else {
        // 显示表头
        SetColor(10);
        GotoXY(37, 7);
        printf("排名");
        GotoXY(45, 7);
        printf("玩家");
        GotoXY(58, 7);
        printf("得分");
        GotoXY(68, 7);
        printf("难度");
        GotoXY(78, 7);
        printf("日期");

        SetColor(7);
        GotoXY(35, 8);
        printf("------------------------------------------------");

        // 显示记录
        for (int i = 0; i < recordCount; i++) {
            int y = 9 + i * 2;

            // 前三名用特殊颜色
            if (i == 0) SetColor(6);      // 金色
            else if (i == 1) SetColor(7); // 银色
            else if (i == 2) SetColor(8); // 青铜色
            else SetColor(7);

            GotoXY(37, y);
            printf("%d", i + 1);
            GotoXY(45, y);
            printf("%-10s", records[i].name);
            GotoXY(58, y);
            printf("%d", records[i].score);
            GotoXY(68, y);
            printf("%s", records[i].difficulty);
            GotoXY(78, y);
            printf("%s", records[i].date);
        }
    }

    SetColor(11);
    GotoXY(35, 9 + recordCount * 2);
    printf("╚════════════════════════════════════════════════╝");

    SetColor(14);
    GotoXY(40, 12 + recordCount * 2);
    printf("按任意键返回主菜单");

    SetColor(7);
    _getch();
    system("cls");
}

/* ========== 设置颜色 ========== */
void SetColor(int color) {
    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hout, color);
}

/* ========== 难度参数定义 ========== */
void InitDifficultyParams() {
    switch (currentDifficulty) {
    case DIFFICULTY_EASY:
        diffParams.baseSpeed = 300;        // 慢
        diffParams.minFoodCount = 4;       // 食物较多
        diffParams.maxFoodCount = 10;
        diffParams.changeFrequency = 80;   // 变化慢
        diffParams.speedIncreaseRate = 1;  // 速度增加慢
        diffParams.obstacleCount = 5;      // 障碍物少
        diffParams.name = "简单";
        break;

    case DIFFICULTY_MEDIUM:
        diffParams.baseSpeed = 250;        // 中等
        diffParams.minFoodCount = 3;
        diffParams.maxFoodCount = 12;
        diffParams.changeFrequency = 50;   // 中等
        diffParams.speedIncreaseRate = 2;
        diffParams.obstacleCount = 12;     // 障碍物中等
        diffParams.name = "中等";
        break;

    case DIFFICULTY_HARD:
        diffParams.baseSpeed = 200;        // 快
        diffParams.minFoodCount = 2;
        diffParams.maxFoodCount = 15;
        diffParams.changeFrequency = 30;   // 变化快
        diffParams.speedIncreaseRate = 3;
        diffParams.obstacleCount = 20;     // 障碍物多
        diffParams.name = "困难";
        break;

    case DIFFICULTY_EXPERT:
        diffParams.baseSpeed = 150;        // 非常快
        diffParams.minFoodCount = 1;
        diffParams.maxFoodCount = 20;
        diffParams.changeFrequency = 20;   // 变化非常快
        diffParams.speedIncreaseRate = 4;
        diffParams.obstacleCount = 25;     // 障碍物非常多
        diffParams.name = "专家";
        break;
    }
    obstacleCount = diffParams.obstacleCount;
}

/* ========== 选择难度 ========== */
Difficulty SelectDifficulty() {
    system("cls");
    GotoXY(40, 8);
    printf("╔══════════════════════╗");
    GotoXY(40, 9);
    printf("║     选择游戏难度      ║");
    GotoXY(40, 10);
    printf("╠══════════════════════╣");
    GotoXY(40, 11);
    printf("║                      ║");
    GotoXY(40, 12);
    printf("║  1. 简 单   🟢       ║");
    GotoXY(40, 13);
    printf("║  2. 中 等   🟡       ║");
    GotoXY(40, 14);
    printf("║  3. 困 难   🟠       ║");
    GotoXY(40, 15);
    printf("║  4. 专 家   🔴       ║");
    GotoXY(40, 16);
    printf("║                      ║");
    GotoXY(40, 17);
    printf("╚══════════════════════╝");

    GotoXY(42, 19);
    printf("请选择 (1-4): ");

    char ch = _getch();

    // 显示选择结果
    GotoXY(42, 21);
    switch (ch) {
    case '1':
        printf("您选择了：简单难度");
        currentDifficulty = DIFFICULTY_EASY;
        break;
    case '2':
        printf("您选择了：中等难度");
        currentDifficulty = DIFFICULTY_MEDIUM;
        break;
    case '3':
        printf("您选择了：困难难度");
        currentDifficulty = DIFFICULTY_HARD;
        break;
    case '4':
        printf("您选择了：专家难度");
        currentDifficulty = DIFFICULTY_EXPERT;
        break;
    default:
        printf("输入无效，使用中等难度");
        currentDifficulty = DIFFICULTY_MEDIUM;
    }

    // 初始化难度参数
    InitDifficultyParams();

    Sleep(1500);
    system("cls");
    return currentDifficulty;
}

/* ========== 主菜单 ========== */
int Menu() {
    GotoXY(40, 8);
    printf("╔══════════════════════╗");
    GotoXY(40, 9);
    printf("║  欢迎来到贪吃蛇游戏  ║");
    GotoXY(40, 10);
    printf("╠══════════════════════╣");
    GotoXY(40, 11);
    printf("║                      ║");
    GotoXY(40, 12);
    printf("║  1. 开始游戏         ║");
    GotoXY(40, 13);
    printf("║  2. 选择难度         ║");
    GotoXY(40, 14);
    printf("║  3. 排行榜           ║");
    GotoXY(40, 15);
    printf("║  4. 帮助             ║");
    GotoXY(40, 16);
    printf("║  5. 关于             ║");
    GotoXY(40, 17);
    printf("║                      ║");
    GotoXY(40, 18);
    printf("║  其他键退出游戏      ║");
    GotoXY(40, 19);
    printf("╚══════════════════════╝");
    Hide();

    char ch;
    ch = _getch();
    system("cls");

    switch (ch) {
    case '1': return 1;  // 开始游戏
    case '2': return 2;  // 选择难度
    case '3': return 3;  // 排行榜
    case '4': return 4;  // 帮助
    case '5': return 5;  // 关于
    default: return 0;   // 退出
    }
}

/* ========== 光标定位 ========== */
void GotoXY(int x, int y) {
    HANDLE hout;
    COORD cor;
    hout = GetStdHandle(STD_OUTPUT_HANDLE);
    cor.X = x;
    cor.Y = y;
    SetConsoleCursorPosition(hout, cor);
}

/* ========== 隐藏光标 ========== */
void Hide() {
    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cor_info = { 1, 0 };
    SetConsoleCursorInfo(hout, &cor_info);
}

/* ========== 关于 ========== */
void About() {
    system("cls");
    GotoXY(35, 10);
    printf("╔══════════════════════════════╗");
    GotoXY(35, 11);
    printf("║    杭州电子科技大学          ║");
    GotoXY(35, 12);
    printf("║    程序设计综合实践案例      ║");
    GotoXY(35, 13);
    printf("╠══════════════════════════════╣");
    GotoXY(35, 14);
    printf("║    贪吃蛇-控制台游戏         ║");
    GotoXY(35, 15);
    printf("║    增强版：                   ║");
    GotoXY(35, 16);
    printf("║    • 5种特色食物             ║");
    GotoXY(35, 17);
    printf("║    • 动态食物数量            ║");
    GotoXY(35, 18);
    printf("║    • 4级难度选择             ║");
    GotoXY(35, 19);
    printf("║    • 障碍物系统              ║");
    GotoXY(35, 20);
    printf("║    • 暂停功能                ║");
    GotoXY(35, 21);
    printf("║    • X键加速                 ║");
    GotoXY(35, 22);
    printf("║    • 自撞截尾（不死）        ║");
    GotoXY(35, 23);
    printf("║    • 积分排行榜               ║");
    GotoXY(35, 24);
    printf("╚══════════════════════════════╝");

    GotoXY(40, 26);
    printf("按任意键返回");
    Hide();
    char ch = _getch();
    system("cls");
}

/* ========== 帮助 ========== */
void Help() {
    system("cls");
    GotoXY(30, 5);
    printf("════════════ 游戏操作说明 ════════════");

    GotoXY(30, 7);
    printf("🎮 基本操作：");
    GotoXY(30, 8);
    printf("   W - 向上移动");
    GotoXY(30, 9);
    printf("   S - 向下移动");
    GotoXY(30, 10);
    printf("   A - 向左移动");
    GotoXY(30, 11);
    printf("   D - 向右移动");

    GotoXY(30, 13);
    printf("⚡ 特殊功能：");
    GotoXY(30, 14);
    printf("   X - 速度加快（5秒）");
    GotoXY(30, 15);
    printf("   P - 暂停/继续游戏");

    GotoXY(30, 17);
    printf("🍎 食物类型：");
    GotoXY(30, 18);
    SetColor(14); printf("   $ 普通"); SetColor(7); printf(" : 生长+1，得分+1");
    GotoXY(30, 19);
    SetColor(13); printf("   @ 大食物"); SetColor(7); printf(" : 生长+3，得分+2");
    GotoXY(30, 20);
    SetColor(10); printf("   * 速长"); SetColor(7); printf("   : 生长+5，得分+1");
    GotoXY(30, 21);
    SetColor(6);  printf("   & 黄金"); SetColor(7); printf("   : 生长+1，得分+5");
    GotoXY(30, 22);
    SetColor(12); printf("   %% 魔法"); SetColor(7); printf("   : 随机效果");

    // ===== 添加：道具系统说明 =====
    GotoXY(30, 24);
    printf("🎁 道具系统：");
    GotoXY(30, 25);
    SetColor(11); printf("   S 护盾"); SetColor(7); printf("  : 免疫一次碰撞");
    GotoXY(30, 26);
    SetColor(13); printf("   M 磁铁"); SetColor(7); printf("  : 吸引附近食物");
    GotoXY(30, 27);
    SetColor(8); printf("   I 隐身"); SetColor(7); printf("  : 穿过障碍物");
    GotoXY(30, 28);
    SetColor(14); printf("   T 减速"); SetColor(7); printf("  : 游戏速度减半");
    GotoXY(30, 29);
    SetColor(15); printf("   F 冰冻"); SetColor(7); printf("  : 暂停一切移动");

    GotoXY(30, 24);
    printf("📊 游戏规则：");
    GotoXY(30, 25);
    printf("   • 撞墙 = 游戏结束");
    GotoXY(30, 26);
    printf("   • 撞到障碍物 # = 游戏结束");
    GotoXY(30, 27);
    printf("   • 撞到自己 = 截断尾部");
    GotoXY(30, 28);
    printf("   • 游戏结束可录入排行榜");

    GotoXY(30, 31);
    printf("按任意键返回主菜单");

    Hide();
    char ch = _getch();
    system("cls");
    SetColor(7);
}

/* ========== 随机获取食物类型 ========== */
FoodType GetRandomFoodType() {
    int r = rand() % 100;
    if (r < 50) return FOOD_NORMAL;      // 50% 普通
    else if (r < 70) return FOOD_BIG;    // 20% 大食物
    else if (r < 85) return FOOD_FAST;   // 15% 速长
    else if (r < 95) return FOOD_GOLDEN; // 10% 黄金
    else return FOOD_MAGIC;               // 5% 魔法
}

/* ========== 应用食物效果 ========== */
void ApplyFoodEffect(FoodType type, int* addLength, int* addScore) {
    switch (type) {
    case FOOD_NORMAL:
        *addLength = 1;
        *addScore = 1;
        break;
    case FOOD_BIG:
        *addLength = 3;
        *addScore = 2;
        break;
    case FOOD_FAST:
        *addLength = 5;
        *addScore = 1;
        break;
    case FOOD_GOLDEN:
        *addLength = 1;
        *addScore = 5;
        break;
    case FOOD_MAGIC:
        // 随机效果
        int r = rand() % 3;
        if (r == 0) {
            *addLength = 2;
            *addScore = 2;
        }
        else if (r == 1) {
            *addLength = 0;
            *addScore = 3;
        }
        else {
            *addLength = 4;
            *addScore = 0;
        }
        break;
    }
}

/* ========== 检查食物是否和任何东西重叠 ========== */
int CheckFoodCollision(int x, int y, int ignoreIndex) {
    // 检查是否和蛇重叠
    for (int i = 0; i < snake.length; i++) {
        if (snake.snakeNode[i].x == x && snake.snakeNode[i].y == y) {
            return 1;  // 重叠
        }
    }

    // 检查是否和其他食物重叠
    for (int i = 0; i < 30; i++) {
        if (i != ignoreIndex && foods[i].active &&
            foods[i].x == x && foods[i].y == y) {
            return 1;
        }
    }

    // 检查是否和障碍物重叠
    for (int i = 0; i < obstacleCount; i++) {
        if (obstacles[i].active &&
            obstacles[i].x == x && obstacles[i].y == y) {
            return 1;
        }
    }

    return 0;  // 不重叠
}

/* ========== 检查是否撞到障碍物 ========== */
int CheckObstacleCollision(int x, int y) {
    // ===== 添加：如果隐身状态，可以穿过障碍物 =====
    if (CanPassThroughObstacles()) {
        return 0;
    }
    for (int i = 0; i < obstacleCount; i++) {
        if (obstacles[i].active &&
            obstacles[i].x == x && obstacles[i].y == y) {
            return 1;  // 撞到障碍物
        }
    }
    return 0;  // 没撞到
}


/* ========== 初始化障碍物 ========== */
void InitObstacles() {
    for (int i = 0; i < obstacleCount; i++) {
        obstacles[i].active = 1;
        int flag = 1;

        while (flag) {
            flag = 0;
            obstacles[i].x = rand() % (MAP_WIDTH - 2) + 1;
            obstacles[i].y = rand() % (MAP_HEIGHT - 2) + 1;

            // 检查是否和蛇重叠
            for (int j = 0; j < snake.length; j++) {
                if (snake.snakeNode[j].x == obstacles[i].x &&
                    snake.snakeNode[j].y == obstacles[i].y) {
                    flag = 1;
                    break;
                }
            }

            // 检查是否和其他障碍物重叠
            for (int j = 0; j < i; j++) {
                if (obstacles[j].active &&
                    obstacles[j].x == obstacles[i].x &&
                    obstacles[j].y == obstacles[i].y) {
                    flag = 1;
                    break;
                }
            }
        }

        // 画障碍物
        SetColor(8);  // 灰色
        GotoXY(obstacles[i].x, obstacles[i].y);
        printf("#");
    }
    SetColor(7);
}

/* ========== 增加一个食物 ========== */
void AddOneFood() {
    if (CURRENT_FOOD_COUNT >= diffParams.maxFoodCount) return;

    // 找到第一个不活跃的食物槽位
    int index = -1;
    for (int i = 0; i < 30; i++) {
        if (!foods[i].active) {
            index = i;
            break;
        }
    }
    if (index == -1) return;  // 没有空位

    // 随机决定食物类型
    foods[index].type = GetRandomFoodType();

    // 根据类型设置属性
    switch (foods[index].type) {
    case FOOD_NORMAL:
        foods[index].symbol = '$';
        foods[index].color = 14;  // 黄色
        foods[index].growValue = 1;
        foods[index].scoreValue = 1;
        foods[index].timer = 9999;
        break;
    case FOOD_BIG:
        foods[index].symbol = '@';
        foods[index].color = 13;  // 紫色
        foods[index].growValue = 3;
        foods[index].scoreValue = 2;
        foods[index].timer = 9999;
        break;
    case FOOD_FAST:
        foods[index].symbol = '*';
        foods[index].color = 10;  // 绿色
        foods[index].growValue = 5;
        foods[index].scoreValue = 1;
        foods[index].timer = 9999;
        break;
    case FOOD_GOLDEN:
        foods[index].symbol = '&';
        foods[index].color = 6;   // 金色
        foods[index].growValue = 1;
        foods[index].scoreValue = 5;
        foods[index].timer = 9999;
        break;
    case FOOD_MAGIC:
        foods[index].symbol = '%';
        foods[index].color = 12;  // 红色
        foods[index].growValue = 0;  // 临时值
        foods[index].scoreValue = 0;
        foods[index].timer = 200;  // 魔法食物会消失
        break;
    }

    // 生成位置
    int flag = 1;
    while (flag) {
        flag = 0;
        foods[index].x = rand() % (MAP_WIDTH - 2) + 1;
        foods[index].y = rand() % (MAP_HEIGHT - 2) + 1;

        // 检查是否和蛇、其他食物、障碍物重叠
        if (CheckFoodCollision(foods[index].x, foods[index].y, index)) {
            flag = 1;
        }
    }

    foods[index].active = 1;
    SetColor(foods[index].color);
    GotoXY(foods[index].x, foods[index].y);
    printf("%c", foods[index].symbol);
    SetColor(7);
    CURRENT_FOOD_COUNT++;
}

/* ========== 减少一个食物 ========== */
void RemoveOneFood() {
    if (CURRENT_FOOD_COUNT <= diffParams.minFoodCount) return;

    // 随机选择一个活跃的食物移除
    int activeIndices[30];
    int activeCount = 0;

    for (int i = 0; i < 30; i++) {
        if (foods[i].active) {
            activeIndices[activeCount++] = i;
        }
    }

    if (activeCount > 0) {
        int removeIndex = activeIndices[rand() % activeCount];
        foods[removeIndex].active = 0;
        GotoXY(foods[removeIndex].x, foods[removeIndex].y);
        printf(" ");
        CURRENT_FOOD_COUNT--;
    }
}

/* ========== 更新食物数量（动态调整到目标值） ========== */
void UpdateFoodCount() {
    if (isPaused) return;  // 暂停时不更新食物数量

    foodChangeTimer--;
    if (foodChangeTimer <= 0) {
        // 根据难度设置的变化频率改变目标数量
        foodChangeTimer = diffParams.changeFrequency;

        // 随机生成新的目标数量
        int newTarget = diffParams.minFoodCount +
            rand() % (diffParams.maxFoodCount - diffParams.minFoodCount + 1);
        TARGET_FOOD_COUNT = newTarget;

        GotoXY(55, 9);
        printf("目标:%d   ", TARGET_FOOD_COUNT);
    }

    // 动态调整到目标数量
    if (CURRENT_FOOD_COUNT < TARGET_FOOD_COUNT) {
        // 需要增加食物
        if (rand() % 10 < 4) {  // 40%概率增加
            AddOneFood();
        }
    }
    else if (CURRENT_FOOD_COUNT > TARGET_FOOD_COUNT) {
        // 需要减少食物
        if (rand() % 10 < 4) {  // 40%概率减少
            RemoveOneFood();
        }
    }

    // 更新显示
    GotoXY(55, 8);
    printf("当前:%d   ", CURRENT_FOOD_COUNT);
}

/* ========== 初始化所有食物 ========== */
void InitFoods() {
    // 初始目标数量
    TARGET_FOOD_COUNT = diffParams.minFoodCount +
        rand() % (diffParams.maxFoodCount - diffParams.minFoodCount + 1);
    CURRENT_FOOD_COUNT = TARGET_FOOD_COUNT;
    foodChangeTimer = diffParams.changeFrequency / 2;

    // 初始化所有食物为不活跃
    for (int i = 0; i < 30; i++) {
        foods[i].active = 0;
    }

    // 生成初始食物
    for (int i = 0; i < CURRENT_FOOD_COUNT; i++) {
        foods[i].type = GetRandomFoodType();

        // 根据类型设置属性
        switch (foods[i].type) {
        case FOOD_NORMAL:
            foods[i].symbol = '$';
            foods[i].color = 14;
            break;
        case FOOD_BIG:
            foods[i].symbol = '@';
            foods[i].color = 13;
            break;
        case FOOD_FAST:
            foods[i].symbol = '*';
            foods[i].color = 10;
            break;
        case FOOD_GOLDEN:
            foods[i].symbol = '&';
            foods[i].color = 6;
            break;
        case FOOD_MAGIC:
            foods[i].symbol = '%';
            foods[i].color = 12;
            foods[i].timer = 200;
            break;
        }

        int flag = 1;
        while (flag) {
            flag = 0;
            foods[i].x = rand() % (MAP_WIDTH - 2) + 1;
            foods[i].y = rand() % (MAP_HEIGHT - 2) + 1;

            // 检查是否和蛇重叠
            if (CheckFoodCollision(foods[i].x, foods[i].y, i)) {
                flag = 1;
            }
        }
        foods[i].active = 1;
        SetColor(foods[i].color);
        GotoXY(foods[i].x, foods[i].y);
        printf("%c", foods[i].symbol);
    }
    SetColor(7);
}

/* ========== 初始化玩家状态 ========== */
void InitPlayerStatus() {
    playerStatus.shieldActive = 0;
    playerStatus.shieldTimer = 0;
    playerStatus.magnetActive = 0;
    playerStatus.magnetTimer = 0;
    playerStatus.magnetRange = MAGNET_RANGE;
    playerStatus.invisibleActive = 0;
    playerStatus.invisibleTimer = 0;
    playerStatus.slowActive = 0;
    playerStatus.slowTimer = 0;
    playerStatus.freezeActive = 0;
    playerStatus.freezeTimer = 0;
}

/* ========== 随机生成道具 ========== */
void GenerateRandomProp() {
    if (propCount >= MAX_PROP_COUNT) return;

    // 找到第一个空位
    int index = -1;
    for (int i = 0; i < MAX_PROP_COUNT; i++) {
        if (!props[i].active) {
            index = i;
            break;
        }
    }
    if (index == -1) return;

    // 随机决定道具类型
    int r = rand() % 100;
    PropType type;

    if (r < 15) {           // 15% 护盾
        type = PROP_SHIELD;
        props[index].symbol = 'S';
        props[index].color = 11;  // 青色
        props[index].effectDuration = SHIELD_DURATION;
    }
    else if (r < 30) {      // 15% 磁铁
        type = PROP_MAGNET;
        props[index].symbol = 'M';
        props[index].color = 13;  // 紫色
        props[index].effectDuration = MAGNET_DURATION;
    }
    else if (r < 45) {      // 15% 隐身
        type = PROP_INVISIBLE;
        props[index].symbol = 'I';
        props[index].color = 8;   // 灰色
        props[index].effectDuration = INVISIBLE_DURATION;
    }
    else if (r < 75) {      // 30% 减速
        type = PROP_SLOW;
        props[index].symbol = 'T';
        props[index].color = 14;  // 黄色
        props[index].effectDuration = SLOW_DURATION;
    }
    else {                  // 25% 冰冻
        type = PROP_FREEZE;
        props[index].symbol = 'F';
        props[index].color = 15;  // 白色
        props[index].effectDuration = FREEZE_DURATION;
    }

    props[index].type = type;

    // 生成位置（避开蛇、食物、障碍物）
    int flag = 1;
    while (flag) {
        flag = 0;
        props[index].x = rand() % (MAP_WIDTH - 2) + 1;
        props[index].y = rand() % (MAP_HEIGHT - 2) + 1;

        // 检查是否和蛇重叠
        for (int i = 0; i < snake.length; i++) {
            if (snake.snakeNode[i].x == props[index].x &&
                snake.snakeNode[i].y == props[index].y) {
                flag = 1;
                break;
            }
        }

        // 检查是否和食物重叠
        for (int i = 0; i < 30; i++) {
            if (foods[i].active && foods[i].x == props[index].x &&
                foods[i].y == props[index].y) {
                flag = 1;
                break;
            }
        }

        // 检查是否和障碍物重叠
        for (int i = 0; i < obstacleCount; i++) {
            if (obstacles[i].active && obstacles[i].x == props[index].x &&
                obstacles[i].y == props[index].y) {
                flag = 1;
                break;
            }
        }

        // 检查是否和其他道具重叠
        for (int i = 0; i < MAX_PROP_COUNT; i++) {
            if (i != index && props[i].active &&
                props[i].x == props[index].x && props[i].y == props[index].y) {
                flag = 1;
                break;
            }
        }
    }

    props[index].active = 1;
    props[index].timer = 300;  // 道具存在300帧后自动消失
    propCount++;

    // 显示道具
    SetColor(props[index].color);
    GotoXY(props[index].x, props[index].y);
    printf("%c", props[index].symbol);
    SetColor(7);
}

/* ========== 应用道具效果 ========== */
void ApplyPropEffect(PropType type) {
    switch (type) {
    case PROP_SHIELD:
        playerStatus.shieldActive = 1;
        playerStatus.shieldTimer = SHIELD_DURATION;
        GotoXY(55, 22);
        SetColor(11);
        printf("护盾激活! 🛡️          ");
        break;

    case PROP_MAGNET:
        playerStatus.magnetActive = 1;
        playerStatus.magnetTimer = MAGNET_DURATION;
        GotoXY(55, 22);
        SetColor(13);
        printf("磁铁激活! 🧲          ");
        break;

    case PROP_INVISIBLE:
        playerStatus.invisibleActive = 1;
        playerStatus.invisibleTimer = INVISIBLE_DURATION;
        GotoXY(55, 22);
        SetColor(8);
        printf("隐身激活! 👻          ");
        break;

    case PROP_SLOW:
        playerStatus.slowActive = 1;
        playerStatus.slowTimer = SLOW_DURATION;
        GotoXY(55, 22);
        SetColor(14);
        printf("减速激活! 🐢          ");
        break;

    case PROP_FREEZE:
        playerStatus.freezeActive = 1;
        playerStatus.freezeTimer = FREEZE_DURATION;
        GotoXY(55, 22);
        SetColor(15);
        printf("冰冻激活! ❄️          ");
        break;
    }
    SetColor(7);
}

/* ========== 更新道具状态 ========== */
void UpdateProps() {
    // 道具消失计时器
    for (int i = 0; i < MAX_PROP_COUNT; i++) {
        if (props[i].active) {
            props[i].timer--;
            if (props[i].timer <= 0) {
                // 道具消失
                props[i].active = 0;
                GotoXY(props[i].x, props[i].y);
                printf(" ");
                propCount--;
            }
        }
    }

    // 随机生成新道具
    if (!isPaused) {
        propGenerateTimer--;
        if (propGenerateTimer <= 0) {
            propGenerateTimer = PROP_GENERATE_INTERVAL;
            if (rand() % 100 < PROP_GENERATE_RATE) {
                GenerateRandomProp();
            }
        }
    }

    // 更新玩家状态计时器
    if (playerStatus.shieldActive) {
        playerStatus.shieldTimer--;
        if (playerStatus.shieldTimer <= 0) {
            playerStatus.shieldActive = 0;
            GotoXY(55, 22);
            printf("                    ");
        }
    }

    if (playerStatus.magnetActive) {
        playerStatus.magnetTimer--;
        if (playerStatus.magnetTimer <= 0) {
            playerStatus.magnetActive = 0;
            GotoXY(55, 22);
            printf("                    ");
        }
    }

    if (playerStatus.invisibleActive) {
        playerStatus.invisibleTimer--;
        if (playerStatus.invisibleTimer <= 0) {
            playerStatus.invisibleActive = 0;
            GotoXY(55, 22);
            printf("                    ");
        }
    }

    if (playerStatus.slowActive) {
        playerStatus.slowTimer--;
        if (playerStatus.slowTimer <= 0) {
            playerStatus.slowActive = 0;
            GotoXY(55, 22);
            printf("                    ");
        }
    }

    if (playerStatus.freezeActive) {
        playerStatus.freezeTimer--;
        if (playerStatus.freezeTimer <= 0) {
            playerStatus.freezeActive = 0;
            GotoXY(55, 22);
            printf("                    ");
        }
    }
    // ===== 添加：显示护盾状态 =====
    GotoXY(55, 29);
    if (playerStatus.shieldActive) {
        SetColor(11);
        printf("护盾状态:激活  ");
    }
    else {
        SetColor(7);
        printf("护盾状态:无    ");
    }
    SetColor(7);
}

/* ========== 磁铁效果：自动吸引附近食物 ========== */
void ApplyMagnetEffect() {
    if (!playerStatus.magnetActive) return;

    for (int i = 0; i < 30; i++) {
        if (!foods[i].active) continue;

        // 计算与蛇头的距离
        int dx = abs(foods[i].x - snake.snakeNode[0].x);
        int dy = abs(foods[i].y - snake.snakeNode[0].y);

        if (dx <= playerStatus.magnetRange && dy <= playerStatus.magnetRange) {
            // 食物被吸引，向蛇头移动一格
            int moveX = 0, moveY = 0;

            if (foods[i].x < snake.snakeNode[0].x) moveX = 1;
            else if (foods[i].x > snake.snakeNode[0].x) moveX = -1;

            if (foods[i].y < snake.snakeNode[0].y) moveY = 1;
            else if (foods[i].y > snake.snakeNode[0].y) moveY = -1;

            // 随机选择一个方向移动（优先直线移动）
            if (dx > dy) {
                // 先水平移动
                if (moveX != 0) {
                    // 擦除原位置
                    GotoXY(foods[i].x, foods[i].y);
                    printf(" ");
                    // 移动到新位置
                    foods[i].x += moveX;
                }
            }
            else {
                // 先垂直移动
                if (moveY != 0) {
                    GotoXY(foods[i].x, foods[i].y);
                    printf(" ");
                    foods[i].y += moveY;
                }
            }

            // 重新绘制食物
            SetColor(foods[i].color);
            GotoXY(foods[i].x, foods[i].y);
            printf("%c", foods[i].symbol);
            SetColor(7);
        }
    }
}

/* ========== 减速效果：可配置的减速 ========== */
int ApplySlowEffect(int originalSpeed) {
    if (playerStatus.slowActive) {
        // 减速因子：数值越大越慢
        #define SLOW_FACTOR 4  // 4倍慢（可以调整这个值）

        int slowedSpeed = originalSpeed * SLOW_FACTOR;

        // 设置合理的速度范围
        if (slowedSpeed > 1000) slowedSpeed = 1000;  // 最慢1秒/帧
        if (slowedSpeed < 100) slowedSpeed = 100;    // 最快100ms/帧

        return slowedSpeed;
    }
    return originalSpeed;
}

/* ========== 冰冻效果：障碍物和敌人暂停 ========== */
int IsFrozen() {
    return playerStatus.freezeActive;
}

/* ========== 护盾效果：免疫一次碰撞 ========== */
int CheckShieldProtection() {
    if (playerStatus.shieldActive) {
        // 消耗护盾
        playerStatus.shieldActive = 0;
        playerStatus.shieldTimer = 0;

        // 显示护盾破碎效果
        GotoXY(55, 23);
        SetColor(12);
        printf("护盾破碎! 💔          ");
        SetColor(7);

        return 1;  // 护盾生效
    }
    return 0;  // 无护盾
}

/* ========== 隐身效果：可以穿过障碍物 ========== */
int CanPassThroughObstacles() {
    return playerStatus.invisibleActive;
}

/* ========== 初始化地图 ========== */
void InitMap() {
    Hide();
    isPaused = 0;  // 初始状态未暂停

    // 设置蛇头在地图中心
    snake.snakeNode[0].x = MAP_WIDTH / 2 - 1;
    snake.snakeNode[0].y = MAP_HEIGHT / 2 - 1;
    GotoXY(snake.snakeNode[0].x, snake.snakeNode[0].y);
    printf("@");

    snake.length = 3;
    snake.baseSpeed = diffParams.baseSpeed;
    snake.speed = diffParams.baseSpeed;
    now_Dir = RIGHT;
    foodEatenCount = 0;
    speedBoostActive = 0;

    // 画蛇身
    for (int i = 1; i < snake.length; i++) {
        snake.snakeNode[i].y = snake.snakeNode[i - 1].y;
        snake.snakeNode[i].x = snake.snakeNode[i - 1].x - 1;
        GotoXY(snake.snakeNode[i].x, snake.snakeNode[i].y);
        printf("o");
    }

    // 画上下边界
    for (int i = 0; i < MAP_WIDTH; i++) {
        GotoXY(i, 0);
        printf("-");
        GotoXY(i, MAP_HEIGHT - 1);
        printf("-");
    }

    // 画左右边界
    for (int i = 1; i < MAP_HEIGHT - 1; i++) {
        GotoXY(0, i);
        printf("|");
        GotoXY(MAP_WIDTH - 1, i);
        printf("|");
    }

    // 初始化所有食物
    InitFoods();

    // 初始化障碍物
    InitObstacles();

    // 初始化道具系统
    for (int i = 0; i < MAX_PROP_COUNT; i++) {
        props[i].active = 0;
    }
    propCount = 0;
    propGenerateTimer = PROP_GENERATE_INTERVAL / 2;
    InitPlayerStatus();

    // 显示游戏信息
    GotoXY(55, 3);
    printf("═══════════════");
    GotoXY(55, 4);
    printf("难度: %s", diffParams.name);
    GotoXY(55, 5);
    printf("得分:0");
    GotoXY(55, 6);
    printf("范围:%d-%d", diffParams.minFoodCount, diffParams.maxFoodCount);
    GotoXY(55, 7);
    printf("速度:%dms", snake.speed);
    GotoXY(55, 8);
    printf("当前:%d", CURRENT_FOOD_COUNT);
    GotoXY(55, 9);
    printf("目标:%d", TARGET_FOOD_COUNT);
    GotoXY(55, 10);
    printf("障碍物:%d", obstacleCount);
    GotoXY(55, 11);
    printf("加速:关闭");
    GotoXY(55, 12);
    printf("状态:运行");
    GotoXY(55, 14);
    printf("操作:");
    GotoXY(55, 15);
    printf("P暂停  X加速");
    GotoXY(55, 16);
    printf("═══════════════");
    GotoXY(55, 17);
    printf("自撞截尾:开启");

    // 显示食物图例
    GotoXY(55, 19);
    SetColor(14); printf("$"); SetColor(7); printf(":+1");
    GotoXY(62, 19);
    SetColor(13); printf("@"); SetColor(7); printf(":+3");
    GotoXY(55, 20);
    SetColor(10); printf("*"); SetColor(7); printf(":+5");
    GotoXY(62, 20);
    SetColor(6); printf("&"); SetColor(7); printf(":+5分");
    GotoXY(55, 21);
    SetColor(12); printf("%%"); SetColor(7); printf(":随机");

    // 显示道具图例
    GotoXY(55, 23);
    SetColor(11); printf("S"); SetColor(7); printf(":护盾 ");
    SetColor(13); printf("M"); SetColor(7); printf(":磁铁 ");
    SetColor(8); printf("I"); SetColor(7); printf(":隐身");
    GotoXY(55, 24);
    SetColor(14); printf("T"); SetColor(7); printf(":减速 ");
    SetColor(15); printf("F"); SetColor(7); printf(":冰冻");
    GotoXY(55, 25);
    printf("═══════════════");

    // 添加状态显示区域
    GotoXY(55, 26);
    printf("当前道具:无");
    GotoXY(55, 27);
    printf("道具效果:  ");
    GotoXY(55, 28);
    printf("═══════════");
    // 在 InitMap 函数的末尾添加
    GotoXY(55, 29);
    printf("护盾状态:无");
}

/* ========== 暂停游戏 ========== */
void PauseGame() {
    isPaused = !isPaused;  // 切换暂停状态

    if (isPaused) {
        // 显示暂停信息
        GotoXY(45, 18);
        printf("══════════════════════");
        GotoXY(45, 19);
        printf("       游 戏 暂 停      ");
        GotoXY(45, 20);
        printf("     按 P 键 继 续     ");
        GotoXY(45, 21);
        printf("══════════════════════");

        GotoXY(55, 12);
        printf("状态:暂停");
    }
    else {
        // 清除暂停信息
        for (int i = 18; i <= 21; i++) {
            GotoXY(45, i);
            printf("                        ");
        }

        GotoXY(55, 12);
        printf("状态:运行");

        // 重绘食物（防止被暂停框覆盖）
        for (int i = 0; i < 30; i++) {
            if (foods[i].active) {
                SetColor(foods[i].color);
                GotoXY(foods[i].x, foods[i].y);
                printf("%c", foods[i].symbol);
            }
        }

        // 重绘障碍物
        SetColor(8);
        for (int i = 0; i < obstacleCount; i++) {
            if (obstacles[i].active) {
                GotoXY(obstacles[i].x, obstacles[i].y);
                printf("#");
            }
        }

        // ===== 添加：重绘道具 =====
        for (int i = 0; i < MAX_PROP_COUNT; i++) {
            if (props[i].active) {
                SetColor(props[i].color);
                GotoXY(props[i].x, props[i].y);
                printf("%c", props[i].symbol);
            }
        }

        // 重绘蛇
        SetColor(7);
        for (int i = 0; i < snake.length; i++) {
            GotoXY(snake.snakeNode[i].x, snake.snakeNode[i].y);
            if (i == 0) printf("@");
            else printf("o");
        }
        SetColor(7);
    }
}

/* ========== 处理输入 ========== */
void HandleInput() {
    if (_kbhit()) {
        char ch = _getch();

        // 暂停键优先处理
        if (ch == PAUSE || ch == PAUSE_CAP) {
            PauseGame();
            return;
        }

        // 如果暂停，不处理移动和加速
        if (isPaused) return;

        switch (ch) {
        case UP:
            if (now_Dir != DOWN) now_Dir = ch;
            break;
        case DOWN:
            if (now_Dir != UP) now_Dir = ch;
            break;
        case LEFT:
            if (now_Dir != RIGHT) now_Dir = ch;
            break;
        case RIGHT:
            if (now_Dir != LEFT) now_Dir = ch;
            break;
        case SPEED_BOOST:  // X键加速
            if (!speedBoostActive) {
                speedBoostActive = 1;
                speedBoostCounter = 20;
                snake.speed = snake.speed / 2;
                GotoXY(55, 11);
                printf("加速:开启  ");
            }
            break;
        }
    }
}

/* ===== 查找碰撞点索引 ===== */
int FindCollisionIndex() {
    for (int i = 1; i < snake.length; i++) {
        if (snake.snakeNode[0].x == snake.snakeNode[i].x &&
            snake.snakeNode[0].y == snake.snakeNode[i].y) {
            return i;
        }
    }
    return -1;
}

/* ===== 截断蛇尾 ===== */
void CutTail(int collideIndex) {
    // 从碰撞点开始擦除后面的所有蛇身
    for (int i = collideIndex; i < snake.length; i++) {
        GotoXY(snake.snakeNode[i].x, snake.snakeNode[i].y);
        printf(" ");
    }

    // 逻辑上缩短蛇长
    snake.length = collideIndex;

    // 显示提示信息
    GotoXY(55, 18);
    printf("自撞截尾!   ");
}

/* ========== 蛇移动 ========== */
int MoveSnake() {
    if (isPaused) {
        Sleep(100);
        return 1;
    }
    // ===== 添加：检查冰冻状态 =====
    if (IsFrozen()) {
        // 冰冻状态，蛇不能移动
        Sleep(snake.speed);
        UpdateProps();  // 但道具系统继续更新
        return 1;
    }

    Snakenode temp;
    int flag = 0;
    int addLength = 0;
    int addScore = 0;

    // 记录蛇尾
    temp = snake.snakeNode[snake.length - 1];

    // 蛇身向前移动
    for (int i = snake.length - 1; i >= 1; i--) {
        snake.snakeNode[i] = snake.snakeNode[i - 1];
    }

    // 在蛇头原来的位置画蛇身
    GotoXY(snake.snakeNode[1].x, snake.snakeNode[1].y);
    printf("o");

    // 移动蛇头
    switch (now_Dir) {
    case UP:    snake.snakeNode[0].y--; break;
    case DOWN:  snake.snakeNode[0].y++; break;
    case LEFT:  snake.snakeNode[0].x--; break;
    case RIGHT: snake.snakeNode[0].x++; break;
    }

    // 画蛇头
    GotoXY(snake.snakeNode[0].x, snake.snakeNode[0].y);
    printf("@");

    // 检查是否吃到任何食物
    for (int i = 0; i < 30; i++) {
        if (foods[i].active &&
            snake.snakeNode[0].x == foods[i].x && snake.snakeNode[0].y == foods[i].y) {

            // 获取食物效果
            ApplyFoodEffect(foods[i].type, &addLength, &addScore);

            // 吃到食物
            foods[i].active = 0;
            GotoXY(foods[i].x, foods[i].y);
            printf(" ");

            // 蛇变长（根据食物类型）
            for (int j = 0; j < addLength; j++) {
                snake.length++;
                snake.snakeNode[snake.length - 1] = temp;
            }

            flag = 1;
            foodEatenCount += addScore;

            GotoXY(55, 5);
            printf("得分:%d   ", foodEatenCount);
            CURRENT_FOOD_COUNT--;

            GotoXY(55, 7);
            printf("速度:%dms", snake.speed);
            GotoXY(55, 8);
            printf("当前:%d   ", CURRENT_FOOD_COUNT);
            break;
        }
    }

    // ===== 添加：检查是否吃到道具 =====
    for (int i = 0; i < MAX_PROP_COUNT; i++) {
        if (props[i].active &&
            snake.snakeNode[0].x == props[i].x && snake.snakeNode[0].y == props[i].y) {

            // 吃到道具
            props[i].active = 0;
            GotoXY(props[i].x, props[i].y);
            printf(" ");
            propCount--;

            // 应用道具效果
            ApplyPropEffect(props[i].type);

            // 可以在这里添加音效
            // Beep(1000, 100);
            break;
        }
    }

    // 没吃到食物时，擦除蛇尾
    if (!flag) {
        GotoXY(temp.x, temp.y);
        printf(" ");
    }

    // 更新食物数量（动态变化）
    UpdateFoodCount();

    // ===== 添加：更新道具系统 =====
    UpdateProps();

    // ===== 添加：应用磁铁效果 =====
    ApplyMagnetEffect();

    // 先检测是否撞到障碍物（优先级最高）
    if (CheckObstacleCollision(snake.snakeNode[0].x, snake.snakeNode[0].y)) {
        // ===== 修复：确保护盾检查被正确调用 =====
        if (CheckShieldProtection()) {
            // 护盾生效！把蛇头从障碍物位置拉回
            // 先擦除蛇头
            GotoXY(snake.snakeNode[0].x, snake.snakeNode[0].y);
            printf(" ");

            // 根据移动方向把蛇头拉回原位
            switch (now_Dir) {
            case UP:    snake.snakeNode[0].y++; break;
            case DOWN:  snake.snakeNode[0].y--; break;
            case LEFT:  snake.snakeNode[0].x++; break;
            case RIGHT: snake.snakeNode[0].x--; break;
            }

            // 重新绘制蛇头
            GotoXY(snake.snakeNode[0].x, snake.snakeNode[0].y);
            printf("@");

            // 显示护盾抵挡提示
            GotoXY(55, 23);
            SetColor(11);
            printf("护盾抵挡障碍! 🛡️     ");
            SetColor(7);

            return 1;  // 游戏继续
        }
        else {
            // 没有护盾，游戏结束
            system("cls");
            GotoXY(45, 14);
            printf("══════════════════════");
            GotoXY(45, 15);
            printf("     游戏结束！");
            GotoXY(45, 16);
            printf("     难度: %s", diffParams.name);
            GotoXY(45, 17);
            printf("     原因: 撞到障碍物");
            GotoXY(45, 18);
            printf("     最终得分：%d", foodEatenCount);
            GotoXY(45, 19);
            printf("══════════════════════");
            GotoXY(45, 21);
            printf("按任意键返回主菜单");
            char c = _getch();
            system("cls");

            LoadRecords();
            AddRecord(foodEatenCount);

            return 0;
        }
    }

    // 检测自撞
    int hitIndex = FindCollisionIndex();

    if (hitIndex != -1) {
        // ===== 修改：添加护盾检查 =====
        if (CheckShieldProtection()) {
            // 护盾生效，不截尾
            GotoXY(55, 23);
            SetColor(11);
            printf("护盾抵挡自撞! 🛡️     ");
            SetColor(7);
        }
        else {
            CutTail(hitIndex);
        }
    }
    else {
        // 没有自撞，才检查撞墙
        if (!IsCorrect()) {
            // ===== 修复：添加护盾检查 =====
            if (CheckShieldProtection()) {
                // 护盾生效！把蛇头从墙边拉回
                // 先擦除蛇头
                GotoXY(snake.snakeNode[0].x, snake.snakeNode[0].y);
                printf(" ");

                // 根据移动方向把蛇头拉回地图内
                switch (now_Dir) {
                case UP:    snake.snakeNode[0].y++; break;
                case DOWN:  snake.snakeNode[0].y--; break;
                case LEFT:  snake.snakeNode[0].x++; break;
                case RIGHT: snake.snakeNode[0].x--; break;
                }

                // 重新绘制蛇头
                GotoXY(snake.snakeNode[0].x, snake.snakeNode[0].y);
                printf("@");

                // 显示护盾抵挡提示
                GotoXY(55, 23);
                SetColor(11);
                printf("护盾抵挡撞墙! 🛡️     ");
                SetColor(7);

                return 1;  // 继续游戏
            }
            else {
                system("cls");
                GotoXY(45, 14);
                printf("══════════════════════");
                GotoXY(45, 15);
                printf("     游戏结束！");
                GotoXY(45, 16);
                printf("     难度: %s", diffParams.name);
                GotoXY(45, 17);
                printf("     原因: 撞墙");
                GotoXY(45, 18);
                printf("     最终得分：%d", foodEatenCount);
                GotoXY(45, 19);
                printf("══════════════════════");
                GotoXY(45, 21);
                printf("按任意键返回主菜单");
                char c = _getch();
                system("cls");

                LoadRecords();
                AddRecord(foodEatenCount);

                return 0;
            }
        }
    }

    // 调整速度
    SpeedControl();
    // ===== 添加：应用减速效果 =====
    snake.speed = ApplySlowEffect(snake.speed);
    ApplySpeedBoost();
    Sleep(snake.speed);
    return 1;
}

/* ========== 碰撞检测（只检测撞墙） ========== */
int IsCorrect() {
    // 撞墙检测
    if (snake.snakeNode[0].x == 0 || snake.snakeNode[0].y == 0 ||
        snake.snakeNode[0].x == MAP_WIDTH - 1 || snake.snakeNode[0].y == MAP_HEIGHT - 1)
        return 0;

    return 1;
}

/* ========== 速度控制 ========== */
void SpeedControl() {
    int speedDecrease = (snake.length - 3) * diffParams.speedIncreaseRate;
    int newBaseSpeed = diffParams.baseSpeed - speedDecrease;

    if (newBaseSpeed < 50) newBaseSpeed = 50;

    if (newBaseSpeed != snake.baseSpeed) {
        snake.baseSpeed = newBaseSpeed;
        if (!speedBoostActive) {
            snake.speed = snake.baseSpeed;
        }
        GotoXY(55, 7);
        printf("速度:%dms", snake.speed);
    }
}

/* ========== 应用加速效果 ========== */
void ApplySpeedBoost() {
    if (speedBoostActive && !isPaused) {
        speedBoostCounter--;
        if (speedBoostCounter <= 0) {
            speedBoostActive = 0;
            snake.speed = snake.baseSpeed;
            GotoXY(55, 11);
            printf("加速:关闭   ");
        }
    }
}

/* ========== 游戏主循环 ========== */
void GameLoop() {
    while (MoveSnake()) {
        HandleInput();
    }
}

/* ========== 主函数 ========== */
int main() {
    // 设置随机种子
    srand((unsigned int)time(NULL));

    // 设置控制台标题
    SetConsoleTitle(TEXT("贪吃蛇 - 排行榜版"));

    // 设置控制台大小
    system("mode con cols=90 lines=45");

    // 初始化默认难度
    currentDifficulty = DIFFICULTY_MEDIUM;
    InitDifficultyParams();

    // 加载排行榜记录
    LoadRecords();

    while (1) {
        int choice = Menu();

        if (choice == 1) {  // 开始游戏
            InitMap();
            GameLoop();
        }
        else if (choice == 2) {  // 选择难度
            SelectDifficulty();
        }
        else if (choice == 3) {  // 排行榜
            ShowRanking();
        }
        else if (choice == 4) {  // 帮助
            Help();
        }
        else if (choice == 5) {  // 关于
            About();
        }
        else {  // 退出
            system("cls");
            GotoXY(45, 15);
            printf("感谢游玩，再见！");
            Sleep(1500);
            break;
        }
    }
    return 0;
}