#include <stdio.h>
int f(int **p) {

    printf("**p:\t%p, val: %08lx\n", p, (unsigned long)*p);
    int j = 100;
    *p = &j;
    return 0;
}

int main() {
    int i = 10;
    int *p;
    p = &i;
    printf("p:\t%p, *p: %d\n", p, *p);

    int ret;
    ret = f(&p);
    printf("p:\t%p, *p: %d\n", p, *p);
}
