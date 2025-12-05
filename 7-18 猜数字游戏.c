#include<stdio.h>
int main()
{
    int yonghucai = 0;
    int youxichansheng = 0;
    int N = 0;
    int guess = 0;
    scanf("%d %d", &youxichansheng, &N);
    while(N > guess)
    {
        scanf("%d", &yonghucai);
        guess++;
        if(yonghucai < 0)
        {
            printf("Game Over\n");
            return 0;
        }
        if(yonghucai > youxichansheng)
        {
            printf("Too big\n");
        }
        else if(yonghucai < youxichansheng)
        {
            printf("Too small\n");
        }
        else
        {
            if(guess == 1)
            {
                printf("Bingo!\n");
            } else if(guess <= 3)
            {
                printf("Lucky You!\n");
            } else if(guess <= N)
            {
                printf("Good Guess!\n");
            }
            return 0;
        }
    }
    printf("Game Over\n");
    return 0;
}
