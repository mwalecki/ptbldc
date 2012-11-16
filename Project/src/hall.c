#include "hall.h"

void HALL_Config(void) {
	//Configuration Structures
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;

	// TIM Clock Enable
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	// AFIO Clock Enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// GPIO clocks enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

	// TIM1 Full Pin Remap
	GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);

	/*	PORT D Floating Inputs:				*\
		PD.12	HALL1	TIM4_CH1
		PD.13	HALL2	TIM4_CH2
		PD.14	HALL3	TIM4_CH3			*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);


	// timer base configuration
	// 10kHz (T=100us) TimerClock [24MHz/Prescaler]
	// 65000 => 6,5s till overflow ;			   
	TIM_TimeBaseStructure.TIM_Prescaler = 2400; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_Period = 65000; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	TIM4->CCR2 = 10; // Commutation Delay = 1ms
													
	// Timer input XOR function 
	// T1F_ED connected to TIMx_CH1,TIMx_CH2,TIMx_CH3 
	TIM_SelectHallSensor(TIM4, ENABLE);
  
	// HallSensor event is delivered with singnal TI1F_ED
	// (this is XOR of the three hall sensor lines)
	// Signal TI1F_ED: falling and rising ddge of the inputs is used 
	TIM_SelectInputTrigger(TIM4, TIM_TS_TI1F_ED);
  
	// On every TI1F_ED event the counter is resetted and update is tiggered 
	TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Reset);
 
	// Channel 1 in input capture mode 
	// on every TCR edge (build from TI1F_ED which is a HallSensor edge)  
	// the timervalue is copied into ccr register and a CCR1 Interrupt
	// TIM_IT_CC1 is fired 
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; 
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; 
	// listen to T1, the  HallSensorEvent 
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_TRC;
	// Div:1, every edge 
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// noise filter: 1111 => 72000kHz / factor (==1) / 32 / 8 -> 281kHz 
	// input noise filter (reference manual page 322) 
	TIM_ICInitStructure.TIM_ICFilter = 0xF; 
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
  
	// channel 2 can be used for commutation delay between hallsensor edge
	// and switching the FET into the next step. if this delay time is
	// over the channel 2 generates the commutation signal to the motor timer
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_Pulse = 1; // 1 is no delay; 2000 = 7ms
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
  
	// clear interrupt flag
	TIM_ClearFlag(TIM4, TIM_FLAG_CC2);
  
	//TIM_SelectMasterSlaveMode(TIM4, TIM_MasterSlaveMode_Enable);
	// TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_OC1);
	// timer2 output compate signal is connected to TRIGO 
	TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_OC2Ref);
  
	// Enable channel 1, 2 compate interrupt requests
	TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);
//	TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);	// ####################!!!!!!!!!!!!!!!!!!!!!!!
 
	// Enable output compare preload 
	//TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
  
	// Enable ARR preload 
	//TIM_ARRPreloadConfig(TIM4, ENABLE);
  
	// Enable update event 
	//TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	//TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);
	 
	// we use preemption interrupts here,  BLDC Bridge switching and
	// Hall has highest priority 
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
	 
	  // -------------------
	// HallSensor is now configured, if BLDC Timer is also configured
	// after enabling timer 3 
	// the motor will start after next overflow of the hall timer because
	// this generates the first startup motor cummutation event
	TIM_Cmd(TIM4, ENABLE);
}

u8 HALL_Pattern(void){
	return (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_12)<<2 | GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_13)<<1
		| GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_14)<<0);
}
 
// ------------- HallSensor interrupt handler -----------------
 
// this handles TIM4 irqs (from HallSensor)
void TIM4_IRQHandler(void) {
  if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
  { 
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
    // calculate motor  speed or else with CCR1 values
//    hallccr1 = TIM4->CCR1;
    BLDCMotorPrepareCommutation(); // <-- To chyba powinno byc tutaj?
	//TIM_GenerateEvent(TIM1, TIM_EventSource_COM);
//	USART3_SendString("intcc1\r\n\n");
  } 
  else if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)
  { 
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
    // this interrupt handler is called AFTER the motor commutaton event
    // is done
    // after commutation the next motor step must be prepared
    // use inline functions in irq handlers static __INLINE funct(..) {..} 
//    BLDCMotorPrepareCommutation(); // <-- A nie tutaj...
//	USART3_SendString("intcc2\r\n\n");
  } else { 
    ; // this should not happen 
  } 
}
