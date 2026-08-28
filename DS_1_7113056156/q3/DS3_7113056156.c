#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum{ //方向
    UP,RIGHT,  DOWN, LEFT
}DIR;

typedef struct {
    int rowchange;
    int colchange;
} Move;

Move move[4] = {
    [UP] = {-1, 0},
    [RIGHT] = {0, 1},
    [DOWN] = {1, 0},
    [LEFT] = {0, -1}
};

typedef struct{
    char content;
    int value;
    DIR dir; 
} MAP;

typedef struct{
    int prow;
    int pcol;
}LOC;

typedef struct PORTAL{
    LOC key;
    LOC des;
    struct PORTAL *next;
    struct PORTAL *pre;
}PORTAL;

typedef struct Shortest{
    int row;
    int col;
    struct Shortest *next;
}sh;

typedef sh *sh_ptr;

typedef PORTAL *portal_ptr; 

#define limit  1000 //每行讀取最多1000個字元

void ins_sh(int x, int y, sh_ptr *head, sh_ptr *rear){
    sh_ptr temp = (sh_ptr)malloc(sizeof(sh));
    temp -> row = x;
    temp -> col = y;
    temp ->next = NULL;
    if(*head == NULL){
        *head = temp;
        *rear = temp;
    }
    else{
        (*rear)->next = temp;
        (*rear) = temp;
    }
}

void print_sh( FILE *outfile ,sh_ptr *head, sh_ptr *rear, int count){
    printf("%d, ", count);
    fprintf(outfile, "%d, ", count);
    while(*head){ //還有很多
        if(*head != *rear){
            sh_ptr temp;
            temp = *head;
            *head = (*head) -> next;
            printf("(%d,%d) -> ", temp->row, temp->col);
            fprintf(outfile, "(%d,%d) -> ", temp->row, temp->col);
            free(temp);
        }
        else{ //最後一個
            sh_ptr temp = *head;
            printf("(%d,%d)\n", temp->row, temp->col);
            fprintf(outfile, "(%d,%d)\n", temp->row, temp->col);
            free(temp);
            *head = NULL;
            *rear = NULL;
        }
    }
}

void init( int row, int col, MAP array[row][col]){
    for( int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            array[i][j].value = 0;
        }
    }
}

void copy(int row, int col, MAP target[row][col], MAP arr[row][col]){
    for( int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            target[i][j] = arr[i][j];
        }
    }
}

portal_ptr hash_first = NULL;

void insert_portal(int x1, int y1, int x2, int y2){
    portal_ptr temp1 = (portal_ptr)malloc(sizeof(PORTAL));
    portal_ptr temp2 = (portal_ptr)malloc(sizeof(PORTAL));
    temp2->key.prow = x2;
    temp2->key.pcol = y2;
    temp2->des.prow = x1;
    temp2->des.pcol = y1;
    temp2->next = NULL;
    temp2->pre = temp1;

    temp1->key.prow = x1;
    temp1->key.pcol = y1;
    temp1->des.prow = x2;
    temp1->des.pcol = y2;
    temp1->next = temp2;
    temp1->pre = NULL;

    if(!hash_first){ //hash empty
        hash_first = temp1;
    }
    else{ //非空
        temp2->next = hash_first;
        hash_first->pre = temp2;
        hash_first = temp1;
    }
}

// 搜尋傳送門
portal_ptr find_portal(int x, int y) {
    portal_ptr find = hash_first;
    while (find) {
        if (find->key.prow == x && find->key.pcol == y) {
            return find;
        }
        //printf("( %d , %d ) <->", find->key.prow, find->key.pcol);
        find = find->next;
    }
    printf("no portal is finded\n");
    return NULL; // 未找到
}

