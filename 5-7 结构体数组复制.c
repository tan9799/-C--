#include <stdio.h>
struct ss
{
  char no[10];
  int data;
};
int main()
{
  struct ss m[2]={{"A01", 5},{"A02", 8}},n[2];
  int i;
  for(i=0;i<2;i++)
     n[i]=m[i];
  for(i=0;i<2;i++)
     printf("%s,%d\n",n[i].no, n[i].data);
  return 0;
}
