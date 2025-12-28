#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <string.h>
#include <conio.h>

// 颜色定义
#define RED 12
#define GREEN 10
#define YELLOW 14
#define CYAN 11
#define MAGENTA 13
#define WHITE 15
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10

// 特质数组
const char* traits[] = {
    "生活有序", "从容", "清醒", "蜕变", "理性",
    "情绪稳定", "平和", "自治", "睡眠安稳", "坚韧",
    "自由", "自信", "有盼头", "常欢喜", "明媚",
    "勇敢", "人格舒展", "明辨是非", "坦率", "松弛",
    "笑常伴", "自愈力强", "无忧无忧", "突破局限", "幸福",
    "幸运", "富贵有余", "生财有道", "平安", "自律",
    "相处无压", "八方来财", "没有烦恼", "独立", "生病有道",
    "温柔坚定", "一帆风顺", "从容不迫", "坦荡", "保持热爱",
    "顺遂无忧", "事顺遂", "快乐", "远见敏锐", "情自治",
    "身自在", "开阔", "做自己", "沉稳清醒", "有分寸",
    "自足", "通透", "乐无忧", "情安稳", "少遗憾",
    "财运旺"
};

const char* wishes[] = {
    "希望2026的你...",
    "平安喜乐",
    "万事顺遂",
    "心想事成",
    "前程似锦",
    "健康长寿",
    "财源广进",
    "笑口常开"
};

// 设置控制台颜色
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// 设置光标位置
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// 清屏
void clearScreen() {
    system("cls");
}

// 绘制边框
void drawBorder() {
    setColor(CYAN);

    // 上边框
    gotoxy(0, 0);
    for(int i = 0; i < 80; i++) printf("═");

    // 下边框
    gotoxy(0, 24);
    for(int i = 0; i < 80; i++) printf("═");

    // 左边框
    for(int i = 1; i < 24; i++) {
        gotoxy(0, i);
        printf("║");
    }

    // 右边框
    for(int i = 1; i < 24; i++) {
        gotoxy(79, i);
        printf("║");
    }

    // 四个角
    gotoxy(0, 0); printf("╔");
    gotoxy(79, 0); printf("╗");
    gotoxy(0, 24); printf("╚");
    gotoxy(79, 24); printf("╝");
}

// 绘制标题
void drawTitle() {
    setColor(YELLOW);
    gotoxy(30, 2);
    printf("✨ 2026 新 年 快 乐 ✨");

    setColor(LIGHT_BLUE);
    gotoxy(35, 3);
    printf("Happy New Year");

    // 装饰线
    setColor(MAGENTA);
    gotoxy(20, 4);
    for(int i = 0; i < 40; i++) printf("━");
}

// 显示特质（轮播效果）
void showTraits() {
    static int traitIndex = 0;
    static int colorIndex = 0;
    static int frame = 0;

    int colors[] = {GREEN, CYAN, YELLOW, MAGENTA, LIGHT_GREEN, RED};

    // 每10帧切换一个特质
    if(frame % 10 == 0) {
        // 清除上一特质显示区域
        setColor(BLACK);
        for(int y = 7; y <= 18; y++) {
            gotoxy(15, y);
            for(int i = 0; i < 50; i++) printf(" ");
        }

        // 随机选择5个特质显示
        for(int i = 0; i < 5; i++) {
            int randTrait = (traitIndex + i) % (sizeof(traits)/sizeof(traits[0]));
            int row = 7 + i * 2;
            int col = 15;

            // 随机颜色
            int traitColor = colors[(colorIndex + i) % 6];
            setColor(traitColor);

            gotoxy(col, row);
            printf("✦ %s", traits[randTrait]);
        }

        traitIndex = (traitIndex + 5) % (sizeof(traits)/sizeof(traits[0]));
        colorIndex = (colorIndex + 1) % 6;
    }

    frame++;
}

