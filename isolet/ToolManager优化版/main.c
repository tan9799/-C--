#include "common.h"
#include "tool.h"
#include "borrow.h"
#include "stats.h"
#include "file.h"
#include "auth.h"
#include "utils.h"
#include "user.h"

Tool* toolHead = NULL;
BorrowRecord* recordHead = NULL;
int nextRecordId = 1;
User* currentUser = NULL;

void ensureDataDir() {
    _mkdir("data");
}

void freeTools() {
    Tool* p = toolHead;
    while (p) {
        Tool* next = p->next;
        free(p);
        p = next;
    }
    toolHead = NULL;
}

void freeRecords() {
    BorrowRecord* p = recordHead;
    while (p) {
        BorrowRecord* next = p->next;
        free(p);
        p = next;
    }
    recordHead = NULL;
}

void freeUsers() {
    User* p = userHead;
    while (p) {
        User* next = p->next;
        free(p);
        p = next;
    }
    userHead = NULL;
}

void initSystem() {
    loadAllData();
    checkOverdueRecords();
}

void exitSystem() {
    saveAllData();
    freeTools();
    freeRecords();
    freeUsers();
    printf("数据已保存，再见！\n");
}

int main() {
    ensureDataDir();
    loadUsersFromFile();   // 必须最先加载用户，否则登录失败
    if (!login()) return 1;
    initSystem();

    int choice = 0;
    do {
        printf("\n========== 工厂工具管理系统 ==========\n");
        printf("当前用户: %s (%s)\n", currentUser->username, currentUser->isAdmin ? "管理员" : "普通用户");
        printf("1. 显示所有工具\n");
        printf("2. 添加工具\n");
        printf("3. 删除工具\n");
        printf("4. 修改工具\n");
        printf("5. 查询工具\n");
        printf("6. 借用工具\n");
        printf("7. 归还工具\n");
        printf("8. 查看借还记录（按日期排序）\n");
        printf("9. 查看借还记录（默认顺序）\n");
        printf("10. 库存整体统计\n");
        printf("11. 低库存预警\n");
        printf("12. 借用次数排行（按次数+柱状图）\n");
        printf("13. 借用次数排行（按数量）\n");
        printf("14. 检查逾期记录\n");
        printf("15. 维修工具\n");
        printf("16. 按工具类型统计\n");
        printf("17. 查看工具借用历史\n");
        if (currentUser->isAdmin) {
            printf("---------- 管理员专属 ----------\n");
            printf("18. 用户管理（列出/添加）\n");
            printf("19. 备份数据\n");
            printf("20. 恢复数据\n");
            printf("21. 导出统计报告\n");
        }
        else {
            printf("18. 导出统计报告\n");
        }
        printf("0. 退出系统\n");
        printf("请选择: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
        case 1: showAllTools(); break;
        case 2: if (currentUser->isAdmin) addTool(); else printf("无权限！\n"); break;
        case 3: if (currentUser->isAdmin) deleteTool(); else printf("无权限！\n"); break;
        case 4: if (currentUser->isAdmin) modifyTool(); else printf("无权限！\n"); break;
        case 5: queryTool(); break;
        case 6: borrowTool(); break;
        case 7: returnTool(); break;
        case 8: listBorrowRecords(1); break;
        case 9: listBorrowRecords(0); break;
        case 10: totalStockAndValue(); break;
        case 11: lowStockWarning(); break;
        case 12: borrowRanking(); break;
        case 13: borrowRankingByQuantity(); break;
        case 14: checkOverdueRecords(); break;
        case 15: repairTool(); break;
        case 16: statisticsByType(); break;
        case 17: {
            int tid = getValidInt("请输入工具编号: ", 0, -1);
            showToolHistory(tid);
            break;
        }
        case 18:
            if (currentUser->isAdmin) {
                printf("\n1-列出用户 2-添加用户: ");
                int sub = getValidInt("", 1, 2);
                if (sub == 1) listUsers();
                else addUser();
            }
            else {
                exportStatsToFile();
            }
            break;
        case 19: if (currentUser->isAdmin) backupData(); else printf("无权限！\n"); break;
        case 20: if (currentUser->isAdmin) restoreData(); else printf("无权限！\n"); break;
        case 21: if (currentUser->isAdmin) exportStatsToFile(); else printf("无权限！\n"); break;
        case 0: exitSystem(); break;
        default: printf("无效选择，请重新输入。\n");
        }
        if (choice != 0) pressAnyKey();
    } while (choice != 0);

    return 0;
}