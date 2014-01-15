#include "eebackup.h"
#include "pid.h"
#include "nf/nfv2.h"
#include "commutator.h"
#include "motor.h"
#include "adc.h"
	 

extern ADC_St				ADC;
extern uint16_t				serialNumber;
extern SERVO_St				Servo;
extern COMMUTATOR_St			Commutator;
extern NF_STRUCT_ComBuf 	NFComBuf;
extern PID_St				PID[];
extern MOTOR_St				Motor;

void eebackup_Recover(void)
{
	PID[0].P_Factor = EEPROM_Read(EEADDR_PID1_P);
	PID[0].I_Factor = EEPROM_Read(EEADDR_PID1_I);
	PID[0].D_Factor = EEPROM_Read(EEADDR_PID1_D);
	PID[1].P_Factor = EEPROM_Read(EEADDR_PID2_P);
	PID[1].I_Factor = EEPROM_Read(EEADDR_PID2_I);
	PID[1].D_Factor = EEPROM_Read(EEADDR_PID2_D);
	PID[2].P_Factor = EEPROM_Read(EEADDR_PID3_P);
	PID[2].I_Factor = EEPROM_Read(EEADDR_PID3_I);
	PID[2].D_Factor = EEPROM_Read(EEADDR_PID3_D);

	NFComBuf.SetDrivesMinPosition.data[0] = 
		(((uint32_t)EEPROM_Read(EEADDR_DRV1_MIN_H)) << 16 ) | EEPROM_Read(EEADDR_DRV1_MIN_L);
	NFComBuf.SetDrivesMaxPosition.data[0] = 
		(((uint32_t)EEPROM_Read(EEADDR_DRV1_MAX_H)) << 16 ) | EEPROM_Read(EEADDR_DRV1_MAX_L);
	
	NFComBuf.SetDrivesMaxSpeed.data[0] =
		(((uint32_t)EEPROM_Read(EEADDR_DRV1_MAXSPEED_H)) << 16 ) | EEPROM_Read(EEADDR_DRV1_MAXSPEED_L);

	serialNumber = EEPROM_Read(EEADDR_SERIAL);

	Commutator.encoderPolarity = EEPROM_Read(EEADDR_ENC_POLARITY);
	Commutator.advanceCoeff = EEPROM_Read(EEADDR_COMM_ADV_COEFF);
	Motor.positionCurrentcascade = EEPROM_Read(EEADDR_MOT_PID_CASC);
	Commutator.encoderResolution = EEPROM_Read(EEADDR_ENC_RESOLUTION);
	Commutator.commutationMode = EEPROM_Read(EEADDR_COMM_MODE);

	Commutator.motorWireRemap1 = EEPROM_Read(EEADDR_MOTOR_REMAP1);
	Commutator.motorWireRemap2 = EEPROM_Read(EEADDR_MOTOR_REMAP2);
	Commutator.motorWireRemap3 = EEPROM_Read(EEADDR_MOTOR_REMAP3);

	Motor.maxPWM = EEPROM_Read(EEADDR_MOTOR_MAXPWM);

	Motor.switchPolarityHome = EEPROM_Read(EEADDR_SW_POL_HOME);
	Motor.switchPolarityUp = EEPROM_Read(EEADDR_SW_POL_UP);
	Motor.switchPolarityDown = EEPROM_Read(EEADDR_SW_POL_DOWN);

	Motor.switchPullupHome = EEPROM_Read(EEADDR_SW_PULLUP_HOME);
	Motor.switchPullupUp = EEPROM_Read(EEADDR_SW_PULLUP_UP);
	Motor.switchPullupDown = EEPROM_Read(EEADDR_SW_PULLUP_DOWN);

	Motor.synchroIncrement = EEPROM_Read(EEADDR_SYNCHRO_SPEED);

	Motor.speedFFCoefA = EEPROM_Read(EEADDR_SPEED_FF_COEF_A);
	Motor.speedFFCoefB = EEPROM_Read(EEADDR_SPEED_FF_COEF_B);

	ADC.currentMeasure_mVOffset = EEPROM_Read(EEADDR_ADC_CURR_MVOFFS);
}

