#include <stdio.h>
#define N 5
struct student
{ 
      char num[6];
      char name[8];
      float score[3];
      float avr;
} stu[N];

int main()
{ 
      int i,j,maxi;
      float sum,max,average;
      for (i=0;i<N;i++)
      {
        scanf("%s",stu[i].num);
        scanf("%s",stu[i].name);
        for (j=0;j<3;j++)
          {
            scanf("%f",stu[i].score);
          }
      }
      average=0;
      max=0;
      maxi=0;
      for (i=0;i<N;i++)
      {
         sum=0;
         for (j=0;j<3;j++)
            sum += stu[i].score[j];
         stu[i].avr=sum/3.0;
         average+=stu[i].avr;
         if (sum>max)
         {
                 max=sum;
            maxi=i;
         }
      }
      average/=N;

      for (i=0;i<N;i++)
      {
         printf("%5s%10s",stu[i].num,stu[i].name);
         for (j=0;j<3;j++)
               printf("%9.2f",stu[i].score[j]);
         printf("    %8.2f\n",stu[i].avr);
      }
    printf("average=%5.2f\n",average);
    printf("The highest score is : student %s,%s\n",stu[maxi].num,stu[maxi].name);
    printf("his scores are:%6.2f,%6.2f,%6.2f,average:%5.2f.\n",stu[maxi].score[0],stu[maxi].score[1],stu[maxi].score[2],stu[maxi].avr);
    return 0;
 }
