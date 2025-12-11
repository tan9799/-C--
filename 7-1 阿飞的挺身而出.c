#include <stdio.h>
#include <string.h>

#define MAX_LEN 31

int main() {
    char num1[MAX_LEN], num2[MAX_LEN];
    char result[MAX_LEN + 1] = {0};

    scanf("%s %s", num1, num2);

    int len1 = strlen(num1);
    int len2 = strlen(num2);
    int max_len = (len1 > len2) ? len1 : len2;

    int i = len1 - 1;
    int j = len2 - 1;
    int k = max_len;
    int carry = 0;

    while (i >= 0 || j >= 0 || carry) {
        int digit1 = (i >= 0) ? num1[i] - '0' : 0;
        int digit2 = (j >= 0) ? num2[j] - '0' : 0;

        int sum = digit1 + digit2 + carry;
        carry = sum / 10;
        result[k] = (sum % 10) + '0';

        if (i >= 0) i--;
        if (j >= 0) j--;
        k--;
    }

    if (k < 0) {
        printf("%s\n", result);
    } else {
        printf("%s\n", result + k + 1);
    }

    return 0;
}
