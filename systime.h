/*
 * systime.h
 *
 *  Created on: 25 wrz 2018
 *      Author: Andrzej
 */

#ifndef SYSTIME_H_
#define SYSTIME_H_


#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long SysTime_t;

bool SysTimeIsAfterCurrent(SysTime_t i_time);
int  SysTimeCompare(SysTime_t i_t1, SysTime_t i_t2);

SysTime_t SysTimeGetCurent(void);

#ifdef __cplusplus
}
#endif
#endif /* SYSTIME_H_ */
