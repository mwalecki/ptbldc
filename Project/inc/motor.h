#ifndef _MOTOR_H_
#define _MOTOR_H_
						
#include "common.h"		

#define POSITION_MAX_INCREMENT	20

#define TIM1PERIOD	1000
#define MAX_PWM		TIM1PERIOD

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
	uint8_t mode;
	int16_t setPWM;
	int16_t setCurrent;
	int16_t setSpeed;
	int32_t setTargetPosition;
	int32_t setPosition;
	int16_t speed;
	int32_t position;
} MOTOR_St;

//##                                      #### ######## ################ PROTOTYPES:

void MOTOR_Config(void);
void MOTOR_SetPWM(s16 input);
void MOTOR_Proc(void);
void MOTOR_MeasureSpeed(void);
void BLDCMotorPrepareCommutation(void);


#endif	// ifndef _MOTOR_H_
