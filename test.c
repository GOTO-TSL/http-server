#include <stdio.h>
#include <unistd.h>

int add_ten(int x);

void add_ten(int *x) {
    *x += 10;
}

int main(void) {
    int a = 10;
    int *p = &a;
    add_ten(p);
    printf("%d\n", a);
    return 0;
}