/*
* External config file.
* Uncomment and update buffers' size [no of param set instances] for commands
* to be supported by module:
*/

#define NF_BroadcastAddress 0xff
#define NF_MasterAddress	0xfe
#define NF_AddressBase		0x01  
#define NF_AddressLCDModule	0x10

/*
* Uncomment and update buffers' size [no of param set instances] for commands
* to be supported by module:
*/
	#define NF_BUFSZ_ReadDeviceStatus		1
	#define NF_BUFSZ_ReadDeviceVitals		1

	#define NF_BUFSZ_SetDrivesMode			4
//	#define NF_BUFSZ_SetDrivesSpeed			4
//	#define NF_BUFSZ_SetDrivesCurrent		4
	#define NF_BUFSZ_SetDrivesPosition		4
	#define NF_BUFSZ_SetDrivesPWM			4
//	#define NF_BUFSZ_SetDrivesMaxCurrent	4
//	#define NF_BUFSZ_SetDrivesMaxSpeed		4
	#define NF_BUFSZ_SetDrivesMinPosition	4
	#define NF_BUFSZ_SetDrivesMaxPosition	4
//	#define NF_BUFSZ_ReadDrivesPosition		4
//	#define NF_BUFSZ_ReadDrivesCurrent		4
//	#define NF_BUFSZ_SetDrivesMisc			4
	#define NF_BUFSZ_ReadDrivesStatus		4
	
//	#define NF_BUFSZ_SetCurrentRegulator	3
//	#define NF_BUFSZ_SetSpeedRegulator		3
	#define NF_BUFSZ_SetPositionRegulator	3

	#define NF_BUFSZ_SetServosMode			28
	#define NF_BUFSZ_SetServosPosition		28
	#define NF_BUFSZ_SetServosSpeed			28
	#define NF_BUFSZ_SetServosMaxSpeed		28
	#define NF_BUFSZ_SetServosMinPosition	28
	#define NF_BUFSZ_SetServosMaxPosition	28
	#define NF_BUFSZ_SetServosPositionAdj	28
	#define NF_BUFSZ_ReadServosPosition		28
	#define NF_BUFSZ_ReadServosCurrent		28
	#define NF_BUFSZ_SetServosMisc			28
	#define NF_BUFSZ_ReadServosStatus		28
											
	#define NF_BUFSZ_SetDigitalOutputs		8
	#define NF_BUFSZ_ReadDigitalInputs		10
	#define NF_BUFSZ_ReadAnalogInputs		10
	
//	#define NF_BUFSZ_ReadDistance			8  
	
	#define NF_BUFSZ_SetDisplayMode			1
	#define NF_BUFSZ_SetDisplayText			33
	#define NF_BUFSZ_SetDisplayBacklight	1

