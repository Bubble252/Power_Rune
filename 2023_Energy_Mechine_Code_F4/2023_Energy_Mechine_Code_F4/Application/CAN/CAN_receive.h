/**
  ****************************(C) COPYRIGHT 2019 DJI****************************
  * @file       can_receive.c/h
  * @brief      there is CAN interrupt function  to receive motor data,
  *             and CAN send function to send motor current to control motor.
  *             这里是CAN中断接收函数，接收电机数据,CAN发送函数发送电机电流控制电机.
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2019 DJI****************************
  */

#ifndef CAN_RECEIVE_H
#define CAN_RECEIVE_H

#include "main.h"
#include "rc.h"


#define BOARD_CAN hcan1
#define MOTOR_CAN hcan2

#define OFFLINE_TIME_MAX   25



/* CAN send and receive ID */
typedef enum
{
    CAN_MOTOR_ALL_ID = 0x200,
    CAN_3508_ID      = 0x203,
	  CAN_SHOOT        = 0x207,


    CAN_BOARD_ID_1 = 0x101,
    CAN_BOARD_ID_2 = 0x102,
    CAN_BOARD_ID_3 = 0x103,
    CAN_BOARD_ID_4 = 0x104,
	  CAN_BOARD_ID_5 = 0x105,
	
	  CAN_BOARD_ID_1_F = 0x301,
    CAN_BOARD_ID_2_F = 0x302,
    CAN_BOARD_ID_3_F = 0x303,
    CAN_BOARD_ID_4_F = 0x304,
	  CAN_BOARD_ID_5_F = 0x305,

} can_msg_id_e;



typedef struct 
{
	uint8_t     offline_cnt_max;
	uint8_t     offline_cnt;
	uint8_t     status;
}motor_info_t;

typedef struct 
{
	uint8_t     B1_offline_cnt;
	uint8_t     B2_offline_cnt;
	uint8_t     B3_offline_cnt;
	uint8_t     B4_offline_cnt;
	uint8_t     B5_offline_cnt;
	uint8_t     offline_cnt_max;
	uint8_t     status;
}Board_info_t;

/*发送给F1的数据*/
typedef struct 
{
	uint8_t    Twinkle_state;
	uint8_t    All_Led_Close;
	uint8_t    All_Led_Open;
	uint8_t    Board_Work;
	uint8_t    Set_Color;
	uint8_t    start_permission;
	uint8_t    RainBow_Flag;
	
}Board_Order_info_t;         //发送给F1

/*发送给F4的数据*/
typedef struct
{   
	  uint8_t  	Color;                 //大符颜色
	  uint8_t  	Last_Color;            //大符颜色
	  uint8_t   Hit_LED_state;         //(击中/未击中)LED状态
	  uint8_t   Single_Hit_State;      //单个扇叶（击中/未击中）状态
	  uint8_t   Single_Working_State;  //单个扇叶待击打状态
	  uint8_t   All_Hit_State;         //全部扇叶（击中/未击中）状态
	  uint8_t   Spin_State;           //大符（旋转/未旋转）状态
	  uint8_t   Twinkle_Already;
}Big_Fu_info_t;


typedef struct {
	CAN_TxHeaderTypeDef header;
	uint8_t				data[8];
} CAN_TxFrameTypeDef;


void CAN1_Init(void);
void CAN2_Init(void);

void CAN_cmd_motor(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
/* Extern  ------------------------------------------------------------------*/

extern motor_info_t motor_info;
extern Board_info_t Board_info;
extern Board_Order_info_t Board_Order_info[5];
extern Big_Fu_info_t Big_Fu_info[5];


extern uint8_t can1_rx_data[8];
extern uint8_t can2_rx_data[8];
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

#endif

