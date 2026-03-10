#include "snake.h"

/* 全局变量定义 */
Snake snake;                    // 蛇
Food foods[20];                 // 食物数组（最大容量20）
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

/* ========== 难度参数定义 ========== */
void InitDifficultyParams() {
    switch (currentDifficulty) {
    case DIFFICULTY_EASY:
        diffParams.baseSpeed = 300;        // 慢
        diffParams.minFoodCount = 4;       // 食物较多
        diffParams.maxFoodCount = 10;
        diffParams.changeFrequency = 80;   // 变化慢
        diffParams.speedIncreaseRate = 1;  // 速度增加慢
        diffParams.name = "简单";
        break;

    case DIFFICULTY_MEDIUM:
        diffParams.baseSpeed = 250;        // 中等
        diffParams.minFoodCount = 3;
        diffParams.maxFoodCount = 12;
        diffParams.changeFrequency = 50;   // 中等
        diffParams.speedIncreaseRate = 2;
        diffParams.name = "中等";
        break;

    case DIFFICULTY_HARD:
        diffParams.baseSpeed = 200;        // 快
        diffParams.minFoodCount = 2;
        diffParams.maxFoodCount = 15;
        diffParams.changeFrequency = 30;   // 变化快
        diffParams.speedIncreaseRate = 3;
        diffParams.name = "困难";
        break;

    case DIFFICULTY_EXPERT:
        diffParams.baseSpeed = 150;        // 非常快
        diffParams.minFoodCount = 1;
        diffParams.maxFoodCount = 20;
        diffParams.changeFrequency = 20;   // 变化非常快
        diffParams.speedIncreaseRate = 4;
        diffParams.name = "专家";
        break;
    }
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
    GotoXY(40, 10);
    printf("╔══════════════════════╗");
    GotoXY(40, 11);
    printf("║  欢迎来到贪吃蛇游戏  ║");
    GotoXY(40, 12);
    printf("╠══════════════════════╣");
    GotoXY(40, 13);
    printf("║                      ║");
    GotoXY(40, 14);
    printf("║  1. 开始游戏         ║");
    GotoXY(40, 15);
    printf("║  2. 选择难度         ║");
    GotoXY(40, 16);
    printf("║  3. 帮助             ║");
    GotoXY(40, 17);
    printf("║  4. 关于             ║");
    GotoXY(40, 18);
    printf("║                      ║");
    GotoXY(40, 19);
    printf("║  其他键退出游戏      ║");
    GotoXY(40, 20);
    printf("╚══════════════════════╝");
    Hide();

    char ch;
    ch = _getch();
    system("cls");

    switch (ch) {
    case '1': return 1;  // 开始游戏
    case '2': return 2;  // 选择难度
    case '3': return 3;  // 帮助
    case '4': return 4;  // 关于
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
    printf("║    • 动态食物数量            ║");
    GotoXY(35, 17);
    printf("║    • 4级难度选择             ║");
    GotoXY(35, 18);
    printf("║    • 暂停功能                ║");
    GotoXY(35, 19);
    printf("║    • X键加速                 ║");
    GotoXY(35, 20);
    printf("╚══════════════════════════════╝");

    GotoXY(40, 22);
    printf("按任意键返回");
    Hide();
    char ch = _getch();
    system("cls");
}

/* ========== 帮助 ========== */
void Help() {
    system("cls");
    GotoXY(35, 5);
    printf("════════════ 游戏操作说明 ════════════");

    GotoXY(35, 7);
    printf("🎮 基本操作：");
    GotoXY(35, 8);
    printf("   W - 向上移动");
    GotoXY(35, 9);
    printf("   S - 向下移动");
    GotoXY(35, 10);
    printf("   A - 向左移动");
    GotoXY(35, 11);
    printf("   D - 向右移动");

    GotoXY(35, 13);
    printf("⚡ 特殊功能：");
    GotoXY(35, 14);
    printf("   X - 速度加快（5秒）");
    GotoXY(35, 15);
    printf("   P - 暂停/继续游戏");

    GotoXY(35, 17);
    printf("📊 难度说明：");
    GotoXY(35, 18);
    printf("   简单：速度慢，食物多且稳定");
    GotoXY(35, 19);
    printf("   中等：速度中等，食物动态变化");
    GotoXY(35, 20);
    printf("   困难：速度快，食物变化快");
    GotoXY(35, 21);
    printf("   专家：极快，食物疯狂变化");

    GotoXY(35, 23);
    printf("按任意键返回主菜单");

    Hide();
    char ch = _getch();
    system("cls");
}

/* ========== 检查食物是否和蛇重叠 ========== */
int CheckFoodCollision(int x, int y) {
    for (int i = 0; i < snake.length; i++) {
        if (snake.snakeNode[i].x == x && snake.snakeNode[i].y == y) {
            return 1;  // 重叠
        }
    }
    return 0;  // 不重叠
}

/* ========== 增加一个食物 ========== */
void AddOneFood() {
    if (CURRENT_FOOD_COUNT >= diffParams.maxFoodCount) return;

    // 找到第一个不活跃的食物槽位
    int index = -1;
    for (int i = 0; i < diffParams.maxFoodCount + 5; i++) {
        if (!foods[i].active) {
            index = i;
            break;
        }
    }
    if (index == -1) return;  // 没有空位

    // 生成新食物
    int flag = 1;
    while (flag) {
        flag = 0;
        foods[index].x = rand() % (MAP_WIDTH - 2) + 1;
        foods[index].y = rand() % (MAP_HEIGHT - 2) + 1;

        // 检查是否和蛇重叠
        if (CheckFoodCollision(foods[index].x, foods[index].y)) {
            flag = 1;
            continue;
        }

        // 检查是否和其他活跃食物重叠
        for (int j = 0; j < diffParams.maxFoodCount + 5; j++) {
            if (j != index && foods[j].active &&
                foods[j].x == foods[index].x && foods[j].y == foods[index].y) {
                flag = 1;
                break;
            }
        }
    }

    foods[index].active = 1;
    GotoXY(foods[index].x, foods[index].y);
    printf("$");
    CURRENT_FOOD_COUNT++;
}

/* ========== 减少一个食物 ========== */
void RemoveOneFood() {
    if (CURRENT_FOOD_COUNT <= diffParams.minFoodCount) return;

    // 随机选择一个活跃的食物移除
    int activeIndices[20];
    int activeCount = 0;

    for (int i = 0; i < diffParams.maxFoodCount + 5; i++) {
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

        GotoXY(52, 9);
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
    GotoXY(52, 8);
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
    for (int i = 0; i < diffParams.maxFoodCount + 5; i++) {
        foods[i].active = 0;
    }

    // 生成初始食物
    for (int i = 0; i < CURRENT_FOOD_COUNT; i++) {
        int flag = 1;
        while (flag) {
            flag = 0;
            foods[i].x = rand() % (MAP_WIDTH - 2) + 1;
            foods[i].y = rand() % (MAP_HEIGHT - 2) + 1;

            // 检查是否和蛇重叠
            if (CheckFoodCollision(foods[i].x, foods[i].y)) {
                flag = 1;
                continue;
            }

            // 检查是否和其他食物重叠
            for (int j = 0; j < i; j++) {
                if (foods[j].active &&
                    foods[j].x == foods[i].x && foods[j].y == foods[i].y) {
                    flag = 1;
                    break;
                }
            }
        }
        foods[i].active = 1;
        GotoXY(foods[i].x, foods[i].y);
        printf("$");
    }
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

    // 显示游戏信息
    GotoXY(52, 3);
    printf("═══════════════");
    GotoXY(52, 4);
    printf("难度: %s", diffParams.name);
    GotoXY(52, 5);
    printf("得分:0");
    GotoXY(52, 6);
    printf("范围:%d-%d", diffParams.minFoodCount, diffParams.maxFoodCount);
    GotoXY(52, 7);
    printf("速度:%dms", snake.speed);
    GotoXY(52, 8);
    printf("当前:%d", CURRENT_FOOD_COUNT);
    GotoXY(52, 9);
    printf("目标:%d", TARGET_FOOD_COUNT);
    GotoXY(52, 10);
    printf("加速:关闭");
    GotoXY(52, 11);
    printf("状态:运行");
    GotoXY(52, 13);
    printf("操作:");
    GotoXY(52, 14);
    printf("P暂停  X加速");
    GotoXY(52, 15);
    printf("═══════════════");
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

        GotoXY(52, 11);
        printf("状态:暂停");
    }
    else {
        // 清除暂停信息
        for (int i = 18; i <= 21; i++) {
            GotoXY(45, i);
            printf("                        ");
        }

        GotoXY(52, 11);
        printf("状态:运行");

        // 重绘食物（防止被暂停框覆盖）
        for (int i = 0; i < diffParams.maxFoodCount + 5; i++) {
            if (foods[i].active) {
                GotoXY(foods[i].x, foods[i].y);
                printf("$");
            }
        }

        // 重绘蛇
        for (int i = 0; i < snake.length; i++) {
            GotoXY(snake.snakeNode[i].x, snake.snakeNode[i].y);
            if (i == 0) printf("@");
            else printf("o");
        }
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
                speedBoostCounter = 20;  // 加速持续20个移动周期
                snake.speed = snake.speed / 2;  // 速度加倍
                GotoXY(52, 10);
                printf("加速:开启  ");
            }
            break;
        }
    }
}

