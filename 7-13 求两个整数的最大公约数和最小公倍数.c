#include <stdio.h>

int big(int n, int m)
{
	int i, max;
	if(n > m)
	{
		max = n;
	}
	else
	{
		max = m;
	}
	for(i = max; i <= n * m; i++)
	{
		if (i % m == 0 && i % n == 0)
		{
			return i;
		}	
	}	
}
int small(int n, int m)
{
	int i, min;
	if(n > m)
	{
		min = m;
	}
	else
	{
		min = n;
	}
	for(i = min; i > 0; i--)
	{
		if (m % i == 0 && n % i == 0)
		{
			return i;
		}	
	}	
}
int main()
{
	int a, b;
	scanf("%d %d", &a, &b);
	printf("large_n=%d,least_n=%d", small(a, b), big(a, b));
	return 0;
}
