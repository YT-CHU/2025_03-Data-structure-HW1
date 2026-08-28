#include <stdio.h>
#include <stdlib.h>

void changePtr(int *ptr) {
    int a = 20;
    ptr = &a;  // 只改變了拷貝，不影響原本的指標
}

int main() {
    int b = 10;
    int *p = &b;

    changePtr(p);

    printf("%d\n", *p);  // 還是 10，沒變
    return 0;
}