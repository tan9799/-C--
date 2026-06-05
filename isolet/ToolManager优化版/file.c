#include "file.h"
#include "user.h"   // 为了调用 saveUsersToFile / loadUsersFromFile

void loadToolsFromFile() {
    FILE* fp = fopen("data/tools.dat", "rb");
    if (!fp) return;
    Tool temp;
    while (fread(&temp, sizeof(Tool), 1, fp) == 1) {
        Tool* newNode = (Tool*)malloc(sizeof(Tool));
        *newNode = temp;
        newNode->next = toolHead;
        toolHead = newNode;
    }
    fclose(fp);
}

void saveToolsToFile() {
    FILE* fp = fopen("data/tools.tmp", "wb");
    if (!fp) { printf("无法保存工具数据！\n"); return; }
    Tool* p = toolHead;
    while (p) {
        fwrite(p, sizeof(Tool), 1, fp);
        p = p->next;
    }
    fclose(fp);
    remove("data/tools.dat");
    rename("data/tools.tmp", "data/tools.dat");
}

void loadRecordsFromFile() {
    FILE* fp = fopen("data/records.dat", "rb");
    if (!fp) return;
    BorrowRecord temp;
    while (fread(&temp, sizeof(BorrowRecord), 1, fp) == 1) {
        BorrowRecord* newNode = (BorrowRecord*)malloc(sizeof(BorrowRecord));
        *newNode = temp;
        newNode->next = recordHead;
        recordHead = newNode;
    }
    fclose(fp);
}

void saveRecordsToFile() {
    FILE* fp = fopen("data/records.tmp", "wb");
    if (!fp) { printf("无法保存记录数据！\n"); return; }
    BorrowRecord* p = recordHead;
    while (p) {
        fwrite(p, sizeof(BorrowRecord), 1, fp);
        p = p->next;
    }
    fclose(fp);
    remove("data/records.dat");
    rename("data/records.tmp", "data/records.dat");
}

void loadNextId() {
    FILE* fp = fopen("data/next_id.dat", "r");
    if (fp) {
        fscanf(fp, "%d", &nextRecordId);
        fclose(fp);
    }
    else {
        nextRecordId = 1;
    }
}

void saveNextId() {
    FILE* fp = fopen("data/next_id.tmp", "w");
    if (fp) {
        fprintf(fp, "%d", nextRecordId);
        fclose(fp);
        remove("data/next_id.dat");
        rename("data/next_id.tmp", "data/next_id.dat");
    }
}

void loadAllData() {
    loadToolsFromFile();
    loadRecordsFromFile();
    loadNextId();
    loadUsersFromFile();
}

void saveAllData() {
    saveToolsToFile();
    saveRecordsToFile();
    saveNextId();
    saveUsersToFile();
}

void backupData() {
    system("copy data\\tools.dat data\\tools_backup.dat");
    system("copy data\\records.dat data\\records_backup.dat");
    system("copy data\\next_id.dat data\\next_id_backup.dat");
    system("copy data\\users.dat data\\users_backup.dat");
    printf("数据已备份到 data 目录下的 *_backup.dat 文件。\n");
}

void restoreData() {
    system("copy data\\tools_backup.dat data\\tools.dat");
    system("copy data\\records_backup.dat data\\records.dat");
    system("copy data\\next_id_backup.dat data\\next_id.dat");
    system("copy data\\users_backup.dat data\\users.dat");
    printf("数据已从备份恢复，请重启程序生效。\n");
}