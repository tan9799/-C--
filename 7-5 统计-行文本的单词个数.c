#include<stdio.h>
#include<string.h>
int main()
{
    char str[100000];
    int count = 0;
    int flag = 0;
    gets(str);
    for(int i = 0; str[i] != '\0'; i++) {
        if(str[i] != ' ') {
            if(!flag) {
                count++;
                flag = 1;
            }
        } else {
                flag = 0;
        }
    }
    printf("%d\n", count);
    return 0;
}
