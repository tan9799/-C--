#include<stdio.h>
struct employee
{
	char name[11];
	float salary;
	float unsteady_salary;
	float out_salary;
	float real_salary;
}; 
int main()
{
	int N, i, j;
	scanf("%d", &N);
	struct employee emp[N]; 
	for(i = 0; i < N; i++){
    	scanf("%s %f %f %f", emp[i].name, &emp[i].salary, &emp[i].unsteady_salary, &emp[i].out_salary);
    	emp[i].real_salary = emp[i].salary + emp[i].unsteady_salary - emp[i].out_salary;
    } 
    for(j = 0; j < N; j++)
    {
    	printf("%s %.2f\n", emp[j].name, emp[j].real_salary);
	}
	return 0;
}
