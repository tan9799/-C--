#include <stdio.h>
#include <stdlib.h>

// 比较函数，用于qsort
int compare(const void *a, const void *b)
{
    return (*(int*)a - *(int*)b);
}

int main()
{
    int N, i;
    scanf("%d", &N);
    
    int arr[1000];
    for(i = 0; i < N; i++)
    {
        scanf("%d", &arr[i]);
    }
    
    // 对数组进行排序
    qsort(arr, N, sizeof(int), compare);
    
    int max_count = 1;       // 最大出现次数
    int current_count = 1;   // 当前数字出现次数
    int most_number = arr[0]; // 出现次数最多的数字
    
    // 遍历排序后的数组
    for(i = 1; i < N; i++)
    {
        if(arr[i] == arr[i-1])
        {
            current_count++;  // 相同数字，计数增加
        }
        else
        {
            // 不同数字，检查是否需要更新最大值
            if(current_count > max_count)
            {
                max_count = current_count;
                most_number = arr[i-1];
            }
            current_count = 1;  // 重置计数
        }
    }
    
    // 检查最后一组数字
    if(current_count > max_count)
    {
        max_count = current_count;
        most_number = arr[N-1];
    }
    
    printf("%d %d", most_number, max_count);
    return 0;
}
