#include <stdio.h>
#include <math.h>

int search( int n );

int main()
{
    int number;

    scanf("%d",&number);
    printf("count=%d\n",search(number));

    return 0;
}


/* 你的代码将被嵌在这里 */
int search(int n)
{
    int count = 0;
    for(int i = 101; i <= n; i++)
    {
        int gewei = i % 10;
        int shiwei = i / 10 % 10;
        int baiwei = i / 100;
        if((gewei == shiwei || shiwei == baiwei || baiwei == gewei) && ((int)sqrt(i) * (int)sqrt(i) == i))
        {
            count++;
        }
    }
    return count;
}
