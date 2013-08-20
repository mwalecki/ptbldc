/*
 *      AVR447: Sinusoidal driving of three-phase permanent motor using
 *      ATmega48/88/168
 *
 ******************************************************************************/

#ifndef _COMMUTATION_H_
#define _COMMUTATION_H_

#define COMM_MODE_BLOCK	0
#define COMM_MODE_SINE	1
#define COMM_MODE_NONE	2

//! The number of elements in the sine modulation table per phase.
#define COMMUTATION_TABLE_LENGTH 192U

#if COMMUTATION_TABLE_LENGTH != 192U
#warning "Changing sine modulation table length can have unwanted side effects. Consult the documentation for more information."
#endif

enum direction {NONE=0, FORWARD, REVERSE};

typedef struct{
	uint8_t commutationMode;
	uint8_t encoderPolarity;
	int32_t previousEncPos;
	int32_t currentEncPos;
	int32_t currentRotorPos;
	uint16_t encoderResolution;
	int32_t zeroRotorPos;
	uint8_t currentHallPattern;
	uint8_t prev0HallPattern;
	uint8_t prev1HallPattern;
	uint8_t synchronized;
	uint8_t oneStepMeasured;
	uint32_t oneStepLength;
	uint16_t lastHallStepDuration;
	uint16_t commTableIndexAdvance;
	uint16_t advanceCoeff;
	enum direction previousZeroCrossDir;
	enum direction zeroCrossDir;
} COMMUTATOR_St;

//TODO: Advance commutation
//TODO: Encoder error detection

#endif