void eebackup_SaveAll(void)
{
	EEPROM_Write(EEADDR_PID1_P, (u16) PID[0].P_Factor);
	EEPROM_Write(EEADDR_PID1_I, (u16) PID[0].I_Factor);
	EEPROM_Write(EEADDR_PID1_D, (u16) PID[0].D_Factor);
	EEPROM_Write(EEADDR_PID2_P, (u16) PID[1].P_Factor);
	EEPROM_Write(EEADDR_PID2_I, (u16) PID[1].I_Factor);
	EEPROM_Write(EEADDR_PID2_D, (u16) PID[1].D_Factor);
	EEPROM_Write(EEADDR_PID3_P, (u16) PID[2].P_Factor);
	EEPROM_Write(EEADDR_PID3_I, (u16) PID[2].I_Factor);
	EEPROM_Write(EEADDR_PID3_D, (u16) PID[2].D_Factor);

	EEPROM_Write(EEADDR_DRV1_MIN_L, (u16) (NFComBuf.SetDrivesMinPosition.data[0] & 0x0000ffff));
	EEPROM_Write(EEADDR_DRV1_MIN_H, (u16) ((NFComBuf.SetDrivesMinPosition.data[0] >> 16) & 0x0000ffff));
	EEPROM_Write(EEADDR_DRV1_MAX_L, (u16) (NFComBuf.SetDrivesMaxPosition.data[0] & 0x0000ffff));
	EEPROM_Write(EEADDR_DRV1_MAX_H, (u16) ((NFComBuf.SetDrivesMaxPosition.data[0] >> 16) & 0x0000ffff));

	EEPROM_Write(EEADDR_DRV1_MAXSPEED_L, (u16) (NFComBuf.SetDrivesMaxSpeed.data[0] & 0x0000ffff));
	EEPROM_Write(EEADDR_DRV1_MAXSPEED_H, (u16) ((NFComBuf.SetDrivesMaxSpeed.data[0] >> 16) & 0x0000ffff));

	EEPROM_Write(EEADDR_ENC_POLARITY, (u16) Commutator.encoderPolarity);
	EEPROM_Write(EEADDR_COMM_ADV_COEFF, (u16) Commutator.advanceCoeff);
	EEPROM_Write(EEADDR_MOT_PID_CASC, (u16) Motor.positionCurrentcascade);
	EEPROM_Write(EEADDR_ENC_RESOLUTION, (u16) Commutator.encoderResolution);
	EEPROM_Write(EEADDR_COMM_MODE, (u16) Commutator.commutationMode);

	EEPROM_Write(EEADDR_MOTOR_REMAP1, (u16) Commutator.motorWireRemap1);
	EEPROM_Write(EEADDR_MOTOR_REMAP2, (u16) Commutator.motorWireRemap2);
	EEPROM_Write(EEADDR_MOTOR_REMAP3, (u16) Commutator.motorWireRemap3);

	EEPROM_Write(EEADDR_MOTOR_MAXPWM, (u16) Motor.maxPWM);

	EEPROM_Write(EEADDR_SW_POL_HOME, (u16) Motor.switchPolarityHome);
	EEPROM_Write(EEADDR_SW_POL_UP, (u16) Motor.switchPolarityUp);
	EEPROM_Write(EEADDR_SW_POL_DOWN, (u16) Motor.switchPolarityDown);

	EEPROM_Write(EEADDR_SW_PULLUP_HOME, (u16) Motor.switchPullupHome);
	EEPROM_Write(EEADDR_SW_PULLUP_UP, (u16) Motor.switchPullupUp);
	EEPROM_Write(EEADDR_SW_PULLUP_DOWN, (u16) Motor.switchPullupDown);

	EEPROM_Write(EEADDR_SYNCHRO_SPEED, (u16) Motor.synchroIncrement);

	EEPROM_Write(EEADDR_SPEED_FF_COEF_A, (u16) Motor.speedFFCoefA);
	EEPROM_Write(EEADDR_SPEED_FF_COEF_B, (u16) Motor.speedFFCoefB);

	EEPROM_Write(EEADDR_ADC_CURR_MVOFFS, (u16) ADC.currentMeasure_mVOffset);
}

void eebackup_SaveInitialValues(void)
{
}

