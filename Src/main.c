//##                                      #### ######## ################ INCLUDES
#include "common.h"
#include "clock.h"
#include "led.h"
#include "io.h"
#include "usart1.h"
#include "systick.h"
#include "interrupts.h"
#include "motor.h"
#include "hall.h"
#include "encoder.h"
#include "adc.h"
#include "eeprom.h"
#include "eebackup.h"
#include "usb.h"
#include "pid.h"
#include "myscpi/myscpi.h"
#include "nf/nfv2.h"



//##                                      #### ######## ################ GLOBALS	 
USART_St			Usart1, USBBufs1, USBBufs2;
LED_St				Led;
STDOWNCNT_St		STDownCnt[ST_Downcounters];	
ADC_St				ADC;
uint16_t 			serialNumber;
NF_STRUCT_ComBuf 	NFComBuf;
PID_St				PID[3];
MOTOR_St			Motor;

//##                                      #### ######## ################ PROTOTYPES
void SystemMonitor(void);

//##                                      #### ######## ################ MAIN
int main(void)									  
{
	uint8_t usartBytesToSend, usbBytesToSend1, usbBytesToSend2, usbLastByteReceived;
	uint8_t commArray[10];
	uint8_t commCnt;
	
	// First init System Clock
	RCC_Configuration();	// Init system clock
	
	// Init nonvolatile memory and recover saved values of peirpherals data buffers
	EEPROM_Init(0);
	eebackup_Recover();

	// Then init peripherals
	SYSTICK_Init(STDownCnt);
	LED_Config();
	IN_Config();
//	OUT_Config();
	USART1_Config();
	HALL_Config();
	MOTOR_Config();
	ENCODER1_Config();
//	PID_LoadDefaults(&PID[0]);
//	PID_LoadDefaults(&PID[1]);
//	PID_LoadDefaults(&PID[2]);
	ADCwithDMA_Config();
	NVIC_Configuration();
	USB_Config();
	NFv2_CrcInit();
	PID_Init(&PID[0]);
//	PID_Init(&PID[1]);
//	PID_Init(&PID[2]);
	
	// Execute NFv2_Config() after init of all peripherals. 
	// NFv2_Config() may set some data in NFComBuf to current values
	// read from already initiated peripherals.
	NFv2_Config(&NFComBuf, NF_AddressBase + IN_ReadAddress());

	LED_Set(LED_ALL, 		//mask
			LED_symMINUS,	//newState
			LED_DP);		//blink

	//#### MAIN LOOP ####//
	while (1){
		if(STDownCnt[ST_UsartTxDelay].tick){
			if(Usart1.txDataReady){
				USART1_SendString((char*) Usart1.txBuf);
				Usart1.txDataReady = 0;
			}
			STDownCnt[ST_UsartTxDelay].tick = 0;
		}	
		if(Usart1.rxDataReady!=0)
		{
			strncpy((char*)Usart1.tmpBuf,(const char*)Usart1.rxBuf, 15);
			Usart1.rxDataReady=0;
			ST_Reset(ST_UsartCmdWD);// USART Command Watchdog Reset
		}		   									
		//#### #### SYSTICK EVENT FOR STATUS LED 
		if(STDownCnt[ST_StatusLed].tick){	
			STDownCnt[ST_StatusLed].tick = 0;
			LED_Proc();
			USART1_SendString("Test\r\n");
		}	
		if(STDownCnt[ST_Monitor].tick){
			STDownCnt[ST_Monitor].tick = 0;
			SystemMonitor();
		}

		//#### #### 		COMMUNICATION
		//#### #### #### #### #### #### ####
		if(NFComBuf.dataReceived){
			if(NFComBuf.SetDigitalOutputs.updated){
				OUT_Set(NFComBuf.SetDigitalOutputs.data[0]);
				NFComBuf.SetDigitalOutputs.updated = 0;
			}
			if(NFComBuf.SetDrivesMode.updated){
				// MOTORS_Proc() gets these values directly from NFComBuf.
				NFComBuf.SetDrivesMode.updated = 0;
			}
			if(NFComBuf.SetDrivesPWM.updated){
				// MOTORS_Proc() gets these values directly from NFComBuf.
				NFComBuf.SetDrivesPWM.updated = 0;
			}
			if(NFComBuf.SetDrivesPosition.updated){
				// MOTORS_Proc() gets these values directly from NFComBuf.
				NFComBuf.SetDrivesPosition.updated = 0;
			}
			if(NFComBuf.SetDrivesMinPosition.updated){
				// MOTORS_Proc() gets these values directly from NFComBuf.
				NFComBuf.SetDrivesMinPosition.updated = 0;
			}
			if(NFComBuf.SetDrivesMaxPosition.updated){
				// MOTORS_Proc() gets these values directly from NFComBuf.
				NFComBuf.SetDrivesMaxPosition.updated = 0;
			}
			if(NFComBuf.SetPositionRegulator.updated){
				PID[0].P_Factor = NFComBuf.SetPositionRegulator.data[0].p;
				PID[0].I_Factor = NFComBuf.SetPositionRegulator.data[0].i;
				PID[0].D_Factor = NFComBuf.SetPositionRegulator.data[0].d;
				PID_Init(&PID[0]);
				NFComBuf.SetPositionRegulator.updated = 0;
			}
			NFComBuf.dataReceived = 0;
		}

		if(USB_RxBufNotEmpty()) {
			while(USB_RxBufNotEmpty()){
				usbLastByteReceived = USB_ReadOneByte();

				USBBufs1.rxBuf[USBBufs1.rxPt] = usbLastByteReceived;

				if(MYSCPI_Interpreter(USBBufs1.rxBuf, &USBBufs1.rxPt, USBBufs1.txBuf, &usbBytesToSend1) > 0){
					if(usbBytesToSend1 > 0){
						USB_SendNBytes((uint8_t*)USBBufs1.txBuf, usbBytesToSend1);
					}
				}

				USBBufs2.rxBuf[USBBufs2.rxPt] = usbLastByteReceived;
				
				if(NF_Interpreter(&NFComBuf, (uint8_t*)USBBufs2.rxBuf, (uint8_t*)&USBBufs2.rxPt, commArray, &commCnt) > 0){
					NFComBuf.dataReceived = 1;
					if(commCnt > 0){
						usbBytesToSend2 = NF_MakeCommandFrame(&NFComBuf, (uint8_t*)USBBufs2.txBuf, (const uint8_t*)commArray, commCnt, NFComBuf.myAddress);
						USB_SendNBytes((uint8_t*)USBBufs2.txBuf, usbBytesToSend2);
					}
					// Prepare to send parameters to slave device
					if(NFComBuf.SetDrivesMode.updated)
						commArray[commCnt++] = NF_COMMAND_SetDrivesMode;
					if(NFComBuf.SetDrivesPWM.updated)
						commArray[commCnt++] = NF_COMMAND_SetDrivesPWM;
					if(NFComBuf.SetDrivesSpeed.updated)
						commArray[commCnt++] = NF_COMMAND_SetDrivesSpeed;
					if(NFComBuf.SetDrivesPosition.updated)
						commArray[commCnt++] = NF_COMMAND_SetDrivesPosition;
					if(NFComBuf.SetDrivesMinPosition.updated)
						commArray[commCnt++] = NF_COMMAND_SetDrivesMinPosition;
					if(NFComBuf.SetDrivesMaxPosition.updated)
						commArray[commCnt++] = NF_COMMAND_SetDrivesMaxPosition;

					usartBytesToSend = NF_MakeCommandFrame(&NFComBuf, (uint8_t*)Usart1.txBuf, (const uint8_t*)commArray, commCnt, NFComBuf.myAddress + 1);
					USART1_SendNBytes((uint8_t*)Usart1.txBuf, usartBytesToSend);
				}
			}
	    }
	} //####  END main while LOOP ####//
}

