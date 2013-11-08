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
	u32 uVoltsPerUnit;
	u16 unitsOffset;
	u16 logicZeroMax_mV;
	u16 logicOneMin_mV;
	u32 currentMeasure_raw[ADC_CurrentChannels];
	u32 currentMeasure_uVoltsPerUnit;
	u16 currentMeasure_unitsOffset;
	u16 currentMeasure_milivolt[ADC_CurrentChannels];
	u32 currentMeasure_uAmperesPermV;
	u16 currentMeasure_mVOffset;
	s32 currentMeasure_miliampere[ADC_CurrentChannels];
} ADC_St;

void ADC_Config(void);
void ADCwithDMA_Config(void);
u16 readADC1(u8 channel);	   
void DMA1_Channel1_IRQHandler(void);

#endif //_ADC_H
