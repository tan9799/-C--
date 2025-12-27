#include <stdio.h>

double fn( double x, int n );

int main()
{
    double x;
    int n;

    scanf("%lf %d", &x, &n);
    printf("%.2f\n", fn(x,n));

    return 0;
}

/* 你的代码将被嵌在这里 */
double fn( double x, int n )
{
    double pow = 1.0;
    if(n == 1)
        return x;
    else
    {
        for(int i = 0; i < n; i++)
            pow *= x;
    }
        if ((n - 1) % 2 == 0)
            return fn(x, n - 1) + pow;
        else
            return fn(x, n - 1) + (-1) * pow;
}
