#ifndef _ENCODER_H
#define _ENCODER_H

#include "common.h"

#define ENCODER_RESOLUTION 4000;

enum direction {NONE=0, FORWARD, REVERSE};

typedef struct{
	uint8_t polarity;
	int32_t previousEncPos;
	int32_t currentEncPos;
	int32_t currentRotorPos;
	int32_t encoderResolution;
	int32_t zeroRotorPos;
	uint8_t currentHallPattern;
	uint8_t previousHallPattern;
	uint8_t synchronized;
	uint8_t oneStepMeasured;
	uint32_t oneStepLength;
	enum direction previousZeroCrossDir;
	enum direction zeroCrossDir;
} COMMUTATOR_St;

#define ENCODER1		TIM2->CNT
#define ENCODER2		TIM3->CNT

void ENCODER1_Config(void);
int32_t ENCODER1_Position(void);

#endif //_MOTOR_H