/* ========== 应用加速效果 ========== */
void ApplySpeedBoost() {
    if (speedBoostActive && !isPaused) {  // 暂停时不计时
        speedBoostCounter--;
        if (speedBoostCounter <= 0) {
            speedBoostActive = 0;
            snake.speed = snake.baseSpeed;
            GotoXY(52, 10);
            printf("加速:关闭   ");
        }
    }
}

/* ========== 蛇移动 ========== */
int MoveSnake() {
    if (isPaused) {
        Sleep(100);  // 暂停时降低CPU占用
        return 1;    // 不移动，但继续循环
    }

    Snakenode temp;
    int flag = 0;

    // 记录蛇尾
    temp = snake.snakeNode[snake.length - 1];

    // 蛇身向前移动
    for (int i = snake.length - 1; i >= 1; i--)
        snake.snakeNode[i] = snake.snakeNode[i - 1];

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
    for (int i = 0; i < diffParams.maxFoodCount + 5; i++) {
        if (foods[i].active &&
            snake.snakeNode[0].x == foods[i].x && snake.snakeNode[0].y == foods[i].y) {
            // 吃到食物
            foods[i].active = 0;  // 食物被吃
            GotoXY(foods[i].x, foods[i].y);
            printf(" ");  // 擦除食物

            snake.length++;  // 蛇变长
            flag = 1;
            snake.snakeNode[snake.length - 1] = temp;  // 蛇尾保留
            foodEatenCount++;

            GotoXY(52, 5);
            printf("得分:%d   ", foodEatenCount);
            CURRENT_FOOD_COUNT--;  // 当前食物数量减少

            GotoXY(52, 7);
            printf("速度:%dms", snake.speed);
            GotoXY(52, 8);
            printf("当前:%d   ", CURRENT_FOOD_COUNT);
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

    // 检查是否撞墙或撞到自己
    if (!IsCorrect()) {
        system("cls");
        GotoXY(45, 14);
        printf("══════════════════════");
        GotoXY(45, 15);
        printf("     游戏结束！");
        GotoXY(45, 16);
        printf("     难度: %s", diffParams.name);
        GotoXY(45, 17);
        printf("     最终得分：%d", foodEatenCount);
        GotoXY(45, 18);
        printf("══════════════════════");
        GotoXY(45, 20);
        printf("按任意键返回主菜单");
        char c = _getch();
        system("cls");
        return 0;
    }

    // 调整速度
    SpeedControl();
    ApplySpeedBoost();  // 处理加速效果
    Sleep(snake.speed);
    return 1;
}

/* ========== 碰撞检测 ========== */
int IsCorrect() {
    // 撞墙检测
    if (snake.snakeNode[0].x == 0 || snake.snakeNode[0].y == 0 ||
        snake.snakeNode[0].x == MAP_WIDTH - 1 || snake.snakeNode[0].y == MAP_HEIGHT - 1)
        return 0;

    // 撞自己检测
    for (int i = 1; i < snake.length; i++) {
        if (snake.snakeNode[0].x == snake.snakeNode[i].x &&
            snake.snakeNode[0].y == snake.snakeNode[i].y)
            return 0;
    }
    return 1;
}

/* ========== 速度控制 ========== */
void SpeedControl() {
    // 根据蛇长和难度调整基础速度
    int speedDecrease = (snake.length - 3) * diffParams.speedIncreaseRate;
    int newBaseSpeed = diffParams.baseSpeed - speedDecrease;

    // 设置最小速度限制
    if (newBaseSpeed < 50) newBaseSpeed = 50;

    // 如果基础速度改变，更新速度
    if (newBaseSpeed != snake.baseSpeed) {
        snake.baseSpeed = newBaseSpeed;
        if (!speedBoostActive) {
            snake.speed = snake.baseSpeed;
        }
        GotoXY(52, 7);
        printf("速度:%dms", snake.speed);
    }
}

/* ========== 游戏主循环 ========== */
void GameLoop() {
    while (MoveSnake()) {
        HandleInput();  // 处理输入（包括暂停键）
    }
}

/* ========== 主函数 ========== */
int main() {
    // 设置随机种子
    srand((unsigned int)time(NULL));

    // 设置控制台标题
    SetConsoleTitle(TEXT("贪吃蛇 - 4级难度 + 动态食物 + 暂停功能"));

    // 设置控制台大小
    system("mode con cols=80 lines=40");

    // 初始化默认难度
    currentDifficulty = DIFFICULTY_MEDIUM;
    InitDifficultyParams();

    while (1) {
        int choice = Menu();

        if (choice == 1) {  // 开始游戏
            InitMap();
            GameLoop();
        }
        else if (choice == 2) {  // 选择难度
            SelectDifficulty();
        }
        else if (choice == 3) {  // 帮助
            Help();
        }
        else if (choice == 4) {  // 关于
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