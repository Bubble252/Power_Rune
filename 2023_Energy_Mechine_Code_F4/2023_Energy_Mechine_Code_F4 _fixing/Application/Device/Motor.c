/********************************************************************************
  * @file		Motor.c
	* @author    RobotPilots@2022
  * @version		V1.0.0
  * @date			
  * @brief   		
  *******************************************************************************/

#include "Motor.h"

pid_t pid_spd[2];

#define ABS(x)		((x>0)? (x): (-x)) 


motor_measure_t motor_measure[2];
static CAN_TxHeaderTypeDef  motor_tx_message;
static uint8_t              motor_can_send_data[8];

void abs_limit(float *a, float ABS_MAX)
{
    if(*a > ABS_MAX)
        *a = ABS_MAX;
    if(*a < -ABS_MAX)
        *a = -ABS_MAX;
 }

void PID_struct_init(pid_t* pid, uint32_t maxout, uint32_t intergral_limit,
    float 	kp, 
    float 	ki, 
    float 	kd)
{
	  pid->IntegralLimit = intergral_limit;
    pid->MaxOutput = maxout;
    pid->p = kp;
    pid->i = ki;
    pid->d = kd;

}
void get_motor_measure(motor_measure_t *ptr)                                    
{                                                                   
	ptr->last_angle = ptr->angle;																
	ptr->angle = (uint16_t)(can2_rx_data[0]<<8 | can2_rx_data[1]);           
	ptr->real_current = (int16_t)(can2_rx_data[4]<<8 |can2_rx_data[5]);     
	ptr->speed_rpm = (int16_t)(can2_rx_data[2]<<8 |can2_rx_data[3]);                          
	ptr->hall = can2_rx_data[6];      
 	
	if(ptr->angle - ptr->last_angle > 4096)                         
	ptr->round_cnt --;                                            
	else if (ptr->angle - ptr->last_angle < -4096)                   
	ptr->round_cnt ++;  
  ptr->total_angle = ptr->round_cnt * 8192 + ptr->angle - ptr->offset_angle;	
	
	

}





void MOTOR_CAN2_RX(uint32_t canId, uint8_t *rxBuf)
{
	motor_info.offline_cnt_max = OFFLINE_TIME_MAX;
	switch (canId)
			{
					case CAN_3508_ID:
					{
						get_motor_measure(&motor_measure[0]);
						motor_info.offline_cnt = 0;
						break;
					}
				
					default:
					{
						break;
					}						
			}
}



void CAN_cmd_motor(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
    uint32_t send_mail_box ;
    motor_tx_message.StdId = CAN_MOTOR_ALL_ID;
    motor_tx_message.IDE = CAN_ID_STD;
    motor_tx_message.RTR = CAN_RTR_DATA;
    motor_tx_message.DLC = 0x08;
    motor_can_send_data[0] = motor1 >> 8;
    motor_can_send_data[1] = motor1;
    motor_can_send_data[2] = motor2 >> 8;
    motor_can_send_data[3] = motor2;
    motor_can_send_data[4] = motor3 >> 8;
    motor_can_send_data[5] = motor3;
    motor_can_send_data[6] = motor4 >> 8;
    motor_can_send_data[7] = motor4;

    HAL_CAN_AddTxMessage(&MOTOR_CAN, &motor_tx_message, motor_can_send_data, &send_mail_box);
}


/*
	µ×ÅÌPID£¨ËÙ¶È»·£©
*/
void pid_calc(pid_t* pid, float measure, float target)
{

	  pid->measure = measure; //measure
    pid->target = target;
    pid->err[2] = target - measure;	//target- measure
	
	  pid->pout = pid->p * pid->err[2];  //kp*err
		pid->iout += pid->i * pid->err[2];  //ki*err
		abs_limit(&(pid->iout), pid->IntegralLimit);
		pid->pos_out = pid->pout + pid->iout + pid->dout ;
		abs_limit(&(pid->pos_out), pid->MaxOutput);
		pid->last_pos_out = pid->pos_out;	//update last time 
}
   

