#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Mymap.h"

#define array_size 100 // 初始容量
#define MAX_STRING_size 10000  // 最大字串長度

//定義element結構
typedef struct{
    char key[3];
    int value;
} element;

/*////////////////////////////////////////////////////
有修改過insert，如果已經存在在hash裡面，會相加而不是取代
////////////////////////////////////////////////////*/
void insert(StringMap *map, const char *key, int value) {
    int exist = 0;
    for (int i = 0; i < map->size; i++) {
        if (strcmp(map->data[i].key, key) == 0) {
            map->data[i].value += value;  // 更新值
            exist = 1;
            return;
        }
    }
    if (map->size == map->capacity) {
        map->capacity *= 2;
        map->data = (MapEntry *)realloc(map->data, map->capacity * sizeof(MapEntry));
        if (!map->data) {
            fprintf(stderr, "Memory reallocation failed!\n");
            exit(1);
        }
    }
    if (!exist) { //沒有在map->data array裡面
        strcpy(map->data[map->size].key, key);
        map->data[map->size].value = value;
        map->size++;
        qsort(map->data, map->size, sizeof(MapEntry), compare_entries);
    }
}

int my_isdigit(char c) {
    return (c >= '0' && c <= '9');
}

int my_isupper(char c) {
    return (c >= 'A' && c <= 'Z');
}

int my_islower(char c) {
    return (c >= 'a' && c <= 'z');
}
/////////////////////////////寫一個stack，用link list，用link list/////////////////////////////////

typedef struct Stacknode{ 
    char key[3]; //元素最多2個char
    int value; //元素數量
    struct Stacknode *next; //下一個元素
}stacknode;

typedef stacknode *stackptr; //定義指向node的指標型態
stackptr top; //指向最上面的node

void init_stack(){
    top = NULL; //初始化
}

void push( char *key, int value){
    stackptr newptr = (stackptr)malloc(sizeof(stacknode));
    if( newptr){
        newptr->value = value;
        strcpy(newptr->key, key);
        newptr->next = top;
        top = newptr;
    }
    else{
        printf("No memory available\n");
    }
}

int isempty(){
    return top == NULL;
}

element pop(){
    if(isempty()){
        printf("stack is empty\n");
        exit(1);
    }
    else{
        element temp;
        temp.value = top->value;
        strcpy(temp.key, top->key);
        stackptr tpr = top;
        top = top->next;
        free(tpr);
        return temp;
    }
}

int main (){
    element ele;
    int count = 0;
    char component[array_size][MAX_STRING_size];
    FILE *file = fopen("testcase1.txt", "r");
    FILE *outfile = fopen("output.txt", "w");
    if (!file) {
        perror("can not open the file");
        exit(1);
    }
    if(!outfile){
        printf("can not open the outfile\n");
        exit(1);
    }
    while(count < array_size && fgets(component[count], MAX_STRING_size, file) != NULL) {
        char *clean_n = strchr(component[count], '\n');
        if (clean_n) {
            *clean_n = '\0';
        }
        count++;
    }
    StringMap map;

    for(int i = 0 ; i < count; i++){
        int mul = 1;
        init_map(&map);
        for(int j = 0 ; j < MAX_STRING_size ; j++){
            if(my_isupper(component[i][j])){//開頭大寫
                ele.key[0] = component[i][j];
                ele.key[1] = '\0';
                j++;
                if(my_islower(component[i][j])){
                    strncat(ele.key, &component[i][j], 1);
                    j++;
                }
                int ele_num = 0;
                while(my_isdigit(component[i][j])) {
                    ele_num = ele_num * 10 + (component[i][j] - '0');
                    j++;
                }
                j--;
                ele.value = (ele_num > 0) ? ele_num : 1;
                push(ele.key, ele.value);
            }
            else if(component[i][j] == '(') { //遇到'('
                ele.key[0] = component[i][j];
                ele.key[1] = '\0';
                push(ele.key, 1);
            }
            else if(component[i][j] == ')') {  //遇到')'
                int ele_num = 0;
                j++;
                while(my_isdigit(component[i][j])) { //後面多少數字
                    ele_num = ele_num * 10 + (component[i][j] - '0');
                    j++;
                }
                j--;
                ele_num = (ele_num > 0) ? ele_num : 1;
                mul = ele_num;
                stackptr cal_ele = NULL; //list的頭
                element temp = pop();
                stackptr clean;
                while(temp.key[0] != '('){ //找出括號內部的元素計算數量
                    stackptr temp_ptr = (stackptr)malloc(sizeof(stacknode));
                    strcpy(temp_ptr->key, temp.key);
                    temp_ptr->value = temp.value * mul; 
                    temp_ptr->next = cal_ele;
                    cal_ele = temp_ptr;
                    temp = pop();
                    }
                while(cal_ele != NULL){ //放回stack中預防後面還有其他右括號
                    push(cal_ele->key, cal_ele->value);
                    clean = cal_ele;
                    cal_ele = cal_ele->next;
                    free(clean);
                }
            }
            else if(component[i][j] == '\0'){ //結束，找完所有元素
                while(!isempty()){ //stack有東西，要insert到map裡面
                    element temp = pop();
                    insert(&map, temp.key, temp.value);
                }
                for( int k = 0; k < map.size; k++) {
                    if(map.data[k].value == 1) {
                        printf("%s", map.data[k].key);
                        fprintf(outfile, "%s", map.data[k].key);
                    } else {
                        printf("%s%d", map.data[k].key, map.data[k].value);
                        fprintf(outfile, "%s%d", map.data[k].key, map.data[k].value);
                    }
                }
                printf("\n");
                fprintf(outfile, "\n");
                break;
            }
        }
    }
    free_map(&map);
    fclose(file);  
    fclose(outfile);
    return 0;
}
