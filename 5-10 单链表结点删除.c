#include <stdio.h>
#include <stdlib.h>
struct ListNode *deletem( struct ListNode *L, int m ) 
{
    struct ListNode *head,*p1,*p2;
    head=L;
    while(head != NULL && head->data==m) 
    {
        p2=head;
        head=head->next;
        free(p2);
    }
    if(head==NULL) return head; 
    p1=head;  
    p2=head->next;
    while(p2!=NULL)
    {
        if(p2->data == m)  
        {
            p1->next=p2->next;
            free(p2);
            p2=p1->next; 
        }
        else  
        {
            p1=p2;
            p2=p2->next;
        }
    }
    return head;
}