// 刪除傳送門
void del_portal(int x1, int y1, int x2, int y2) {
    portal_ptr temp1 = find_portal(x1, y1);
    portal_ptr temp2 = find_portal(x2, y2);

    if (temp1) {
        if (temp1->pre) temp1->pre->next = temp1->next;
        if (temp1->next) temp1->next->pre = temp1->pre;
        if (temp1 == hash_first) hash_first = temp1->next; // 特殊情況: 若刪除的是頭部
        free(temp1);
    }

    if (temp2) {
        if (temp2->pre) temp2->pre->next = temp2->next;
        if (temp2->next) temp2->next->pre = temp2->pre;
        if (temp2 == hash_first) hash_first = temp2->next; // 特殊情況: 若刪除的是頭部
        free(temp2);
    }
}

int main(){
    FILE *file = fopen("testcase3.txt", "r");
    FILE *outfile = fopen("output3.txt", "w");
    if(!file){
        printf("can not open the file\n");
        exit(1);
    }
    char line[limit];
    int row;
    int col;
    int rowchange, colchange;
    if(fgets(line, limit, file)){ // get map size (row,col)
        if(sscanf(line, "%d %d", &row ,&col) != 2){
            printf("can not read the row and col\n");
            exit(1);
        }
    }
    else{
        printf("file is empty\n");
        exit(1);
    }
    MAP map[row][col];
    MAP newmap[row][col];

    for(int i = 0; i < row; i++){
        if(!fgets(line, limit, file)){
            printf("can not read the map\n");
            exit(1);
        }
        for(int j = 0; j < col; j++){
            //key in map data to array
            map[i][j].content = line[j];
            map[i][j].value = 0; //初始化
            map[i][j].dir = UP;
            newmap[i][j].content = line[j];
            newmap[i][j].value = 0; 
            newmap[i][j].dir = UP;
        }
    }
    if(!fgets(line, limit, file)){
        printf("error to read command quantity\n");
        exit(1);
    }
    int instruction;
    sscanf(line, "%d" , &instruction);

    for(int i = 0; i < instruction; i++){
        if(!fgets(line, limit, file)){
            printf("fail to get the command\n");
            exit(1);
        }

        if(strncmp(line, "path", 4) == 0){ //find the shortest path
            int count = 0;
            sh_ptr head = NULL, rear = NULL;
            init(row, col, map);
            copy(row, col, newmap, map);
            while(count < 30 ){ //更新value，直到不再變化或是超過500次，無法收斂
                for(int j = 0; j < row; j++){
                    for(int k = 0; k < col; k++){
                        int maxvalue = -500;
                        DIR bestdirection = UP;
                        if(map[j][k].content == '#') continue;
                        if(map[j][k].content == 'G') continue;
                        for(DIR  choose = UP; choose <= LEFT; choose++){ //測試所有方向，找價值最高的更新
                            int next_row, next_col;
                            next_row = j + move[choose].rowchange;
                            next_col = k + move[choose].colchange;
                            int reward = -1; //每次走都-1
                            if(next_row >= 0 && next_row < row && next_col >= 0 && next_col < col){
                                if( map[next_row][next_col].content == 'G'){
                                    reward = 10;
                                }
                                if (map[next_row][next_col].content == '#'){
                                    reward = -500;
                                }
                                if(map[next_row][next_col].content == 'P'){
                                    portal_ptr p = find_portal(next_row , next_col);
                                    next_row = p->des.prow;
                                    next_row = p->des.pcol;
                                }
                                int value = reward + map[next_row][next_col].value;
                                if( value > maxvalue ){ //往大的價值前進
                                    maxvalue = value;
                                    bestdirection = choose;
                                }
                            }
                        }
                        newmap[j][k].value = maxvalue;
                        newmap[j][k].dir = bestdirection;
                    }
                } //全部map更新一次
                if(memcmp(newmap, map, row * col * sizeof(MAP)) == 0){
                    break;
                }
                count++;
                copy(row, col, map, newmap);
            } //value收斂
            int find = 0;
            int srow = -1, scol = -1, prow = -1, pcol = -1;
            for(int j = 0; j < row; j++){
                for(int k = 0; k < col; k++){
                    if(map[j][k].content == 'S'){
                        srow = j;
                        scol  = k;
                        find = 1;
                        break;
                    }
                }
                if(find == 1) {
                    break;
                }
            }
            prow = srow;
            pcol = scol;
            int sh_path = 0;



            for(;;){
                ins_sh(prow, pcol, &head, &rear);
                sh_path++;
                int rowchange = move[map[prow][pcol].dir].rowchange;
                int colchange = move[map[prow][pcol].dir].colchange;
                prow = prow + rowchange;
                pcol = pcol + colchange;
                if(map[prow][pcol].content !=  '.' && map[prow][pcol].content !=  'P' && map[prow][pcol].content !=  'G'){
                    printf("NO PATH\n");
                    fprintf(outfile, "NO PATH\n");
                    break;
                }
                if(map[prow][pcol].content == 'G'){
                    ins_sh(prow, pcol, &head, &rear);
                    sh_path++;
                    print_sh(outfile ,&head, &rear, sh_path);
                    break;
                }
                if(map[prow][pcol].content == 'P'){
                    ins_sh(prow, pcol, &head, &rear);
                    sh_path++;
                    portal_ptr trace;
                    trace = find_portal(prow, pcol);
                    prow = trace->des.prow;
                    pcol = trace->des.pcol;
                }
            }
        }
        else if(strncmp(line, "block" , 5) == 0){ //更改地形為牆壁
            int x, y;
            if(sscanf(line, "block %d %d", &x, &y) != 2){
                printf("INVALID OPERATION\n");
                fprintf(outfile, "INVALID OPERATION\n");
                continue;
            }
            if(x < 0 || x >= row || y < 0 || y >= col || map[x][y].content != '.'){
                printf("INVALID OPERATION\n");
                fprintf(outfile, "INVALID OPERATION\n");
                continue;
            }
            map[x][y].content = '#';
        }
        else if (strncmp(line, "unblock", 7) == 0){ //取消牆壁
            int x, y;
            if(sscanf(line, "unblock %d %d", &x, &y) != 2){
                printf("INVALID OPERATION\n");
                fprintf(outfile, "INVALID OPERATION\n");
                continue;
            }
            if( x < 0 || x >= row || y < 0 || y >= col || map[x][y].content != '#'){
                printf("INVALID OPERATION\n");
                fprintf(outfile, "INVALID OPERATION\n");
                continue;
            }
            map[x][y].content = '.';
        }
        else if(strncmp(line, "portal", 6) == 0){ //設傳送點
            int x1, y1, x2, y2;
            if(sscanf(line, "portal %d %d %d %d", &x1, &y1, &x2, &y2 ) != 4){
                printf("INVALID OPERATION\n");
                fprintf(outfile, "INVALID OPERATION\n");
                continue;
            }
            if(x1 < 0 || x1 >= row || y1 < 0 || y1 >= col || x2 < 0 || x2 >= row || y2 < 0 || y2 >= col ||
                map[x1][y1].content != '.' || map[x2][y2].content != '.'){
                printf("INVALID OPERATION\n");
                fprintf(outfile, "INVALID OPERATION\n");
                continue;
            }
            map[x1][y1].content = 'P';
            map[x2][y2].content = 'P';
            insert_portal(x1, y1, x2, y2);
        }
        else if(strcmp( line, "print\n") == 0){// 印出地圖
            for(int j = 0; j < row; j++){
                for(int k = 0; k < col; k++){
                    printf("%c", map[j][k].content);
                    fprintf(outfile, "%c", map[j][k].content);
                }
                printf("\n");
                fprintf(outfile, "\n");
            }
        }
        else{
            printf("INVALID OPERATION NON\n");
            fprintf(outfile, "INVALID OPERATION\n");
            continue;
        }
    }
    fclose(file);
    fclose(outfile);
    return 0;
}