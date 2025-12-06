#include<stdio.h>
int main()
{
    int jia_beer_amount, yi_beer_amount;
    scanf("%d %d", &jia_beer_amount, &yi_beer_amount);
    int N;
    scanf("%d", &N);
    int count_jia = 0;
    int count_yi = 0;
    int arr[100][4];
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            scanf("%d", &arr[i][j]);
        }
    }
    for(int i = 0; i < N; i++)
    {
        if (arr[i][0] + arr[i][2] == arr[i][1] && arr[i][0] + arr[i][2] != arr[i][3])
        {
            count_jia++;
            if(count_jia > jia_beer_amount)
            {
                printf("A\n%d", count_yi);
                return 0;
            }
        }
        else if(arr[i][0] + arr[i][2] == arr[i][3] && arr[i][0] + arr[i][2] != arr[i][1])
        {
            count_yi++;
            if(count_yi > yi_beer_amount)
            {
                printf("B\n%d", count_jia);
                return 0;
            }
        }
    }
}
