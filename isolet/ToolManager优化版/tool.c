#include "tool.h"
#include "utils.h"

void showAllTools() {
    if (toolHead == NULL) {
        printf("暂无工具数据。\n");
        return;
    }
    printf("\n%-8s %-20s %-12s %-8s %-8s %-8s %-12s %-10s\n",
        "编号", "名称", "类型", "库存", "已借", "状态", "位置", "单价");
    Tool* p = toolHead;
    while (p) {
        printf("%-8d %-20s %-12s %-8d %-8d %-8s %-12s %-10.2f\n",
            p->id, p->name, p->type, p->stock, p->borrowedCount,
            p->status == TOOL_NORMAL ? "正常" : "损坏",
            p->location, p->price);
        p = p->next;
    }
}

Tool* findToolById(int id) {
    Tool* p = toolHead;
    while (p) {
        if (p->id == id) return p;
        p = p->next;
    }
    return NULL;
}

void addTool() {
    Tool* newTool = (Tool*)malloc(sizeof(Tool));
    printf("请输入工具编号: ");
    scanf("%d", &newTool->id);
    clearInputBuffer();
    if (findToolById(newTool->id) != NULL) {
        printf("工具编号已存在！\n");
        free(newTool);
        return;
    }
    printf("请输入工具名称: ");
    fgets(newTool->name, MAX_NAME, stdin);
    newTool->name[strcspn(newTool->name, "\n")] = '\0';
    printf("请输入工具类型: ");
    fgets(newTool->type, MAX_TYPE, stdin);
    newTool->type[strcspn(newTool->type, "\n")] = '\0';
    newTool->stock = getValidInt("请输入库存数量: ", 0, -1);
    newTool->status = TOOL_NORMAL;
    newTool->borrowedCount = 0;
    printf("请输入存放位置: ");
    fgets(newTool->location, MAX_LOCATION, stdin);
    newTool->location[strcspn(newTool->location, "\n")] = '\0';
    newTool->price = getValidFloat("请输入单价: ", 0);
    strcpy(newTool->history, "");
    newTool->next = toolHead;
    toolHead = newTool;
    printf("工具添加成功！\n");
}

void deleteTool() {
    int id = getValidInt("请输入要删除的工具编号: ", 0, -1);
    Tool* p = toolHead, * prev = NULL;
    while (p) {
        if (p->id == id) {
            printf("确认删除工具 %s (编号%d)? (y/n): ", p->name, p->id);
            char confirm = getchar();
            clearInputBuffer();
            if (confirm == 'y' || confirm == 'Y') {
                if (prev == NULL) toolHead = p->next;
                else prev->next = p->next;
                free(p);
                printf("删除成功！\n");
            }
            else {
                printf("取消删除。\n");
            }
            return;
        }
        prev = p;
        p = p->next;
    }
    printf("未找到编号为 %d 的工具。\n", id);
}

void modifyTool() {
    int id = getValidInt("请输入要修改的工具编号: ", 0, -1);
    Tool* t = findToolById(id);
    if (!t) {
        printf("工具不存在！\n");
        return;
    }
    printf("正在修改工具 %s (编号%d):\n", t->name, t->id);
    printf("是否修改名称？(y/n): ");
    if (getchar() == 'y') {
        clearInputBuffer();
        printf("新名称: ");
        fgets(t->name, MAX_NAME, stdin);
        t->name[strcspn(t->name, "\n")] = '\0';
    }
    else clearInputBuffer();
    printf("是否修改类型？(y/n): ");
    if (getchar() == 'y') {
        clearInputBuffer();
        printf("新类型: ");
        fgets(t->type, MAX_TYPE, stdin);
        t->type[strcspn(t->type, "\n")] = '\0';
    }
    else clearInputBuffer();
    printf("是否修改库存？(y/n): ");
    if (getchar() == 'y') {
        clearInputBuffer();
        t->stock = getValidInt("新库存: ", 0, -1);
    }
    else clearInputBuffer();
    printf("是否修改状态（正常/损坏）？(y/n): ");
    if (getchar() == 'y') {
        clearInputBuffer();
        printf("1-正常 2-损坏: ");
        int st;
        scanf("%d", &st);
        clearInputBuffer();
        t->status = (st == 1) ? TOOL_NORMAL : TOOL_DAMAGED;
    }
    else clearInputBuffer();
    printf("是否修改位置？(y/n): ");
    if (getchar() == 'y') {
        clearInputBuffer();
        printf("新位置: ");
        fgets(t->location, MAX_LOCATION, stdin);
        t->location[strcspn(t->location, "\n")] = '\0';
    }
    else clearInputBuffer();
    printf("是否修改单价？(y/n): ");
    if (getchar() == 'y') {
        clearInputBuffer();
        t->price = getValidFloat("新单价: ", 0);
    }
    else clearInputBuffer();
    printf("修改完成！\n");
}

void queryTool() {
    printf("\n查询方式: 1-按编号精确 2-按名称模糊 3-按类型: ");
    int choice = getValidInt("", 1, 3);
    if (choice == 1) {
        int id = getValidInt("请输入工具编号: ", 0, -1);
        Tool* t = findToolById(id);
        if (t) {
            printf("编号:%d 名称:%s 类型:%s 库存:%d 状态:%s 已借:%d 位置:%s 单价:%.2f\n",
                t->id, t->name, t->type, t->stock,
                t->status == 0 ? "正常" : "损坏",
                t->borrowedCount, t->location, t->price);
        }
        else printf("未找到。\n");
    }
    else if (choice == 2) {
        char keyword[MAX_NAME];
        printf("请输入名称关键词: ");
        fgets(keyword, MAX_NAME, stdin);
        keyword[strcspn(keyword, "\n")] = '\0';
        Tool* p = toolHead;
        int found = 0;
        while (p) {
            if (strstr(p->name, keyword)) {
                printf("编号:%d 名称:%s 类型:%s 库存:%d\n", p->id, p->name, p->type, p->stock);
                found = 1;
            }
            p = p->next;
        }
        if (!found) printf("未找到。\n");
    }
    else if (choice == 3) {
        char type[MAX_TYPE];
        printf("请输入工具类型: ");
        fgets(type, MAX_TYPE, stdin);
        type[strcspn(type, "\n")] = '\0';
        Tool* p = toolHead;
        int found = 0;
        while (p) {
            if (strcmp(p->type, type) == 0) {
                printf("编号:%d 名称:%s 库存:%d\n", p->id, p->name, p->stock);
                found = 1;
            }
            p = p->next;
        }
        if (!found) printf("未找到。\n");
    }
}

void repairTool() {
    int id = getValidInt("请输入要维修的工具编号: ", 0, -1);
    Tool* t = findToolById(id);
    if (!t) {
        printf("工具不存在！\n");
        return;
    }
    if (t->status == TOOL_NORMAL) {
        printf("工具状态已是正常，无需维修。\n");
        return;
    }
    t->status = TOOL_NORMAL;
    printf("工具 %s 已维修恢复为正常状态。\n", t->name);
}

void showToolHistory(int toolId) {
    Tool* t = findToolById(toolId);
    if (!t) {
        printf("工具不存在！\n");
        return;
    }
    printf("\n工具 %s (编号%d) 借用历史：\n", t->name, t->id);
    if (strlen(t->history) == 0) printf("无历史记录。\n");
    else printf("%s\n", t->history);
}

int getToolCount() {
    int count = 0;
    Tool* p = toolHead;
    while (p) {
        count++;
        p = p->next;
    }
    return count;
}