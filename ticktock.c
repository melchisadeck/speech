#include <stdio.h>
#include <sys/time.h>

#include "ticktock.h"

void tick(tick_time)
struct timeval *tick_time;
{
  gettimeofday(tick_time, NULL);
}

double tock(tick_time)
struct timeval tick_time;
{
  double tick_dtime;
  double tock_dtime;
  struct timeval tock_time;

  gettimeofday(&tock_time, NULL);

  tick_dtime = (double) tick_time.tv_sec +
    (double) tick_time.tv_usec / 1000000.0;
  tock_dtime = (double) tock_time.tv_sec +
    (double) tock_time.tv_usec / 1000000.0;

  return tock_dtime - tick_dtime;
}
