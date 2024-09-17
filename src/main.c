#include <stdio.h>

int inc(int* value) {
    return 0;
}

int main(void) {
    int value = 0;
    printf("%d", (value = 5) == 5);
    printf("%d", value);
    return 0;
}