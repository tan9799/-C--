# include <stdio.h>
void swap ( int *pa, int *pb );

int main( )
{
    int a, b;

    scanf ( "%d%d", &a, &b);
    swap (&a, &b);
    printf("%d %d\n", a, b);
}

void swap ( int *pa, int *pb )
{
    int temp; /* 定义整型变量 temp */

    temp = *pa, *pa = *pb, *pb = temp;
}
