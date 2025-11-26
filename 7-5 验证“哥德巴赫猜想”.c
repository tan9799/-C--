#include<stdio.h>
#include<math.h>
int is_prime(int n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (int i = 3; i <= sqrt(n); i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}
int main() {
    int N;
    scanf("%d", &N);
    for(int p = 2; p <= N/2; p++){
        int q = N - p;
        if (is_prime(p) && is_prime(q)) {
                printf("%d = %d + %d\n", N, p, q);
                return 0;
        }
    }
    return 0;
}
