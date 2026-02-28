#include <stdio.h>
#include <string.h>

int main()
{
	int n, i;
	scanf("%d", &n);
	int arr[11];
	for(i = 0; i < n; i++)
	{
		scanf("%d", &arr[i]);
	}
	int start = 0;
	int end = n - 1;
	while(start < end)
	{
		int temp = arr[start];
		arr[start] = arr[end];
		arr[end] = temp;
		start++;
		end--;
	}
	printf("%d", arr[0]);
	for(i = 1; i < n; i++)
	{
		printf(" %d", arr[i]);
	}
	return 0;
}
