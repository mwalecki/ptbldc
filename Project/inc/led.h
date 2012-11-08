#ifndef _LED_H_
#define _LED_H_

#include "common.h"

//##                                      #### ######## ################ LED patterns:
#define LED_ALL	0xff
#define LED_A	1<<0
#define LED_B	1<<1
#define LED_C	1<<2
#define LED_D	1<<3
#define LED_E	1<<4
#define LED_F	1<<5
#define LED_G	1<<6
#define LED_DP	1<<7
#define LED_DIGIT		0x7f
#define LED_symMINUS	LED_G
#define LED_symS		LED_A|LED_F|LED_G|LED_C|LED_D
#define LED_symP		LED_A|LED_F|LED_B|LED_G|LED_E


//##                                      #### ######## ################ LED port map:
#if defined MW1110A || defined MW1110B
	#define LED_APB2PERIPH	RCC_APB2Periph_GPIOD
	#define LED_GPIO		GPIOD
	#define LED2_PIN		GPIO_Pin_3
	#define LED1_PIN		GPIO_Pin_2
#elif defined MW1110C
	#define LED_APB2PERIPH	RCC_APB2Periph_GPIOD
	#define LED_GPIO		GPIOD
	#define LED2_PIN		GPIO_Pin_0
	#define LED1_PIN		GPIO_Pin_1
#elif defined MW1209A
	#define LEDA_GPIO		GPIOD
	#define LEDA_PIN		GPIO_Pin_3
	#define LEDB_GPIO		GPIOD
	#define LEDB_PIN		GPIO_Pin_4
	#define LEDC_GPIO		GPIOC
	#define LEDC_PIN		GPIO_Pin_11
	#define LEDD_GPIO		GPIOC
	#define LEDD_PIN		GPIO_Pin_12
	#define LEDE_GPIO		GPIOD
	#define LEDE_PIN		GPIO_Pin_0
	#define LEDF_GPIO		GPIOD
	#define LEDF_PIN		GPIO_Pin_1
	#define LEDG_GPIO		GPIOD
	#define LEDG_PIN		GPIO_Pin_2
	#define LEDDP_GPIO		GPIOC
	#define LEDDP_PIN		GPIO_Pin_10
#else
	#error MW1110x version undefined
#endif

//##                                      #### ######## ################ PROTOTYPES:

void	LED_Config(void);
void	LED_Set(u8 mask, u8 newState, u8 blink);
void	LED_Set(u8 mask, u8 newState, u8 blink);
void	LED_Proc(void);

void	LEDA_ON(void);
void	LEDA_OFF(void);
void	LEDB_ON(void);
void	LEDB_OFF(void);
void	LEDC_ON(void);
void	LEDC_OFF(void);
void	LEDD_ON(void);
void	LEDD_OFF(void);
void	LEDE_ON(void);
void	LEDE_OFF(void);
void	LEDF_ON(void);
void	LEDF_OFF(void);
void	LEDG_ON(void);
void	LEDG_OFF(void);
void	LEDDP_ON(void);
void	LEDDP_OFF(void);

#endif	// ifndef _LED_H_
