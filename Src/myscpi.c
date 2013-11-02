#include "myscpi/myscpi.h"
#include "common.h"		
#include "led.h"  	
#include "motor.h"
#include "eeprom.h"
#include "eebackup.h"
#include "io.h"
#include "adc.h"
#include "nf/nfv2.h"
#include "encoder.h"
#include "commutator.h"

extern MOTOR_St				Motor;
extern uint16_t 			serialNumber;
extern NF_STRUCT_ComBuf 	NFComBuf;
extern COMMUTATOR_St		Commutator;

uint8_t MYSCPI_Interpreter(volatile uint8_t *rxBuf, volatile uint8_t *rxPt, volatile uint8_t *txBuf, volatile uint8_t *txCnt)
{
	// Do not modify this section
	//	||	||	||	||	||	||	||
	//	\/	\/	\/	\/	\/	\/	\/
	uint8_t grNameLen = 0;
	uint8_t memNameLen = 0;
	uint8_t ret = 0;  
//	char ax12Data[] = {0, 0, 0, 0, 0};

	*txCnt = 0;
	
	// Wrong start byte
	if((*rxPt) == 0 && rxBuf[0]!=':' && rxBuf[0]!='*'){
		return 0;
	}
	// Start byte or data received
	else if(rxBuf[*rxPt]!='\r' && rxBuf[*rxPt]!='\n'){
		// Pointer increment, Buffer size overflow check
		if(++(*rxPt) >= RXBUFSZ){
			(*rxPt) = 0;
		}
		return 0;
	}
	// End byte '\r' or '\n' received.
	else{
		// Increment rxPt to avoid rxBuf modification during interpretation
		(*rxPt)++;
	}
	/*	/\	/\	/\	/\	/\	/\	/\
		||	||	||	||	||	||	||
	    Do not modify this section

	
	
	    Here you build your own command parser
		||	||	||	||	||	||	||
		\/	\/	\/	\/	\/	\/	\/	*/

	_IF_MEMBER_THEN("*IDN?")
	//	_PRINT_RESPONSE("%s, %s %s \r\n", MODULE_NAME, __DATE__, __TIME__);
		_PRINT_STR_RESPONSE(MODULE_NAME)
	
	else
	_IF_MEMBER_THEN(":DOUT?")
		_PRINT_STR_RESPONSE(u8_to_binary(NFComBuf.SetDigitalOutputs.data[0], 7))
	else
	_IF_MEMBER_THEN(":DOUT"){
		NFComBuf.SetDigitalOutputs.data[0] = 
					(rxBuf[6]=='1')<<7
				|   (rxBuf[7]=='1')<<6
				|   (rxBuf[8]=='1')<<5
				|   (rxBuf[9]=='1')<<4
				|   (rxBuf[10]=='1')<<3
				|   (rxBuf[11]=='1')<<2
				|   (rxBuf[12]=='1')<<1
				|   (rxBuf[13]=='1')<<0;
		NFComBuf.SetDigitalOutputs.updated = 1;
		NFComBuf.dataReceived = 1;
	}
	
	else
	_IF_MEMBER_THEN(":DIN?")
		_PRINT_STR_RESPONSE(u8_to_binary(NFComBuf.ReadDigitalInputs.data[0], 7))
		
	else
	_IF_MEMBER_THEN(":AIN0?")
		_PRINT_INT_RESPONSE(NFComBuf.ReadAnalogInputs.data[0])
	else
	_IF_MEMBER_THEN(":AIN1?")
		_PRINT_INT_RESPONSE(NFComBuf.ReadAnalogInputs.data[1])
	else
	_IF_MEMBER_THEN(":AIN2?")
		_PRINT_INT_RESPONSE(NFComBuf.ReadAnalogInputs.data[2])
	else
	_IF_MEMBER_THEN(":AIN3?")
		_PRINT_INT_RESPONSE(NFComBuf.ReadAnalogInputs.data[3])
	else
	_IF_MEMBER_THEN(":AIN4?")
		_PRINT_INT_RESPONSE(NFComBuf.ReadAnalogInputs.data[4])
	else
	_IF_MEMBER_THEN(":AIN5?")
		_PRINT_INT_RESPONSE(NFComBuf.ReadAnalogInputs.data[5])
	else
	_IF_MEMBER_THEN(":AIN6?")
		_PRINT_INT_RESPONSE(NFComBuf.ReadAnalogInputs.data[6])
	else
	_IF_MEMBER_THEN(":AIN7?")
		_PRINT_INT_RESPONSE(NFComBuf.ReadAnalogInputs.data[7])
	else
	_IF_MEMBER_THEN(":AIN8?")
		_PRINT_INT_RESPONSE(NFComBuf.ReadAnalogInputs.data[8])

	else
	_GROUP(":PWM")
		_GET_SET_MEMBER(NFComBuf.SetDrivesPWM.data[0], "")
		MOTOR_SetPWM(NFComBuf.SetDrivesPWM.data[0]);
	_ENDGROUP

	else
	_GROUP(":POS")
		_GET_SETANDDO_MEMBER(NFComBuf.SetDrivesMinPosition.data[0], ":MIN")
			NFComBuf.SetDrivesMinPosition.updated = 1;
			NFComBuf.dataReceived = 1;
		_END_GET_SETANDDO_MEMBER
		else
		_GET_SETANDDO_MEMBER(NFComBuf.SetDrivesMaxPosition.data[0], ":MAX")
			NFComBuf.SetDrivesMaxPosition.updated = 1;
			NFComBuf.dataReceived = 1;
		_END_GET_SETANDDO_MEMBER
		else
		_IF_MEMBER_THEN("?")
			_PRINT_INT_RESPONSE(Motor.currentPosition)
	_ENDGROUP

	else
	_GROUP(":SP")
		_GET_SETANDDO_MEMBER(NFComBuf.SetDrivesMaxSpeed.data[0], ":MAX")
			NFComBuf.SetDrivesMaxSpeed.updated = 1;
			NFComBuf.dataReceived = 1;
		_END_GET_SETANDDO_MEMBER
		else
		_IF_MEMBER_THEN("?")
			_PRINT_INT_RESPONSE(Motor.currentIncrement)
	_ENDGROUP

	else
	_GROUP(":PID:POS")
		_GET_SETANDDO_MEMBER(NFComBuf.SetPositionRegulator.data[0].p, ":P")
			NFComBuf.SetPositionRegulator.updated = 1;
			NFComBuf.dataReceived = 1;
		_END_GET_SETANDDO_MEMBER 
		else
		_GET_SETANDDO_MEMBER(NFComBuf.SetPositionRegulator.data[0].i, ":I")
			NFComBuf.SetPositionRegulator.updated = 1;
			NFComBuf.dataReceived = 1;
		_END_GET_SETANDDO_MEMBER 
		else
		_GET_SETANDDO_MEMBER(NFComBuf.SetPositionRegulator.data[0].d, ":D")
			NFComBuf.SetPositionRegulator.updated = 1;
			NFComBuf.dataReceived = 1;
		_END_GET_SETANDDO_MEMBER 
	_ENDGROUP

	else
	_GROUP(":PID:CURR")
		_GET_SETANDDO_MEMBER(NFComBuf.SetCurrentRegulator.data[0].p, ":P")
			NFComBuf.SetCurrentRegulator.updated = 1;
			NFComBuf.dataReceived = 1;
		_END_GET_SETANDDO_MEMBER
		else
		_GET_SETANDDO_MEMBER(NFComBuf.SetCurrentRegulator.data[0].i, ":I")
			NFComBuf.SetCurrentRegulator.updated = 1;
			NFComBuf.dataReceived = 1;
		_END_GET_SETANDDO_MEMBER
		else
		_GET_SETANDDO_MEMBER(NFComBuf.SetCurrentRegulator.data[0].d, ":D")
			NFComBuf.SetCurrentRegulator.updated = 1;
			NFComBuf.dataReceived = 1;
		_END_GET_SETANDDO_MEMBER
	_ENDGROUP

	else
	_GROUP(":PID")
		_IF_MEMBER_THEN(":CASC ON")
			Motor.positionCurrentcascade = 1;
		else
		_IF_MEMBER_THEN(":CASC OFF")
			Motor.positionCurrentcascade = 0;
	_ENDGROUP

	else
	_GROUP(":ENC")
		_GET_SET_MEMBER(Commutator.encoderResolution, ":RES")
		else
		_GET_SET_MEMBER(Commutator.encoderPolarity, ":POL")
	_ENDGROUP

	else
	_GROUP(":COMM")
		_IF_MEMBER_THEN(":MOD:BLOC")
			Commutator.commutationMode = COMM_MODE_BLOCK;
		else
		_IF_MEMBER_THEN(":MOD:SIN")
			Commutator.commutationMode = COMM_MODE_SINE;
		else
		_IF_MEMBER_THEN(":MOD:NON")
			Commutator.commutationMode = COMM_MODE_NONE;
		else
		_GET_SET_MEMBER(Commutator.advanceCoeff, ":ADV")
		else
		_GET_SET_MEMBER(Commutator.motorWireRemap1, ":REM1")
		else
		_GET_SET_MEMBER(Commutator.motorWireRemap2, ":REM2")
		else
		_GET_SET_MEMBER(Commutator.motorWireRemap3, ":REM3")
	_ENDGROUP

	else
	_GROUP(":MEM") 
		_IF_MEMBER_THEN(":DEF")
			eebackup_SaveInitialValues();
		else
		_IF_MEMBER_THEN(":STO")
			eebackup_SaveAll();
		else
		_IF_MEMBER_THEN(":REC")
			eebackup_Recover();
	_ENDGROUP

	else
	_GROUP(":LED") 
		_IF_MEMBER_THEN(":SET")
			switch(rxBuf[9]){
				case '0':
					LED_Set(1<<0, 1<<0, 0);
					break;
				case '1':
					LED_Set(1<<1, 1<<1, 0);
					break;
				case '2':
					LED_Set(1<<2, 1<<2, 0);
					break;
			}
		else  
		_IF_MEMBER_THEN(":RES")
			switch(rxBuf[9]){
				case '0':
					LED_Set(1<<0, 0, 0);
					break;
				case '1':
					LED_Set(1<<1, 0, 0);
					break;
				case '2':
					LED_Set(1<<2, 0, 0);
					break;
			}
	_ENDGROUP

/*	else
	_GROUP(":AX1")
		_GET_SETANDDO_MEMBER(ax12Data[0], ":LED")
			AX12write(1, AX12_REG_LED, AX12_SIZE_LED, ax12Data, 0);
		_END_GET_SETANDDO_MEMBER 
		else
		_IF_MEMBER_THEN(":POS?")
			AX12AskPosition(1);
		else
		_GET_SETANDDO_MEMBER(NFComBuf.SetServosPosition.data[8], ":POS")
			AX12SetGoal(1, NFComBuf.SetServosPosition.data[8], 0);
		_END_GET_SETANDDO_MEMBER 
		else
		_GET_SETANDDO_MEMBER(NFComBuf.SetServosMinPosition.data[8], ":MIN")
			AX12SetCWLimit(1, NFComBuf.SetServosMinPosition.data[8]);
		_END_GET_SETANDDO_MEMBER
		else
		_GET_SETANDDO_MEMBER(NFComBuf.SetServosMaxPosition.data[8], ":MAX")
			AX12SetCCWLimit(1, NFComBuf.SetServosMaxPosition.data[8]);
		_END_GET_SETANDDO_MEMBER 
		else
		_GET_SETANDDO_MEMBER(NFComBuf.SetServosSpeed.data[8], ":SP")
			AX12SetSpeed(1, NFComBuf.SetServosSpeed.data[8]);
		_END_GET_SETANDDO_MEMBER 
	_ENDGROUP			   */
	/*	/\	/\	/\	/\	/\	/\	/\
		||	||	||	||	||	||	||
	    Here you build your own command parser
	
	
	    Do not modify this section
		||	||	||	||	||	||	||
		\/	\/	\/	\/	\/	\/	\/ */
	ret = *rxPt;
	// Reset rxPt for next incoming command
	*rxPt = 0;
	// Return length of received command
	return ret;
}

/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 * Modified to return string length
 */
int my_itoa(int value, char* result, int base) {
	int len = 0;
	// check that the base if valid
	if (base < 2 || base > 36) {
		*result = '\0';
		return 0;
	}

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );

	// Apply negative sign
	if (tmp_value < 0)
		*ptr++ = '-';
	*(ptr) = '\0';
	ptr--;
	// calculate string length
	len = ptr +1 - result;

	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return len;
}
