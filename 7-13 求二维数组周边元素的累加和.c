#include<stdio.h>
int main()
{
    int hang;
    int sum = 0;
    int lie;
    int arr[10][10];
    scanf("%d %d", &hang, &lie);
    for(int i = 0; i < hang; i++) {
        for(int j = 0; j < lie; j++) {
            scanf("%d", &arr[i][j]);
            if(i == 0 || j == 0 || i == hang - 1 || j == lie - 1){
                sum += arr[i][j];
            }
        }
    }
    printf("%d\n", sum);
    return 0;
}
