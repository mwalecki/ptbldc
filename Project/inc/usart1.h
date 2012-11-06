#ifndef _USART1_H
#define _USART1_H  

#include "common.h"	 
#include <stdlib.h>
#include <stdio.h>		
#include <string.h>		
#include "mycrc.h"
#include "led.h"  
#include "systick.h" 
#include "eeprom.h"
#include "eebackup.h"
#include "io.h"
#include "adc.h"
			 
#define USART1_RxFrameSz	12
					
void USART1_Config(void);
void USART1_TXEN_H(void);
void USART1_TXEN_L(void);
void USART1_SendNBytes(uint8_t* buf, uint8_t cnt);
void USART1_SendString(char* buf);

#endif //_USART1_H

