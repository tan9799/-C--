#include <stdio.h>
main()
{  
    FILE *fp;
        char str[10];
    fp=fopen("myfile.dat","w");
    fputs("abc",fp); 
    fclose(fp);
    fp=fopen("myfile.dat","a+");
    fprintf(fp,"%d",28);
    rewind(fp);
    fscanf(fp,"%s",str); 
    puts(str);
    fclose(fp);
}
