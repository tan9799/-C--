#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define MAX_SAMPLES 600
#define FEATURES 13
#define EPOCHS 1000
#define ALPHA 0.01
#define TOP_K 4

typedef struct {
    double features[FEATURES];
    double price;
} Sample;

Sample samples[MAX_SAMPLES];
int sample_count = 0;

void trim(char *str) {
    char *start = str;
    char *end;
    while (*start == ' ' || *start == '\t' || *start == '\r' || *start == '\n') start++;
    end = start + strlen(start) - 1;
    while (end > start && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n')) end--;
    *(end + 1) = '\0';
    if (start != str) memmove(str, start, end - start + 2);
}

void skip_bom(FILE *fp) {
    unsigned char bom[3];
    if (fread(bom, 1, 3, fp) == 3) {
        if (!(bom[0] == 0xEF && bom[1] == 0xBB && bom[2] == 0xBF)) {
            fseek(fp, 0, SEEK_SET);
        }
    } else {
        fseek(fp, 0, SEEK_SET);
    }
}

void read_data(const char* filename) {
    FILE* fp = fopen(filename, "r");
    char line[1024];
    int i, n;
    double vals[FEATURES + 1];

    if (!fp) {
        printf("错误：无法打开文件 %s\n", filename);
        exit(1);
    }

    skip_bom(fp);

    while (fgets(line, sizeof(line), fp) != NULL) {
        trim(line);
        if (strlen(line) == 0) continue;

        n = sscanf(line, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
                &vals[0], &vals[1], &vals[2], &vals[3], &vals[4],
                &vals[5], &vals[6], &vals[7], &vals[8], &vals[9],
                &vals[10], &vals[11], &vals[12], &vals[13]);

        if (n == FEATURES + 1) {
            for (i = 0; i < FEATURES; i++) {
                samples[sample_count].features[i] = vals[i];
            }
            samples[sample_count].price = vals[FEATURES];
            sample_count++;
        }
        if (sample_count >= MAX_SAMPLES) break;
    }
    fclose(fp);
    printf("成功读取 %d 个样本\n", sample_count);
    if (sample_count == 0) {
        printf("未读取到有效数据，请检查文件格式。\n");
        exit(1);
    }
}

double compute_correlation(double* x, double* y, int n) {
    double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0, sum_y2 = 0;
    int i;
    for (i = 0; i < n; i++) {
        sum_x  += x[i];
        sum_y  += y[i];
        sum_xy += x[i] * y[i];
        sum_x2 += x[i] * x[i];
        sum_y2 += y[i] * y[i];
    }
    double numerator = n * sum_xy - sum_x * sum_y;
    double denominator = sqrt((n * sum_x2 - sum_x * sum_x) * (n * sum_y2 - sum_y * sum_y));
    if (denominator == 0) return 0;
    return numerator / denominator;
}

// 单特征归一化（减均值除标准差）
void normalize_feature(double* x, int n, double* mean, double* std) {
    int i;
    *mean = 0.0;
    for (i = 0; i < n; i++) *mean += x[i];
    *mean /= n;
    *std = 0.0;
    for (i = 0; i < n; i++) *std += (x[i] - *mean) * (x[i] - *mean);
    *std = sqrt(*std / n);
    if (*std == 0) *std = 1.0;
    for (i = 0; i < n; i++) x[i] = (x[i] - *mean) / *std;
}

// 一元线性回归训练
void train_unary(double* theta, double* x_train, double* y_train, int n, double alpha, int epochs) {
    int i, epoch;
    theta[0] = 0.0;
    theta[1] = 0.0;
    for (epoch = 0; epoch < epochs; epoch++) {
        double grad0 = 0.0, grad1 = 0.0;
        for (i = 0; i < n; i++) {
            double pred = theta[0] + theta[1] * x_train[i];
            double error = pred - y_train[i];
            grad0 += error;
            grad1 += error * x_train[i];
        }
        theta[0] -= alpha * grad0 / n;
        theta[1] -= alpha * grad1 / n;
    }
}

double compute_rmse_unary(double* theta, double* x_test, double* y_test, int n) {
    double sum_sq = 0.0;
    int i;
    for (i = 0; i < n; i++) {
        double pred = theta[0] + theta[1] * x_test[i];
        double err = pred - y_test[i];
        sum_sq += err * err;
    }
    return sqrt(sum_sq / n);
}

// 批量归一化（多个特征）
void normalize_features_matrix(double** X, int n, int m, double* means, double* stds) {
    int i, j;
    for (j = 0; j < m; j++) {
        double sum = 0.0;
        for (i = 0; i < n; i++) sum += X[i][j];
        means[j] = sum / n;
        double sq_sum = 0.0;
        for (i = 0; i < n; i++) sq_sum += (X[i][j] - means[j]) * (X[i][j] - means[j]);
        stds[j] = sqrt(sq_sum / n);
        if (stds[j] == 0) stds[j] = 1.0;
        for (i = 0; i < n; i++) X[i][j] = (X[i][j] - means[j]) / stds[j];
    }
}

void train_multi(double* theta, double** X, double* y, int n, int m, double alpha, int epochs) {
    int epoch, i, j;
    double* grad = (double*)calloc(m + 1, sizeof(double));
    for (j = 0; j <= m; j++) theta[j] = 0.0;
    for (epoch = 0; epoch < epochs; epoch++) {
        for (j = 0; j <= m; j++) grad[j] = 0.0;
        for (i = 0; i < n; i++) {
            double pred = theta[0];
            for (j = 0; j < m; j++) pred += theta[j + 1] * X[i][j];
            double error = pred - y[i];
            grad[0] += error;
            for (j = 0; j < m; j++) grad[j + 1] += error * X[i][j];
        }
        for (j = 0; j <= m; j++) theta[j] -= alpha * grad[j] / n;
    }
    free(grad);
}

double compute_rmse_multi(double* theta, double** X_test, double* y_test, int test_n, int m) {
    double sum_sq = 0.0;
    int i, j;
    for (i = 0; i < test_n; i++) {
        double pred = theta[0];
        for (j = 0; j < m; j++) pred += theta[j + 1] * X_test[i][j];
        double err = pred - y_test[i];
        sum_sq += err * err;
    }
    return sqrt(sum_sq / test_n);
}

typedef struct {
    int idx;
    double corr;
} CorrEntry;
int cmp_corr(const void* a, const void* b) {
    double ca = fabs(((CorrEntry*)a)->corr);
    double cb = fabs(((CorrEntry*)b)->corr);
    if (ca < cb) return 1;
    if (ca > cb) return -1;
    return 0;
}

int main() {
    int i, j;
    int train_n, test_n;

    read_data("housing-price.txt");
    if (sample_count == 0) return 1;

    srand(time(NULL));
    for (i = sample_count - 1; i > 0; i--) {
        int r = rand() % (i + 1);
        Sample tmp = samples[i];
        samples[i] = samples[r];
        samples[r] = tmp;
    }

    train_n = (int)(sample_count * 0.8);
    test_n = sample_count - train_n;
    Sample* train = samples;
    Sample* test = samples + train_n;

    // 计算相关系数
    double* price_train = (double*)malloc(train_n * sizeof(double));
    for (i = 0; i < train_n; i++) price_train[i] = train[i].price;

    CorrEntry entries[FEATURES];
    for (j = 0; j < FEATURES; j++) {
        double* feat = (double*)malloc(train_n * sizeof(double));
        for (i = 0; i < train_n; i++) feat[i] = train[i].features[j];
        entries[j].idx = j;
        entries[j].corr = compute_correlation(feat, price_train, train_n);
        free(feat);
    }
    free(price_train);

    printf("\n各特征与房价的相关系数：\n");
    for (j = 0; j < FEATURES; j++) {
        printf("特征 %2d : %.4f\n", j + 1, entries[j].corr);
    }

    // 找出最相关特征
    qsort(entries, FEATURES, sizeof(CorrEntry), cmp_corr);
    int best_idx = entries[0].idx;
    printf("\n最相关特征: 特征 %d (相关系数 = %.4f)\n", best_idx + 1, entries[0].corr);

    // ================== (1) 一元线性回归（任意特征：特征6 RM） ==================
    int demo_feat = 5;  // 特征6（平均房间数）
    double* x_train_demo = (double*)malloc(train_n * sizeof(double));
    double* y_train_demo = (double*)malloc(train_n * sizeof(double));
    double* x_test_demo  = (double*)malloc(test_n  * sizeof(double));
    double* y_test_demo  = (double*)malloc(test_n  * sizeof(double));
    for (i = 0; i < train_n; i++) {
        x_train_demo[i] = train[i].features[demo_feat];
        y_train_demo[i] = train[i].price;
    }
    for (i = 0; i < test_n; i++) {
        x_test_demo[i] = test[i].features[demo_feat];
        y_test_demo[i] = test[i].price;
    }
    double mean_demo, std_demo;
    normalize_feature(x_train_demo, train_n, &mean_demo, &std_demo);
    for (i = 0; i < test_n; i++) x_test_demo[i] = (x_test_demo[i] - mean_demo) / std_demo;
    double theta_demo[2];
    train_unary(theta_demo, x_train_demo, y_train_demo, train_n, ALPHA, EPOCHS);
    double rmse_demo = compute_rmse_unary(theta_demo, x_test_demo, y_test_demo, test_n);
    printf("\n=== (1) 一元线性回归（特征6: 平均房间数）===\n");
    printf("截距 = %.4f, 斜率 = %.4f\n", theta_demo[0], theta_demo[1]);
    printf("测试集 RMSE = %.4f\n", rmse_demo);
    free(x_train_demo); free(y_train_demo); free(x_test_demo); free(y_test_demo);

    // ================== (2) 基于最相关特征的一元线性回归 ==================
    double* x_train_best = (double*)malloc(train_n * sizeof(double));
    double* y_train_best = (double*)malloc(train_n * sizeof(double));
    double* x_test_best  = (double*)malloc(test_n  * sizeof(double));
    double* y_test_best  = (double*)malloc(test_n  * sizeof(double));
    for (i = 0; i < train_n; i++) {
        x_train_best[i] = train[i].features[best_idx];
        y_train_best[i] = train[i].price;
    }
    for (i = 0; i < test_n; i++) {
        x_test_best[i] = test[i].features[best_idx];
        y_test_best[i] = test[i].price;
    }
    double mean_best, std_best;
    normalize_feature(x_train_best, train_n, &mean_best, &std_best);
    for (i = 0; i < test_n; i++) x_test_best[i] = (x_test_best[i] - mean_best) / std_best;
    double theta_best[2];
    train_unary(theta_best, x_train_best, y_train_best, train_n, ALPHA, EPOCHS);
    double rmse_best = compute_rmse_unary(theta_best, x_test_best, y_test_best, test_n);
    printf("\n=== (2) 基于最相关特征的一元线性回归（特征%d）===\n", best_idx + 1);
    printf("截距 = %.4f, 斜率 = %.4f\n", theta_best[0], theta_best[1]);
    printf("测试集 RMSE = %.4f\n", rmse_best);
    free(x_train_best); free(y_train_best); free(x_test_best); free(y_test_best);

    // ================== (3) 13个特征多元线性回归 ==================
    double** X_train_all = (double**)malloc(train_n * sizeof(double*));
    double** X_test_all  = (double**)malloc(test_n  * sizeof(double*));
    for (i = 0; i < train_n; i++) {
        X_train_all[i] = (double*)malloc(FEATURES * sizeof(double));
        for (j = 0; j < FEATURES; j++) X_train_all[i][j] = train[i].features[j];
    }
    for (i = 0; i < test_n; i++) {
        X_test_all[i] = (double*)malloc(FEATURES * sizeof(double));
        for (j = 0; j < FEATURES; j++) X_test_all[i][j] = test[i].features[j];
    }
    double* y_train_all = (double*)malloc(train_n * sizeof(double));
    double* y_test_all  = (double*)malloc(test_n  * sizeof(double));
    for (i = 0; i < train_n; i++) y_train_all[i] = train[i].price;
    for (i = 0; i < test_n; i++)  y_test_all[i]  = test[i].price;

    double* means_all = (double*)malloc(FEATURES * sizeof(double));
    double* stds_all  = (double*)malloc(FEATURES * sizeof(double));
    normalize_features_matrix(X_train_all, train_n, FEATURES, means_all, stds_all);
    for (j = 0; j < FEATURES; j++) {
        for (i = 0; i < test_n; i++) {
            X_test_all[i][j] = (X_test_all[i][j] - means_all[j]) / stds_all[j];
        }
    }

    double theta_all[FEATURES + 1];
    train_multi(theta_all, X_train_all, y_train_all, train_n, FEATURES, ALPHA, EPOCHS);
    double rmse_all = compute_rmse_multi(theta_all, X_test_all, y_test_all, test_n, FEATURES);
    printf("\n=== (3) 13个特征多元线性回归模型 ===\n");
    printf("测试集 RMSE = %.4f\n", rmse_all);

    for (i = 0; i < train_n; i++) free(X_train_all[i]);
    for (i = 0; i < test_n; i++) free(X_test_all[i]);
    free(X_train_all); free(X_test_all);
    free(y_train_all); free(y_test_all);
    free(means_all); free(stds_all);

    // ================== (4) 使用最相关4个特征的多元线性回归 ==================
    int m = TOP_K;
    int top_indices[TOP_K];
    for (i = 0; i < TOP_K; i++) top_indices[i] = entries[i].idx;

    double** X_train_top = (double**)malloc(train_n * sizeof(double*));
    double** X_test_top  = (double**)malloc(test_n  * sizeof(double*));
    for (i = 0; i < train_n; i++) {
        X_train_top[i] = (double*)malloc(m * sizeof(double));
        for (j = 0; j < m; j++) X_train_top[i][j] = train[i].features[top_indices[j]];
    }
    for (i = 0; i < test_n; i++) {
        X_test_top[i] = (double*)malloc(m * sizeof(double));
        for (j = 0; j < m; j++) X_test_top[i][j] = test[i].features[top_indices[j]];
    }
    double* y_train_top = (double*)malloc(train_n * sizeof(double));
    double* y_test_top  = (double*)malloc(test_n  * sizeof(double));
    for (i = 0; i < train_n; i++) y_train_top[i] = train[i].price;
    for (i = 0; i < test_n; i++)  y_test_top[i]  = test[i].price;

    double* means_top = (double*)malloc(m * sizeof(double));
    double* stds_top  = (double*)malloc(m * sizeof(double));
    normalize_features_matrix(X_train_top, train_n, m, means_top, stds_top);
    for (j = 0; j < m; j++) {
        for (i = 0; i < test_n; i++) {
            X_test_top[i][j] = (X_test_top[i][j] - means_top[j]) / stds_top[j];
        }
    }

    double theta_top[m + 1];
    train_multi(theta_top, X_train_top, y_train_top, train_n, m, ALPHA, EPOCHS);
    double rmse_top = compute_rmse_multi(theta_top, X_test_top, y_test_top, test_n, m);
    printf("\n=== (4) 使用最相关 %d 个特征的多元线性回归模型 ===\n", m);
    printf("所选特征索引: ");
    for (j = 0; j < m; j++) printf("%d ", top_indices[j] + 1);
    printf("\n测试集 RMSE = %.4f\n", rmse_top);

    for (i = 0; i < train_n; i++) free(X_train_top[i]);
    for (i = 0; i < test_n; i++) free(X_test_top[i]);
    free(X_train_top); free(X_test_top);
    free(y_train_top); free(y_test_top);
    free(means_top); free(stds_top);

    return 0;
}
