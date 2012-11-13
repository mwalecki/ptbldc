#include "nfv2.h"
#include "pid.h"
#include "stdio.h"
		 
extern PID_St			PID[];

void NFv2_Config(NF_STRUCT_ComBuf *NFComBuf, uint8_t myAddress){

	NFComBuf->myAddress = myAddress;

	NFComBuf->SetPositionRegulator.data[0].p = PID[0].P_Factor;
	NFComBuf->SetPositionRegulator.data[0].i = PID[0].I_Factor;
	NFComBuf->SetPositionRegulator.data[0].d = PID[0].D_Factor;
	NFComBuf->SetPositionRegulator.data[1].p = PID[1].P_Factor;
	NFComBuf->SetPositionRegulator.data[1].i = PID[1].I_Factor;
	NFComBuf->SetPositionRegulator.data[1].d = PID[1].D_Factor;
	NFComBuf->SetPositionRegulator.data[2].p = PID[2].P_Factor;
	NFComBuf->SetPositionRegulator.data[2].i = PID[2].I_Factor;
	NFComBuf->SetPositionRegulator.data[2].d = PID[2].D_Factor;

	// Address map is essential only when device acts as Master on NFv2 bus.

	NFComBuf->dataReceived = 1;

	NFComBuf->ReadAnalogInputs.addr[0] = NF_AddressBase;
	NFComBuf->ReadAnalogInputs.addr[1] = NF_AddressBase;
/*	NFComBuf->ReadAnalogInputs.addr[2] = NF_AddressBase;
	NFComBuf->ReadAnalogInputs.addr[3] = NF_AddressBase;
	NFComBuf->ReadAnalogInputs.addr[4] = NF_AddressBase;
	NFComBuf->ReadAnalogInputs.addr[5] = NF_AddressBase;
	NFComBuf->ReadAnalogInputs.addr[6] = NF_AddressBase;
	NFComBuf->ReadAnalogInputs.addr[7] = NF_AddressBase;
	NFComBuf->ReadAnalogInputs.addr[8] = NF_AddressBase;
	NFComBuf->ReadAnalogInputs.addr[9] = NF_AddressBase;
	
	NFComBuf->SetDigitalOutputs.addr[0] = NF_AddressBase;
	NFComBuf->SetDigitalOutputs.addr[1] = NF_AddressBase;
	NFComBuf->SetDigitalOutputs.addr[2] = NF_AddressBase;
	NFComBuf->SetDigitalOutputs.addr[3] = NF_AddressBase;
	NFComBuf->SetDigitalOutputs.addr[4] = NF_AddressBase;
	NFComBuf->SetDigitalOutputs.addr[5] = NF_AddressBase;
	NFComBuf->SetDigitalOutputs.addr[6] = NF_AddressBase;
	NFComBuf->SetDigitalOutputs.addr[7] = NF_AddressBase;   */

}
