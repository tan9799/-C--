#include <stdio.h>
#include <math.h>
int isprime(int n)
{
	int i, flag = 0;
	for(i = 2; i <= sqrt(n); i++)
	{
		if(n % i == 0)
		{
			flag = 1;
			break; 
		}
	}
	if(flag == 0)
		return n;
	else
		return 0;
}
int main()
{
	int n, k = 0, j, m = 0, sum = 0;
	scanf("%d %d", &n, &k);
	for(j = n; j >=2; j--)
	{
		if(isprime(j) != 0 && m < k)
		{
			if(m == 0)
				printf("%d", j);
			else
				printf("+%d", j);
			m++;
			sum += j;
		}
	}
	printf("=%d", sum);
	return 0;
} 
