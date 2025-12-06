#include<stdio.h>
int main()
{
    int n;
    int count = 0;
    scanf("%d", &n);
    for(int i = 1; i < 1000; i++)
    {
        if ((i % 10 + i / 10 % 10 + i / 100 % 10) == n)
        {
            count++;
            printf("%8d", i);
            if(count % 6 == 0)
            {
            printf("\n");
            }
        }
    }
    return 0;
}
