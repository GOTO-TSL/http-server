#include <stdio.h>
#include <unistd.h>

int main(void) {
    char a[1024];
    int read_result;
    read_result = read(0, &a, sizeof(a));
    printf("%s\n", a);
    return 0;
}