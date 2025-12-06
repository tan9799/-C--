#include<stdio.h>
#include<math.h>
int main()
{
    int N;
    scanf("%d", &N);
    int sum = 0;
    int result = pow(10, N - 1);
    int divisors[10] = { 1 };
    for(int j = 1; j <= N; j++)
        divisors[j] = divisors[j - 1] * 10;
    for(int i = result; i < 10 * result; i++)
    {
        sum = 0;
        for(int j = 0; j <= N - 1; j++)
        {
            int a = (i / divisors[j]) % 10;
            sum += pow(a, N);
        }
        if(sum == i)
        {
            printf("%d\n", i);
        }
    }
    return 0;
}