void SystemMonitor(void){
	static uint8_t oldAddress = 0xff;
	static uint32_t oldDevSate = UNCONNECTED;
	static uint8_t oldDrivesMode = 0xff;
	static uint8_t oldEnableSignal = 0xff;
	static uint8_t displayCnt = 0;
	#define DISPLAY_LAG  10
	
	if(displayCnt){
		displayCnt --;
		return;
	}

	if(NFComBuf.myAddress != oldAddress){
		switch(NFComBuf.myAddress){
			case 0: LED_Set(LED_DIGIT, LED_sym0, 0); break;
			case 1: LED_Set(LED_DIGIT, LED_sym1, 0); break;
			case 2: LED_Set(LED_DIGIT, LED_sym2, 0); break;
			case 3: LED_Set(LED_DIGIT, LED_sym3, 0); break;
			case 4: LED_Set(LED_DIGIT, LED_sym4, 0); break;
		}
		oldAddress = NFComBuf.myAddress;
		displayCnt = DISPLAY_LAG;
		return;
	}

	if((bDeviceState != oldDevSate) && (bDeviceState == CONFIGURED)){
		LED_Set(LED_DIGIT, 	//mask
				LED_symu,	//newState
				0);			//blink
		oldDevSate = bDeviceState;
		displayCnt = DISPLAY_LAG;
		return;
	}

	if(Motor.enableSignal != oldEnableSignal){
		if(Motor.enableSignal == 0){
			LED_Set(LED_DIGIT, LED_symStop, 0);
		}
		else
			oldDrivesMode = 0xff;
		oldEnableSignal = Motor.enableSignal;
	}

	if(Motor.mode != oldDrivesMode){
		switch(Motor.mode){
			case NF_DrivesMode_ERROR:		LED_Set(LED_DIGIT, LED_symE, 0); break;
			case NF_DrivesMode_PWM:			LED_Set(LED_DIGIT, LED_symt, 0); break;
			case NF_DrivesMode_SPEED:		LED_Set(LED_DIGIT, LED_symS, 0); break;
			case NF_DrivesMode_POSITION:	LED_Set(LED_DIGIT, LED_symP, 0); break;
			case NF_DrivesMode_SYNC_POS0:	LED_Set(LED_DIGIT, LED_symb, 0); break;
		}
		oldEnableSignal = 0xff;
		oldDrivesMode = Motor.mode;
		displayCnt = DISPLAY_LAG;
		return;
	}
}
