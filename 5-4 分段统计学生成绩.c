#include<stdio.h>
#define MAXN 100
struct student{
  char name[20];
  int score;
};
void cnt_score( struct student *p, int n );

int main()
{   
  int i, n;
  struct student stu[MAXN];

  scanf("%d", &n);
  for(i = 0; i < n; i++){
    scanf("%s%d", stu[i].name, &stu[i].score);
  } 
  cnt_score(stu, n);

  return 0;
}

void cnt_score(struct student *p, int n)
{   
    int cnt_a = 0, cnt_p = 0, cnt_f = 0;
    struct student *q = p + n - 1; 

    while ( p <= q ){
        if (p->score >= 80) cnt_a++;
        else if (p->score >= 60) cnt_p++;
        else cnt_f++;
        p++;
    }
    printf("%d %d %d\n", cnt_a, cnt_p, cnt_f);
}

