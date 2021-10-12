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

    int bb = 1;
    for (int i = 0; i < 200; i++)
    {
        printf("===%d\n", bb);
        if (bb++ > 9) {
            bb = 1;
        }
    }
    

    return 0;
}