#include<stdio.h>
struct student
{
	char number[6];
	char name[11];
	int score;
};
int main()
{
	int N, i, sum = 0, j;
	scanf("%d", &N);
	struct student stu[N];
	for(i = 0; i < N; i++)
	{
		scanf("%s %s %d", stu[i].number, stu[i].name, &stu[i].score);
		sum += stu[i].score;
	}
	double ave = 1.00 * sum / N;
	printf("%.2f\n", ave);
	for(j = 0; j < N; j++)
	{
		if(ave > stu[j].score)
		{
			printf("%s %s\n", stu[j].name, stu[j].number);
		}
	}
	return 0;
}
