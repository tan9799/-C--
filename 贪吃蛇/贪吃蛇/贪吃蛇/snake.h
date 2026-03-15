#ifndef SNAKE_H
#define SNAKE_H

#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

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

// 排行榜相关定义
#define MAX_RECORDS 10     // 最多保存10条记录
#define NAME_LENGTH 20     // 玩家姓名长度

#define MAX_PROP_COUNT 20      // 最大道具数量
#define PROP_GENERATE_RATE 60  // 道具生成概率（百分比）
#define PROP_GENERATE_INTERVAL 50 // 道具生成间隔（帧数）

// 道具效果持续时间（帧数）
#define SHIELD_DURATION 150     // 护盾持续150帧
#define MAGNET_DURATION 200     // 磁铁持续200帧
#define INVISIBLE_DURATION 120  // 隐身持续120帧
#define SLOW_DURATION 180       // 减速持续180帧
#define FREEZE_DURATION 100     // 冰冻持续100帧

// 磁铁范围
#define MAGNET_RANGE 4

#define SHIELD_DURATION 150     // 护盾持续150帧
#define MAGNET_DURATION 200     // 磁铁持续200帧
#define INVISIBLE_DURATION 120  // 隐身持续120帧
#define SLOW_DURATION 180       // 减速持续180帧
#define FREEZE_DURATION 100     // 冰冻持续100帧

// 道具生成相关
#define MAX_PROP_COUNT 20       // 最大道具数量
#define PROP_GENERATE_RATE 60   // 道具生成概率
#define PROP_GENERATE_INTERVAL 50 // 道具生成间隔

// 食物类型枚举
typedef enum {
    FOOD_NORMAL = 0,    // 普通食物 $
    FOOD_BIG = 1,       // 大食物 @ (生长+3)
    FOOD_FAST = 2,      // 速长食物 * (生长+5)
    FOOD_GOLDEN = 3,    // 黄金食物 & (得分+5，生长+1)
    FOOD_MAGIC = 4      // 魔法食物 % (随机效果)
} FoodType;

// 食物结构
typedef struct {
    int x;              // 食物横坐标
    int y;              // 食物纵坐标
    FoodType type;      // 食物类型
    int active;         // 食物是否存在（1=存在，0=已被吃）
    int growValue;      // 生长值（吃后增加几节）
    int scoreValue;     // 分值
    char symbol;        // 显示符号
    int color;          // 颜色
    int timer;          // 存在时间（特殊食物会消失）
} Food;

// 道具类型枚举
typedef enum {
    PROP_SHIELD,     // 护盾 🛡️
    PROP_MAGNET,     // 磁铁 🧲
    PROP_INVISIBLE,  // 隐身 👻
    PROP_SLOW,       // 减速 🐢
    PROP_FREEZE      // 冰冻 ❄️
} PropType;

// 道具结构体
typedef struct {
    int x, y;
    int active;
    int timer;           // 持续时间或消失倒计时
    PropType type;
    char symbol;         // 显示符号
    int color;           // 颜色
    int effectDuration;  // 效果持续时间（帧数）
} Prop;

// 玩家状态结构体
typedef struct {
    int shieldActive;     // 护盾激活
    int shieldTimer;      // 护盾剩余时间
    int magnetActive;     // 磁铁激活
    int magnetTimer;      // 磁铁剩余时间
    int magnetRange;      // 磁铁范围
    int invisibleActive;  // 隐身激活
    int invisibleTimer;   // 隐身剩余时间
    int slowActive;       // 减速激活
    int slowTimer;
    int freezeActive;     // 冰冻激活
    int freezeTimer;
} PlayerStatus;

// 障碍物数量（根据难度动态调整）
#define MAX_OBSTACLE_COUNT 30

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
    int obstacleCount;       // 障碍物数量
    const char* name;        // 难度名称
} DifficultyParams;

// 蛇身节点结构
typedef struct {
    int x;  // 横坐标
    int y;  // 纵坐标
} Snakenode;

// 蛇结构
typedef struct {
    Snakenode snakeNode[200];  // 蛇身节点数组（最多200节）
    int length;                 // 当前蛇的长度
    int speed;                  // 移动速度（毫秒）
    int baseSpeed;              // 基础速度（用于恢复）
} Snake;

// 障碍物结构
typedef struct {
    int x;           // 障碍物横坐标
    int y;           // 障碍物纵坐标
    int active;      // 是否活跃（1=存在，0=不存在）
} Obstacle;

// 排行榜记录结构
typedef struct {
    char name[NAME_LENGTH];     // 玩家姓名
    int score;                   // 得分
    char difficulty[20];         // 难度
    char date[20];               // 日期时间
} ScoreRecord;

// 全局变量声明
extern int CURRENT_FOOD_COUNT;   // 当前实际食物数量
extern int TARGET_FOOD_COUNT;    // 目标食物数量（会随机变化）
extern int foodChangeTimer;      // 食物数量变化计时器
extern int isPaused;             // 暂停状态
extern Difficulty currentDifficulty;  // 当前难度
extern DifficultyParams diffParams;   // 当前难度参数
extern Obstacle obstacles[MAX_OBSTACLE_COUNT]; // 障碍物数组
extern int obstacleCount;        // 当前障碍物数量
extern Food foods[30];           // 食物数组（扩大容量）
extern ScoreRecord records[MAX_RECORDS]; // 排行榜记录
extern int recordCount;          // 当前记录数量

// 函数声明
void GotoXY(int x, int y);      // 光标定位
void Hide();                    // 隐藏光标
void SetColor(int color);       // 设置颜色
int Menu();                     // 主菜单
void About();                   // 关于
void Help();                    // 帮助
Difficulty SelectDifficulty();  // 选择难度
void InitDifficultyParams();    // 初始化难度参数
void InitMap();                 // 初始化地图
void InitFoods();               // 初始化所有食物
void InitObstacles();           // 初始化障碍物
void UpdateFoodCount();         // 更新食物数量（动态调整）
void AddOneFood();              // 增加一个食物
void RemoveOneFood();           // 减少一个食物
FoodType GetRandomFoodType();   // 随机获取食物类型
void ApplyFoodEffect(FoodType type, int* addLength, int* addScore); // 应用食物效果
int CheckFoodCollision(int x, int y, int ignoreIndex); // 检查食物是否和蛇重叠
int CheckObstacleCollision(int x, int y); // 检查是否撞到障碍物
int MoveSnake();                // 蛇移动
int IsCorrect();                // 碰撞检测（只检测撞墙）
void SpeedControl();            // 速度控制
void ApplySpeedBoost();         // 应用加速效果
void PauseGame();               // 暂停游戏
void HandleInput();             // 处理输入

// 自撞截尾相关函数
int FindCollisionIndex();       // 查找碰撞点索引
void CutTail(int collideIndex); // 截断蛇尾

// 排行榜相关函数
void LoadRecords();             // 从文件加载记录
void SaveRecords();             // 保存记录到文件
void AddRecord(int score);      // 添加新记录
void ShowRanking();             // 显示排行榜
void SortRecords();             // 对记录排序
void GetCurrentDateTime(char* buffer); // 获取当前日期时间
void InputPlayerName(char* name); // 输入玩家姓名

#endif