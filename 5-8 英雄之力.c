#include<stdio.h>
struct Hero{
    char name[20];//英雄的名字
    double Health_value;//英雄的血量
    int aggressivity;//攻击力
};
int main(){
    struct Hero hero[2];//定义两个struct Hero的英雄
    int i;
    for(i=0;i<2;i++)//初始化每个英雄的基本属性
        scanf("%s%lf%d",hero[i].name,&hero[i].Health_value,&hero[i].aggressivity);
    if(hero[0].aggressivity > hero[1].aggressivity)
    {
        printf("英雄 %s 的攻击力大于英雄 %s 的攻击力\n",hero[0].name,hero[1].name);
    }
    else
    {
         printf("英雄 %s 的攻击力大于英雄 %s 的攻击力\n",hero[1].name,hero[0].name);
    }

    return 0;
}

