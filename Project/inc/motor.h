#ifndef _MOTOR_H_
#define _MOTOR_H_
						
#include "common.h"		

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
	int16_t setSpeed;
	int16_t setPosition;
	uint8_t dir;
	int16_t speed;
} MOTOR_St;

//##                                      #### ######## ################ PROTOTYPES:

void MOTORS12_Config(void);
void MOTORS1234_Config(void);
void MOTOR1_SetInput(s16 input);
void MOTOR2_SetInput(s16 input);
void MOTOR3_SetInput(s16 input);
void MOTOR4_SetInput(s16 input);
s16 ENC1_ReadIncrement(void);
s16 ENC2_ReadIncrement(void);
void MOTORS_Proc(void);
void MOTORS_MeasureSpeed(void);

#endif	// ifndef _MOTOR_H_
