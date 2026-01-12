#include <stdio.h>
#include <stdlib.h> 

void Require(struct student *head)
{
    int cnt;
    struct student *p;

     if ( head != NULL ) {
         cnt = 0;
         p = head;
         while (p != NULL) {
             if(p->score < 60){
                 printf ("%d %.1f\n", p->num, p->score); 
                 cnt++; 
             }
             p = p->next;
         }
         printf ("%d\n", cnt);
      }
  }
