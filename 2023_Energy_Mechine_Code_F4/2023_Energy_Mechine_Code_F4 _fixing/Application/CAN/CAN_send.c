#include "CAN_send.h"


/*
   ����Can���ͺ���
*/

uint8_t Board_Tx(uint32_t std, uint8_t *data,char hcan,uint32_t DL)
{
	uint8_t i;
	CAN_TxFrameTypeDef TxMes;
	uint32_t CAN_Tx_Mail_Box;

	TxMes.header.IDE = CAN_ID_STD;
	TxMes.header.RTR = CAN_RTR_DATA;
	TxMes.header.DLC = DL;    //
	TxMes.header.StdId = std; 	  //
	
	TxMes.data[0] = data[0];
	TxMes.data[1] = data[1];	
	TxMes.data[2] = data[2];	
	TxMes.data[3] = data[3];

	TxMes.data[4] = data[4];
	TxMes.data[5] = data[5];	
	TxMes.data[6] = data[6];	
	TxMes.data[7] = data[7];
	
	if(hcan == 1)i = HAL_CAN_AddTxMessage(&hcan1,&TxMes.header,TxMes.data,  &CAN_Tx_Mail_Box);
	
	return i;
}

