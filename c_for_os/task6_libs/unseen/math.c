#include "math.h"

int subtract(int a, int b) {
    return a - b;
}

int power(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}