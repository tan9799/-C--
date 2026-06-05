#include "stats.h"
#include "tool.h"
#include "utils.h"

void totalStockAndValue() {
    int totalStock = 0;
    float totalValue = 0;
    Tool* p = toolHead;
    while (p) {
        totalStock += p->stock;
        totalValue += p->stock * p->price;
        p = p->next;
    }
    printf("\n工厂工具总数量: %d\n", totalStock);
    printf("工厂工具总价值: %.2f\n", totalValue);
}

void lowStockWarning() {
    int threshold = getValidInt("请输入安全库存阈值: ", 0, -1);
    Tool* p = toolHead;
    int found = 0;
    printf("\n以下工具库存低于阈值 %d:\n", threshold);
    while (p) {
        if (p->stock < threshold) {
            printf("编号:%d 名称:%s 当前库存:%d\n", p->id, p->name, p->stock);
            found = 1;
        }
        p = p->next;
    }
    if (!found) printf("所有工具库存充足。\n");
}

void borrowRanking() {
    int toolCount = getToolCount();
    if (toolCount == 0) { printf("暂无工具数据。\n"); return; }
    typedef struct {
        int id;
        char name[MAX_NAME];
        int times;
    } RankNode;
    RankNode* ranks = (RankNode*)malloc(toolCount * sizeof(RankNode));
    Tool* t = toolHead;
    int idx = 0;
    while (t) {
        ranks[idx].id = t->id;
        strcpy(ranks[idx].name, t->name);
        ranks[idx].times = 0;
        idx++;
        t = t->next;
    }
    BorrowRecord* r = recordHead;
    while (r) {
        if (r->status == BORROW_STATUS_BORROWED || r->status == BORROW_STATUS_RETURNED) {
            for (int i = 0; i < toolCount; i++) {
                if (ranks[i].id == r->toolId) {
                    ranks[i].times++;
                    break;
                }
            }
        }
        r = r->next;
    }
    for (int i = 0; i < toolCount - 1; i++) {
        for (int j = 0; j < toolCount - i - 1; j++) {
            if (ranks[j].times < ranks[j + 1].times) {
                RankNode tmp = ranks[j];
                ranks[j] = ranks[j + 1];
                ranks[j + 1] = tmp;
            }
        }
    }
    printf("\n借用次数排行榜（前5）:\n");
    for (int i = 0; i < toolCount && i < 5; i++) {
        printf("%d. %s (编号%d) - 借用%d次 ", i + 1, ranks[i].name, ranks[i].id, ranks[i].times);
        for (int j = 0; j < ranks[i].times && j < 50; j++) printf("#");
        printf("\n");
    }
    free(ranks);
}

void borrowRankingByQuantity() {
    int toolCount = getToolCount();
    if (toolCount == 0) { printf("暂无工具数据。\n"); return; }
    typedef struct {
        int id;
        char name[MAX_NAME];
        int totalQty;
    } RankNode;
    RankNode* ranks = (RankNode*)malloc(toolCount * sizeof(RankNode));
    Tool* t = toolHead;
    int idx = 0;
    while (t) {
        ranks[idx].id = t->id;
        strcpy(ranks[idx].name, t->name);
        ranks[idx].totalQty = 0;
        idx++;
        t = t->next;
    }
    BorrowRecord* r = recordHead;
    while (r) {
        if (r->status == BORROW_STATUS_BORROWED || r->status == BORROW_STATUS_RETURNED) {
            for (int i = 0; i < toolCount; i++) {
                if (ranks[i].id == r->toolId) {
                    ranks[i].totalQty += r->quantity;
                    break;
                }
            }
        }
        r = r->next;
    }
    for (int i = 0; i < toolCount - 1; i++) {
        for (int j = 0; j < toolCount - i - 1; j++) {
            if (ranks[j].totalQty < ranks[j + 1].totalQty) {
                RankNode tmp = ranks[j];
                ranks[j] = ranks[j + 1];
                ranks[j + 1] = tmp;
            }
        }
    }
    printf("\n借用总数量排行榜（前5）:\n");
    for (int i = 0; i < toolCount && i < 5; i++) {
        printf("%d. %s (编号%d) - 总借用数量%d\n", i + 1, ranks[i].name, ranks[i].id, ranks[i].totalQty);
    }
    free(ranks);
}

void checkOverdueRecords() {
    time_t now = time(NULL);
    struct tm* local = localtime(&now);
    char today[DATE_LEN];
    sprintf(today, "%04d-%02d-%02d", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday);
    BorrowRecord* p = recordHead;
    int overdueCount = 0;
    while (p) {
        if (p->status == BORROW_STATUS_BORROWED && strcmp(p->dueDate, today) < 0) {
            if (overdueCount == 0) printf("\n========== 逾期未归还提醒 ==========\n");
            Tool* t = findToolById(p->toolId);
            printf("记录编号:%d 工具:%s 借用人:%s 应还日期:%s 已逾期\n",
                p->recordId, t ? t->name : "未知", p->borrower, p->dueDate);
            overdueCount++;
        }
        p = p->next;
    }
    if (overdueCount > 0) printf("=====================================\n");
    else printf("无逾期记录。\n");
}

void statisticsByType() {
    if (toolHead == NULL) { printf("暂无工具数据。\n"); return; }
    typedef struct {
        char type[MAX_TYPE];
        int count;
        float totalValue;
    } TypeStat;
    TypeStat* stats = NULL;
    int typeCount = 0;
    Tool* p = toolHead;
    while (p) {
        int idx = -1;
        for (int i = 0; i < typeCount; i++) {
            if (strcmp(stats[i].type, p->type) == 0) { idx = i; break; }
        }
        if (idx == -1) {
            stats = (TypeStat*)realloc(stats, (typeCount + 1) * sizeof(TypeStat));
            strcpy(stats[typeCount].type, p->type);
            stats[typeCount].count = 0;
            stats[typeCount].totalValue = 0;
            idx = typeCount;
            typeCount++;
        }
        stats[idx].count += p->stock;
        stats[idx].totalValue += p->stock * p->price;
        p = p->next;
    }
    printf("\n===== 按工具类型统计 =====\n");
    for (int i = 0; i < typeCount; i++) {
        float avg = (stats[i].count > 0) ? (stats[i].totalValue / stats[i].count) : 0;
        printf("类型: %-15s 总数量: %-8d 总价值: %-10.2f 平均单价: %.2f\n",
            stats[i].type, stats[i].count, stats[i].totalValue, avg);
    }
    free(stats);
}

void exportStatsToFile() {
    FILE* fp = fopen("data/stats_export.txt", "w");
    if (!fp) { printf("无法创建文件！\n"); return; }
    fprintf(fp, "===== 工厂工具统计报告 =====\n");
    int totalStock = 0;
    float totalValue = 0;
    Tool* p = toolHead;
    while (p) {
        totalStock += p->stock;
        totalValue += p->stock * p->price;
        p = p->next;
    }
    fprintf(fp, "工具总数量: %d\n", totalStock);
    fprintf(fp, "工具总价值: %.2f\n", totalValue);
    fprintf(fp, "\n工具详细信息:\n");
    fprintf(fp, "编号\t名称\t\t库存\t单价\n");
    p = toolHead;
    while (p) {
        fprintf(fp, "%d\t%s\t\t%d\t%.2f\n", p->id, p->name, p->stock, p->price);
        p = p->next;
    }
    fclose(fp);
    printf("统计结果已导出到 data/stats_export.txt\n");
}