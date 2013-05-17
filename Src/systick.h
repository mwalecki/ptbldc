#ifndef _SYSTICK_H
#define _SYSTICK_H

#include "common.h"

#define ST_StatusLed	0
#define ST_Period0			5000 // all values in [ *0.1ms]
#define ST_MotorsPid	1
#define ST_Period1			10
#define ST_MotorProc	2
#define ST_Period2			10
#define ST_CommandWD	3
#define ST_Period3			20000
#define ST_UsartTxDelay	4
#define ST_Period4			50
#define ST_Servo		5
#define ST_Period5			10
#define ST_Monitor		6
#define ST_Period6			1000
#define ST_Downcounters	7

#define ST_Period(n)	ST_Period ## n
#define ST_SetPeriod(n,i)	{STDownCnt[n].period = i; STDownCnt[n].val=i;}
#define ST_Reset(n)			STDownCnt[n].val=STDownCnt[n].period

void SYSTICK_Init(STDOWNCNT_St*	STDownCnt);	
void SysTick_Handler(void);

#endif //_SYSTICK_H
