#include<stdio.h>
#include<stdlib.h>
#include<time.h>
//1.用电脑产生一个个随机数（1~100）
//2.猜数字
//3.猜大了
//4.猜小了
//5.直到猜对了，结束
void menu()
{
    printf("*****************************\n");
    printf("*********   1. play   *********\n");
    printf("*********   0. exit    *********\n");
    printf("*****************************\n");
}

//0~RAND_MAX(32767)
void game()
{
    int guess = 0;
    //1.生成随机数
    //0~99 -- > 1~100
    int ret = rand()%100+1;//生成随机数的函数
    //2.猜数字
    while (1)
    {
        printf("请猜数字：>");
        scanf("%d", &guess);
        if (guess < ret)
            printf("猜小了\n");
        else if (guess > ret)
            printf("猜大了\n");
        else
        {
            printf("恭喜你，猜对了\n");
            break;
        }
    }
}

//指针
//int *p = NULL;(空指针)
//int a = 0;
int main()
{
    int input = 0;
    srand((unsigned int)time(NULL));
    //使用time函数返回的时间戳作为随机数的生成的起点
    do{
        menu();
        printf("请选择:>");
        scanf("%d", &input);
        switch (input)
        {
        case 1:
            game();//猜数字的整个逻辑
            break;
        default:
            printf("选择错误，重新选择！\n");
            break;
        case 0:
            printf("退出游戏\n");
            break;
        }
    } while (input);
    return 0;
}
