/*
 * systime.c
 *
 *  Created on: 25 wrz 2018
 *      Author: Andrzej
 */

#include "systime.h"
#include <esp_timer.h>
#include <limits.h>

#define TIME_HALF_VALUE (ULONG_MAX / 2)

int SysTimeCompare(SysTime_t i_t1, SysTime_t i_t2)
{
  int64_t diff = (int64_t)i_t1 - (int64_t)i_t2;

  if (diff < 0)
  {
    if (diff <= INT_MIN)
    {
      return INT_MIN;
    }
    else
    {
      return (int)(diff);
    }
  }
  else
  {
    if (diff >= INT_MAX)
    {
      return INT_MAX;
    }
    else
    {
      return (int)(diff);
    }
  }
}

bool SysTimeIsAfterCurrent(SysTime_t i_time)
{
  if (SysTimeCompare(i_time, SysTimeGetCurent()) > 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

SysTime_t SysTimeGetCurent(void)
{
  return esp_timer_get_time() / 1000;
}
