#include "motor.h"
#include "hall.h"
#include "encoder.h"
#include "pid.h"
#include "io.h"
#include "nf/nfv2.h"
#include "commutator.h"
#include <stdlib.h>

extern MOTOR_St				Motor;
extern NF_STRUCT_ComBuf 	NFComBuf;
extern PID_St				PID[];
extern COMMUTATOR_St		Commutator;
//extern const uint8_t 		sineTable[];
extern uint8_t sineTable[COMMUTATION_TABLE_LENGTH * 3] = {
0,       0,      221,
8,       0,      225,
17,      0,      229,
25,      0,      232,
33,      0,      236,
42,      0,      239,
50,      0,      241,
58,      0,      244,
66,      0,      246,
74,      0,      248,
82,      0,      250,
90,      0,      252,
98,      0,      253,
105,     0,      254,
113,     0,      254,
120,     0,      255,
128,     0,      255,
135,     0,      255,
142,     0,      254,
149,     0,      254,
155,     0,      253,
162,     0,      252,
168,     0,      250,
174,     0,      248,
180,     0,      246,
186,     0,      244,
192,     0,      241,
197,     0,      239,
202,     0,      236,
207,     0,      232,
212,     0,      229,
217,     0,      225,
221,     0,      221,
225,     0,      217,
229,     0,      212,
232,     0,      207,
236,     0,      202,
239,     0,      197,
241,     0,      192,
244,     0,      186,
246,     0,      180,
248,     0,      174,
250,     0,      168,
252,     0,      162,
253,     0,      155,
254,     0,      149,
254,     0,      142,
255,     0,      135,
255,     0,      127,
255,     0,      120,
254,     0,      113,
254,     0,      105,
253,     0,      98,
252,     0,      90,
250,     0,      82,
248,     0,      74,
246,     0,      66,
244,     0,      58,
241,     0,      50,
239,     0,      42,
236,     0,      33,
232,     0,      25,
229,     0,      17,
225,     0,      8,
221,     0,      0,
225,     8,      0,
229,     17,     0,
232,     25,     0,
236,     33,     0,
239,     42,     0,
241,     50,     0,
244,     58,     0,
246,     66,     0,
248,     74,     0,
250,     82,     0,
252,     90,     0,
253,     98,     0,
254,     105,    0,
254,     113,    0,
255,     120,    0,
255,     127,    0,
255,     135,    0,
254,     142,    0,
254,     149,    0,
253,     155,    0,
252,     162,    0,
250,     168,    0,
248,     174,    0,
246,     180,    0,
244,     186,    0,
241,     192,    0,
239,     197,    0,
236,     202,    0,
232,     207,    0,
229,     212,    0,
225,     217,    0,
221,     221,    0,
217,     225,    0,
212,     229,    0,
207,     232,    0,
202,     236,    0,
197,     239,    0,
192,     241,    0,
186,     244,    0,
180,     246,    0,
174,     248,    0,
168,     250,    0,
162,     252,    0,
155,     253,    0,
149,     254,    0,
142,     254,    0,
135,     255,    0,
128,     255,    0,
120,     255,    0,
113,     254,    0,
105,     254,    0,
98,      253,    0,
90,      252,    0,
82,      250,    0,
74,      248,    0,
66,      246,    0,
58,      244,    0,
50,      241,    0,
42,      239,    0,
33,      236,    0,
25,      232,    0,
17,      229,    0,
8,       225,    0,
0,       221,    0,
0,       225,    8,
0,       229,    17,
0,       232,    25,
0,       236,    33,
0,       239,    42,
0,       241,    50,
0,       244,    58,
0,       246,    66,
0,       248,    74,
0,       250,    82,
0,       252,    90,
0,       253,    98,
0,       254,    105,
0,       254,    113,
0,       255,    120,
0,       255,    128,
0,       255,    135,
0,       254,    142,
0,       254,    149,
0,       253,    155,
0,       252,    162,
0,       250,    168,
0,       248,    174,
0,       246,    180,
0,       244,    186,
0,       241,    192,
0,       239,    197,
0,       236,    202,
0,       232,    207,
0,       229,    212,
0,       225,    217,
0,       221,    221,
0,       217,    225,
0,       212,    229,
0,       207,    232,
0,       202,    236,
0,       197,    239,
0,       192,    241,
0,       186,    244,
0,       180,    246,
0,       174,    248,
0,       168,    250,
0,       162,    252,
0,       155,    253,
0,       149,    254,
0,       142,    254,
0,       135,    255,
0,       128,    255,
0,       120,    255,
0,       113,    254,
0,       105,    254,
0,       98,     253,
0,       90,     252,
0,       82,     250,
0,       74,     248,
0,       66,     246,
0,       58,     244,
0,       50,     241,
0,       42,     239,
0,       33,     236,
0,       25,     232,
0,       17,     229,
0,       8,      225
};



