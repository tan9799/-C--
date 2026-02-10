#include <stdio.h>

void reverse(int n)
{
	while(n != 0)
	{
		printf("%d", n % 10);
		n = n / 10;
    }
}
int main()
{
	int n;
	scanf("%d", &n);
	reverse(n);
	return 0;
}
