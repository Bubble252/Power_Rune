#ifndef  _CAN_SEND_H
#define  _CAN_SEND_H

#include "main.h"
#include "CAN_receive.h"

typedef struct {
	CAN_TxHeaderTypeDef header;
	uint8_t				data[8];
} CAN_TxFrameTypeDef;

/*���͸�F4������*/
typedef struct
{   
	  uint8_t  	Color;                 //�����ɫ
	  uint8_t  	Last_Color;            //�����ɫ
	  uint8_t   Hit_LED_state;         //(����/δ����)LED״̬
	  uint8_t   Single_Hit_State;      //������Ҷ������/δ���У�״̬
	  uint8_t   Single_Working_State;  //������Ҷ������״̬
	  uint8_t   All_Hit_State;         //ȫ����Ҷ������/δ���У�״̬
	  uint8_t   Spin_State;            //�������ת/δ��ת��״̬
	  uint8_t   Twinkle_Already;
}Big_Fu_info_t;

extern Big_Fu_info_t Big_Fu_info[5];

uint8_t Board_Tx(uint32_t std, uint8_t *data,char can,uint32_t DL);




#endif



