#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// ---------- 工具函数 ----------
// 生成随机数组（使用固定种子，保证可重复实验）
void generateRandomArray(int arr[], int n, int seed) {
    srand(seed);
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % (n * 10) + 1;   // 范围 1 ~ n*10
    }
}

// 生成几乎有序数组（先创建有序，再随机交换若干次）
void generateNearlySortedArray(int arr[], int n, int swapTimes) {
    for (int i = 0; i < n; i++) arr[i] = i;   // 完全有序
    srand((unsigned)time(NULL));
    for (int k = 0; k < swapTimes; k++) {
        int i = rand() % n;
        int j = rand() % n;
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

// 验证排序结果是否正确（升序）
int Check(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        if (arr[i] < arr[i-1]) return 0;
    }
    return 1;
}

// 复制数组（因为排序会修改原数组，每次测试需用副本）
void copyArray(int dest[], int src[], int n) {
    for (int i = 0; i < n; i++) dest[i] = src[i];
}

// ---------- 排序算法 ----------
// 1. 冒泡排序
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        int swapped = 0;
        for (int j = 0; j < n-1-i; j++) {
            if (arr[j] > arr[j+1]) {
                int tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

// 2. 简单选择排序
void selectionSort(int arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        int minIdx = i;
        for (int j = i+1; j < n; j++) {
            if (arr[j] < arr[minIdx]) minIdx = j;
        }
        if (minIdx != i) {
            int tmp = arr[i];
            arr[i] = arr[minIdx];
            arr[minIdx] = tmp;
        }
    }
}

// 3. 简单插入排序
void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i-1;
        while (j >= 0 && arr[j] > key) {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = key;
    }
}

// 4. 归并排序（辅助函数）
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int *L = (int*)malloc(n1 * sizeof(int));
    int *R = (int*)malloc(n2 * sizeof(int));
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    free(L);
    free(R);
}
void mergeSortRec(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortRec(arr, left, mid);
        mergeSortRec(arr, mid+1, right);
        merge(arr, left, mid, right);
    }
}
void mergeSort(int arr[], int n) {
    mergeSortRec(arr, 0, n-1);
}

// 5. 快速排序（辅助函数）
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            int tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
        }
    }
    int tmp = arr[i+1];
    arr[i+1] = arr[high];
    arr[high] = tmp;
    return i+1;
}
void quickSortRec(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSortRec(arr, low, pi-1);
        quickSortRec(arr, pi+1, high);
    }
}
void quickSort(int arr[], int n) {
    quickSortRec(arr, 0, n-1);
}

// ---------- 计时测试函数 ----------
// 返回耗时（秒），若超时（>300秒）或排序错误则返回负数
double testSort(void (*sortFunc)(int[], int), int original[], int n, int timeoutSec) {
    int *arr = (int*)malloc(n * sizeof(int));
    copyArray(arr, original, n);
    clock_t start = clock();
    sortFunc(arr, n);
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    int correct = Check(arr, n);
    free(arr);
    if (!correct) return -2.0;   // 排序错误
    if (elapsed > timeoutSec) return -1.0; // 超时
    return elapsed;
}

// ---------- 打印表格 ----------
void printTable(int sizes[], int numSizes, const char* dataType,
                double results[][5], int timeoutFlag[][5]) {
    const char* algoNames[] = {"冒泡排序", "选择排序", "插入排序", "归并排序", "快速排序"};
    printf("\n==================== %s ====================\n", dataType);
    printf("%-10s", "规模");
    for (int i = 0; i < 5; i++) printf("%-14s", algoNames[i]);
    printf("\n");
    for (int i = 0; i < 70; i++) printf("-");
    printf("\n");
    for (int i = 0; i < numSizes; i++) {
        printf("%-10d", sizes[i]);
        for (int j = 0; j < 5; j++) {
            if (timeoutFlag[i][j]) {
                printf("%-14s", ">300s");
            } else if (results[i][j] < 0) {
                printf("%-14s", "错误");
            } else {
                printf("%-14.4f", results[i][j]);
            }
        }
        printf("\n");
    }
}

int main() {
    int sizes[] = {100, 1000, 10000, 100000, 1000000};
    int numSizes = 5;
    const int timeoutSec = 300;   // 5分钟

    // 存储结果：每种规模下5种算法的耗时（秒），以及超时标志
    double randomResults[5][5] = {0};
    int randomTimeout[5][5] = {0};
    double nearlyResults[5][5] = {0};
    int nearlyTimeout[5][5] = {0};

    // 定义排序函数指针数组
    void (*sorts[5])(int[], int) = {bubbleSort, selectionSort, insertionSort, mergeSort, quickSort};

    // ------ 测试随机数组 ------
    printf("正在生成随机数组并测试...\n");
    for (int i = 0; i < numSizes; i++) {
        int n = sizes[i];
        int *original = (int*)malloc(n * sizeof(int));
        generateRandomArray(original, n, 42);   // 固定种子
        for (int j = 0; j < 5; j++) {
            double t = testSort(sorts[j], original, n, timeoutSec);
            if (t < -0.5) {   // 错误或超时
                if (t == -1.0) randomTimeout[i][j] = 1;
                else randomTimeout[i][j] = 0; // 已经标记为错误
            } else {
                randomResults[i][j] = t;
            }
        }
        free(original);
        // 进度提示
        printf("随机数组 规模 %d 测试完成\n", n);
    }

    // ------ 测试几乎有序数组 ------
    printf("\n正在生成几乎有序数组并测试...\n");
    for (int i = 0; i < numSizes; i++) {
        int n = sizes[i];
        int *original = (int*)malloc(n * sizeof(int));
        // 交换次数设为 n/100，但至少10次
        int swapTimes = (n / 100) + 10;
        generateNearlySortedArray(original, n, swapTimes);
        for (int j = 0; j < 5; j++) {
            double t = testSort(sorts[j], original, n, timeoutSec);
            if (t < -0.5) {
                if (t == -1.0) nearlyTimeout[i][j] = 1;
                else nearlyTimeout[i][j] = 0;
            } else {
                nearlyResults[i][j] = t;
            }
        }
        free(original);
        printf("几乎有序数组 规模 %d 测试完成\n", n);
    }

    // 打印结果表
    printTable(sizes, numSizes, "随机数组", randomResults, randomTimeout);
    printTable(sizes, numSizes, "几乎有序数组", nearlyResults, nearlyTimeout);

    return 0;
}
