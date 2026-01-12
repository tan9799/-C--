#include <stdio.h>
int add(int n);
int main()
{
  int n=100;
  printf("1+2+3+...+100=%d",add(n));
  return 0;
}


/* 请在这里填写答案 */
int add(int n)
{
	if(n <= 1)
	{
		return 1; 
	}
	else
	{
		return n + add( n - 1 );
	}
}
