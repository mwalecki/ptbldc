#ifndef _IO_H
#define _IO_H

#include "common.h"

#if defined MW1209A
	//##                                      #### ######## ################ ADDR/MODE port map:
	#define	MDSET0_PORT	GPIOD
	#define	MDSET0_PIN	GPIO_Pin_5
	#define	MDSET1_PORT	GPIOD
	#define	MDSET1_PIN	GPIO_Pin_6
	#define	MDSET2_PORT	GPIOD
	#define	MDSET2_PIN	GPIO_Pin_7
	#define	MDSET3_PORT	GPIOE
	#define	MDSET3_PIN	GPIO_Pin_0
	#define	MDSET4_PORT	GPIOE
	#define	MDSET4_PIN	GPIO_Pin_1

	//##                                      #### ######## ################ IN port map:
	#define IN_HOME_GPIO	GPIOC
	#define IN_HOME_PIN		GPIO_Pin_2
	#define IN_LIM_NEG_GPIO	GPIOC
	#define IN_LIM_NEG_PIN	GPIO_Pin_3
	#define IN_LIM_POS_GPIO	GPIOA
	#define IN_LIM_POS_PIN	GPIO_Pin_3

	//##                                      #### ######## ################ OUT port map:
	#define OUT0_GPIO	GPIOC
	#define OUT0_PIN	GPIO_Pin_13
	#define OUT1_GPIO	GPIOE
	#define OUT1_PIN	GPIO_Pin_6
	#define OUT2_GPIO	GPIOE
	#define OUT2_PIN	GPIO_Pin_5
	#define OUT3_GPIO	GPIOE
	#define OUT3_PIN	GPIO_Pin_4
	#define OUT4_GPIO	GPIOE
	#define OUT4_PIN	GPIO_Pin_3
	#define OUT5_GPIO	GPIOE
	#define OUT5_PIN	GPIO_Pin_2

	//##                                      #### ######## ################ PROTOTYPES:
	void OUT1_ON(void);
	void OUT1_OFF(void);
	void OUT2_ON(void);
	void OUT2_OFF(void);
	void OUT3_ON(void);
	void OUT3_OFF(void);
	void OUT4_ON(void);
	void OUT4_OFF(void);
	void OUT5_ON(void);
	void OUT5_OFF(void);
	void OUT6_ON(void);
	void OUT6_OFF(void);
	void OUT7_ON(void);
	void OUT7_OFF(void);
	void OUT8_ON(void);
	void OUT8_OFF(void);

#else
	#error Define Module Version: MW1209A/B/C
#endif

uint8_t	MDSET0_isH(void);
uint8_t	MDSET1_isH(void);
uint8_t	MDSET2_isH(void);
uint8_t	MDSET3_isH(void);
uint8_t	MDSET4_isH(void);

uint8_t IN_HOME_isH(void);
uint8_t IN_LIM_NEG_isH(void);
uint8_t IN_LIM_POS_isH(void);

void OUT_Config(void);
void IN_Config(void);
u8 IN_Read(void);	 
const char *u8_to_binary(u8 x, u8 oldestBitNo);
void OUT_Set(u8 set);
uint8_t OUT_Read(void);

#endif //_IO_H
