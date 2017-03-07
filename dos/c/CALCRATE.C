#include <stdio.h>
#include <math.h>


#define freq       1193180.0
#define sys_const  65536.0
#define our_const  1193.0



void main(void)
{
  double r = sys_const / our_const;
  double ratio;
  double count1, count2;
  double times1, times2;
  double sys_ticks_per_hour;
  double our_ticks_per_hour;
  double extra_seconds;
  double percent_error;

  printf("---------------------------------------------------------------------\n");
  for (count1 = 50.0; count1 < 60.5; count1 = count1 + 1.0)
  {
    for (count2 = 50.0; count2 < 60.5; count2 = count2 + 1.0)
    {
      ratio = (r - count1) / (count2 - r);
      if (ratio > 1.0)
      {
        times1 = floor(ratio+0.5);
        sys_ticks_per_hour = 3600.0 * (freq/sys_const);
        our_ticks_per_hour = 3600.0 * 1000.0 / ((times1 * count1 + count2) / (times1 + 1.0));
        extra_seconds = (our_ticks_per_hour - sys_ticks_per_hour) / (freq/sys_const);
        percent_error = extra_seconds / 3600.0 * 100.0;
        printf(" %2.0lf %2.0lf %8.4lf  %3.0lf:1  %12.2lf  %12.2lf  %9.5lf%%  %6.3lf\n",
                count1, count2, ratio, times1,
                sys_ticks_per_hour, our_ticks_per_hour,
                percent_error, extra_seconds);
      }
    }
  }

  count1 = 50.0;
  count2 = 55.0;
  ratio = (r - count1) / (count2 - r);
  if (ratio > 1.0)
  {
    times1 = 2.0;
    times2 = 150.0;
    sys_ticks_per_hour = 3600.0 * (freq/sys_const);
    our_ticks_per_hour = 3600.0 * 1000.0 / ((times1 * count1 + times2 * count2) /
                                            (times1 + times2));
    extra_seconds = (our_ticks_per_hour - sys_ticks_per_hour) / (freq/sys_const);
    percent_error = extra_seconds / 3600.0 * 100.0;
    printf(" %2.0lf %2.0lf %8.4lf  %3.0lf:1  %12.2lf  %12.2lf  %9.5lf%%  %6.3lf\n",
            count1, count2, ratio, times1,
            sys_ticks_per_hour, our_ticks_per_hour,
            percent_error, extra_seconds);
  }


}   //main
