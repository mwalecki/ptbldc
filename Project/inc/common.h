#ifndef _COMMON_H_
#define _COMMON_H_

#include "stm32f10x.h"

#define MODULE_NAME			"RobTrain v2"
#define USART_BufSz			256	 
#define USART_TxSuffix		"\r\n"

typedef struct{
	vu8 blink;
	vu8 blinkPhase;
} LED_St;	 

typedef struct{
	vu32 val;
	vu32 period;
	vu8 tick;
} STDOWNCNT_St;	

typedef struct{
	vu8 rxBuf[USART_BufSz];
	vu8 txBuf[USART_BufSz];		
	vs8 tmpBuf[USART_BufSz];
	vu8 rxPt, txPt;
	vu8 rxDataReady	:1;
	vu8 txDataReady	:1;
} USART_St;

typedef struct{
	vu16 set[8];
	vu16 target[8];
	vu16 min[8];
	vu16 max[8];
	vs16 adj[8];
	vu8 chEnable;
} SERVO_St;

#endif //_COMMON_H_
