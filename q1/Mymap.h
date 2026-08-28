#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 100  // 初始容量
#define MAX_STRING_LENGTH 10000  // 最大字串長度
#define array_size 100 // 初始arr容量
#define MAX_STRING_size 10000  // 最大arr row 字串長度

// 定義鍵值對 (string -> int)
typedef struct {
    char key[MAX_STRING_LENGTH];
    int value;
} MapEntry;

// 定義 Map 結構
typedef struct {
    MapEntry *data;
    int size;  // 當前大小
    int capacity;  // 總容量
} StringMap;


void init_map(StringMap *map) {
    map->size = 0;
    map->capacity = INITIAL_SIZE;
    map->data = (MapEntry *)malloc(map->capacity * sizeof(MapEntry));
    if (!map->data) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }
}

void free_map(StringMap *map) {
    free(map->data);
}

// 按 key 升冪排序
int compare_entries(const void *a, const void *b) {
    return strcmp(((MapEntry *)a)->key, ((MapEntry *)b)->key);
}

// 回傳指標，若不存在則回傳 NULL
int *find(StringMap *map, const char *key) {
    for (int i = 0; i < map->size; i++) {
        if (strcmp(map->data[i].key, key) == 0) {
            return &map->data[i].value;
        }
    }
    return NULL;    
}







/* 只能處理欠套括號的狀況，兩個相鄰的括號會不行

void comp_anal(int *front, int *rear, char* component, int mul, StringMap *map) {
    while(*front != *rear) {  // 搜尋完一個化合物
        (*front)++;  
        int temp;
        int ele_num;
        element ele;
        
        if(my_isupper(component[*front])) {
            ele.key[0] = component[*front];
            (*front)++;  // 先取值再增加
            ele.key[1] = '\0';
            
            if(my_islower(component[*front])) {
                strncat(ele.key, &component[*front], 1);
                (*front)++;
            }
            
            ele_num = 0;
            while(my_isdigit(component[*front])) {
                ele_num = ele_num * 10 + (component[*front] - '0');
                (*front)++;
            }
            
            (*front)--;  // 回退一位，因為跳過了不是數字的字符
            ele.value = (ele_num > 0) ? ele_num : 1;
            ele.value *= mul;
            //printf("ele.key = %s, ele.value = %d\n", ele.key, ele.value);
            insert(map, ele.key, ele.value);
        }
        else { //  front 指向 "("
            if(my_isdigit(component[*rear])) { // 處理右邊數過來的元素數量
                ele_num = 0;
                while(my_isdigit(component[*rear])) {
                    ele_num = ele_num * 10 + (component[*rear] - '0');
                    (*rear)--;
                }
                
                ele_num = (ele_num > 0) ? ele_num : 1;
                temp = ele_num;
                
                if(my_islower(component[*rear])) { //數字 + 小寫
                    ele.key[2] = '\0';
                    ele.key[1] = component[*rear];
                    (*rear)--;
                    ele.key[0] = component[*rear];
                    (*rear)--;
                    ele.value = ele_num * mul;
                    insert(map, ele.key, ele.value);
                } 
                else if(my_isupper(component[*rear])) { //數字 + 大寫
                    ele.key[0] = component[*rear];
                    (*rear)--;
                    ele.key[1] = '\0';
                    ele.value = ele_num * mul;
                    insert(map, ele.key, ele.value);
                }
                else { // 數字 + rear指向")" (component[*rear] == ')')
                    int new_mul = mul * temp;
                    //printf("mul = %d\n", new_mul);
                    (*rear)--;
                    comp_anal(front, rear, component, new_mul, map);
                }
            }
            else if(my_isupper(component[*rear])) { //右邊是大寫
                ele.key[0] = component[*rear];
                (*rear)--;
                ele.key[1] = '\0';
                ele.value = 1;
                ele.value *= mul;
                insert(map, ele.key, ele.value);
            } 
            else if(my_islower(component[*rear])) { //右邊是小寫
                ele.key[2] = '\0';
                ele.key[1] = component[*rear];
                (*rear)--;
                ele.key[0] = component[*rear];
                (*rear)--;
                ele.value = 1;
                ele.value *= mul;
                insert(map, ele.key, ele.value);
            }
            else{ //右邊是")"(component[*rear] == ')') 
                (*rear)--;
                //printf("mul = %d\n", mul);
                comp_anal(front, rear, component, mul, map);
            }
        }
    }
}
*/