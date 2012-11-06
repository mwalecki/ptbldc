#include "motor.h"
#include "nfv2.h"
#include "adc.h"
#include "pid.h"

extern ADC_St				ADC;
extern PID_St				PID[3];
extern MOTOR_St				Motor[3];
extern NF_STRUCT_ComBuf 	NFComBuf;

void MOTORS12_Config(void) { 					
	//Configuration Structures
	GPIO_InitTypeDef		GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef		TIM_OCInitStructure;
	TIM_BDTRInitTypeDef		TIM_BDTRInitStructure;

	// TIM1 clock enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);  				  
	// AFIO Clock Enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// GPIO clocks enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE, ENABLE);
		
	// TIM1 Full Pin Remap
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);

	/*	PORT B Push-Pull 10MHz Outputs:	*\
		PB.0	M1B						
		PB.1	M1A						
		PB.2	M2B						*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*	PORT E Push-Pull 10MHz Outputs:	*\
		PE.7	M2A						*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/*	PORT E Push-Pull 10MHz AF Out:	*\
		PE.8	M1EN						
		PE.11	M2EN						*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
 
	// Time Base configuration
	TIM_TimeBaseStructure.TIM_Prescaler = 3;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = TIM1PERIOD;		// 72MHz / 3 / (TIM1PERIOD==1000) = 24kHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
 
	// TIM1_CH1N: M1EN
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = TIM1PERIOD;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
																
	// TIM1_CH2: M2EN
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
 
	/* automatic output enable, break off, dead time ca. 200ns and  
	// no lock of configuration */
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
	// DeadTime[ns] = value * (1/SystemCoreFreq) (on 72MHz: 7 is 98ns, on 24MHz: 7 is 296ns)
	TIM_BDTRInitStructure.TIM_DeadTime = 0;
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
	 
	// enabel this if you use emergency stop signal
	// TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
	// TIM_BDTRInitStructure.TIM_BreakPolarity = MOTOR_TMC603_EMSTOP_POLARITY; 
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;
	TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);
 
	// enable motor timer
	TIM_Cmd(TIM1, ENABLE);
	// enable motor timer main output (the bridge signals)
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void MOTORS1234_Config(void) { 					
	//Configuration Structures
	GPIO_InitTypeDef		GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef		TIM_OCInitStructure;
	TIM_BDTRInitTypeDef		TIM_BDTRInitStructure;

	// TIM1 clock enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);  				  
	// AFIO Clock Enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// GPIO clocks enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE, ENABLE);
		
	// TIM1 Full Pin Remap
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);

	/*	PORT B Push-Pull 10MHz Outputs:	*\
		PB.0	M1B						
		PB.1	M1A						
		PB.2	M2B						*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*	PORT E Push-Pull 10MHz Outputs:	*\
		PE.7	M2A						
		PE.9	M3B						
		PE.10	M3A						
		PE.13	M4B						
		PE.15	M4A						*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/*	PORT E Push-Pull 10MHz AF Out:	*\
		PE.8	M1EN						
		PE.11	M2EN					
		PE.12	M3EN					
		PE.14	M4EN					*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
 
	// Time Base configuration
	TIM_TimeBaseStructure.TIM_Prescaler = 3;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = TIM1PERIOD;		// 72MHz / 3 / (TIM1PERIOD==1000) = 24kHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
 
	// TIM1_CH1N: M1EN
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = TIM1PERIOD;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
																
	// TIM1_CH2: M2EN
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
 
	// TIM1_CH1N: M3EN
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = TIM1PERIOD;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
																
	// TIM1_CH4: M2EN
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
 
	/* automatic output enable, break off, dead time ca. 200ns and  
	// no lock of configuration */
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
	// DeadTime[ns] = value * (1/SystemCoreFreq) (on 72MHz: 7 is 98ns, on 24MHz: 7 is 296ns)
	TIM_BDTRInitStructure.TIM_DeadTime = 0;
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
	 
	// enable this if you use emergency stop signal
	// TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
	// TIM_BDTRInitStructure.TIM_BreakPolarity = MOTOR_TMC603_EMSTOP_POLARITY; 
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;
	TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);
 
	// enable motor timer
	TIM_Cmd(TIM1, ENABLE);
	// enable motor timer main output (the bridge signals)
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
 
void MOTOR1_SetInput(s16 input){	
	if(input > MAX_PWM)
		input = MAX_PWM;
	else if(input < -MAX_PWM)
		input = -MAX_PWM;

	if(input == 0){		
		GPIO_ResetBits(MOTOR1A_GPIO, MOTOR1A_PIN);
		GPIO_ResetBits(MOTOR1B_GPIO, MOTOR1B_PIN);
		//TIM1->CCR1 = TIM1PERIOD - 0;
	}	   
	else if(input > 0){
		GPIO_SetBits(MOTOR1A_GPIO, MOTOR1A_PIN);
		GPIO_ResetBits(MOTOR1B_GPIO, MOTOR1B_PIN);
		//TIM1->CCR1 = TIM1PERIOD - input;
		TIM1->CCR1 = input;
	}
	else{
		GPIO_ResetBits(MOTOR1A_GPIO, MOTOR1A_PIN);
		GPIO_SetBits(MOTOR1B_GPIO, MOTOR1B_PIN);
		//TIM1->CCR1 = TIM1PERIOD + input;
		TIM1->CCR1 = -input;
	}
}
 
