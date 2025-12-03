#include<stdio.h>
#define ROW 3
#define COL 3
#include<stdlib.h>
#include<time.h>
void menu()
{
    printf("********************************\n");
    printf("*****   1. play    0. exit     ******\n");
    printf("********************************\n");
}
//初始化棋盘
void InitBoard(char board[ROW][COL], int row, int col)
{
    int i = 0;
    int j = 0;
    for(i = 0; i < row; i++)
    {
        for(j = 0; j < col; j++)
        {
            board[i][j] = ' ';
        }
    }
}
//打印棋盘
void DisplayBoard(char board[ROW][COL], int row, int col)
{
    int i = 0;
    for(i = 0; i < row; i++)
    {
        //打印数据
        int j = 0;
        for(j = 0; j < col; j++)
        {
            printf(" %c ", board[i][j]);
            if(j < col - 1)
                printf(" | ");
        }
        printf("\n");
        //打印分割信息
        if(i < row - 1)
        {
            int j = 0;
            for(j = 0; j < col; j++)
            {
                printf("---");
                if(j < col - 1)
                    printf(" | ");
            }
            printf("\n");
        }
    }
}
//玩家下棋
void PlayerMove(char board[ROW][COL], int row, int col)
{
    int x = 0;
    int y = 0;
    printf("玩家下棋:>\n");
    while (1)
    {
        printf("请输入坐标:>");
        scanf("%d %d", &x, &y);
        //坐标范围合法的判断
        if(x >= 1 && x <= row && y >= 1 && y <= col)
        {
            if (board[x - 1][y - 1] == ' ')
            {
                board[x - 1][y - 1] = '*';
                break;
            }
            else
            {
                printf("坐标被占用,不能下棋，请选择其他位置\n");
            }
        }
        else
        {
            printf("坐标非法，请重新输入\n");
        }
    }
}
//电脑下棋
//找没有下棋的位置随机下
void ComputerMove(char board[ROW][COL], int row, int col)
{
    printf("电脑下棋:>\n");
    int x = 0;
    int y = 0;
    while(1)
    {
        x = rand() % row;//0~2
        y = rand() % col;//0~2
        if(board[x][y] == ' ')
        {
            board[x][y] = '#';
            break;
        }
    }
}
//如果满了就返回1
//如果不满就返回0
int IsFull(char board[ROW][COL], int row, int col)
{
    int i = 0;
    int j = 0;
    for(i = 0; i < row; i++)
    {
        for(j = 0; j < col; j++)
        {
            if(board[i][j] == ' ')
            {
                return 0;
            }
        }
    }
}
//判断输赢
//玩家赢('*') 电脑赢('#') 平局('Q') 继续('C')
char IsWin(char board[ROW][COL], int row, int col)
{
    //列
    int j = 0;
    for(j = 0; j < row; j++)
    {
        if(board[0][j] == board[1][j] && board[1][j] == board[2][j] && board[1][j] != ' ')
        {
            return board[1][j];
        }
    }
    //行
    int i = 0;
    for(i = 0; i < row; i++)
    {
        if(board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][1] != ' ')
        {
            return board[i][1];
        }
    }
    //对角线
    if(board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[1][1] != ' ')
    {
        return board[1][1];
    }
    if(board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[1][1] != ' ')
    {
        return board[1][1];
    }
    //没有人赢，就要平局
    if (IsFull(board, row, col))
    {
        return 'Q';
    }
    //游戏继续
    return 'C';
}
void game()
{
    char board[3][3] = { 0 };
    char ret = 'C';
    //初始化棋盘的功能
    InitBoard(board, ROW, COL);
    DisplayBoard(board, ROW, COL);
    //下棋
    //ComputerMove();
    while(1)
    {
        PlayerMove(board, ROW, COL);
        //判断输赢
        ret = IsWin(board, ROW, COL);
        if(ret != 'C')
        {
            break;
        }
        DisplayBoard(board, ROW, COL);
        ComputerMove(board, ROW, COL);
        //判断输赢
        ret = IsWin(board, ROW, COL);
        if(ret != 'C')
        {
            break;
        }
        DisplayBoard(board, ROW, COL);
    }
    if (ret == '*')
    {
        printf("玩家赢\n");
    }
    else if (ret == '#')
    {
        printf("电脑赢\n");
    }
    else
    {
        printf("平局\n");
    }
    DisplayBoard(board, ROW, COL);
}
int main()
{
    srand((unsigned int)time(NULL));//设置随机数的生成起点
    int input = 0;
    do
    {
        menu();//打印菜单
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
                printf("选择错误！\n");
                break;
        }
    } while (input);
    return 0;
}
