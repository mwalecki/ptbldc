#include "encoder.h"
#include "commutator.h"
#include "motor.h"

extern MOTOR_St				Motor;
extern COMMUTATOR_St		Commutator;

//##                                      #### ######## ################ PRIVATE GLOBALS
static uint32_t old16Position = 0;
static int32_t ovfCounter = 0;

void  ENCODER1_Config(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;

	// AFIO Clock Enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// GPIO clocks enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/*	PORT A Floating Inputs:				*\
		PA.0	EncA
		PA.1	EncB
		PA.2	EncI						*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// TIM2 clock enable
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	TIM_DeInit(TIM2);

	// TimeBase configuration
	TIM_TimeBaseStructure.TIM_Prescaler     = 0x00;
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period        = 0xFFFF;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	// Input Capture configuration
	TIM_ICStructInit(&TIM_ICInitStructure);
	// Input Capture Strucrure for CH1
	TIM_ICInitStructure.TIM_Channel     = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;
	//TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	//TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	//TIM_ICInitStructure.TIM_ICFilter    = 0;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);

	// Input Capture Strucrure for CH2
	TIM_ICInitStructure.TIM_Channel     = TIM_Channel_2;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);

	// Encoder Interface Config
	if(Commutator.polarity == 0)
		TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);
	else
		TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

	//TIM_ClearFlag(TIM2, TIM_FLAG_Update | TIM_FLAG_COM | TIM_FLAG_Break | TIM_FLAG_CC1 | TIM_FLAG_CC2 |TIM_FLAG_CC3|TIM_FLAG_CC4);
	//TIM_ITConfig(TIM2, TIM_IT_Update | TIM_IT_COM | TIM_IT_Break | TIM_IT_CC1 | TIM_IT_CC2 |TIM_IT_CC3|TIM_IT_CC4, ENABLE);

	// Clear all pending interrupts
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	//TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	TIM_SetCounter(TIM2, 0);

	TIM_Cmd(TIM2, ENABLE);


	// Index line Interrupt
	/* Connect EXTI0 Line to PA.2 pin */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
	/* Configure EXTI0 line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	Commutator.encoderResolution = ENCODER_RESOLUTION;
}

/**
  * @brief  This function handles External line 0 interrupt request (Encoder1 Index Line rising)
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		if(Motor.synchronizingToEncIndex){
			TIM2->CNT = 0;
			ovfCounter = 0;
			old16Position = 0;
			Motor.previousPosition = 0;
			Motor.setPosition = 0;				//!!!!
			Motor.setTargetPosition = 0;		//!!!!
			Motor.isSynchronized = 1;
			Motor.synchronizingToEncIndex = 0;
		}
		/* Clear the  EXTI line 0 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}


int32_t ENCODER1_Position(void){
	uint32_t new16Position;
	int32_t position;

	new16Position = TIM2->CNT;

	if(((old16Position & 0xC000) == 0xC000) && ((new16Position & 0xC000) == 0)){
		ovfCounter ++;
	}
	else if(((old16Position & 0xC000) == 0) && ((new16Position & 0xC000) == 0xC000)){
		ovfCounter --;
	}

	position = (ovfCounter * 0x10000) + new16Position;
	old16Position = new16Position;
	return position;
}
