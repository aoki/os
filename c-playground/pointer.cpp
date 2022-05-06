#include <stdio.h>

int main() {
    printf("# Hello Pointer\n");

    int i = 42;
    int *p = &i;

    printf("var\t%-14s\t%-12s\t%-12s\n", "addr", "val", "deref");
    printf("i\t%p\t%12d\t%-s\n", &i, i, "-");
    printf("*p\t%p\t%08lx\t%12d\n", &p, (unsigned long)p, *p);

    int r1 = *p;
    *p = 1;
    int r2 = i;

    printf("r1\t%p\t%12d\t%-s\n", &r1, r1, "-");
    printf("r2\t%p\t%12d\t%-s\n", &r2, r2, "-");

    uintptr_t addr = reinterpret_cast<uintptr_t>(p);
    int *q = reinterpret_cast<int *>(addr);
    printf("addr\t%p\t%08lx\t\n", &addr, addr);
    printf("*q\t%p\t%08lx\t%12d\n", &q, (unsigned long)q, *q);
}
