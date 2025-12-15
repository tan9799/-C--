#include <stdio.h>
#include <string.h>
void fsort(char *color[ ], int n);

int main(void )
{
    int i;
    char *pcolor[ ] = {"red", "blue", "yellow", "green", "black"};
    void fsort(char *color[ ], int n);

    fsort(pcolor, 5);   /* 调用函数 */
    for(i = 0; i < 5; i++)
        printf("%s ", pcolor[i]);
    printf("\n");
    return 0;
}


/* 请在这里填写答案 */
void fsort(char *color[ ], int n)
{
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < n - i - 1; j++)
        {
            if(strcmp(*(color + j), *(color + j + 1)) > 0)
            {
                char *temp = *(color + j);
                *(color + j) = *(color + j + 1);
                *(color + j + 1) = temp;
            }
        }
    }
}