void MOTOR2_SetInput(s16 input){	
	if(input > MAX_PWM)
		input = MAX_PWM;
	else if(input < -MAX_PWM)
		input = -MAX_PWM;

	if(input == 0){		
		GPIO_ResetBits(MOTOR2A_GPIO, MOTOR2A_PIN);
		GPIO_ResetBits(MOTOR2B_GPIO, MOTOR2B_PIN);
		//TIM1->CCR2 = 0;
	}	   
	else if(input > 0){
		GPIO_SetBits(MOTOR2A_GPIO, MOTOR2A_PIN);
		GPIO_ResetBits(MOTOR2B_GPIO, MOTOR2B_PIN);
		TIM1->CCR2 = input;
	}
	else{
		GPIO_ResetBits(MOTOR2A_GPIO, MOTOR2A_PIN);
		GPIO_SetBits(MOTOR2B_GPIO, MOTOR2B_PIN);
		TIM1->CCR2 = -input;
	}
}
 
void MOTOR3_SetInput(s16 input){	
	if(input > MAX_PWM)
		input = MAX_PWM;
	else if(input < -MAX_PWM)
		input = -MAX_PWM;

	if(input == 0){		
		GPIO_ResetBits(MOTOR3A_GPIO, MOTOR3A_PIN);
		GPIO_ResetBits(MOTOR3B_GPIO, MOTOR3B_PIN);
		//TIM1->CCR3 = TIM1PERIOD - 0;
	}	   
	else if(input > 0){
		GPIO_SetBits(MOTOR3A_GPIO, MOTOR3A_PIN);
		GPIO_ResetBits(MOTOR3B_GPIO, MOTOR3B_PIN);
		//TIM1->CCR3 = TIM1PERIOD - input;
		TIM1->CCR3 = input;
	}
	else{
		GPIO_ResetBits(MOTOR3A_GPIO, MOTOR3A_PIN);
		GPIO_SetBits(MOTOR3B_GPIO, MOTOR3B_PIN);
		//TIM1->CCR3 = TIM1PERIOD + input;
		TIM1->CCR3 = -input;
	}
}
 
void MOTOR4_SetInput(s16 input){	
	if(input > MAX_PWM)
		input = MAX_PWM;
	else if(input < -MAX_PWM)
		input = -MAX_PWM;

	if(input == 0){		
		GPIO_ResetBits(MOTOR4A_GPIO, MOTOR4A_PIN);
		GPIO_ResetBits(MOTOR4B_GPIO, MOTOR4B_PIN);
		//TIM1->CCR4 = 0;
	}	   
	else if(input > 0){
		GPIO_SetBits(MOTOR4A_GPIO, MOTOR4A_PIN);
		GPIO_ResetBits(MOTOR4B_GPIO, MOTOR4B_PIN);
		TIM1->CCR4 = input;								  
	}
	else{
		GPIO_ResetBits(MOTOR4A_GPIO, MOTOR4A_PIN);
		GPIO_SetBits(MOTOR4B_GPIO, MOTOR4B_PIN);
		TIM1->CCR4 = -input;
	}
}

s16 ENC1_ReadIncrement(void){
	return 0;
}

s16 ENC2_ReadIncrement(void){
	return 0;
}

