#include "usart1.h"
#include "systick.h"
#include "nf/nfv2.h"
#include "circbuf.h"
								 
extern SERVO_St		Servo;
extern USART_St		Usart1;
extern STDOWNCNT_St	STDownCnt[ST_Downcounters];	
extern NF_STRUCT_ComBuf 	NFComBuf;	 


uint8_t cbUsart1RxData[256];
extern CircularBuffer	cbUsart1Received;

//vu8 crcbuf[20];
//crc  crcTable[256];

void USART1_Config(void){						   
	GPIO_InitTypeDef GPIO_InitStructure;  
    USART_ClockInitTypeDef  USART_ClockInitStructure;
	USART_InitTypeDef USART_InitStructure;

	// USB Received Data Circular Buffer Init
	cbInit(&cbUsart1Received, cbUsart1RxData, 256);

	// IO Clocks Enable
	RCC_APB2PeriphClockCmd(TX_APB2 | RX_APB2 | TXEN_APB2, ENABLE);
	// AFIO Clock Enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// USARTx Clock Enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	
	// USARTx Pin Remap
	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
	// USARTx IO Pins
	// USARTx_TX	as Push-Pull Alternate Function Output
	GPIO_InitStructure.GPIO_Pin = TX_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(TX_GPIO, &GPIO_InitStructure);
	// USARTx_RX	as Floating Input
	GPIO_InitStructure.GPIO_Pin = RX_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(RX_GPIO, &GPIO_InitStructure);
	// USARTx_TXEN	as Push-Pull General Purpose Output
	GPIO_InitStructure.GPIO_Pin = TXEN_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(TXEN_GPIO, &GPIO_InitStructure);
   
	// USARTx Clock Configuration
    USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
    USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
    USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
    USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable; 
    USART_ClockInit(USART1, &USART_ClockInitStructure); 

	// USARTx Configuration
	USART_InitStructure.USART_BaudRate = 57600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	// Enable USARTx Receive interrupt
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	// Enable USARTx
	USART_Cmd(USART1, ENABLE);

	// USART Interrupt set in interrupts.c
}

void USART1_SendString(char* buf)
{
	int i=0;			  						   
	USART1_TXEN_H();
	// Disable USART1 Receive 
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	while(buf[i]!='\0')
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			;
		USART_SendData(USART1,buf[i]);
		if(++i == USART_BufSz)
			break;
	}		
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
		;
	//ST_Reset(ST_UsartTxDelay);	
	// Wait for TX Complete Flag
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
		;
	USART1_TXEN_L();	 
	// Enable USART1 Receive 
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

void USART1_SendNBytes(uint8_t* buf, int len)
{
	int i=0;			  						   
	USART1_TXEN_H();
	// Disable USART1 Receive 
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	while(i < len)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			;
		USART_SendData(USART1,buf[i]);
		if(++i == USART_BufSz)
			break;
	}		
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
		;
	// Wait for TX Complete Flag
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
		;
	USART1_TXEN_L();	 
	// Enable USART1 Receive 
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

void USART1_IRQHandler(void){	
	uint8_t u1commArray[10];
	uint8_t u1commCnt;
	uint8_t u1BytesToSend;
	uint8_t u1ByteReceived;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){	 
		// Clear the USARTx Receive interrupt
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);

		u1ByteReceived = USART_ReceiveData(USART1);
		cbWrite(&cbUsart1Received, &u1ByteReceived);

		/*
		// Read one byte from the receive data register
		Usart1.rxBuf[Usart1.rxPt] = USART_ReceiveData(USART1);
		
		if(NF_Interpreter(&NFComBuf, (uint8_t*) Usart1.rxBuf, (uint8_t*) &Usart1.rxPt, u1commArray, &u1commCnt) > 0){
			NFComBuf.dataReceived = 1;
			if(u1commCnt > 0){
				u1BytesToSend = NF_MakeCommandFrame(&NFComBuf, (uint8_t*)Usart1.txBuf, (const uint8_t*)u1commArray, u1commCnt, NFComBuf.myAddress);
				if(u1BytesToSend > 0){
					USART1_SendNBytes((uint8_t*)Usart1.txBuf, u1BytesToSend);
				}
			}
		}*/
	}
}

void	USART1_TXEN_H(void)		{GPIO_SetBits(TXEN_GPIO, TXEN_PIN);}
void	USART1_TXEN_L(void)		{GPIO_ResetBits(TXEN_GPIO, TXEN_PIN);}
