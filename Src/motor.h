#ifndef _MOTOR_H_
#define _MOTOR_H_
						
#include "common.h"

#define MAX_SPEED 250000

#define PRECISE_INCREMENT_UNIT	1000
// Speed command sets increment in [motors/s] = [0.001motors/(regulator period)]

//#define SYNCHRONIZATION_INCREMENT 	(5*PRECISE_INCREMENT_UNIT)
#define POSITION_MAX_INCREMENT			(70*PRECISE_INCREMENT_UNIT)

#define TIM1PERIOD	600
#define TIM1MAX_CCR	500
// Software value of PWM is divided by 2 before put into CCRx
#define SET_PWM_LIMIT 1000

#define MOTOR1A_GPIO	GPIOB
#define MOTOR1A_PIN		GPIO_Pin_1
#define MOTOR1B_GPIO	GPIOB
#define MOTOR1B_PIN		GPIO_Pin_0
#define MOTOR2A_GPIO	GPIOE
#define MOTOR2A_PIN		GPIO_Pin_7
#define MOTOR2B_GPIO	GPIOB
#define MOTOR2B_PIN		GPIO_Pin_2
#define MOTOR3A_GPIO	GPIOE
#define MOTOR3A_PIN		GPIO_Pin_10
#define MOTOR3B_GPIO	GPIOE
#define MOTOR3B_PIN		GPIO_Pin_9
#define MOTOR4A_GPIO	GPIOE
#define MOTOR4A_PIN		GPIO_Pin_15
#define MOTOR4B_GPIO	GPIOE
#define MOTOR4B_PIN		GPIO_Pin_13


typedef struct{
	uint8_t positionCurrentcascade;
	uint8_t mode;
	uint8_t previousMode;
	int16_t setPWM;
	int16_t setCurrent;
	int32_t setIncrement;
	int32_t setTargetPosition;
	int32_t setPosition;
	int32_t previousSetPosition;
	int32_t currentIncrement;
	int32_t previousPosition;
	int32_t currentPosition;
	uint8_t synchronizingToEncIndex;
	uint8_t isSynchronized;
	uint8_t positionLimit;
	uint8_t limitSwitchUp;
	uint8_t limitSwitchDown;
	uint8_t switchHome;
	uint8_t enableSignal;
	uint16_t maxPWM;
	uint8_t switchPolarityHome;
	uint8_t switchPolarityUp;
	uint8_t switchPolarityDown;
	uint8_t switchPullupHome;
	uint8_t switchPullupUp;
	uint8_t switchPullupDown;
	int16_t synchroIncrement;
	int16_t speedFFCoefA;
	int16_t speedFFCoefB;
} MOTOR_St;

//##                                      #### ######## ################ PROTOTYPES:

inline void motorSetCurrent(void);
inline void motorSetSynchronizationSpeed(void);
inline void motorSpeedToPosition(void);
inline void motorTargetPositionAndIncrementToPosition(void);
inline void motorLimitSpeed(void);
inline void motorLimitPosition(void);
inline void motorPositionToPWM(void);
inline void motorSetPositionDerivativeFFAddsToPWM(void);
inline void motorPWMpositionLimit(void);

void MOTOR_Config(void);
void MOTOR_SetPWM(s16 input);
void MOTOR_Proc(void);
void MOTOR_MeasureSpeed(void);
void BLDCMotorPrepareCommutation(void);


#endif	// ifndef _MOTOR_H_
