#include <stdio.h>

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int lcm(int a, int b) {
    return a / gcd(a, b) * b;
}

int main() {
    int n, i;
    
    while (scanf("%d", &n) != EOF) {
        int num, gcd_result, lcm_result;
        
        scanf("%d", &num);
        gcd_result = num;
        lcm_result = num;
        
        for (i = 1; i < n; i++) {
            scanf("%d", &num);
            gcd_result = gcd(gcd_result, num);
            lcm_result = lcm(lcm_result, num);
        }
        
        printf("%d %d\n", gcd_result, lcm_result);
    }
    
    return 0;
}
