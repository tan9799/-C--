#include "borrow.h"
#include "tool.h"
#include "utils.h"

void borrowTool() {
    int toolId = getValidInt("请输入工具编号: ", 0, -1);
    Tool* t = findToolById(toolId);
    if (!t) {
        printf("工具不存在！\n");
        return;
    }
    if (t->status == TOOL_DAMAGED) {
        printf("该工具已损坏，无法借用。\n");
        return;
    }
    if (t->stock <= 0) {
        printf("库存不足，无法借用。\n");
        return;
    }
    int qty = getValidInt("请输入借用数量: ", 1, t->stock);
    char borrower[MAX_PERSON];
    printf("请输入借用人姓名: ");
    fgets(borrower, MAX_PERSON, stdin);
    borrower[strcspn(borrower, "\n")] = '\0';
    char borrowDate[DATE_LEN], dueDate[DATE_LEN];
    do {
        printf("请输入借用日期(YYYY-MM-DD): ");
        fgets(borrowDate, DATE_LEN, stdin);
        borrowDate[strcspn(borrowDate, "\n")] = '\0';
        if (!isDateValid(borrowDate)) printf("日期格式错误，请重新输入。\n");
    } while (!isDateValid(borrowDate));
    do {
        printf("请输入应还日期(YYYY-MM-DD): ");
        fgets(dueDate, DATE_LEN, stdin);
        dueDate[strcspn(dueDate, "\n")] = '\0';
        if (!isDateValid(dueDate)) printf("日期格式错误，请重新输入。\n");
    } while (!isDateValid(dueDate));

    t->stock -= qty;
    t->borrowedCount += qty;

    char newHistory[200];
    sprintf(newHistory, "%s %s 借 %d 件; ", borrowDate, borrower, qty);
    strcat(t->history, newHistory);

    BorrowRecord* newRec = (BorrowRecord*)malloc(sizeof(BorrowRecord));
    newRec->recordId = nextRecordId++;
    newRec->toolId = toolId;
    strcpy(newRec->borrower, borrower);
    strcpy(newRec->borrowDate, borrowDate);
    strcpy(newRec->dueDate, dueDate);
    newRec->quantity = qty;
    newRec->status = BORROW_STATUS_BORROWED;
    newRec->fine = 0;
    newRec->next = recordHead;
    recordHead = newRec;

    printf("借用成功！记录编号: %d\n", newRec->recordId);
}

void returnTool() {
    printf("归还方式: 1-按记录编号 2-按工具编号+借用人: ");
    int way = getValidInt("", 1, 2);
    BorrowRecord* p = recordHead;
    BorrowRecord* target = NULL;
    if (way == 1) {
        int rid = getValidInt("请输入记录编号: ", 0, -1);
        while (p) {
            if (p->recordId == rid && p->status == BORROW_STATUS_BORROWED) {
                target = p;
                break;
            }
            p = p->next;
        }
    }
    else {
        int tid = getValidInt("请输入工具编号: ", 0, -1);
        char name[MAX_PERSON];
        printf("请输入借用人姓名: ");
        fgets(name, MAX_PERSON, stdin);
        name[strcspn(name, "\n")] = '\0';
        while (p) {
            if (p->toolId == tid && strcmp(p->borrower, name) == 0 && p->status == BORROW_STATUS_BORROWED) {
                target = p;
                break;
            }
            p = p->next;
        }
    }
    if (!target) {
        printf("未找到有效的未归还记录。\n");
        return;
    }
    Tool* t = findToolById(target->toolId);
    if (t) {
        t->stock += target->quantity;
        t->borrowedCount -= target->quantity;
    }

    time_t now = time(NULL);
    struct tm* local = localtime(&now);
    char today[DATE_LEN];
    sprintf(today, "%04d-%02d-%02d", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday);
    int overdue = daysBetween(today, target->dueDate);
    if (overdue > 0) {
        float fine = overdue * 1.0f;
        target->fine = fine;
        printf("逾期 %d 天，罚款 %.2f 元。\n", overdue, fine);
    }
    target->status = BORROW_STATUS_RETURNED;
    printf("归还成功！\n");

    char returnHistory[200];
    sprintf(returnHistory, "%s %s 还 %d 件; ", target->borrowDate, target->borrower, target->quantity);
    strcat(t->history, returnHistory);
}

void listBorrowRecords(int sortByDate) {
    if (recordHead == NULL) {
        printf("暂无借还记录。\n");
        return;
    }
    int count = 0;
    BorrowRecord* p = recordHead;
    while (p) {
        count++;
        p = p->next;
    }
    BorrowRecord** arr = (BorrowRecord**)malloc(count * sizeof(BorrowRecord*));
    p = recordHead;
    for (int i = 0; i < count; i++) {
        arr[i] = p;
        p = p->next;
    }
    if (sortByDate) {
        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                if (strcmp(arr[j]->borrowDate, arr[j + 1]->borrowDate) > 0) {
                    BorrowRecord* tmp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = tmp;
                }
            }
        }
    }
    int pageSize = 10;
    int totalPages = (count + pageSize - 1) / pageSize;
    int currentPage = 0;
    char key;
    do {
        system("cls");
        printf("\n===== 借还记录列表 (第%d/%d页) =====\n", currentPage + 1, totalPages);
        printf("%-6s %-6s %-12s %-12s %-12s %-6s %-8s %s\n",
            "记录ID", "工具ID", "借用人", "借出日期", "应还日期", "数量", "罚款", "状态");
        int start = currentPage * pageSize;
        int end = (start + pageSize) < count ? start + pageSize : count;
        for (int i = start; i < end; i++) {
            BorrowRecord* rec = arr[i];
            printf("%-6d %-6d %-12s %-12s %-12s %-6d %-8.2f %s\n",
                rec->recordId, rec->toolId, rec->borrower, rec->borrowDate, rec->dueDate,
                rec->quantity, rec->fine, rec->status == 0 ? "已借" : "已还");
        }
        printf("\n按 N 下一页, P 上一页, Q 退出: ");
        key = _getch();
        if (key == 'n' || key == 'N') {
            if (currentPage < totalPages - 1) currentPage++;
        }
        else if (key == 'p' || key == 'P') {
            if (currentPage > 0) currentPage--;
        }
    } while (key != 'q' && key != 'Q');
    free(arr);
}