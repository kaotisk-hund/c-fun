#include <stdio.h>

int main(void)
{
    unsigned int             a = 0xffffffff;
    long unsigned int        b = 0xffffffffffffffff;
    long long unsigned int   c = 0xffffffffffffffff;

    printf("u   \t0x%016x  \t(%lu)\n", a, sizeof(a));
    printf("lu  \t0x%016lx \t(%lu)\n", b, sizeof(b));
    printf("llu \t0x%016llx\t(%lu)\n", c, sizeof(c));
    printf("llu \t%llu\t(%lu)\n", c, sizeof(c));

    double d = 0xffffffffffffffff;
    printf("llu \t%f\t(%lu)\n", d, sizeof(d));


    return 0;
}
