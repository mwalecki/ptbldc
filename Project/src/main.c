//##                                      #### ######## ################ INCLUDES
#include "common.h"
#include "clock.h"
#include "led.h"
#include "io.h"
#include "usart1.h"
#include "systick.h"
#include "interrupts.h"
#include "motor.h"
#include "encoder.h"
#include "adc.h"
#include "eeprom.h"
#include "eebackup.h"
#include "myscpi.h"
#include "mycrc.h"
#include "nfv2.h"
#include "usb.h"
#include "pid.h"

//##                                      #### ######## ################ GLOBALS	 
USART_St			Usart1, USBBufs1, USBBufs2;
LED_St				Led;
STDOWNCNT_St		STDownCnt[ST_Downcounters];	
ADC_St				ADC;
uint16_t 			serialNumber;
NF_STRUCT_ComBuf 	NFComBuf;
PID_St				PID[3];
MOTOR_St			Motor[1];

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
//	EEPROM_Init(0);
//	eebackup_Recover();

	// Then init peripherals
	SYSTICK_Init(STDownCnt);
	LED_Config();
//	IN_Config();
//	OUT_Config();
//	USART1_Config();
//	MOTORS1234_Config();
	ENCODER1_Config();
//	PID_LoadDefaults(&PID[0]);
//	PID_LoadDefaults(&PID[1]);
//	PID_LoadDefaults(&PID[2]);
//	ADCwithDMA_Config();
	NVIC_Configuration();
	USB_Config();
//	PID_Init(&PID[0]);
//	PID_Init(&PID[1]);
//	PID_Init(&PID[2]);
	
	// Execute NFv2_Config() after init of all peripherals. 
	// NFv2_Config() may set some data in NFComBuf to current values
	// read from already initiated peripherals.
	NFv2_Config(&NFComBuf, NF_AddressBase);

	LED_Set(LED_ALL, 		//mask
			LED_symMINUS,	//newState
			LED_DP);		//blink


	//#### MAIN LOOP ####//
	while (1){
		commCnt = 0;

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
		//	USART1_Interpreter((u8*)Usart1.tmpBuf);
			ST_Reset(ST_UsartCmdWD);// USART Command Watchdog Reset
		}		   									
		//#### #### SYSTICK EVENT FOR STATUS LED 
		if(STDownCnt[ST_StatusLed].tick){	
			STDownCnt[ST_StatusLed].tick = 0;
			LED_Proc();
			if(bDeviceState == CONFIGURED){
				usbBytesToSend1 = my_itoa(ENCODER1_Position(), USBBufs1.txBuf, 10);
				USB_SendNBytes(USBBufs1.txBuf, usbBytesToSend1);
				USB_SendNBytes("\r\n", 2);
			}
		//	USART1_SendString(Usart1.rxBuf);
		//	USART1_SendString("\r\n\n");
		//	USART1_SendNBytes("test\r\n", 6);
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
				Motor[0].mode = NFComBuf.SetDrivesMode.data[0];
				NFComBuf.SetDrivesMode.updated = 0;
			}
			if(NFComBuf.SetDrivesPWM.updated){
				// MOTORS_Proc() gets desired values directly from NFComBuf.
				NFComBuf.SetDrivesPWM.updated = 0;
			}
			if(NFComBuf.SetDrivesPosition.updated){
				// MOTORS_Proc() gets desired values directly from NFComBuf.
				NFComBuf.SetDrivesPosition.updated = 0;
			}
			if(NFComBuf.SetDrivesMinPosition.updated){
				// MOTORS_Proc() gets desired values directly from NFComBuf.
				NFComBuf.SetDrivesMinPosition.updated = 0;
			}
			if(NFComBuf.SetDrivesMaxPosition.updated){
				// MOTORS_Proc() gets desired values directly from NFComBuf.
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
		//	if(MDSET0_isH()) {
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
					}
				}
		//	}
	    }
	} //####  END main while LOOP ####//
}

void SystemMonitor(void){
	static uint32_t oldDevSate = UNCONNECTED;
	
	if((bDeviceState != oldDevSate) && (bDeviceState == CONFIGURED)){
		LED_Set(LED_DIGIT, 		//mask
				LED_symu,		//newState
				0);		//blink
		oldDevSate = bDeviceState;
	}
}
