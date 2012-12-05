#ifndef _USART1_H
#define _USART1_H  

#include "common.h"

#define TX_APB2		RCC_APB2Periph_GPIOB
#define TX_GPIO		GPIOB
#define TX_PIN		GPIO_Pin_6
#define RX_APB2		RCC_APB2Periph_GPIOB
#define RX_GPIO		GPIOB
#define RX_PIN		GPIO_Pin_7
#define TXEN_APB2	RCC_APB2Periph_GPIOB
#define TXEN_GPIO	GPIOB
#define TXEN_PIN	GPIO_Pin_5
					
void USART1_Config(void);
void USART1_TXEN_H(void);
void USART1_TXEN_L(void);		
void USART1_Interpreter(u8* buf);
void USART1_SendString(char* buf);
void USART1_SendNBytes(uint8_t* buf, int len);

#endif //_USART1_H

