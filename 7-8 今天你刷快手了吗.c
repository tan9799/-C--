#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;          // 学号
    int total_seconds; // 总时间（秒）
} Student;

// 将时间字符串转换为秒数
int time_to_seconds(char* time_str) {
    int h, m, s;
    sscanf(time_str, "%d:%d:%d", &h, &m, &s);
    return h * 3600 + m * 60 + s;
}

// 比较函数：先按总时间降序，时间相同按学号升序
int compare(const void* a, const void* b) {
    Student* s1 = (Student*)a;
    Student* s2 = (Student*)b;
    
    if (s1->total_seconds != s2->total_seconds) {
        return s2->total_seconds - s1->total_seconds; // 降序
    } else {
        return s1->id - s2->id; // 学号升序
    }
}

// 将秒数转换为"hh:mm:ss"格式
void seconds_to_time(int seconds, char* time_str) {
    int h = seconds / 3600;
    int m = (seconds % 3600) / 60;
    int s = seconds % 60;
    sprintf(time_str, "%02d:%02d:%02d", h, m, s);
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    
    // 初始化学生数组
    Student students[1000];
    for (int i = 0; i < n; i++) {
        students[i].id = i;
        students[i].total_seconds = 0;
    }
    
    // 读取每条记录并累加时间
    for (int i = 0; i < m; i++) {
        char id_str[4], start_time[9], end_time[9];
        scanf("%s %s %s", id_str, start_time, end_time);
        
        int id;
        sscanf(id_str, "%d", &id);
        
        int start_sec = time_to_seconds(start_time);
        int end_sec = time_to_seconds(end_time);
        int duration = end_sec - start_sec;
        
        // 累加时间
        students[id].total_seconds += duration;
    }
    
    // 排序（从大到小）
    qsort(students, n, sizeof(Student), compare);
    
    // 输出每个学生的信息
    char time_str[9];
    for (int i = 0; i < n; i++) {
        seconds_to_time(students[i].total_seconds, time_str);
        printf("%03d %s\n", students[i].id, time_str);
    }
    
    return 0;
}
