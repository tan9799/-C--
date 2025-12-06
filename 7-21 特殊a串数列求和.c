int result(int a, int n)
{
    if(n == 1)
    {
        return a;
    }
    else
    {
        return result(a, n - 1) * 10 + a;
    }
}

int calculate(int a, int n)
{
    if(n == 1)
    {
        return a;
    }
    else
        return result(a, n) + calculate(a, n -1);
}
#include<stdio.h>
int main()
{
    int a, n;
    scanf("%d %d", &a, &n);
    int ret = calculate(a, n);
    printf("s = %d", ret);
    return 0;
}
