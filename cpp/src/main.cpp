#include <stdio.h>
#include "../include/plus.hpp"

void test() {
    static u_int8_t a = 0;
    printf("\n");
    if (a == 0) {
        printf("000\n");
    } else if (a == 1) {
        printf("111\n");
    }
    a++;
}

int main() {
    printf("hello world! %d", plus(3, 4));

    test();
    test();
    return 0;
}