// 显示愿望
void showWishes() {
    static int wishIndex = 0;
    static int wishTimer = 0;

    if(wishTimer++ >= 30) {
        wishTimer = 0;
        wishIndex = (wishIndex + 1) % (sizeof(wishes)/sizeof(wishes[0]));

        // 清除愿望显示区域
        setColor(BLACK);
        gotoxy(25, 20);
        for(int i = 0; i < 30; i++) printf(" ");

        // 显示新愿望
        setColor(LIGHT_BLUE);
        gotoxy(25, 20);
        printf("%s", wishes[wishIndex]);
    }
}

// 显示计数器
void showCounters() {
    static int counter1 = 126;
    static int counter2 = 288;
    static int timer = 0;

    if(timer++ >= 5) {
        timer = 0;
        counter1++;
        counter2++;

        setColor(YELLOW);
        gotoxy(60, 21);
        printf("祝福: %d", counter1);

        gotoxy(60, 22);
        printf("点赞: %d", counter2);
    }
}

// 显示底部信息
void showBottomInfo() {
    setColor(GREEN);
    gotoxy(20, 22);
    printf("生活有序 • 从容淡定 • 情绪稳定 • 保持热爱");

    setColor(CYAN);
    gotoxy(25, 23);
    printf("愿2026的你，成为更好的自己！");
}

// 显示烟花特效
void showFireworks() {
    static int fireworkPos = 0;
    static int fireworkState = 0;

    char fireworkChars[] = {'*', '+', 'o', 'x', '@', '#'};

    // 清除旧烟花
    setColor(BLACK);
    for(int x = 5; x < 75; x++) {
        gotoxy(x, 5);
        printf(" ");
        gotoxy(x, 6);
        printf(" ");
    }

    // 绘制新烟花
    setColor(RED + fireworkState);
    int x = 10 + fireworkPos;
    gotoxy(x, 5);
    printf("%c", fireworkChars[fireworkPos % 6]);

    // 烟花尾迹
    setColor(YELLOW);
    for(int i = 1; i <= 3; i++) {
        gotoxy(x, 5 + i);
        printf("|");
    }

    fireworkPos = (fireworkPos + 1) % 60;
    fireworkState = (fireworkState + 1) % 8;
}

// 主函数
int main() {
    // 设置控制台窗口
    system("mode con cols=80 lines=25");
    system("title 2026新年特质展示");

    // 隐藏光标
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    printf("\n\n正在加载2026新年祝福...\n");
    Sleep(2000);
    clearScreen();

    // 主循环
    while(1) {
        // 检查按键
        if(_kbhit()) {
            char key = _getch();
            if(key == 27 || key == 'q' || key == 'Q') break; // ESC或Q退出
            if(key == ' ') {
                // 空格键切换模式
                setColor(YELLOW);
                gotoxy(30, 1);
                printf("✨ 新年快乐！继续前行 ✨");
            }
        }

        // 绘制界面
        drawBorder();
        drawTitle();
        showTraits();
        showWishes();
        showFireworks();
        showCounters();
        showBottomInfo();

        // 控制帧率
        Sleep(100);
    }

    // 显示退出信息
    clearScreen();
    setColor(YELLOW);
    printf("\n\n\n");
    printf("    ╔══════════════════════════════════════════════════╗\n");
    printf("    ║                                                  ║\n");
    printf("    ║         感谢使用2026新年特质展示程序           ║\n");
    printf("    ║                                                  ║\n");
    printf("    ║       愿你在2026年：                            ║\n");
    printf("    ║       拥有所有美好的特质                        ║\n");
    printf("    ║       实现所有的愿望                            ║\n");
    printf("    ║       成为最好的自己！                          ║\n");
    printf("    ║                                                  ║\n");
    printf("    ║       www.1dly.cn • 44x.top/100                 ║\n");
    printf("    ║                                                  ║\n");
    printf("    ╚══════════════════════════════════════════════════╝\n\n");

    setColor(WHITE);

    // 恢复光标
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    return 0;
}
