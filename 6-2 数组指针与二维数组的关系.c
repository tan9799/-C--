#include <stdio.h>
int main()
{
    void reOut(int (*p)[3], int n, int m);
    int i, j, a[2][3], (*p)[3];
    for (i = 0; i < 2; i++)
        for (j = 0; j < 3; j++)
        scanf("%d", &a[i][j]);
    p= a;
    reOut(p, 2, 3);
}
/* 请在这里填写答案 */
void reOut(int (*p)[3], int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%-3d", p[i][j]);
        }
        printf("\n");
    }
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n; i++) {
            printf("%-3d", p[i][j]);
        }
        printf("\n");
    }
}
