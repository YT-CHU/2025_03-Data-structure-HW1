#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//C(1, N , i) = max{選到第一個num_1 + L(3 , N-1, i-1) , 沒選到第一個 L(S+1, N, i)} //環狀
//L(S, N, i) = max{L(S+2, N, i-1) + num_s, L(S+1, N, i) } //(N-S+1)/2 < i 即沒有解 || i = 1 即選最大

int L(int *array, int start, int last, int num){
    int first_get, first_nget;
    int max = -9999;
    if(num == 1){
        for(int i = start; i < last; i++){
            if( array[i] > max) max = array[i];
        }
    }
    else if ((last - start +1)/2 < num){
    }
    else{
        first_get = L(array, start+2, last, num-1) + array[start];
        first_nget = L(array, start+1, last, num);
        max = (first_get > first_nget) ? first_get : first_nget;
    }
    return max;
}



int main(){
    FILE *file= fopen("testcase4.txt", "r");
    FILE *outfile = fopen("output4.txt", "w");
    if(!file){
        printf("fail to open file\n");
        exit(1);
    }
    int testcase;
    fscanf(file, "%d", &testcase);
    for(int i = 0; i < testcase; i++){
        char line[1000];
        int arrsize;
        int count = 0;
        int num;
        int max = -9999;
        int cfi_get, cfi_nget;
        fscanf(file, "%d", &arrsize);
        fscanf(file, "%d", &num);
        if(arrsize/2 < num){
            printf("Error!\n");
            fprintf(outfile, "Error!\n");
            do{
            fgets(line, sizeof(line), file); //丟棄錯誤的資料
            }while(line[0] == '\n'); //空行跳過
            continue;
        }
        int array[arrsize];
        do {
            fgets(line, sizeof(line), file);
        } while (line[0] == '\n'); // 跳過空行並讀取資料
        char *token = strtok(line, " ");
        while(token != NULL){ //此行有東西
            array[count] = atoi(token);
            count++;
            token = strtok(NULL, " ");
        }
        cfi_get = L(array, 2, arrsize-1, num-1) + array[0];
        cfi_nget = L(array, 1, arrsize, num);
        max = (cfi_get > cfi_nget) ? cfi_get : cfi_nget;
        printf("%d\n", max);
        fprintf(outfile, "%d\n", max);
    }

    fclose(file);
    return 0;
}