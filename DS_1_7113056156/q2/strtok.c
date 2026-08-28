#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char line[] = "1 2 3 4 5";  // 範例字串，這裡是空格分隔的數字
    int cpu[5];  // 假設最多有 5 個數字
    int i = 0;
    char *token;

    // 使用 strtok 分割字串，取得每一個數字
    token = strtok(line, " ");  // 以空格為分隔符分割字串
    while (token != NULL) {
        cpu[i] = atoi(token);  // 將字串轉換為整數並存入陣列
        i++;
        token = strtok(NULL, " ");  // 繼續分割下一個字串
    }

    // 顯示結果
    for (int j = 0; j < i; j++) {
        printf("cpu[%d] = %d\n", j, cpu[j]);
    }

    return 0;
}