//##                                      #### ######## ################ PRIVATE GLOBALS

void MOTOR_Proc(void) {

	Motor.previousPosition = Motor.currentPosition;
	Motor.currentPosition = ENCODER1_Position();
	Motor.currentIncrement = Motor.currentPosition - Motor.previousPosition;
	Motor.enableSignal = (IN_ReadENABLE() == 0) ? 1 : 0;
	Motor.limitSwitchUp = (IN_ReadLIMITPOS() == 0) ? 1 : 0;
	Motor.limitSwitchDown = (IN_ReadLIMITNEG() == 0) ? 1 : 0;
	Motor.switchHome = (IN_ReadHOME() == 0) ? 1 : 0;

	Motor.previousMode = Motor.mode;
	Motor.mode = NFComBuf.SetDrivesMode.data[0];

	switch(Motor.mode){
	case NF_DrivesMode_SYNC_POS0:
		motorSetSynchronizationSpeed();
		motorSpeedToPosition();
		motorPositionToPWM();
		break;
	case NF_DrivesMode_SPEED:
		Motor.setIncrement = NFComBuf.SetDrivesSpeed.data[0];
		motorLimitSpeed();
		motorSpeedToPosition();
		motorLimitPosition();
		motorPositionToPWM();
		break;
	case NF_DrivesMode_POSITION:
		Motor.setTargetPosition = NFComBuf.SetDrivesPosition.data[0];
		Motor.setIncrement = NFComBuf.SetDrivesSpeed.data[0];
		motorTargetPositionAndIncrementToPosition();
		motorLimitPosition();
		motorPositionToPWM();
		break;
	case NF_DrivesMode_PWM:
		Motor.setPWM = NFComBuf.SetDrivesPWM.data[0];
		motorPWMpositionLimit();
		break;
	default:
		Motor.setPWM = 0;
		break;
	}

//	BLDCMotorPrepareCommutation();
//	TIM_GenerateEvent(TIM1, TIM_EventSource_COM);

	NFComBuf.ReadDrivesPosition.data[0] = Motor.currentPosition;
	NFComBuf.ReadDrivesStatus.data[0] = (Motor.isSynchronized ? NF_DrivesStatus_Synchronized : 0)
									| 	(Motor.positionLimit ? NF_DrivesStatus_PositionLimit : 0)
									| 	(Motor.enableSignal ? 0 : NF_DrivesStatus_Error)
									| 	((Motor.mode == NF_DrivesMode_ERROR) ? NF_DrivesStatus_Error : 0);
	NFComBuf.ReadDigitalInputs.data[0] = (Motor.limitSwitchUp ? (1<<0) : 0)
									|	(Motor.limitSwitchDown ? (1<<1) : 0)
									|	(Motor.switchHome ? (1<<2) : 0);
}

inline void motorSetSynchronizationSpeed(void) {
	static uint8_t homeAreaChecked;

	if(Motor.previousMode != NF_DrivesMode_SYNC_POS0){
		Motor.isSynchronized = 0;
		Motor.setIncrement = Motor.currentIncrement;
		homeAreaChecked = 0;
	}
	if(Motor.isSynchronized){
		Motor.setIncrement = 0;
		return;
	}
	if(IN_ReadHOME()){
		homeAreaChecked = 1;
//		if(Motor.setIncrement < SYNCHRONIZATION_INCREMENT)
//			Motor.setIncrement ++;
//		else if(Motor.setIncrement > SYNCHRONIZATION_INCREMENT)
//			Motor.setIncrement --;
		Motor.setIncrement = SYNCHRONIZATION_INCREMENT;
	}
	else {
		if(homeAreaChecked){
			Motor.synchronizingToEncIndex = 1;
			return;
		}
//		if(Motor.setIncrement < - SYNCHRONIZATION_INCREMENT)
//			Motor.setIncrement ++;
//		else if(Motor.setIncrement > - SYNCHRONIZATION_INCREMENT)
//			Motor.setIncrement --;
		Motor.setIncrement = - SYNCHRONIZATION_INCREMENT;
	}
}

