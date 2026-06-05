#ifndef TOOL_H
#define TOOL_H

#include "common.h"

void showAllTools();
void addTool();
void deleteTool();
void modifyTool();
void queryTool();
void repairTool();
void showToolHistory(int toolId);
Tool* findToolById(int id);
int getToolCount();

#endif