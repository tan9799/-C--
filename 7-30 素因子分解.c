#include<stdio.h>
#include<math.h>

int main()
{
    long int N;
    scanf("%ld", &N);
    printf("%d=", N);
    if(N == 1)
    {
        printf("1");
        return 0;
    }
    int is_first = 1;

    int count_2 = 0;
    while(N % 2 == 0)
    {
        count_2++;
        N /= 2;
    }
    if(count_2 > 0)
    {
        if(count_2 == 1)
            printf("2");
        else
            printf("2^%d", count_2);
        is_first = 0;
    }
    for(long int i = 3; i <= sqrt(N); i += 2) {
        int count = 0;
        while(N % i == 0) {
            count++;
            N /= i;
        }

        if(count > 0) {
            if(!is_first) {
                printf("*");
            }
            if(count == 1) {
                printf("%ld", i);
            } else {
                printf("%ld^%d", i, count);
            }
            is_first = 0;
        }
    }
    if(N > 1) {
        if(!is_first) {
            printf("*");
        }
        printf("%ld", N);
    }

    return 0;
}
