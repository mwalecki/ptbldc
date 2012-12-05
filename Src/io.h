#ifndef _IO_H
#define _IO_H

#include "common.h"

#if defined MW1209A
	//##                                      #### ######## ################ ADDR/MODE port map:
	#define	ADDR0_GPIO	GPIOD
	#define	ADDR0_PIN	GPIO_Pin_5
	#define	ADDR1_GPIO	GPIOD
	#define	ADDR1_PIN	GPIO_Pin_6
	#define	ADDR2_GPIO	GPIOD
	#define	ADDR2_PIN	GPIO_Pin_7
	#define	ADDR3_GPIO	GPIOE
	#define	ADDR3_PIN	GPIO_Pin_0
	#define	ADDR4_GPIO	GPIOE
	#define	ADDR4_PIN	GPIO_Pin_1

	#define	MODE0_GPIO	ADDR4_GPIO
	#define	MODE0_PIN	ADDR4_PIN

	//##                                      #### ######## ################ IN port map:
	#define IN_HOME_GPIO	GPIOC
	#define IN_HOME_PIN		GPIO_Pin_2
	#define IN_LIM_NEG_GPIO	GPIOC
	#define IN_LIM_NEG_PIN	GPIO_Pin_3
	#define IN_LIM_POS_GPIO	GPIOA
	#define IN_LIM_POS_PIN	GPIO_Pin_3
	#define IN_ENABLE_GPIO	GPIOE
	#define IN_ENABLE_PIN	GPIO_Pin_14

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

uint8_t IN_ReadHOME(void);
uint8_t IN_ReadENABLE(void);
uint8_t IN_ReadLIMITPOS(void);
uint8_t IN_ReadLIMITNEG(void);
uint8_t IN_ReadAddress(void);
uint8_t IN_ReadMode(void);

void OUT_Config(void);
void IN_Config(void);
u8 IN_Read(void);	 
const char *u8_to_binary(u8 x, u8 oldestBitNo);
void OUT_Set(u8 set);
uint8_t OUT_Read(void);

#endif //_IO_H
