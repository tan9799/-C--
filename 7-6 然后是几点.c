#include <stdio.h>
#include <stdlib.h>

int main()
{
    int start, duration, hour, minute, a, b, c, d;
    scanf("%d %d", &start, &duration);
    hour = start / 100;
    minute = start - hour * 100;
    a = duration / 60;
    b = duration % 60;
    c = hour + a;
    d = minute + b;
    if (d >= 60) {
        c += 1;
        d -= 60;
    } else if (d < 0) {
        c -= 1;
        d += 60;
    }
    if (c < 0) {
        c +=24;
    }
    printf("%d%02d", c, d);
    return 0;
}
