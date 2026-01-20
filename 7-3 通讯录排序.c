#include<stdio.h>
#include<string.h>

struct friends
{
    char name[11];
    char birth[9];
    char tele[18];
};

int main()
{
    int n, i, j;
    scanf("%d", &n);
    struct friends fri[n];
    
    for(i = 0; i < n; i++)
    {
        scanf("%s %s %s", fri[i].name, fri[i].birth, fri[i].tele);
    }
    
    for(i = 0; i < n-1; i++)
    {
        for(j = 0; j < n-1-i; j++)
        {
            if(strcmp(fri[j].birth, fri[j+1].birth) > 0)
            {
                struct friends temp = fri[j];
                fri[j] = fri[j+1];
                fri[j+1] = temp;
            }
        }
    }
    
    for(i = 0; i < n; i++)
    {
        printf("%s %s %s\n", fri[i].name, fri[i].birth, fri[i].tele);
    }
    
    return 0;
}
