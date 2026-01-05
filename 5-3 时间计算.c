#include <stdio.h>
struct { 
     int hour, minute, second;
} time;
int main(void)
{  
      scanf("%d:%d:%d", &time.hour, &time.minute, &time.second);
      time.second++;
      if(time.second == 60){
          time.minute++; 
          time.second = 0;
          if(time.minute == 60){
              time.hour++; 
              time.minute = 0;
              if(time.hour == 24) 
                 time.hour = 0; 
          }
      }
      printf ("%d:%d:%d\n", time.hour, time.minute, time.second );
            
      return 0;
}
