#include "eebackup.h"
#include "pid.h"
#include "nf/nfv2.h"
	 
extern uint16_t				serialNumber;
extern SERVO_St				Servo;
extern NF_STRUCT_ComBuf 	NFComBuf;
extern PID_St				PID[];

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
}

void eebackup_SaveInitialValues(void)
{
}

