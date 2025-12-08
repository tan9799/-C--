#include <stdio.h>

double Fac(int x);

int main()
{
    int n;
    scanf("%d", &n);
    printf("%.14g\n", Fac(n));
    return 0;
}

/* 你提交的代码将被嵌在这里 */
double Fac(int x)
{
    if(x == 0)
        return 1;
    else if(x == 1)
        return 1;
    else
        return x * Fac(x - 1);
}
