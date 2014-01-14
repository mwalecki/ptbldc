#ifndef _ADC_H
#define _ADC_H

#include "common.h"

#define ADC1_DR_Address    ((uint32_t)0x4001244C)
#define ADC_DMA_TransferCompleteInterrupt

#define ADC_VoltageChannels 8
#define ADC_CurrentChannels 2
#define ADC_DigitalBytes ((ADC_VoltageChannels-1)/8 + 1)

typedef struct{
	vu16 raw[ADC_VoltageChannels];
	vu16 milivolt[ADC_VoltageChannels];
	vu16 mvTreshold[ADC_VoltageChannels];
	vu8 digital[ADC_DigitalBytes];
	s32 uVoltsPerUnit;
	s16 unitsOffset;
	s16 logicZeroMax_mV;
	s16 logicOneMin_mV;
	s32 currentMeasure_raw[ADC_CurrentChannels];
	s32 currentMeasure_uVoltsPerUnit;
	s16 currentMeasure_unitsOffset;
	s16 currentMeasure_milivolt[ADC_CurrentChannels];
	s32 currentMeasure_uAmperesPermV;
	s16 currentMeasure_mVOffset;
	s32 currentMeasure_miliampere[ADC_CurrentChannels];
	s32 currentMeasure_miliampereInsensitivity;
} ADC_St;

void ADC_Config(void);
void ADCwithDMA_Config(void);
u16 readADC1(u8 channel);	   
void DMA1_Channel1_IRQHandler(void);

#endif //_ADC_H
