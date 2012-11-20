#include "io.h"

uint8_t outSetByte = 0;

void OUT0_ON(void)		{GPIO_SetBits(OUT0_GPIO, OUT0_PIN);}
void OUT0_OFF(void)		{GPIO_ResetBits(OUT0_GPIO, OUT0_PIN);}
void OUT1_ON(void)		{GPIO_SetBits(OUT1_GPIO, OUT1_PIN);}
void OUT1_OFF(void)		{GPIO_ResetBits(OUT1_GPIO, OUT1_PIN);}					 
void OUT2_ON(void)		{GPIO_SetBits(OUT2_GPIO, OUT2_PIN);}
void OUT2_OFF(void)		{GPIO_ResetBits(OUT2_GPIO, OUT2_PIN);}					 
void OUT3_ON(void)		{GPIO_SetBits(OUT3_GPIO, OUT3_PIN);}
void OUT3_OFF(void)		{GPIO_ResetBits(OUT3_GPIO, OUT3_PIN);}					 
void OUT4_ON(void)		{GPIO_SetBits(OUT4_GPIO, OUT4_PIN);}
void OUT4_OFF(void)		{GPIO_ResetBits(OUT4_GPIO, OUT4_PIN);}				 
void OUT5_ON(void)		{GPIO_SetBits(OUT5_GPIO, OUT5_PIN);}
void OUT5_OFF(void)		{GPIO_ResetBits(OUT5_GPIO, OUT5_PIN);}				 
/*void OUT6_ON(void)		{GPIO_SetBits(OUT6_GPIO, OUT6_PIN);}
void OUT6_OFF(void)		{GPIO_ResetBits(OUT6_GPIO, OUT6_PIN);}				 
void OUT7_ON(void)		{GPIO_SetBits(OUT7_GPIO, OUT7_PIN);}
void OUT7_OFF(void)		{GPIO_ResetBits(OUT7_GPIO, OUT7_PIN);}				 
void OUT8_ON(void)		{GPIO_SetBits(OUT8_GPIO, OUT8_PIN);}
void OUT8_OFF(void)		{GPIO_ResetBits(OUT8_GPIO, OUT8_PIN);}*/

uint8_t	MDSET0_isH(void)	{return(GPIO_ReadInputDataBit(MDSET0_PORT, MDSET0_PIN)!=0);}
uint8_t	MDSET1_isH(void)	{return(GPIO_ReadInputDataBit(MDSET1_PORT, MDSET1_PIN)!=0);}
uint8_t	MDSET2_isH(void)	{return(GPIO_ReadInputDataBit(MDSET2_PORT, MDSET2_PIN)!=0);}
uint8_t	MDSET3_isH(void)	{return(GPIO_ReadInputDataBit(MDSET3_PORT, MDSET3_PIN)!=0);}
uint8_t	MDSET4_isH(void)	{return(GPIO_ReadInputDataBit(MDSET4_PORT, MDSET4_PIN)!=0);}

void OUT_Config(void){					  
	GPIO_InitTypeDef GPIO_InitStructure;
	// IO Clocks Enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC
						 | RCC_APB2Periph_GPIOE, ENABLE);
	/*	PORT B Push-Pull 10MHz Outputs:	*\
		PC.13	OUT0					*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/*	PORT E Push-Pull 10MHz Outputs:	*\
		PE.6	OUT1
		PE.5	OUT2
		PE.4	OUT3
		PE.3	OUT_FAN
		PE.2	OUT4					*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void IN_Config(void){					  
	GPIO_InitTypeDef GPIO_InitStructure;
	// IO Clocks Enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA
						 | RCC_APB2Periph_GPIOB
						 | RCC_APB2Periph_GPIOC
						 | RCC_APB2Periph_GPIOD
						 | RCC_APB2Periph_GPIOE, ENABLE);
	/*	PORT C 10MHz Inputs Floating:	*\
		PC.0	AIN0
		PC.1	AIN1
		PC.4	SEN_PVCC
		PC.5	SEN_24V					*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/*	PORT A 10MHz Inputs Floating:	*\
		PA.5	THERM0
		PA.6	THERM1					*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*	PORT B 10MHz Inputs Floating:	*\
		PB.0	SEN_12V
		PB.1	SEN_5V					*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*	PORT D 10MHz Inputs with PullUp:*\
		PD.5	ADDR0
		PD.6	ADDR1
		PD.7	ADDR2					*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	/*	PORT E 10MHz Inputs with PullUp:*\
		PE.0	ADDR3
		PE.1	ADDR4/MODE				*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/*	PORT E 10MHz Inputs Floating:	*\
		PE.14	ENABLE					*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/*	PORT A 10MHz Inputs Floating:	*\
		PA.3	LIM_POS					*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*	PORT C 10MHz Inputs Floating:	*\
		PC.2	HOME
		PC.3	LIM_NEG					*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

const char *u8_to_binary(u8 x, u8 oldestBitNo)
{
    static char b[9];
	u8 i;
    b[0] = '\0';

	for(i=0; i<=oldestBitNo; i++)
		b[i] = (x&(1<<(oldestBitNo-i)))?'1':'0';
	b[i]='\0';
    return b;
}

void OUT_Set(u8 set){
	outSetByte = set;
	if(set & (1<<0)) OUT0_ON();	else OUT0_OFF();
	if(set & (1<<1)) OUT1_ON();	else OUT1_OFF();
	if(set & (1<<2)) OUT2_ON();	else OUT2_OFF();
	if(set & (1<<3)) OUT3_ON();	else OUT3_OFF();
	if(set & (1<<4)) OUT4_ON();	else OUT4_OFF();
	if(set & (1<<5)) OUT5_ON();	else OUT5_OFF();
}

uint8_t OUT_Read(void){
	return outSetByte;
}

uint8_t IN_ReadHOME(void){
	return GPIO_ReadInputDataBit(IN_HOME_GPIO, IN_HOME_PIN);
}

uint8_t IN_ReadENABLE(void){
	return GPIO_ReadInputDataBit(IN_ENABLE_GPIO, IN_ENABLE_PIN);
}

uint8_t IN_ReadLIMITPOS(void){
	return GPIO_ReadInputDataBit(IN_LIM_POS_GPIO, IN_LIM_POS_PIN);
}

uint8_t IN_ReadLIMITNEG(void){
	return GPIO_ReadInputDataBit(IN_LIM_NEG_GPIO, IN_LIM_NEG_PIN);
}
