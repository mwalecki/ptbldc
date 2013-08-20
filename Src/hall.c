#include "hall.h"
#include "nf/nfv2.h"
#include "encoder.h"
#include "commutator.h"

extern NF_STRUCT_ComBuf 	NFComBuf;
extern COMMUTATOR_St		Commutator;

void HALL_Config(void) {
	//Configuration Structures
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
	// 100kHz (T=10us) TimerClock [72MHz/Prescaler]
	// 65000 => 650ms till overflow ;
	TIM_TimeBaseStructure.TIM_Prescaler = 720;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_Period = 65000; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
													
	// Timer input XOR function 
	// T1F_ED connected to TIMx_CH1,TIMx_CH2,TIMx_CH3 
	TIM_SelectHallSensor(TIM4, ENABLE);
  
	// HallSensor event is delivered with singnal TI1F_ED
	// (this is XOR of the three hall sensor lines)
	// Signal TI1F_ED: falling and rising ddge of the inputs is used 
	TIM_SelectInputTrigger(TIM4, TIM_TS_TI1F_ED);
  
	// On every TI1F_ED event the counter is resetted and update is tiggered 
	TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Reset);

	TIM_SelectMasterSlaveMode(TIM4, TIM_MasterSlaveMode_Enable);
 
	// Channel 1 in input capture mode 
	// on every TCR edge (build from TI1F_ED which is a HallSensor edge)  
	// the timervalue is copied into ccr register and a CCR1 Interrupt
	// TIM_IT_CC1 is fired 
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; 
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
	// listen to T1, the  HallSensorEvent 
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_TRC;
	// Div:1, every edge 
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// noise filter: 1111 => 72000kHz / factor (==1) / 32 / 8 -> 281kHz 
	// input noise filter (reference manual page 322) 
	TIM_ICInitStructure.TIM_ICFilter = 0x0f; //0x0f
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
  
	// Enable channel 1 interrupt request
	TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);
  
	// Enable update event 
	//TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	//TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);
	TIM_Cmd(TIM4, ENABLE);
}

u8 HALL_Pattern(void){
	return (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_12)<<2 | GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_13)<<1
		| GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_14)<<0);
}

uint32_t COMM_RotorPosition(void){
	uint32_t tempPosition;

	Commutator.currentEncPos = ENCODER1_Position();

	if(Commutator.currentEncPos < Commutator.zeroRotorPos)
		Commutator.zeroRotorPos -= Commutator.encoderResolution;
	 tempPosition = (Commutator.currentEncPos - Commutator.zeroRotorPos) % Commutator.encoderResolution;

	tempPosition *= COMMUTATION_TABLE_LENGTH;
	tempPosition /= Commutator.encoderResolution;

	Commutator.currentRotorPos = (tempPosition > (COMMUTATION_TABLE_LENGTH-1)) ?
			(COMMUTATION_TABLE_LENGTH-1) : tempPosition;
	return Commutator.currentRotorPos;
}
 
// ------------- HallSensor interrupt handler -----------------
void TIM4_IRQHandler(void) {
	Commutator.currentEncPos = ENCODER1_Position();
	Commutator.currentHallPattern = HALL_Pattern();

	TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);

	// Encoder error detection
//	if(Commutator.prev0HallPattern != Commutator.currentHallPattern){
//		encoderIncrement = Commutator.currentEncPos - Commutator.previousEncPos;
//		if(encoderIncrement < 0)
//			encoderIncrement = - encoderIncrement;
//		if((Commutator.encoderResolution/7 > encoderIncrement)
//				|| (Commutator.encoderResolution/5 < encoderIncrement))
//			NFComBuf.SetDrivesMode.data[0] = NF_DrivesMode_ERROR;
//	}


	if(((Commutator.prev0HallPattern == 0b011) && (Commutator.currentHallPattern == 0b001))
			|| ((Commutator.prev0HallPattern == 0b001) && (Commutator.currentHallPattern == 0b011))){
		Commutator.zeroRotorPos = Commutator.currentEncPos;
		// First zero-crossing of rotor position
		if(Commutator.synchronized == 0)
			Commutator.synchronized ++;
	}

	// Measure last Hall step duration only if motor moves continously in one direction
	if(Commutator.currentHallPattern != Commutator.prev1HallPattern){
		Commutator.lastHallStepDuration = TIM4->CCR1;
	}

	// Commutation Table Index Advance
	// Iadv = Ni * Tpwm / tcomm
	if(Commutator.lastHallStepDuration > 0){
		Commutator.commTableIndexAdvance =
				Commutator.advanceCoeff * ((COMMUTATION_TABLE_LENGTH / 6) * 5)
				/ Commutator.lastHallStepDuration;
	}
	else
		Commutator.commTableIndexAdvance = 0;


	Commutator.prev1HallPattern = Commutator.prev0HallPattern;
	Commutator.prev0HallPattern = Commutator.currentHallPattern;
	Commutator.previousEncPos = Commutator.currentEncPos;
}
