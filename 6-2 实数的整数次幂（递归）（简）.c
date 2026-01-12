#include <stdio.h>

double Power(double x, int n);

int main()
{
    double x;
    int n;
    scanf("%lg%d", &x, &n);
    printf("%.8g\n", Power(x, n));
    return 0;
}

/* 你提交的代码将被嵌在这里 */
double Power(double x, int n)
{
	if(n == 1)
	{
		return x;
	}
	if(n == 0)
	{
		return 1.0;
	}
	else
	{
		return x * Power(x, n - 1);
	}
}
