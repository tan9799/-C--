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
int search( int n )
{
    int sum = 0;
    for(int i = 101; i < n + 1; i++)
    {
        int sqrt_i = (int)sqrt(i);
        if(sqrt_i * sqrt_i == i)
        {
            int gewei = i % 10;
            int shiwei = i / 10 % 10;
            int baiwei = i / 100;
            if(gewei == shiwei || shiwei == baiwei || baiwei == gewei)
            {
                sum++;
            }
        }
    }
    return sum;
}
