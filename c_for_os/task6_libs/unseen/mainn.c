#include <stdio.h>
#include "math.h"

int main(void) {
    int s = subtract(10, 4);
    int p = power(2, 3);

    printf("subtract(10, 4) = %d\n", s);
    printf("power(2, 3)     = %d\n", p);
    return 0;
}