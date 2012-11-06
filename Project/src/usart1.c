#include "usart1.h"
								 
extern SERVO_St		Servo;
extern USART_St		Usart1;
extern STDOWNCNT_St	STDownCnt[ST_Downcounters];		 

//vu8 crcbuf[20];
//crc  crcTable[256];

void USART1_Config(void){						   
	GPIO_InitTypeDef GPIO_InitStructure;  
    USART_ClockInitTypeDef  USART_ClockInitStructure;
	USART_InitTypeDef USART_InitStructure;

	// IO Clocks Enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);				  
	// AFIO Clock Enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// USARTx Clock Enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	
	// NO USARTx Pin Remap 
	// GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);			
	// USARTx IO Pins
	// PA.9		USARTx_TX	as Push-Pull Alternate Function Output							  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// PA.10	USARTx_RX	as Floating Input
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	   
	// PA.8		USARTx_TXEN	as Push-Pull General Purpose Output
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
   
	// USART1 Clock Configuration 
    USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
    USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
    USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
    USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable; 
    USART_ClockInit(USART1, &USART_ClockInitStructure); 

	// USART1 Configuration
	USART_InitStructure.USART_BaudRate = 57600;
	//USART_InitStructure.USART_BaudRate = 28800; //57600
	//USART_InitStructure.USART_BaudRate = 19200; //38400
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	// Enable USART1 Receive interrupt
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	// Enable USART1
	USART_Cmd(USART1, ENABLE);
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

void USART1_SendNBytes(uint8_t* buf, uint8_t cnt)
{
	uint8_t i=0;			  						   
	USART1_TXEN_H();
	// Disable USART1 Receive 
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	for(i=0; i<cnt; i++)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			;
		USART_SendData(USART1,buf[i]);
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

void USART1_IRQHandler(void)
{	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
		// Read one byte from the receive data register
		Usart1.rxBuf[Usart1.rxPt] = USART_ReceiveData(USART1);      
		// Clear the USARTx Receive interrupt
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
																										 
		// Unknown start byte detected
		if(Usart1.rxPt==0 && Usart1.rxBuf[Usart1.rxPt]!='#' && Usart1.rxBuf[Usart1.rxPt]!='*' && Usart1.rxBuf[Usart1.rxPt]!=':')
			Usart1.rxPt=0; 
		// Text mode, 'CR' or 'LF' detected
		else if((Usart1.rxBuf[0]==':' || Usart1.rxBuf[0]=='*')
				&& (Usart1.rxBuf[Usart1.rxPt]==13 || Usart1.rxBuf[Usart1.rxPt]==10)) 
		{
			Usart1.rxDataReady=1;
			Usart1.rxPt=0;
		}
		else
			Usart1.rxPt++;
	}
}

void	USART1_TXEN_H(void)		{GPIO_SetBits(GPIOA, GPIO_Pin_8);}
void	USART1_TXEN_L(void)		{GPIO_ResetBits(GPIOA, GPIO_Pin_8);}