void MOTORS_Proc_old(void) {
	int16_t pwm1, pwm2, temp_posErr;

	if(Motor[0].mode == NF_DrivesMode_PWM) {
		Motor[0].setSpeed = NFComBuf.SetDrivesPWM.data[0];
	}
	else if(NF_DrivesMode_POSITION) {
		temp_posErr = ADC.milivolt[0] - NFComBuf.SetDrivesPosition.data[0];
		Motor[0].setSpeed = temp_posErr;
	}
	else {
		Motor[0].setSpeed = 0;
	}
    
    // #### Motor1 PID input data preparation
	if((Motor[0].setSpeed == 0) && (Motor[0].speed == 0))
	{
	//	PID[0].referenceValue = 0;
	//	PID[0].measurementValue = 0;
        if(PID[0].sumError > 0)
            PID[0].sumError --;
        else if(PID[0].sumError < 0)
            PID[0].sumError ++;
        if(PID[0].lastProcessValue > 0)
            PID[0].lastProcessValue --;
        else if(PID[0].lastProcessValue < 0)
            PID[0].lastProcessValue ++;
	}

    PID[0].referenceValue =  Motor[0].setSpeed;
    PID[0].measurementValue = Motor[0].speed;

	// #### Motor2 PID input data preparation
	if(Motor[1].setSpeed == 0 && Motor[1].speed == 0)
	{
	//	PID[1].referenceValue = 0;
	//	PID[1].measurementValue = 0;
        if(PID[1].sumError > 0)
            PID[1].sumError --;
        else if(PID[1].sumError < 0)
            PID[1].sumError ++;
        if(PID[1].lastProcessValue > 0)
            PID[1].lastProcessValue --;
        else if(PID[1].lastProcessValue < 0)
            PID[1].lastProcessValue ++;
	}
	PID[1].referenceValue =  Motor[1].setSpeed;
	PID[1].measurementValue = Motor[1].speed;

	// #### do PID 
	PID[0].inputValue = PID_Controller(&PID[0]);
	PID[1].inputValue = PID_Controller(&PID[1]);

	// #### Motor1 set input
	if(PID[0].inputValue < -190)
		pwm1 = -190;
	else if(PID[0].inputValue > 190)
		pwm1 = 190;
	else
		pwm1 = PID[0].inputValue;
    PID[0].lastProcessValue = PID[0].inputValue;
	
    MOTOR1_SetInput(5 * pwm1);	// MOTORx_SetInput: -1000 <= inputValue <= 1000

	// #### Motor2 set input
	if(PID[1].inputValue < -190)
		pwm2 = -190;
	else if(PID[1].inputValue > 190)
		pwm2 = 190;
	else
		pwm2 = PID[1].inputValue;
    
    PID[1].lastProcessValue = PID[1].inputValue;

    MOTOR2_SetInput(5 * pwm2);	// MOTORx_SetInput: -1000 <= inputValue <= 1000

}

void MOTORS_Proc(void) {
	uint8_t i;
	int16_t pwmToBeSet[4];

	for(i=0; i<3; i++) {
		// Set desired position to current position value in case of switch to Position Mode
		if(Motor[i].mode != NF_DrivesMode_POSITION){
			NFComBuf.SetDrivesPosition.data[i] = ADC.milivolt[i];
		}

		// Mode PWM
		if(Motor[i].mode == NF_DrivesMode_PWM) {
			pwmToBeSet[i] = NFComBuf.SetDrivesPWM.data[i];
		}			 
		// Mode POSITION
		else if(Motor[i].mode == NF_DrivesMode_POSITION) {
			// Limit Position value
			if(NFComBuf.SetDrivesPosition.data[i] < NFComBuf.SetDrivesMinPosition.data[i])
				Motor[i].setPosition = NFComBuf.SetDrivesMinPosition.data[i];
			else if(NFComBuf.SetDrivesPosition.data[i] > NFComBuf.SetDrivesMaxPosition.data[i])
				Motor[i].setPosition = NFComBuf.SetDrivesMaxPosition.data[i];
			else 
				Motor[i].setPosition = NFComBuf.SetDrivesPosition.data[i];

			// #### PID input data preparation
			PID[i].referenceValue	= Motor[i].setPosition;
	    	PID[i].measurementValue	= ADC.milivolt[i];
	
			// #### do PID 
			PID[i].inputValue = PID_Controller(&PID[i]);
	
			// #### Motor1 set input
			if(PID[i].inputValue < -200)
				pwmToBeSet[i] = -200;
			else if(PID[i].inputValue > 200)
				pwmToBeSet[i] = 200;
			else
				pwmToBeSet[i] = PID[i].inputValue;
		    PID[i].lastProcessValue = pwmToBeSet[i];

			// Do some magic to limit integrated error
			if(PID[i].sumError > 200)
				PID[i].sumError = 200;
			else if(PID[i].sumError < -200)
				PID[i].sumError = -200;
			// And magic continues to make PWM fade out as much as possible			
	        if(PID[i].sumError > 0)
	            PID[i].sumError --;
	        else if(PID[i].sumError < 0)
	            PID[i].sumError ++;
	        if(PID[i].lastProcessValue > 0)
	            PID[i].lastProcessValue --;
	        else if(PID[i].lastProcessValue < 0)
	            PID[i].lastProcessValue ++;
			
		    pwmToBeSet[i] = 5 * pwmToBeSet[i];	// MOTORx_SetInput: -1000 <= inputValue <= 1000
		}
		// Mode unknown
		else {
			pwmToBeSet[i] = 0;
		}
	}

	MOTOR1_SetInput(pwmToBeSet[0]);
	MOTOR2_SetInput(pwmToBeSet[1]);
	MOTOR3_SetInput(pwmToBeSet[2]);
}

void MOTORS_MeasureSpeed(void){
	static int16_t oldPosition[3], positionBuf[3];

	positionBuf[0] = ADC.milivolt[0];
	Motor[0].speed = positionBuf[0] - oldPosition[0];
	oldPosition[0] = positionBuf[0];
}
