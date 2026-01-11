#include<stdio.h>
int main()
{
    char a[7][10]={"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};
    char *p;
    char (*q)[10];//定义指针变量q为指向行的指针变量，用来指向数组a的每一行。
    int i,x;
    scanf("%d",&x);
//第一次输出星期x的英文单词前三个字母 
    q= a + (x - 1);
    for(i=0; i<3; i++)
        printf("%c", (*q)[i]);
    printf("\n");
//第二次输出星期x的英文单词前三个字母
    p= a[x - 1];
    for(i=0; i<3; i++)
        printf("%c",p[i]);
    return 0;
}
