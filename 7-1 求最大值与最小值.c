#include<stdio.h>
void max_min(double arr[], int n, double *max, double *min);
int main()
{
    int n = 0;
    scanf("%d", &n);
    double arr[n];
    for(int i = 0; i < n; i ++)
    {
        scanf("%lf", &arr[i]);
    }
    double max, min;
    max_min(arr, n, &max, &min);
    printf("%.2f\n%.2f", max, min);
    return 0;
}
void max_min(double arr[], int n, double *max, double *min)
{
    *max = arr[0];
    *min = arr[0];
    for(int i = 1; i < n; i++)
    {
        if(arr[i] > *max)
            *max = arr[i];
        if(arr[i] < *min)
            *min = arr[i];
    }
}
