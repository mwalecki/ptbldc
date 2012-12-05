#include "systick.h"
#include "motor.h"

extern STDOWNCNT_St	STDownCnt[ST_Downcounters];	

void SYSTICK_Init(STDOWNCNT_St*	STDownCnt)
{
	STDownCnt[0].period = ST_Period0;
	STDownCnt[1].period = ST_Period1;
	STDownCnt[2].period = ST_Period2;
	STDownCnt[3].period = ST_Period3;
	STDownCnt[4].period = ST_Period4;
	STDownCnt[5].period = ST_Period5;
	STDownCnt[6].period = ST_Period6;
//	STDownCnt[7].period = ST_Period7;
//	STDownCnt[8].period = ST_Period8;
	/* SysTick end of count event each 100us with input clock equal to 9MHz (HCLK/8, default) */
//	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
//	SysTick_SetReload(900);	// 1 tick = 100us
//	/* Enable SysTick interrupt */
//	SysTick_ITConfig(ENABLE);
//	SysTick_CounterCmd(SysTick_Counter_Enable);
	SysTick_Config(7200);
}

void SysTick_Handler(void)
{	
	vu8 i=0;
	for(; i<ST_Downcounters; i++)
	{
		if(STDownCnt[i].val != 0)
			STDownCnt[i].val--;
		else
		{
			STDownCnt[i].val = STDownCnt[i].period;
			STDownCnt[i].tick = 1;
		}
	}

	
	if(STDownCnt[ST_MotorProc].tick){
		MOTOR_Proc();
		STDownCnt[ST_MotorProc].tick = 0;
	}
}
