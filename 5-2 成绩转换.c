#include<stdio.h>
#define MAXN 100
struct student{
  char name[20];
  int score;
  char grade;
};
void set_grade( struct student *p, int n );

int main()
{   
  int i, n;
  struct student stu[MAXN];
  scanf("%d", &n);
  for(i = 0; i < n; i++){
    scanf("%s%d", stu[i].name, &stu[i].score);
  } 
  set_grade(stu, n);
  for(i = 0; i < n; i++)
    printf("%s %c\n", stu[i].name, stu[i].grade);

  return 0;
}

void set_grade(struct student *p, int n)
{    
    struct student *q = p + n - 1;

    while(p <= q){
        if(p->score >= 60)
            p->grade = 'P';
        else
            p->grade = 'F';
        p++;
    }
}

