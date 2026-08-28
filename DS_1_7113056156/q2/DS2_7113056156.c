#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{ //工作的結構
    int id;
    int arr_time;
    int work_time;
    int priority;
    int complete_time;
}job;

typedef struct List{ //放排隊的工作
    job* data;
    struct List *next;
}wait_list; 

typedef wait_list *wait_listptr; //指向list最大優先權的工作

wait_listptr prior;

void init_list(){
    prior = NULL;
}

void wait ( job wjob ){ //把工作放進去排隊等待
    wait_listptr newptr = (wait_listptr)malloc(sizeof(wait_list));
    if(!newptr){
        printf("memory allocate failed\n");
        exit(1);
    }
    newptr -> data = (job*)malloc(sizeof(job));
    if(!newptr->data){
        printf("memory allocate failed\n");
        exit(1);
    }
    newptr->data->id = wjob.id;
    newptr->data->arr_time = wjob.arr_time;  
    newptr->data->work_time = wjob.work_time;
    newptr->data->priority = wjob.priority;
    newptr->data->complete_time = wjob.complete_time;
    newptr->next = NULL;

    if(!prior){ //list是空的
        prior = newptr;
    }
    else {
        if( (prior)->data->priority < newptr->data->priority){ //list不是空的，新工作的優先權比list最大的還大
                newptr->next = prior;
                prior = newptr;
                return;
            }

        wait_listptr curr = prior;
        wait_listptr next_node = prior->next;
        // 遍歷列表尋找合適的插入位置
        while (next_node != NULL && next_node->data->priority >= newptr->data->priority) {
            curr = next_node;
            next_node = next_node->next;
        }
        // 插入節點
        curr->next = newptr;
        newptr->next = next_node;
            /*
            else{ //list不是空的，新工作比prior還小，往後繼續找
                find_position( &prior, &(prior -> next), &newptr);
            }
                */
    }
}

int is_empty(){
    return prior == NULL;
}

job pop(){ //最高優先度的拿出來
    wait_listptr temp;
    job pop_job;
    temp = prior;
    if(prior->next == NULL){
        prior = NULL;
    }
    else{
    prior = prior->next;
    //prior指向錯誤的記憶體錯誤，系統崩潰
    }
    pop_job.id = temp->data->id;
    pop_job.arr_time = temp->data->arr_time;
    pop_job.work_time = temp->data->work_time;
    pop_job.priority = temp->data->priority;
    pop_job.complete_time = temp->data->complete_time;
    free(temp->data);
    free(temp);
    return pop_job;
        
}

int main() {
    FILE *file = fopen("testcase2.txt", "r");
    FILE *outfile = fopen("output.txt", "w");
    if(!file){
        printf("can not open the file\n");
        exit(1);
    }
    if(!outfile){
        printf("can not open the outfile\n");
        exit(1);
    }
    #define limit  1000 //每行讀取最多1000個字元
    int total_test;
    int cpu[limit];
    int comp[limit];
    char line[limit];
    char *token;
    int line_bit;
    int idel = 1;
    int task;
    int loading_num;
    int complete_num;
    int time = 0;
    job working, compare;

    fgets(line, limit, file);
    total_test = atoi(line);
    for(int i = 0 ; i < total_test; i++ ){ //執行多少組
        fgets(line, limit, file);
        task = atoi(line);
        loading_num = 0;
        complete_num = 0;
        init_list();

        while(loading_num < task){ //每組有多少工作，依序(抵達時間)載入cpu
            fgets( line, limit, file);
            loading_num++;
            line_bit = 0;
            if(idel){ //cpu空著
                
                token = strtok(line, " ");  // 以空格為分隔符分割字串
                while (token != NULL) {
                    cpu[line_bit] = atoi(token);  // 將字串轉換為整數並存入陣列
                    line_bit++;
                    token = strtok(NULL, " ");  // 繼續分割下一個字串
                }
                idel = 0;
                working.id = cpu[0]; //工作id
                working.arr_time = cpu[1]; //工作抵達時間
                working.work_time = cpu[2]; //工作執行時間
                working.priority = cpu[3]; //工作優先權
                working.complete_time = 0; //工作完成時間
                time =  working.arr_time; //電腦的時間
            }
            else{ //cpu有再忙
                token = strtok(line, " ");  // 以空格為分隔符分割字串
                while (token != NULL) {
                    comp[line_bit] = atoi(token);  // 將字串轉換為整數並存入陣列
                    line_bit++;
                    token = strtok(NULL, " ");  // 繼續分割下一個字串
                }
                compare.id = comp[0]; //工作id
                compare.arr_time = comp[1]; //工作抵達時間
                compare.work_time = comp[2]; //工作執行時間
                compare.priority = comp[3]; //工作優先權
                compare.complete_time = 0; //工作完成時間
                //time = compare.arr_time; 
                while( time + working.work_time <= compare.arr_time){ //工作完成時間比新工作抵達時間早
                    complete_num++;
                    time += working.work_time; //更換工作才可更新時間
                    working.complete_time = time;
                    printf("%d %d\n", working.id, working.complete_time);
                    fprintf(outfile, "%d %d\n", working.id, working.complete_time);
                    if(!is_empty()){
                    working = pop();
                    working.arr_time = time;
                    }
                    else{ //沒有工作了
                        time = compare.arr_time;//更換工作才可更新時間
                        working = compare;
                        break;
                    }
                }
                if(working.id != compare.id && (time + working.work_time) > compare.arr_time ){ //要中斷程式來比較優先度
                    //time = compare.arr_time;
                    if(working.priority >= compare.priority){ //原本工作的優先權高
                        wait( compare);
                    }
                    else{ // 換新工作進來
                        working.work_time -= (compare.arr_time - working.arr_time);
                        wait( working);
                        //list_print();
                        working = compare;
                        time = compare.arr_time;
                    }
                }
            }
        }
        while(loading_num == task && complete_num < task){ //工作都進入排序，要把list全部完成才結束
            time += working.work_time;
            complete_num++;
            working.complete_time = time;
            
            printf("%d %d", working.id, working.complete_time);
            fprintf(outfile,"%d %d", working.id, working.complete_time );
            if(i != total_test-1 || complete_num != task){
                printf("\n");
                fprintf(outfile, "\n");
            }
            if(!is_empty()){
            working = pop();
            working.arr_time = time;
            }
        }
        if(i != total_test-1){
        printf("\n");
        fprintf(outfile,"\n");
        }
        idel = 1;
    }
    fclose(file);
    fclose(outfile);
    return 0;
}