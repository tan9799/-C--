#include <stdio.h>
# include <math.h>

unsigned int reverse( unsigned int number );

int main()
{
    unsigned int n;

    scanf("%u", &n);
    printf("%u\n", reverse(n));

    return 0;
}


/* 请在这里填写答案 */
unsigned int reverse( unsigned int number )
{
    int count = 0;
    if (number <= 9)
        return number;
    unsigned int temp = number;
    while(temp > 0)
    {
        count++;
        temp /= 10;
    }
    int last = number % 10;
    unsigned int remaining = number / 10;
    unsigned int reversed_remaining = reverse(remaining);
    return last * (unsigned int)pow(10, count - 1) + reversed_remaining;
}
