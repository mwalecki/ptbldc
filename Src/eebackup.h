#ifndef EEBACKUP_H
#define EEBACKUP_H

#include "common.h"
#include "eeprom.h"

#define EEADDR_SRV0MIN	1
#define EEADDR_SRV1MIN	2
#define EEADDR_SRV2MIN	3
#define EEADDR_SRV3MIN	4
#define EEADDR_SRV4MIN	5
#define EEADDR_SRV5MIN	6
#define EEADDR_SRV6MIN	7
#define EEADDR_SRV7MIN	8

#define EEADDR_SRV0MAX	9
#define EEADDR_SRV1MAX	10
#define EEADDR_SRV2MAX	11
#define EEADDR_SRV3MAX	12
#define EEADDR_SRV4MAX	13
#define EEADDR_SRV5MAX	14
#define EEADDR_SRV6MAX	15
#define EEADDR_SRV7MAX	16

#define EEADDR_SRV0ADJ	17
#define EEADDR_SRV1ADJ	18
#define EEADDR_SRV2ADJ	19
#define EEADDR_SRV3ADJ	20
#define EEADDR_SRV4ADJ	21
#define EEADDR_SRV5ADJ	22
#define EEADDR_SRV6ADJ	23
#define EEADDR_SRV7ADJ	24
						  
#define EEADDR_SERIAL	25

#define EEADDR_PID1_P	26
#define EEADDR_PID1_I	27
#define EEADDR_PID1_D	28
#define EEADDR_PID2_P	29
#define EEADDR_PID2_I	30
#define EEADDR_PID2_D	31
#define EEADDR_PID3_P	32
#define EEADDR_PID3_I	33
#define EEADDR_PID3_D	34
#define EEADDR_PID4_P	35
#define EEADDR_PID4_I	36
#define EEADDR_PID4_D	37

#define EEADDR_DRV1_MIN_L 38
#define EEADDR_DRV1_MIN_H 39
#define EEADDR_DRV1_MAX_L 40
#define EEADDR_DRV1_MAX_H 41

#define EEADDR_DRV1_MAXSPEED_L 42
#define EEADDR_DRV1_MAXSPEED_H 43

#define EEADDR_ENC_POLARITY 44
#define EEADDR_COMM_ADV_COEFF 45
#define EEADDR_ENC_RESOLUTION 46

#define EEADDR_MOT_PID_CASC 47
#define EEADDR_COMM_MODE 48

#define EEADDR_MOTOR_REMAP1 49
#define EEADDR_MOTOR_REMAP2 50
#define EEADDR_MOTOR_REMAP3 51

#define EEADDR_MOTOR_MAXPWM 52

#define EEADDR_SW_POL_HOME 53
#define EEADDR_SW_POL_UP 54
#define EEADDR_SW_POL_DOWN 55
#define EEADDR_SW_PULLUP_HOME 56
#define EEADDR_SW_PULLUP_UP 57
#define EEADDR_SW_PULLUP_DOWN 58

#define EEADDR_SYNCHRO_SPEED 59

#define EEADDR_SPEED_FF_COEF_A 60
#define EEADDR_SPEED_FF_COEF_B 61

void eebackup_Recover(void);	
void eebackup_SaveAll(void);
void eebackup_SaveInitialValues(void);

#endif //EEBACKUP_H
