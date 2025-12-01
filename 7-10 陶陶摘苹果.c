#include<stdio.h>
int main()
{
    int sum = 0;
    int arr[10] = { 0 };
    int reach = 0;
    for(int i = 0; i < 10; i++) {
        scanf("%d", &arr[i]);
    }
    scanf("%d", &reach);
    for(int i = 0; i < 10; i++) {
        if (arr[i] <= reach + 30) {
            sum++;
        }
    }
    printf("%d\n", sum);
    return 0;
}
