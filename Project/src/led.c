#include "led.h"

//##                                      #### ######## ################ GLOBALS
extern LED_St	Led;

void LED_Config(void){					  
	GPIO_InitTypeDef GPIO_InitStructure;
	// IO Clocks Enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
	/*	PORT D Push-Pull 10MHz Outputs:	*\
		PD.0	LEDE
		PD.1	LEDF
		PD.2	LEDG
		PD.3	LEDA
		PD.4	LEDB					*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	/*	PORT C Push-Pull 10MHz Outputs:	*\
		PC.10	LEDDP
		PC.11	LEDC
		PC.12	LEDD					*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	Led.blink = 0;
	Led.blinkPhase = 0;
}

void LED_Set(u8 mask, u8 newState, u8 blink){
	if(mask & 1<<0){
		GPIO_WriteBit(LEDA_GPIO, LEDA_PIN, ((newState & 1<<0)?Bit_SET:Bit_RESET));
		Led.blink &= (blink | ~(1<<0));
		Led.blink |= (blink & 1<<0);
	}
	if(mask & 1<<1){
		GPIO_WriteBit(LEDB_GPIO, LEDB_PIN, ((newState & 1<<1)?Bit_SET:Bit_RESET));
		Led.blink &= (blink | ~(1<<1));
		Led.blink |= (blink & 1<<1);
	}
	if(mask & 1<<2){
		GPIO_WriteBit(LEDC_GPIO, LEDC_PIN, ((newState & 1<<2)?Bit_SET:Bit_RESET));
		Led.blink &= (blink | ~(1<<2));
		Led.blink |= (blink & 1<<2);
	}
	if(mask & 1<<3){
		GPIO_WriteBit(LEDD_GPIO, LEDD_PIN, ((newState & 1<<3)?Bit_SET:Bit_RESET));
		Led.blink &= (blink | ~(1<<3));
		Led.blink |= (blink & 1<<3);
	}
	if(mask & 1<<4){
		GPIO_WriteBit(LEDE_GPIO, LEDE_PIN, ((newState & 1<<4)?Bit_SET:Bit_RESET));
		Led.blink &= (blink | ~(1<<4));
		Led.blink |= (blink & 1<<4);
	}
	if(mask & 1<<5){
		GPIO_WriteBit(LEDF_GPIO, LEDF_PIN, ((newState & 1<<5)?Bit_SET:Bit_RESET));
		Led.blink &= (blink | ~(1<<5));
		Led.blink |= (blink & 1<<5);
	}
	if(mask & 1<<6){
		GPIO_WriteBit(LEDG_GPIO, LEDG_PIN, ((newState & 1<<6)?Bit_SET:Bit_RESET));
		Led.blink &= (blink | ~(1<<6));
		Led.blink |= (blink & 1<<6);
	}
	if(mask & 1<<7){
		GPIO_WriteBit(LEDDP_GPIO, LEDDP_PIN, ((newState & 1<<7)?Bit_SET:Bit_RESET));
		Led.blink &= (blink | ~(1<<7));
		Led.blink |= (blink & 1<<7);
	}
}

void LED_Proc(void){
	Led.blinkPhase^=1;
	if(Led.blink & 1<<0)
		GPIO_WriteBit(LEDA_GPIO, LEDA_PIN, (Led.blinkPhase?Bit_SET:Bit_RESET));
	if(Led.blink & 1<<1)
		GPIO_WriteBit(LEDB_GPIO, LEDB_PIN, (Led.blinkPhase?Bit_SET:Bit_RESET));
	if(Led.blink & 1<<2)
		GPIO_WriteBit(LEDC_GPIO, LEDC_PIN, (Led.blinkPhase?Bit_SET:Bit_RESET));
	if(Led.blink & 1<<3)
		GPIO_WriteBit(LEDD_GPIO, LEDD_PIN, (Led.blinkPhase?Bit_SET:Bit_RESET));
	if(Led.blink & 1<<4)
		GPIO_WriteBit(LEDE_GPIO, LEDE_PIN, (Led.blinkPhase?Bit_SET:Bit_RESET));
	if(Led.blink & 1<<5)
		GPIO_WriteBit(LEDF_GPIO, LEDF_PIN, (Led.blinkPhase?Bit_SET:Bit_RESET));
	if(Led.blink & 1<<6)
		GPIO_WriteBit(LEDG_GPIO, LEDG_PIN, (Led.blinkPhase?Bit_SET:Bit_RESET));
	if(Led.blink & 1<<7)
		GPIO_WriteBit(LEDDP_GPIO, LEDDP_PIN, (Led.blinkPhase?Bit_SET:Bit_RESET));
}

void	LEDA_ON(void)		{GPIO_SetBits(LEDA_GPIO, LEDA_PIN);}
void	LEDA_OFF(void)		{GPIO_ResetBits(LEDA_GPIO, LEDA_PIN);}
void	LEDB_ON(void)		{GPIO_SetBits(LEDB_GPIO, LEDB_PIN);}
void	LEDB_OFF(void)		{GPIO_ResetBits(LEDB_GPIO, LEDB_PIN);}
void	LEDC_ON(void)		{GPIO_SetBits(LEDC_GPIO, LEDC_PIN);}
void	LEDC_OFF(void)		{GPIO_ResetBits(LEDC_GPIO, LEDC_PIN);}
void	LEDD_ON(void)		{GPIO_SetBits(LEDD_GPIO, LEDD_PIN);}
void	LEDD_OFF(void)		{GPIO_ResetBits(LEDD_GPIO, LEDD_PIN);}
void	LEDE_ON(void)		{GPIO_SetBits(LEDE_GPIO, LEDE_PIN);}
void	LEDE_OFF(void)		{GPIO_ResetBits(LEDE_GPIO, LEDE_PIN);}
void	LEDF_ON(void)		{GPIO_SetBits(LEDF_GPIO, LEDF_PIN);}
void	LEDF_OFF(void)		{GPIO_ResetBits(LEDF_GPIO, LEDF_PIN);}
void	LEDG_ON(void)		{GPIO_SetBits(LEDG_GPIO, LEDG_PIN);}
void	LEDG_OFF(void)		{GPIO_ResetBits(LEDG_GPIO, LEDG_PIN);}
void	LEDDP_ON(void)		{GPIO_SetBits(LEDDP_GPIO, LEDDP_PIN);}
void	LEDDP_OFF(void)		{GPIO_ResetBits(LEDDP_GPIO, LEDDP_PIN);}

