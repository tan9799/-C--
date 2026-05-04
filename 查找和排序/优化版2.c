#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>

/* ---------- 工具函数 ---------- */
void generateRandomArray(int arr[], int n, int seed) {
    int i;
    srand(seed);
    for (i = 0; i < n; i++)
        arr[i] = rand() % (n * 10) + 1;
}

void generateNearlySortedArray(int arr[], int n, int swapTimes) {
    int i, k, a, b, tmp;
    for (i = 0; i < n; i++) arr[i] = i;
    srand((unsigned)time(NULL));
    for (k = 0; k < swapTimes; k++) {
        a = rand() % n;
        b = rand() % n;
        tmp = arr[a]; arr[a] = arr[b]; arr[b] = tmp;
    }
}

int Check(int arr[], int n) {
    int i;
    for (i = 1; i < n; i++)
        if (arr[i] < arr[i-1]) return 0;
    return 1;
}

void copyArray(int dest[], int src[], int n) {
    int i;
    for (i = 0; i < n; i++) dest[i] = src[i];
}

/* ---------- 排序算法 ---------- */
void bubbleSort(int arr[], int n) {
    int i, j, swapped, tmp;
    for (i = 0; i < n-1; i++) {
        swapped = 0;
        for (j = 0; j < n-1-i; j++) {
            if (arr[j] > arr[j+1]) {
                tmp = arr[j]; arr[j] = arr[j+1]; arr[j+1] = tmp;
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

void selectionSort(int arr[], int n) {
    int i, j, minIdx, tmp;
    for (i = 0; i < n-1; i++) {
        minIdx = i;
        for (j = i+1; j < n; j++)
            if (arr[j] < arr[minIdx]) minIdx = j;
        if (minIdx != i) {
            tmp = arr[i]; arr[i] = arr[minIdx]; arr[minIdx] = tmp;
        }
    }
}

void insertionSort(int arr[], int n) {
    int i, j, key;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i-1;
        while (j >= 0 && arr[j] > key) {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = key;
    }
}

/* 归并排序 */
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1, n2 = right - mid;
    int *L = (int*)malloc(n1 * sizeof(int));
    int *R = (int*)malloc(n2 * sizeof(int));
    int i, j, k;
    for (i = 0; i < n1; i++) L[i] = arr[left + i];
    for (i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];
    i = 0; j = 0; k = left;
    while (i < n1 && j < n2) arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    free(L); free(R);
}
void mergeSortRec(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortRec(arr, left, mid);
        mergeSortRec(arr, mid+1, right);
        merge(arr, left, mid, right);
    }
}
void mergeSort(int arr[], int n) { mergeSortRec(arr, 0, n-1); }

/* 快速排序（修正版，随机选基准） */
void swap(int *a, int *b) {
    int t = *a; *a = *b; *b = t;
}

void quickSortRec(int arr[], int low, int high) {
    if (low >= high) return;
    int pivot_idx = low + rand() % (high - low + 1);
    int pivot = arr[pivot_idx];
    swap(&arr[pivot_idx], &arr[high]);
    int i = low - 1;
    int j;
    for (j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    int pi = i + 1;
    quickSortRec(arr, low, pi - 1);
    quickSortRec(arr, pi + 1, high);
}

void quickSort(int arr[], int n) {
    if (n <= 1) return;
    quickSortRec(arr, 0, n - 1);
}

/* ---------- 线程超时测试函数 ---------- */
typedef struct {
    void (*sortFunc)(int[], int);
    int *arr;
    int n;
    volatile int finished;
    volatile int correct;
} SortThreadParam;

DWORD WINAPI sortThreadFunc(LPVOID lpParam) {
    SortThreadParam *param = (SortThreadParam*)lpParam;
    param->sortFunc(param->arr, param->n);
    param->finished = 1;
    param->correct = Check(param->arr, param->n);
    return 0;
}

double testSort(void (*sortFunc)(int[], int), int original[], int n, int timeoutSec) {
    int *arr = (int*)malloc(n * sizeof(int));
    if (!arr) return -2.0;
    copyArray(arr, original, n);

    SortThreadParam param;
    param.sortFunc = sortFunc;
    param.arr = arr;
    param.n = n;
    param.finished = 0;
    param.correct = 0;

    DWORD startTime = GetTickCount();
    HANDLE hThread = CreateThread(NULL, 0, sortThreadFunc, &param, 0, NULL);
    if (!hThread) {
        free(arr);
        return -2.0;
    }

    DWORD waitResult = WaitForSingleObject(hThread, timeoutSec * 1000);
    DWORD endTime = GetTickCount();
    double elapsed = (double)(endTime - startTime) / 1000.0;

    if (waitResult == WAIT_OBJECT_0) {
        CloseHandle(hThread);
        if (!param.correct) {
            free(arr);
            return -2.0;
        }
        free(arr);
        return elapsed;
    } else {
        TerminateThread(hThread, 0);
        CloseHandle(hThread);
        free(arr);
        return -1.0;
    }
}

/* ---------- 打印表格 ---------- */
void printTable(int sizes[], int numSizes, const char* dataType,
                double results[][5], int timeoutFlag[][5]) {
    const char* algoNames[] = {"冒泡排序", "选择排序", "插入排序", "归并排序", "快速排序"};
    int i, j;
    printf("\n==================== %s ====================\n", dataType);
    printf("%-10s", "规模");
    for (i = 0; i < 5; i++) printf("%-14s", algoNames[i]);
    printf("\n");
    for (i = 0; i < 70; i++) printf("-");
    printf("\n");
    for (i = 0; i < numSizes; i++) {
        printf("%-10d", sizes[i]);
        for (j = 0; j < 5; j++) {
            if (timeoutFlag[i][j])
                printf("%-14s", ">300s");
            else if (results[i][j] < 0)
                printf("%-14s", "错误");
            else
                printf("%-14.4f", results[i][j]);
        }
        printf("\n");
    }
}

/* ---------- 主函数 ---------- */
int main() {
    srand(time(NULL));
    int sizes[] = {100, 1000, 10000, 100000, 1000000};
    int numSizes = 5;
    const int timeoutSec = 300;
    double randomResults[5][5] = {0};
    int randomTimeout[5][5] = {0};
    double nearlyResults[5][5] = {0};
    int nearlyTimeout[5][5] = {0};
    void (*sorts[5])(int[], int) = {bubbleSort, selectionSort, insertionSort, mergeSort, quickSort};
    int i, j, n, *original;

    /* 随机数组测试 */
    printf("正在生成随机数组并测试...\n");
    for (i = 0; i < numSizes; i++) {
        n = sizes[i];
        original = (int*)malloc(n * sizeof(int));
        if (!original) { printf("内存分配失败\n"); return 1; }
        generateRandomArray(original, n, 42);
        for (j = 0; j < 5; j++) {
            double t = testSort(sorts[j], original, n, timeoutSec);
            if (t < -0.5) {
                if (t == -1.0) randomTimeout[i][j] = 1;
                else randomResults[i][j] = -2.0;
            } else {
                randomResults[i][j] = t;
            }
        }
        free(original);
        printf("随机数组 规模 %d 测试完成\n", n);
    }

    /* 几乎有序数组测试 */
    printf("\n正在生成几乎有序数组并测试...\n");
    for (i = 0; i < numSizes; i++) {
        n = sizes[i];
        original = (int*)malloc(n * sizeof(int));
        if (!original) { printf("内存分配失败\n"); return 1; }
        int swapTimes = (n / 100) + 10;
        generateNearlySortedArray(original, n, swapTimes);
        for (j = 0; j < 5; j++) {
            double t = testSort(sorts[j], original, n, timeoutSec);
            if (t < -0.5) {
                if (t == -1.0) nearlyTimeout[i][j] = 1;
                else nearlyResults[i][j] = -2.0;
            } else {
                nearlyResults[i][j] = t;
            }
        }
        free(original);
        printf("几乎有序数组 规模 %d 测试完成\n", n);
    }

    printTable(sizes, numSizes, "随机数组", randomResults, randomTimeout);
    printTable(sizes, numSizes, "几乎有序数组", nearlyResults, nearlyTimeout);
    
    system("pause");
    return 0;
}
