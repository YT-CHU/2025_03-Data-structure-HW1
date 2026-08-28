#include "Mymap.h"
#include <stdio.h>

int main(){
    StringMap mymap;
    init_map(&mymap);
    char* s1 = "H";
    char* s2 = "O";
    char* s3 = "Mn";
    insert(&mymap, s1, 1);
    printf("H = %d\n", *find(&mymap, s1));
    insert(&mymap, s1, *find(&mymap, s1)+2);
    printf("H = %d\n", *find(&mymap, s1));
    insert(&mymap, s2, 3);
    insert(&mymap, s3, 5);
    for(int i = 0; i < mymap.size; i++){
        printf("%s%d", mymap.data[i].key, mymap.data[i].value);
    }
}