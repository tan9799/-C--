#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct stud_node {
    int num;                
    char name[20];         
    int score;              
    struct stud_node *next; 
};

struct stud_node *Creat_Stu_Doc();
void Print_Stu_Doc(struct stud_node *head, int min_score);

int main() {
    struct stud_node *head;
    int min_score;
    
    head = Creat_Stu_Doc();
    
    scanf("%d", &min_score);
    
    Print_Stu_Doc(head, min_score);
    
    struct stud_node *p;
    while (head != NULL) {
        p = head;
        head = head->next;
        free(p);
    }
    
    return 0;
}

struct stud_node *Creat_Stu_Doc() {
    struct stud_node *head = NULL, *tail = NULL, *p;
    int num;
    char name[20];
    int score;
    
    while (1) {
        scanf("%d", &num);
        if (num == 0) break;  
        
        scanf("%s %d", name, &score);
        
        p = (struct stud_node *)malloc(sizeof(struct stud_node));
        p->num = num;
        strcpy(p->name, name);
        p->score = score;
        p->next = NULL;
        
        if (head == NULL) {
            head = p;
        } else {
            tail->next = p;
        }
        tail = p;
    }
    
    return head;
}
void Print_Stu_Doc(struct stud_node *head, int min_score) {
    struct stud_node *p = head;
    int found = 0;  
    
    while (p != NULL) {
        if (p->score >= min_score) {
            printf("%d %s %d\n", p->num, p->name, p->score);
            found = 1;
        }
        p = p->next;
    }
}

    
