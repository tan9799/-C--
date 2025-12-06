#include<stdio.h>
#include<math.h>
double fantan(double height, int n)
{
    if(n == 0)
    {
        return height;
    }
    else
    {
        return fantan(height / 2.0, n -1);
    }
}

double sum(double height, int n)
{
    if(n == 1)
    {
        return height;
    }
    else
    {
        return sum(height / 2.0, n - 1) + height / 2 + height;
    }
}

int main()
{
    long long h, n;
    scanf("%lld %lld", &h, &n);
    if(n == 0)
    {
        printf("0.0 0.0\n");
        return 0;
    }
    double distance, fantan_height;
    double height = (double)h;
    distance = sum(height, n);
    fantan_height = fantan(height, n);
    printf("%.1f %.1f", distance, fantan_height);
    return 0;
}
