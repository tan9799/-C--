#ifndef SNAKE_H
#define SNAKE_H

#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

// 方向定义（WASD）
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'

// 功能键
#define SPEED_BOOST 'x'  // 加速
#define PAUSE 'p'        // 暂停
#define PAUSE_CAP 'P'    // 大写P也支持

// 地图大小
#define MAP_WIDTH 50      // 地图宽度
#define MAP_HEIGHT 25     // 地图高度

// 难度级别
typedef enum {
    DIFFICULTY_EASY = 0,    // 简单
    DIFFICULTY_MEDIUM = 1,  // 中等
    DIFFICULTY_HARD = 2,    // 困难
    DIFFICULTY_EXPERT = 3   // 专家
} Difficulty;

// 难度参数结构
typedef struct {
    int baseSpeed;           // 基础速度（毫秒）
    int minFoodCount;        // 最少食物数
    int maxFoodCount;        // 最多食物数
    int changeFrequency;     // 食物变化频率（帧数）
    int speedIncreaseRate;   // 速度增加速率
    const char* name;        // 难度名称
} DifficultyParams;

// 蛇身节点结构
typedef struct {
    int x;  // 横坐标
    int y;  // 纵坐标
} Snakenode;

// 蛇结构
typedef struct {
    Snakenode snakeNode[100];  // 蛇身节点数组（最多100节）
    int length;                 // 当前蛇的长度
    int speed;                  // 移动速度（毫秒）
    int baseSpeed;              // 基础速度（用于恢复）
} Snake;

// 食物结构
typedef struct {
    int x;           // 食物横坐标
    int y;           // 食物纵坐标
    int active;      // 食物是否存在（1=存在，0=已被吃）
} Food;

// 全局变量声明
extern int CURRENT_FOOD_COUNT;   // 当前实际食物数量
extern int TARGET_FOOD_COUNT;    // 目标食物数量（会随机变化）
extern int foodChangeTimer;      // 食物数量变化计时器
extern int isPaused;             // 暂停状态
extern Difficulty currentDifficulty;  // 当前难度
extern DifficultyParams diffParams;   // 当前难度参数

// 函数声明
void GotoXY(int x, int y);      // 光标定位
void Hide();                    // 隐藏光标
int Menu();                     // 主菜单
void About();                   // 关于
void Help();                    // 帮助
Difficulty SelectDifficulty();  // 选择难度
void ApplyDifficulty();         // 应用难度设置
void InitMap();                 // 初始化地图
void InitFoods();               // 初始化所有食物
void UpdateFoodCount();         // 更新食物数量（动态调整）
void AddOneFood();              // 增加一个食物
void RemoveOneFood();           // 减少一个食物
int CheckFoodCollision(int x, int y); // 检查食物是否和蛇重叠
int MoveSnake();                // 蛇移动
int IsCorrect();                // 碰撞检测
void SpeedControl();            // 速度控制
void ApplySpeedBoost();         // 应用加速效果
void PauseGame();               // 暂停游戏
void HandleInput();             // 处理输入

#endif