#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    int a, b, c;
    scanf("%d %d %d", &a, &b, &c);
    if ((a + b <= c) || (a + c <= b ) || (b + c <= a)) {
        printf("These sides do not correspond to a valid triangle");
    } else {
        double s = (a + b + c) / 2.00;
        double area = sqrt(s * (s - a) * (s - b) * (s - c));
        double perimeter = a +b + c;
        printf("area = %.2f; perimeter = %.2f", area, perimeter);
    }
    return 0;
}
