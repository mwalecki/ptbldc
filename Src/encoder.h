#ifndef _ENCODER_H
#define _ENCODER_H

#include "common.h"

#define ENCODER_RESOLUTION 4000;

#define ENCODER1		TIM2->CNT
#define ENCODER2		TIM3->CNT

void ENCODER1_Config(void);
int32_t ENCODER1_Position(void);

//TODO: Configurable encoder resolution


#endif //_MOTOR_H
