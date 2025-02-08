/**
  ****************************(C) COPYRIGHT 2022 DJI****************************
  * @file       can_receive.c/h
  * @brief      there is CAN interrupt function  to receive motor data,
  *             and CAN send function to send motor current to control motor.
  *             ������CAN�жϽ��պ��������յ������,CAN���ͺ������͵���������Ƶ��.
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0          RM              1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2022 DJI****************************
  */

#include "CAN_receive.h"
#include "main.h"


extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

uint8_t can1_rx_data[8];
uint8_t can2_rx_data[8];
CAN_RxHeaderTypeDef hcan1RxFrame;
CAN_RxHeaderTypeDef hcan2RxFrame;

CAN_TxFrameTypeDef hcan1TxFrame;
CAN_TxFrameTypeDef hcan2TxFrame;


__WEAK void CAN1_rxDataHandler(uint32_t canId, uint8_t *rxBuf);
__WEAK void CAN2_rxDataHandler(uint32_t canId, uint8_t *rxBuf);

Board_info_t Board_info;
motor_info_t motor_info;
Board_Order_info_t Board_Order_info[5];
Big_Fu_info_t Big_Fu_info[5];

void get_DAFU_measure(Big_Fu_info_t *ptr)                                    
{                                                                   
	ptr->Color = can1_rx_data[0];																
	ptr->Last_Color = can1_rx_data[1];  
	ptr->Hit_LED_state =  can1_rx_data[2];  
	ptr->Single_Hit_State =  can1_rx_data[3];                
	ptr->Single_Working_State = can1_rx_data[4];      
 	ptr->All_Hit_State = can1_rx_data[5];    
	ptr->Spin_State = can1_rx_data[6];    
	ptr->Twinkle_Already= can1_rx_data[7]; 

}


void BOARD_CAN1_RX(uint32_t canId, uint8_t *rxBuf)
{
	Board_info.offline_cnt_max = OFFLINE_TIME_MAX;
	
	switch (canId)
	{
			case CAN_BOARD_ID_1:
			{
				get_DAFU_measure(&Big_Fu_info[0]);
				Board_info.B1_offline_cnt = 0;
				break;
			}
			case CAN_BOARD_ID_2:	
			{
				get_DAFU_measure(&Big_Fu_info[1]);
				Board_info.B2_offline_cnt = 0;
				break;
			}				
			case CAN_BOARD_ID_3:
			{
				get_DAFU_measure(&Big_Fu_info[2]);
				Board_info.B3_offline_cnt = 0;
				break;
			}
			case CAN_BOARD_ID_4:		
			{
				get_DAFU_measure(&Big_Fu_info[3]);
				Board_info.B4_offline_cnt = 0;
				break;
			}
			case CAN_BOARD_ID_5:
			{ 
				get_DAFU_measure(&Big_Fu_info[4]);
				Board_info.B5_offline_cnt = 0;

				break;
			}			
			
			default:
			{
				break;
			}	
	
		}
}

/**
 *	@brief	CAN �����жϻص�����
 */
static void CAN_Rx_Callback(CAN_HandleTypeDef *hcan)
{
	if(hcan->Instance == CAN1)
	{
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &hcan1RxFrame, can1_rx_data);
		
		CAN1_rxDataHandler(hcan1RxFrame.StdId, can1_rx_data);
	}
	else if(hcan->Instance == CAN2)
	{
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &hcan2RxFrame, can2_rx_data);
		
		CAN2_rxDataHandler(hcan2RxFrame.StdId, can2_rx_data);
	}
}


/**
  * @brief          hal CAN fifo call back, receive motor data
  * @param[in]      hcan, the point to CAN handle
  * @retval         none
  */
/**
  * @brief          hal��CAN�ص�����,���յ������
  * @param[in]      hcan:CAN���ָ��
  * @retval         none
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    
	
	  if(hcan->Instance == CAN1)
		{		
			 CAN_Rx_Callback(hcan);
		}
		else if(hcan->Instance == CAN2)
		{						
       CAN_Rx_Callback(hcan);			
		}
		
}		



/* Private functions ---------------------------------------------------------*/
/**
 *	@brief	CAN ��ʶ����������λ��Ĭ������
 */
static void CAN_Filter_ParamsInit(CAN_FilterTypeDef *sFilterConfig)
{
	sFilterConfig->FilterIdHigh = 0;						
	sFilterConfig->FilterIdLow = 0;							
	sFilterConfig->FilterMaskIdHigh = 0;					// ������
	sFilterConfig->FilterMaskIdLow = 0;						// ������
	sFilterConfig->FilterFIFOAssignment = CAN_FILTER_FIFO0;	// ������������FIFO0
	sFilterConfig->FilterBank = 0;							// ���ù�����0
	sFilterConfig->FilterMode = CAN_FILTERMODE_IDMASK;		// ��ʶ������ģʽ
	sFilterConfig->FilterScale = CAN_FILTERSCALE_32BIT;		// 32λ��
	sFilterConfig->FilterActivation = ENABLE;				// �����˲���
	sFilterConfig->SlaveStartFilterBank = 0;
}

/* Exported functions --------------------------------------------------------*/
/**
 *	@brief	CAN1 ��ʼ��
 */
void CAN1_Init(void)
{
	CAN_FilterTypeDef sFilterConfig;
	
	// ����CAN��ʶ���˲���
	CAN_Filter_ParamsInit(&sFilterConfig);
	HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);
	// ʹ�ܽ����ж�
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
	
	// ����CAN1
	HAL_CAN_Start(&hcan1);
}

/**
 *	@brief	CAN2 ��ʼ��
 */
void CAN2_Init(void)
{
	CAN_FilterTypeDef sFilterConfig;
	
	// ����CAN��ʶ���˲���
	CAN_Filter_ParamsInit(&sFilterConfig);               
	HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig);
	
	// ʹ�ܽ����ж�
	HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
	
	// ����CAN2
	HAL_CAN_Start(&hcan2);
}



/* rxData Handler [Weak] functions -------------------------------------------*/
/**
 *	@brief	[__WEAK] ��Ҫ��  ��ʵ�־���� CAN1 ���ݴ���Э�� 
 */
__WEAK void CAN1_rxDataHandler(uint32_t canId, uint8_t *rxBuf)
{
}

/**
 *	@brief	[__WEAK] ��Ҫ��  ��ʵ�־���� CAN2 ���ݴ���Э��
 */
__WEAK void CAN2_rxDataHandler(uint32_t canId, uint8_t *rxBuf)
{
}


