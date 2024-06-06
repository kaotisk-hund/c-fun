#include <stdio.h>

int main(void){
    unsigned long int c = 100000000000;
    printf("%ld\n", c);
    printf("Size: %li\n", sizeof(c));
    printf("%li\n", c-1);
    return 0;
}
