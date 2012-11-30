#include "nfv2.h"
#include "pid.h"
#include "stdio.h"
		 
extern PID_St			PID[];

void NFv2_Config(NF_STRUCT_ComBuf *NFComBuf, uint8_t myAddress){

	NFComBuf->myAddress = myAddress;

	NFComBuf->SetCurrentRegulator.data[0].p = PID[0].P_Factor;
	NFComBuf->SetCurrentRegulator.data[0].i = PID[0].I_Factor;
	NFComBuf->SetCurrentRegulator.data[0].d = PID[0].D_Factor;
	NFComBuf->SetSpeedRegulator.data[0].p = PID[0].P_Factor;
	NFComBuf->SetSpeedRegulator.data[0].i = PID[0].I_Factor;
	NFComBuf->SetSpeedRegulator.data[0].d = PID[0].D_Factor;
	NFComBuf->SetPositionRegulator.data[0].p = PID[0].P_Factor;
	NFComBuf->SetPositionRegulator.data[0].i = PID[0].I_Factor;
	NFComBuf->SetPositionRegulator.data[0].d = PID[0].D_Factor;

	NFComBuf->dataReceived = 1;

	// Address map is essential only when device acts as Master on NFv2 bus.

	NFComBuf->ReadDeviceStatus.addr[1] = NF_AddressBase + 1;
	NFComBuf->ReadDeviceVitals.addr[1] = NF_AddressBase + 1;
	NFComBuf->SetDrivesMode.addr[1] = NF_AddressBase + 1;
	NFComBuf->SetDrivesSpeed.addr[1] = NF_AddressBase + 1;
	NFComBuf->SetDrivesPosition.addr[1] = NF_AddressBase + 1;
	NFComBuf->SetDrivesPWM.addr[1] = NF_AddressBase + 1;
	NFComBuf->SetDrivesMaxCurrent.addr[1] = NF_AddressBase + 1;
	NFComBuf->SetDrivesMaxSpeed.addr[1] = NF_AddressBase + 1;
	NFComBuf->SetDrivesMinPosition.addr[1] = NF_AddressBase + 1;
	NFComBuf->SetDrivesMaxPosition.addr[1] = NF_AddressBase + 1;
	NFComBuf->ReadDrivesPosition.addr[1] = NF_AddressBase + 1;
	NFComBuf->ReadDrivesCurrent.addr[1] = NF_AddressBase + 1;
	NFComBuf->SetDrivesMisc.addr[1] = NF_AddressBase + 1;
	NFComBuf->ReadDrivesStatus.addr[1] = NF_AddressBase + 1;
	NFComBuf->SetCurrentRegulator.addr[1] = NF_AddressBase + 1;
	NFComBuf->SetSpeedRegulator.addr[1] = NF_AddressBase + 1;
	NFComBuf->SetPositionRegulator.addr[1] = NF_AddressBase + 1;
	NFComBuf->SetDigitalOutputs.addr[1] = NF_AddressBase + 1;
	NFComBuf->ReadDigitalInputs.addr[1] = NF_AddressBase + 1;
	NFComBuf->ReadAnalogInputs.addr[9] = NF_AddressBase + 1;
	NFComBuf->ReadAnalogInputs.addr[10] = NF_AddressBase + 1;
	NFComBuf->ReadAnalogInputs.addr[11] = NF_AddressBase + 1;
	NFComBuf->ReadAnalogInputs.addr[12] = NF_AddressBase + 1;
	NFComBuf->ReadAnalogInputs.addr[13] = NF_AddressBase + 1;
	NFComBuf->ReadAnalogInputs.addr[14] = NF_AddressBase + 1;
	NFComBuf->ReadAnalogInputs.addr[15] = NF_AddressBase + 1;
	NFComBuf->ReadAnalogInputs.addr[16] = NF_AddressBase + 1;
	NFComBuf->ReadAnalogInputs.addr[17] = NF_AddressBase + 1;

}
