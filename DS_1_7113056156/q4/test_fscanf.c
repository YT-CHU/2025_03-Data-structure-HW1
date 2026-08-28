#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *file = fopen("ss.txt", "r");
    if (!file) {
        printf("fail to open file\n");
        return 1;
    }

    int num, num1;
    if (fscanf(file, "%d", &num) == 1) {
        printf("讀取的數字是: %d\n", num);
    } else {
        printf("未能成功讀取數字\n");
    }

    if (fscanf(file, "%d", &num1) == 1) {
        printf("讀取的數字是: %d\n", num1);
    } else {
        printf("未能成功讀取數字\n");
    }

    fclose(file);
    return 0;
}