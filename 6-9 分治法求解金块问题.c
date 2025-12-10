#include <stdio.h>
#define MAXN 101

int max(int a[ ], int m, int n);
int min(int a[ ], int m, int n);

int main(void)
{
    int i, n;
    int a[MAXN];

    scanf ("%d", &n);
    if(n >= 2 && n <= MAXN-1 ){
        for(i = 0; i < n; i++){
            scanf ("%d", &a[i]);
        }
        printf("max = %d\n", max(a, 0, n-1));
        printf("min = %d\n", min(a, 0, n-1));
    }else{
        printf("Invalid Value.\n");
    }

    return 0;
}


/* 请在这里填写答案 */
int max(int a[ ], int m, int n)
{
    if(m == n)
        return a[m];
    int mid = (m + n) / 2;
    int left_max = max(a, m , mid);
    int right_max = max(a, mid + 1, n);
    return (left_max > right_max) ? left_max : right_max;
}
int min(int a[ ], int m, int n)
{
    if(m == n)
        return a[m];
    int mid = (m + n) / 2;
    int left_min = min(a, m, mid);
    int right_min = min(a, mid + 1, n);
    return (left_min > right_min) ? right_min : left_min;
}