inline void motorSpeedToPosition(void) {
	static int32_t setFineIncrement = 0;
	int32_t setCoarseIncrement;
	int32_t sgn;

	setFineIncrement += Motor.setIncrement;
	sgn = (setFineIncrement < 0)?(-1):1;
	setCoarseIncrement = sgn * (abs(setFineIncrement) / PRECISE_INCREMENT_UNIT);
	setFineIncrement = sgn * (abs(setFineIncrement) % PRECISE_INCREMENT_UNIT);

	Motor.setPosition = Motor.setPosition + setCoarseIncrement;
}

inline void motorTargetPositionAndIncrementToPosition(void) {
	static int32_t maxFineIncrement = 0;
	int32_t maxCoarseIncrement;

	// Calculate Position limit
	maxFineIncrement += abs(Motor.setIncrement);
	maxCoarseIncrement = maxFineIncrement / PRECISE_INCREMENT_UNIT;
	maxFineIncrement = maxFineIncrement % PRECISE_INCREMENT_UNIT;

	// Limit Position increment
	if(Motor.setTargetPosition > Motor.setPosition + maxCoarseIncrement)
		Motor.setPosition += maxCoarseIncrement;
	else if(Motor.setTargetPosition < Motor.setPosition - maxCoarseIncrement)
		Motor.setPosition -= maxCoarseIncrement;
	else
		Motor.setPosition = Motor.setTargetPosition;
}

inline void motorLimitSpeed(void) {
//	if(Motor.setIncrement > NFComBuf.SetDrivesMaxSpeed.data[0])
//		Motor.setIncrement = NFComBuf.SetDrivesMaxSpeed.data[0];
//	else if(Motor.setIncrement < - NFComBuf.SetDrivesMaxSpeed.data[0])
//		Motor.setIncrement = - NFComBuf.SetDrivesMaxSpeed.data[0];
	if(Motor.setIncrement > MAX_SPEED)
		Motor.setIncrement = MAX_SPEED;
	else if(Motor.setIncrement < - MAX_SPEED)
		Motor.setIncrement = - MAX_SPEED;
}

inline void motorLimitPosition(void) {
	if(NFComBuf.SetDrivesMinPosition.data[0] == NFComBuf.SetDrivesMaxPosition.data[0]){
		Motor.positionLimit = 0;
		return;
	}

	if(Motor.setPosition < NFComBuf.SetDrivesMinPosition.data[0]) {
		Motor.positionLimit = 1;
		Motor.setPosition = NFComBuf.SetDrivesMinPosition.data[0];
	}
	else if(Motor.setPosition > NFComBuf.SetDrivesMaxPosition.data[0]) {
		Motor.positionLimit = 1;
		Motor.setPosition = NFComBuf.SetDrivesMaxPosition.data[0];
	}
	else
		Motor.positionLimit = 0;

	if(Motor.setTargetPosition < NFComBuf.SetDrivesMinPosition.data[0]) {
		Motor.setTargetPosition = NFComBuf.SetDrivesMinPosition.data[0];
	}
	else if(Motor.setTargetPosition > NFComBuf.SetDrivesMaxPosition.data[0]) {
		Motor.setTargetPosition = NFComBuf.SetDrivesMaxPosition.data[0];
	}
}

