#include <stdio.h>

int main()
{
    int arr1[20], arr2[20], result[40];
    int N1, N2, i, j, k;
    int flag, found, count = 0;
    
    // 读取第一个数组
    scanf("%d", &N1);
    for(i = 0; i < N1; i++)
    {
        scanf("%d", &arr1[i]);
    }
    
    // 读取第二个数组
    scanf("%d", &N2);
    for(i = 0; i < N2; i++)
    {
        scanf("%d", &arr2[i]);
    }
    
    // 找出在arr1中但不在arr2中的元素
    for(i = 0; i < N1; i++)
    {
        flag = 0;
        // 检查arr1[i]是否在arr2中
        for(j = 0; j < N2; j++)
        {
            if(arr1[i] == arr2[j])
            {
                flag = 1;
                break;
            }
        }
        
        // 如果arr1[i]不在arr2中，且之前没存过
        if(flag == 0)
        {
            found = 0;
            for(k = 0; k < count; k++)
            {
                if(result[k] == arr1[i])
                {
                    found = 1;
                    break;
                }
            }
            if(!found)
            {
                result[count] = arr1[i];
                count++;
            }
        }
    }
    
    // 找出在arr2中但不在arr1中的元素
    for(i = 0; i < N2; i++)
    {
        flag = 0;
        // 检查arr2[i]是否在arr1中
        for(j = 0; j < N1; j++)
        {
            if(arr2[i] == arr1[j])
            {
                flag = 1;
                break;
            }
        }
        
        // 如果arr2[i]不在arr1中，且之前没存过
        if(flag == 0)
        {
            found = 0;
            for(k = 0; k < count; k++)
            {
                if(result[k] == arr2[i])
                {
                    found = 1;
                    break;
                }
            }
            if(!found)
            {
                result[count] = arr2[i];
                count++;
            }
        }
    }
    
    // 输出结果
    if(count > 0)
    {
        printf("%d", result[0]);
        for(i = 1; i < count; i++)
        {
            printf(" %d", result[i]);
        }
    }
    
    return 0;
}
