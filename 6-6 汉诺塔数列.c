#include <stdio.h>

double NumHanoi(int index);

int main()
{
    int n;
    scanf("%d", &n);
    printf("%.15g\n", NumHanoi(n));
    return 0;
}

/* 你提交的代码将被嵌在这里 */
double NumHanoi(int index)
{
    if(index == 1)
        return 1;
    else
        return NumHanoi(index - 1) * 2 + 1;
}
