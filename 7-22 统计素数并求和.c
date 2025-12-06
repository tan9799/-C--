#include<stdio.h>
int main()
{
    int M, N;
    int num = 0;
    int count =0;
    int flag =0;
    scanf("%d %d", &M, &N);
    for(int i = M; i <= N; i++)
    {
        flag = 0;
        if(i < 2)
        {
            flag = 1;
            continue;
        }
        for(int j =2; j < i; j++)
        {
            if(i % j == 0)
            {
                flag = 1;
                break;
            }
        }
        if(flag == 0)
        {
            num++;
            count += i;
        }
    }
    printf("%d %d", num, count);
    return 0;
}
