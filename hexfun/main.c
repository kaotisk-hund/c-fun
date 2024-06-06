#include <stdio.h>

int main(){
    unsigned int i = 69;
    printf("%10u, 0x%8x, 0b%32b\n", i, i, i);
    i = 420;
    printf("%10u, 0x%8x, 0b%32b\n", i, i, i);
    i = i << 1;
    printf("%10u, 0x%8x, 0b%32b\n", i, i, i);
    i = i >> 2;
    printf("%10u, 0x%8x, 0b%32b\n", i, i, i);
    i = i ^ 0xfffffffffffff;
    printf("%10u, 0x%8x, 0b%32b\n", i, i, i);
    i = 0xffffffff;
    printf("%10u, 0x%8x, 0b%32b\n", i, i, i);
    long unsigned int l = 0xffffffffffffffff;
    printf("Size: %lu\n", sizeof(l));
    printf("%20lu, 0x%16lx, 0b%64lb\n", l, l, l);
    l = 100000000000;
    printf("%20lu, 0x%16lx, 0b%64lb\n", l, l, l);
    return 0;
}

