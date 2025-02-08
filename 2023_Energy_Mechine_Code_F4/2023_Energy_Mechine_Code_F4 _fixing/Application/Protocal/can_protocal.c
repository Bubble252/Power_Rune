/**
 * @file        can_potocal.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        2022
 * @brief       CAN Potocal.
 */
 
/* Includes ------------------------------------------------------------------*/
#include "can_protocal.h"
#include "main.h"
#include "string.h"
uint8_t Can_Board_Order_data[8];
extern Board_Order_info_t Board_Order_info[5];

/*************** 信息接收处理 ***************/
/**
 *	@brief	CAN1 接收数据 数据处理
 */

void CAN1_rxDataHandler(uint32_t canId, uint8_t *rxBuf)
{	
	BOARD_CAN1_RX(canId,rxBuf);
}
/**
 *	@brief	CAN2 接收数据
 */
void CAN2_rxDataHandler(uint32_t canId, uint8_t *rxBuf)
{

	MOTOR_CAN2_RX(canId, rxBuf);	
}
/************** 信息发送处理 ***************/
/**
 *	@brief	CAN1 发送数据 数据处理
 */
uint16_t i = 0;
void Board_Info_Tx(void)
{
	
	i++;
	if(i%5 == 0)
	{
		memcpy(Can_Board_Order_data, &Board_Order_info[0], 8);
		Board_Tx(CAN_BOARD_ID_1_F,Can_Board_Order_data,1,0x08);
			
	}
	if(i%5 == 1)
	{
		memcpy(Can_Board_Order_data, &Board_Order_info[1], 8);
		Board_Tx(CAN_BOARD_ID_2_F,Can_Board_Order_data,1,0x08);
	}
	if(i%5 == 2)
	{
		memcpy(Can_Board_Order_data, &Board_Order_info[2], 8);
		Board_Tx(CAN_BOARD_ID_3_F,Can_Board_Order_data,1,0x08);
	}
	if(i%5 == 3)
	{
		memcpy(Can_Board_Order_data, &Board_Order_info[3], 8);
		Board_Tx(CAN_BOARD_ID_4_F,Can_Board_Order_data,1,0x08);
		
	}
	if(i%5 == 4)
	{
		memcpy(Can_Board_Order_data, &Board_Order_info[4], 8);
		Board_Tx(CAN_BOARD_ID_5_F,Can_Board_Order_data,1,0x08);
	}
	
	if(i>=30000)
	{
		i = 0;
	}
}
