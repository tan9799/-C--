#include <stdio.h>
#include <string.h>
int main()
{
	char arr[81];
	fgets(arr, sizeof(arr), stdin);
	int N;
	scanf("%c", &N);
	int i;
	int k = 0;
	int len = strlen(arr);
    if (len > 0 && arr[len-1] == '\n') {
        arr[len-1] = '\0';
        len--;
    }
	for(i = 0; arr[i] != '\0'; i++)
	{
		if(arr[i] == N)
		{
			k++;
		}
	}
	printf("%d", k);
	return 0;
}
