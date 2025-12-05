#include<stdio.h>
int main()
{
    int a = 0;
    int count = 0;
    printf("请输入一个整数:");
    scanf("%d", &a);
    int temp = a;
    for(int i = 0; i < 32; i++)
    {
        if(a & 1)
        {
            count++;
        }
        a = a >> 1;
    }
    printf("数字 %d 的二进制补码中有 %d 个1\n", temp, count);
    return 0;
}
