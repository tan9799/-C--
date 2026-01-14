#include <stdio.h>

struct stu
{
	int num[13];
	char name[21];
	int score;
	char grade;
};
int main()
{
	int n, sum = 0, i, j;
	scanf("%d", &n);
	struct stu student[10];
	for(i = 0; i < n; i++)
	{
		scanf("%s %s %d", student[i].num, student[i].name, &student[i].score);
		if(student[i].score >= 85)
		{
			student[i].grade = 'A';
		}
		else if(student[i].score >= 70)
		{
			student[i].grade = 'B';
		}
		else if(student[i].score >= 60)
		{
			student[i].grade = 'C';
		}
		else
		{
			student[i].grade = 'D';
			sum++;
		}
	}
	for(j = 0; j < n; j++)
	{
		printf("%s %s %c\n", student[j].num, student[j].name, student[j].grade);
	}
	printf("%d", sum);
	return 0;
}
