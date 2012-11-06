#ifndef _LED_H_
#define _LED_H_

#include "common.h"

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

u8		LED1_isON(void);
void	LED1_ON(void);
void	LED1_OFF(void);
void	LED1_TOGGLE(void);
u8		LED2_isON(void);
void	LED2_ON(void);
void	LED2_OFF(void);
void	LED2_TOGGLE(void);

#endif	// ifndef _LED_H_
