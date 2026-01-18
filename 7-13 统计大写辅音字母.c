#include <stdio.h>
#include <string.h>

int main()
{
    char arr[81];  // 不超过80个字符，加1给结束符
    int k = 0;
    int i;
    
    // 读取一行字符串
    fgets(arr, sizeof(arr), stdin);
    
    // 遍历字符串
    for(i = 0; i < strlen(arr); i++)
    {
        // 判断是否是大写字母
        if(arr[i] >= 'A' && arr[i] <= 'Z')
        {
            // 排除元音字母 A、E、I、O、U
            if(arr[i] != 'A' && arr[i] != 'E' && arr[i] != 'I' && 
               arr[i] != 'O' && arr[i] != 'U')
            {
                k++;
            }
        }
    }
    
    // 输出结果
    printf("%d\n", k);
    
    return 0;	
}