inline void motorPositionToPWM(void) {
	if((Motor.previousMode != NF_DrivesMode_POSITION)
			&& (Motor.previousMode != NF_DrivesMode_SPEED)
			&& (Motor.previousMode != NF_DrivesMode_SYNC_POS0)){
		Motor.setPosition = Motor.currentPosition;
	}

	// #### PID input data preparation
	PID[0].referenceValue	= Motor.setPosition;
	PID[0].measurementValue	= Motor.currentPosition;

	// #### do PID
	PID[0].inputValue = PID_Controller(&PID[0]);

	// #### Motor set input
	if(PID[0].inputValue < -SET_PWM_LIMIT)
		Motor.setPWM = -SET_PWM_LIMIT;
	else if(PID[0].inputValue > SET_PWM_LIMIT)
		Motor.setPWM = SET_PWM_LIMIT;
	else
		Motor.setPWM = PID[0].inputValue;
    PID[0].lastProcessValue = Motor.setPWM;

	// Do some magic to limit integrated error
	if(PID[0].sumError > SET_PWM_LIMIT)
		PID[0].sumError = SET_PWM_LIMIT;
	else if(PID[0].sumError < -SET_PWM_LIMIT)
		PID[0].sumError = -SET_PWM_LIMIT;
	// And magic continues to make PWM fade out as much as possible
    if(PID[0].sumError > 0)
        PID[0].sumError --;
    else if(PID[0].sumError < 0)
        PID[0].sumError ++;
    if(PID[0].lastProcessValue > 0)
        PID[0].lastProcessValue --;
    else if(PID[0].lastProcessValue < 0)
        PID[0].lastProcessValue ++;

    //Motor.setPWM = 5 * Motor.setPWM;	// MOTORx_SetInput: -1000 <= inputValue <= 1000
}

inline void motorPWMpositionLimit(void) {
	if(NFComBuf.SetDrivesMinPosition.data[0] == NFComBuf.SetDrivesMaxPosition.data[0]){
		Motor.positionLimit = 0;
		return;
	}

	if(Motor.currentPosition < NFComBuf.SetDrivesMinPosition.data[0]) {
		Motor.positionLimit = 1;
		if(Motor.setPWM < 0)
				Motor.setPWM = 0;
	}
	else if(Motor.currentPosition > NFComBuf.SetDrivesMaxPosition.data[0]) {
		Motor.positionLimit = 1;
		if(Motor.setPWM > 0)
				Motor.setPWM = 0;
	}
	else
		Motor.positionLimit = 0;
}

// BLDC motor steps
// every row from 1 to 6 is called by a hall state
// every column a FET from 3-phase bridge
// motor off is at row 0 BLDC_BRIDGE_STATE_VORWARD[0]
// cw - rechtslauf - positiv
// {    1H,1L      ,      2H,2L      ,     3H,3L    }
// BLDC motor steps
// every row from 1 to 6 is one of the 6 motor vector state
// every column a FET from 3-phase bridge
// all FETs off at row 0 or 8 (this pattern should not come from the hallsensor)
// cw - rechtslauf - positiv
// {    1H,1L      ,      2H,2L      ,     3H,3L    }
static const u8 BLDC_BRIDGE_STATE_FORWARD[8][6] =   // Motor step
{
//									BRIDGE	SEQ		HALL
//												mik.net	Parvalux
   { 0,0   ,   0,0   ,  0,0 },  //	. . .	0	000		000
   { 0,0   ,   0,1   ,  1,0 },  //	. L H	4	010		001
   { 0,1   ,   1,0   ,  0,0 },  //	L H .	2	001		010
   { 0,1   ,   0,0   ,  1,0 },  //	L . H	3	011		011
   { 1,0   ,   0,0   ,  0,1 },  //	H . L	6	100		100
   { 1,0   ,   0,1   ,  0,0 },  //	H L .	5	110		101
   { 0,0   ,   1,0   ,  0,1 },  //	. H L	1	101		110
   { 0,0   ,   0,0   ,  0,0 }  //	. . .	0	111		000
};
static const u8 BLDC_BRIDGE_STATE_REVERSE[8][6] =   // Motor step
{
//									BRIDGE	SEQ		HALL
//												mik.net	Parvalux
   { 0,0   ,   0,0   ,  0,0 },  //	. . .	0	000		000
   { 0,0   ,   1,0   ,  0,1 },  //	. H L	4	010		001
   { 1,0   ,   0,1   ,  0,0 },  //	H L .	2	001		010
   { 1,0   ,   0,0   ,  0,1 },  //	H . L	3	011		011
   { 0,1   ,   0,0   ,  1,0 },  //	L . H	6	100		100
   { 0,1   ,   1,0   ,  0,0 },  //	L H .	5	110		101
   { 0,0   ,   0,1   ,  1,0 },  //	. L H	1	101		110
   { 0,0   ,   0,0   ,  0,0 }  //	. . .	0	111		000
};
static const u8 BLDC_BRIDGE_STATE_STILL[8][6] =   // Motor step
{
//									BRIDGE	SEQ		HALL
//												mik.net	Parvalux
   { 0,0   ,   0,0   ,  0,0 },  //	. . .	0	000		000
   { 0,0   ,   0,0   ,  0,0 },  //	. . .	4	010		001
   { 0,0   ,   0,0   ,  0,0 },  //	. . .	2	001		010
   { 0,0   ,   0,0   ,  0,0 },  //	. . .	3	011		011
   { 0,0   ,   0,0   ,  0,0 },  //	. . .	6	100		100
   { 0,0   ,   0,0   ,  0,0 },  //	. . .	5	110		101
   { 0,0   ,   0,0   ,  0,0 },  //	. . .	1	101		110
   { 0,0   ,   0,0   ,  0,0 }  //	. . .	0	111		000
};

