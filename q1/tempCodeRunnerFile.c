int main() {
    int count = 0;
    char component[array_size][MAX_STRING_size];
    FILE *file = fopen("D:/Master_degree/degree_2/data_structure/hw1/testcase/testcase1.txt", "r");
    if (file == NULL) {
        perror("can not open the file");
        return 1;
    }
    while(count < array_size && fgets(component[count], MAX_STRING_size, file) != NULL) {
        char *clean_n = strchr(component[count], '\n');
        if (clean_n) {
            *clean_n = '\0';
        }
        count++;
    }

    StringMap map;

    for(int i = 0; i < count; i++) {
        init_map(&map);  // 每次處理新的一行前重新初始化 map

        int front = -1; // front指向element的前一個位置
        int rear = 0;
        int mul = 1;
        while(component[i][rear] != '\0') {
            rear++;
        }
        rear--; // rear指向最後一個字母
        comp_anal(&front, &rear, component[i], mul, &map);

        for( int j = 0; j < map.size; j++) {
            if(map.data[j].value == 1) {
                printf("%s", map.data[j].key);
            } else {
                printf("%s%d", map.data[j].key, map.data[j].value);
            }
        }
        printf("\n");
    }

    free_map(&map);
    fclose(file);  // 別忘了關閉文件
    return 0;
}