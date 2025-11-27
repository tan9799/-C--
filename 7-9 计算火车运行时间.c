#include <stdio.h>
#include <stdlib.h>

int main()
{
    int start, end, hour1, hour2, minute1, minute2, hour3, minute3;
    scanf("%d %d", &start, &end);
    hour1 = start / 100;
    minute1 = start % 100;
    hour2 = end / 100;
    minute2 = end % 100;
    hour3 = hour2 - hour1;
    minute3 = minute2 - minute1;
    if (hour3 < 10) {
        if (minute3 >= 60) {
            printf("%02d:%02d", hour3 + 1, minute3 - 60);
        } else if  (minute3 < 0) {
            printf("%02d:%02d", hour3 - 1, minute3 + 60);
        } else {
            printf("%02d:%02d", hour3, minute3);
        }
    } else {
         if (minute3 >= 60) {
            printf("%02d:%02d", hour3 + 1, minute3 - 60);
        } else if  (minute3 < 0) {
            printf("%02d:%02d", hour3 - 1, minute3 + 60);
        } else {
            printf("%02d:%02d", hour3, minute3);
        }
    }
    return 0;
}