void MOTOR_Config(void) {
	//Configuration Structures
//	NVIC_InitTypeDef		NVIC_InitStructure;
	GPIO_InitTypeDef		GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef		TIM_OCInitStructure;
//	TIM_ICInitTypeDef		TIM_ICInitStructure;
	TIM_BDTRInitTypeDef		TIM_BDTRInitStructure;

	// TIM1 clock enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	// AFIO Clock Enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// GPIO clocks enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

	// TIM1 Full Pin Remap
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);

	/*	PORT E Push-Pull Outputs:				*\
		PE.9	(TIM1_CH1)	HIN1
		PE.11	(TIM1_CH2)	HIN2
		PE.13	(TIM1_CH3)	HIN3
		PE.8	(TIM1_CH1N)	LIN1
		PE.10	(TIM1_CH2N)	LIN2
		PE.12	(TIM1_CH3N)	LIN3				*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
 
	// Time Base configuration
	TIM_TimeBaseStructure.TIM_Prescaler = 3;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 1200;		// 24MHz / 1SET_PWM_LIMIT = 20kHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
 
	// Channel 1, 2, 3 set to PWM mode - all 6 outputs
	// per channel on output is  low side fet, the opposite is for high side fet

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 500; // BLDC_ccr_val

	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;

	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_Pulse = 1000; // Just before update event, fire CC4 interrupt to preload PWM values

	TIM_OC4Init(TIM1, &TIM_OCInitStructure);

	// activate preloading the CCR register
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
 
	/* automatic output enable, break off, dead time ca. SET_PWM_LIMITns and
	// no lock of configuration */
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;

	// DeadTime value n=1 bis 31: from 14ns to 1,7us
	// DeadTime value n=129 bis 159: from 1,7us to 3,5ms
	// DeadTime value 7 => 98ns
	// ... see programmers reference manual
	// DeadTime[ns] = value * (1/SystemCoreFreq) (on 72MHz: 7 is 98ns, on 24MHz: 7 is 296ns)
	TIM_BDTRInitStructure.TIM_DeadTime = 7;
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
	 
	// enabel this if you use emergency stop signal
	// TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
	// TIM_BDTRInitStructure.TIM_BreakPolarity = MOTOR_TMC603_EMSTOP_POLARITY; 
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;
	TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);
 
	// preload ARR register
	TIM_CCPreloadControl(TIM1, DISABLE);

	// Clear update interrupt bit
	TIM_ClearITPendingBit(TIM1,TIM_IT_CC4);
	// Enable capture/compare interrupt
	TIM_ITConfig(TIM1,TIM_IT_CC4,ENABLE);

	// enable motor timer
	TIM_Cmd(TIM1, ENABLE);

	// enable motor timer main output (the bridge signals)
	TIM_CtrlPWMOutputs(TIM1, ENABLE);

	// Data structures init
	Motor.setPWM = 0;
	Motor.setIncrement = 0;
	Motor.setPosition = 0;
	Motor.setTargetPosition = 0;
	Motor.setCurrent = 0;
}
 
/**
 * Every PWM cycle this interrupt routine is called
 * to calculate and preload PWM settings for the next cycle
 */
