//扫雷游戏
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#define ROW 9
#define COL 9
#define ROWS ROW+2
#define COLS COL+2
#define EASY_COUNT 10
void menu()
{
    printf("**********************\n");
    printf("*****   1. play    *****\n");
    printf("*****   0. exit     *****\n");
    printf("**********************\n");
}

void InitBoard(char board[ROWS][COLS], int rows, int cols, char set)
{
    int i = 0;
    int j = 0;
    for(i = 0; i < rows; i++)
    {
        for(j = 0; j < cols; j++)
        {
            board[i][j] = set;
        }
    }
}

void DisPlayBoard(char board[ROWS][COLS], int row, int col)
{
    int i = 0;
    int j = 0;
    printf("-------扫雷游戏-------\n");
    for(j = 0; j <= col; j++)
    {
        printf("%d ", j);
    }
    printf("\n");
    for( i = 1; i <= row; i++)
    {
        printf("%d ", i);
        for(j = 1; j <= col; j++)
        {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
    printf("-------扫雷游戏-------\n");
}

void SetMine(char board[ROWS][COLS], int row, int col)
{
    int count = EASY_COUNT;
    //1~9
    //1~9
    while(count)
    {
        int x = rand() % row + 1;
        int y = rand() % col + 1;
        if(board[x][y] == '0')
        {
            board[x][y] = '1';
            count--;
        }
    }
}

int get_mine_count(char board[ROWS][COLS], int x, int y)
{
    return (board[x - 1][y] +
        board[x - 1][y - 1] +
        board[x][y - 1] +
        board[x + 1][y - 1] +
        board[x + 1][y] +
        board[x + 1][y + 1] +
        board[x][y + 1] +
        board[x - 1][y + 1] - 8 * '0');

}
void FindMine(char mine[ROWS][COLS], char show[ROWS][COLS], int row, int col)
{
    int x = 0;
    int y = 0;
    int win = 0;//找到非雷的个数
    while(win < row * col - EASY_COUNT)
    {
        printf("请输入要排查的坐标:>");
        scanf("%d%d", &x, &y);
        if(x >= 1 && x <= row && y >= 1 && y <= col)
        {
            if(show[x][y] != '*')
            {
                printf("该坐标被排查过了，不能重复排查\n");
            }
            else
            {
                //如果是雷
                if(mine[x][y] == '*')
                {
                    printf("很遗憾，你被炸死了\n");
                    DisPlayBoard(mine, ROW, COL);
                    break;
                }
                else//如果不是雷
                {
                    win++;
                    //统计mine数组中x，y坐标周围有几个雷
                    int count = get_mine_count(mine, x, y);
                    show[x][y] = count + '0';//转换成数字字符
                    DisPlayBoard(show, ROW, COL);
                }
            }
        }
        else
        {
            printf("输入的坐标非法，请重新输入:>");
        }
    }
    if(win == row * col - EASY_COUNT)
    {
        printf("恭喜你，排雷成功\n");
        DisPlayBoard(mine, ROW, COL);
    }
}
void game()
{
    char mine[ROWS][COLS] = { 0 };//存放布置好的雷的信息
    char show[ROWS][COLS] = { 0 };//存放排查出的雷的信息
    //初始化数组的内容为指定的内容
    //mine 数组在没有布置雷的时候，都是‘0’
    InitBoard(mine, ROWS, COLS, '0');
    //show 数组在没有排查雷的时候，都是‘*’
    InitBoard(show, ROWS, COLS, '*');

    //DisPlayBoard(mine, ROW, COL);
    //设置雷
    SetMine(mine, ROW, COL);
    DisPlayBoard(show, ROW, COL);
    //排查雷
    FindMine(mine, show, ROW, COL);
}
int main()
{
    int input = 0;
    //设置随机数的生成起点
    srand((unsigned int)time(NULL));
    do
    {
        menu();
        printf("请选择:>");
        scanf("%d", &input);
        switch(input)
        {
            case 1:
                game();
                break;
            case 0:
                printf("退出游戏\n");
                break;
            default:
                printf("选择错误\n");
                break;
        }
    } while(input);
    return 0;
}
//基础的功能
//1.标记功能
//2.当排查（x，y）坐标的时候：该坐标不是雷，该坐标周围没有雷，该坐标没有被排查过，展开一片的功能

