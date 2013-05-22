#include "stm32f10x.h"
#include "nf/nfv2.h"

extern NF_STRUCT_ComBuf 	NFComBuf;

uint16_t devid;

#define GPIO_Pin_CAN_RX GPIO_Pin_8
#define GPIO_Pin_CAN_TX GPIO_Pin_9
#define GPIO_CAN GPIOB
#define GPIO_Remapping_CAN GPIO_Remap1_CAN1

void CAN_Config() {
	GPIO_InitTypeDef  GPIO_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	/* GPIO clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	/* CANx Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	/* Configure CAN pin: RX */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_CAN_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIO_CAN, &GPIO_InitStructure);

	/* Configure CAN pin: TX */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_CAN_TX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_CAN, &GPIO_InitStructure);

	GPIO_PinRemapConfig(GPIO_Remapping_CAN , ENABLE);

	/* CAN register init */
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);

	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;

	/* CAN Baudrate = 1MBps*/
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_8tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
	CAN_InitStructure.CAN_Prescaler = 3;
	CAN_Init(CAN1, &CAN_InitStructure);

	devid = NFComBuf.myAddress - 16;
	/* CAN filter init */

	// filter single receiver messages
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = (0 | devid) << 5;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (16 | 15) << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	// filter dual receiver messages
	CAN_FilterInitStructure.CAN_FilterNumber = 1;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = (16 | (devid/2)) << 5;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (16 | 15) << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	// filter quad receiver messages
	CAN_FilterInitStructure.CAN_FilterNumber = 2;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = (24 | (devid/4)) << 5;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (16 | 15) << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

CanRxMsg RxMessage;


void CAN_Istr(void)
{
  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
  CAN_Process(&RxMessage);
}

#define MSG_MASK (63 << 5)

#define MSG_POS_MSR 48
#define MSG_VEL_MSR 49
#define MSG_POS_AND_VEL_MSR 50

#define MSG_SET_PARAM 53
#define MSG_GET_PARAM 54
#define MSG_GET_PARAM_REP 55

#define MSG_MODE_CMD 56
#define MSG_STATUS 57
#define MSG_SW_VER 58

#define MSG_PWM_CMD 16
#define MSG_TRQ_CMD 17

#define MSG_VEL_CMD 32
#define MSG_POS_CMD 33

void CAN_Process(CanRxMsg *rx) {
	CanTxMsg TxMessage;

	if(rx->RTR == CAN_RTR_REMOTE) {
		if((rx->StdId & MSG_MASK) == (MSG_STATUS << 5)) {
			TxMessage.StdId = (MSG_STATUS << 5) | devid;
			TxMessage.ExtId = 0x01;
			TxMessage.RTR = CAN_RTR_DATA;
			TxMessage.IDE = CAN_ID_STD;
			TxMessage.DLC = 5;

			requestStatus((uint32_t*)&TxMessage.Data[0], (uint8_t*)&TxMessage.Data[4]);

			CAN_Transmit(CAN1, &TxMessage);
		} else if((rx->StdId & MSG_MASK) == (MSG_POS_MSR << 5)) {
			TxMessage.StdId = (MSG_POS_MSR << 5) | devid;
			TxMessage.ExtId = 0x01;
			TxMessage.RTR = CAN_RTR_DATA;
			TxMessage.IDE = CAN_ID_STD;
			TxMessage.DLC = 4;

			requestPosition((int32_t*)&TxMessage.Data[0]);

			CAN_Transmit(CAN1, &TxMessage);
		} else if((rx->StdId & MSG_MASK) == (MSG_VEL_MSR << 5)) {
			TxMessage.StdId = (MSG_VEL_MSR << 5) | devid;
			TxMessage.ExtId = 0x01;
			TxMessage.RTR = CAN_RTR_DATA;
			TxMessage.IDE = CAN_ID_STD;
			TxMessage.DLC = 4;

			requestVelocity((int32_t*)&TxMessage.Data[0]);

			CAN_Transmit(CAN1, &TxMessage);
		} else if((rx->StdId & MSG_MASK) == (MSG_POS_AND_VEL_MSR << 5)) {
			TxMessage.StdId = (MSG_POS_AND_VEL_MSR << 5) | devid;
			TxMessage.ExtId = 0x01;
			TxMessage.RTR = CAN_RTR_DATA;
			TxMessage.IDE = CAN_ID_STD;
			TxMessage.DLC = 8;

			requestPosition((int32_t*)&TxMessage.Data[0]);
			requestVelocity((int32_t*)&TxMessage.Data[4]);

			CAN_Transmit(CAN1, &TxMessage);
		}
	} else {
		if((rx->StdId & MSG_MASK) == (MSG_MODE_CMD << 5)) {
			setMode(rx->Data[0]);
		} else if((rx->StdId & MSG_MASK) == (MSG_PWM_CMD << 5)) {
			setPWM(*(int16_t*)&rx->Data[(devid%2)*2]);
		} else if((rx->StdId & MSG_MASK) == (MSG_TRQ_CMD << 5)) {
			setTrq(*(int16_t*)&rx->Data[(devid%2)*2]);
		} else if((rx->StdId & MSG_MASK) == (MSG_VEL_CMD << 5)) {
			setVel(*(int32_t*)&rx->Data[(devid%2)*4]);
		} else if((rx->StdId & MSG_MASK) == (MSG_POS_CMD << 5)) {
			setPos(*(int32_t*)&rx->Data[(devid%2)*4]);
		}

	}

}