void TIM1_CC_IRQHandler(void)
{
	vu8			hallpos;
	vu8			BH1, BL1, BH2, BL2, BH3, BL3;
	const u8 	(*bldcBridgeState)[6];
	vs16 		pwm;
	int32_t		pwm1, pwm2, pwm3;
	uint32_t	rotorPosition;

	// 200 ticks to TIM1 overflow
	if (TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET) {
		TIM_ClearITPendingBit(TIM1,TIM_IT_CC4);

		if(Commutator.synchronized != 0){

			rotorPosition = COMM_RotorPosition();

			if(Motor.setPWM == 0){
				pwm = pwm1 = pwm2 = pwm3 = 0;
			}
			else if(Motor.setPWM > 0){
				pwm = Motor.setPWM;
				pwm1 = (pwm * sineTable[commutationTableIndexFwd(rotorPosition, 0)]) >> 8;
				pwm2 = (pwm * sineTable[commutationTableIndexFwd(rotorPosition, 1)]) >> 8;
				pwm3 = (pwm * sineTable[commutationTableIndexFwd(rotorPosition, 2)]) >> 8;
			}
			else{
				pwm = - Motor.setPWM;
				pwm1 = (pwm * sineTable[commutationTableIndexRev(rotorPosition, 0)]) >> 8;
				pwm2 = (pwm * sineTable[commutationTableIndexRev(rotorPosition, 1)]) >> 8;
				pwm3 = (pwm * sineTable[commutationTableIndexRev(rotorPosition, 2)]) >> 8;
			}

			// Bridge FETs for Motor Phase U
			TIM1->CCR1 = pwm1;
			TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Enable);
			// Bridge FETs for Motor Phase V
			TIM1->CCR2 = pwm2;
			TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Enable);
			// Bridge FETs for Motor Phase W
			TIM1->CCR3 = pwm3;
			TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Enable);


			//NFComBuf.ReadDeviceVitals.data[0] = Commutator.currentHallPattern;
			NFComBuf.ReadDeviceVitals.data[1] = Commutator.zeroRotorPos;
			NFComBuf.ReadDeviceVitals.data[2] = Commutator.currentRotorPos;
			NFComBuf.ReadDeviceVitals.data[3] = pwm1;
			NFComBuf.ReadDeviceVitals.data[4] = pwm2;
			NFComBuf.ReadDeviceVitals.data[5] = pwm3;

		}
		else {

			hallpos = HALL_Pattern();

			if(Motor.setPWM == 0){
				bldcBridgeState = BLDC_BRIDGE_STATE_STILL;
				pwm = 0;
			}
			else if(Motor.setPWM > 0){
				bldcBridgeState = BLDC_BRIDGE_STATE_FORWARD;
				pwm = Motor.setPWM;
			}
			else{
				bldcBridgeState = BLDC_BRIDGE_STATE_REVERSE;
				pwm = - Motor.setPWM;
			}

			BH1 = bldcBridgeState[hallpos][0];
			BL1 = bldcBridgeState[hallpos][1];

			BH2 = bldcBridgeState[hallpos][2];
			BL2 = bldcBridgeState[hallpos][3];

			BH3 = bldcBridgeState[hallpos][4];
			BL3 = bldcBridgeState[hallpos][5];

			// Bridge FETs for Motor Phase U
			if (BH1) {
				TIM1->CCR1 = pwm;
				TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Enable);
			} else {
				TIM1->CCR1 = 0;
				if (BL1){
					TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Enable);
				} else {
					TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Disable);
				}
			}

			// Bridge FETs for Motor Phase V
			if (BH2) {
				TIM1->CCR2 = pwm;
				TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Enable);
			} else {
				TIM1->CCR2 = 0;
				if (BL2){
					TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Enable);
				} else {
					TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Disable);
				}
			}

			// Bridge FETs for Motor Phase W
			if (BH3) {
				TIM1->CCR3 = pwm;
				TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Enable);
			} else {
				TIM1->CCR3 = 0;
				if (BL3){
					TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Enable);
				} else {
					TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Disable);
				}
			}
		}
	}
	else {
		while(1)
			; // this should not happen
	}
}

void MOTOR_SetPWM(s16 pwm) {
	if(pwm > MAX_PWM)
		pwm = MAX_PWM;
	else if(pwm < -MAX_PWM)
		pwm = -MAX_PWM;
	Motor.setPWM = pwm;
}
