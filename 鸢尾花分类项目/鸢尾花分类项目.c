#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define MAX_SAMPLES 200
#define FEATURES 4
#define K 5                 // KNN 参数
#define TRAIN_RATIO 0.8     // 训练集比例

// 样本结构体
typedef struct {
    double features[FEATURES];  // 4个特征
    int label;                  // 类别：1,2,3
} Sample;

Sample dataset[MAX_SAMPLES];
int sample_count = 0;

// 将类别名称转换为数字
int label_to_int(const char* name) {
    if (strcmp(name, "Iris-setosa") == 0) return 1;
    if (strcmp(name, "Iris-versicolor") == 0) return 2;
    if (strcmp(name, "Iris-virginica") == 0) return 3;
    return -1;
}

// 读取数据文件
void read_data(const char* filename) {
    FILE* fp = fopen(filename, "r");
    char line[256];
    char* token;
    if (!fp) {
        printf("无法打开文件 %s\n", filename);
        exit(1);
    }
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0) continue;
        
        token = strtok(line, ",");
        if (!token) continue;
        dataset[sample_count].features[0] = atof(token);
        
        token = strtok(NULL, ",");
        dataset[sample_count].features[1] = atof(token);
        
        token = strtok(NULL, ",");
        dataset[sample_count].features[2] = atof(token);
        
        token = strtok(NULL, ",");
        dataset[sample_count].features[3] = atof(token);
        
        token = strtok(NULL, ",");
        dataset[sample_count].label = label_to_int(token);
        
        sample_count++;
        if (sample_count >= MAX_SAMPLES) break;
    }
    fclose(fp);
    printf("成功读取 %d 个样本\n", sample_count);
}

// 打乱数据集
void shuffle_data() {
    int i, j;
    Sample tmp;
    srand((unsigned)time(NULL));
    for (i = sample_count - 1; i > 0; i--) {
        j = rand() % (i + 1);
        tmp = dataset[i];
        dataset[i] = dataset[j];
        dataset[j] = tmp;
    }
}

// 欧氏距离
double euclidean_distance(double* a, double* b) {
    double sum = 0.0;
    int i;
    for (i = 0; i < FEATURES; i++) {
        double diff = a[i] - b[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}

// KNN 分类
int knn_predict(Sample* train, int train_n, double* test_feat) {
    typedef struct {
        double dist;
        int label;
    } DistLabel;
    DistLabel* distances;
    int i, j, best_label, max_votes;
    int votes[4] = {0};
    
    distances = (DistLabel*)malloc(train_n * sizeof(DistLabel));
    if (!distances) {
        printf("内存分配失败\n");
        exit(1);
    }
    for (i = 0; i < train_n; i++) {
        distances[i].dist = euclidean_distance(test_feat, train[i].features);
        distances[i].label = train[i].label;
    }
    // 冒泡排序，按距离升序
    for (i = 0; i < train_n - 1; i++) {
        for (j = i + 1; j < train_n; j++) {
            if (distances[i].dist > distances[j].dist) {
                DistLabel tmp = distances[i];
                distances[i] = distances[j];
                distances[j] = tmp;
            }
        }
    }
    // 统计前K个的类别
    for (i = 0; i < K && i < train_n; i++) {
        votes[distances[i].label]++;
    }
    free(distances);
    // 找出票数最多的类别
    best_label = 1;
    max_votes = votes[1];
    for (i = 2; i <= 3; i++) {
        if (votes[i] > max_votes) {
            max_votes = votes[i];
            best_label = i;
        }
    }
    return best_label;
}

int main() {
    int train_n, test_n, i, correct;
    Sample* train;
    Sample* test;
    double accuracy;
    
    // 1. 读取数据
    read_data("data.txt");
    if (sample_count == 0) {
        printf("没有读取到数据，请检查文件路径。\n");
        return 1;
    }
    
    // 2. 打乱数据集
    shuffle_data();
    
    // 3. 划分训练集和测试集
    train_n = (int)(sample_count * TRAIN_RATIO);
    test_n = sample_count - train_n;
    train = dataset;
    test = dataset + train_n;
    printf("训练集样本数: %d, 测试集样本数: %d\n", train_n, test_n);
    
    // 4. 对每个测试样本进行预测
    correct = 0;
    for (i = 0; i < test_n; i++) {
        int predicted = knn_predict(train, train_n, test[i].features);
        if (predicted == test[i].label) {
            correct++;
        }
    }
    
    // 5. 计算准确率
    accuracy = (double)correct / test_n * 100.0;
    printf("\n=== KNN 分类结果 (K=%d) ===\n", K);
    printf("正确预测数: %d / %d\n", correct, test_n);
    printf("准确率: %.2f%%\n", accuracy);
    
    return 0;
}
