#ifndef __MOTOR_H
#define __MOTOR_H

#include "main.h"
#include "CAN_receive.h"

//rm motor data
typedef struct{   
    int16_t   speed_rpm;
	  int16_t   real_current;
    int16_t   given_current;
	  uint8_t  	hall;
	  int16_t 	angle;				//abs angle range:[0,8191]
	  uint16_t 	last_angle;	  //abs angle range:[0,8191]
	  uint16_t	offset_angle;
	  int32_t		round_cnt;
	  int32_t		total_angle;
	  uint8_t		buf_idx;
	  uint16_t	fited_angle;
	  uint32_t  msg_cnt;
} motor_measure_t;


typedef struct __pid_t
{
    float p;
    float i;
    float d;

    float target;				//目标值
    float measure;			//测量值
    float err[3];				//误差
	   
    float pout;							//p输出
    float iout;							//i输出
    float dout;							//d输出
	  
    float pos_out;						//本次位置式输出
    float last_pos_out;				//上次输出

    float last_delta_out;
    
	  float max_err;
	  float deadband;				

		uint32_t MaxOutput;				//输出限幅
    uint32_t IntegralLimit;		//积分限幅

		

}pid_t;

void PID_struct_init(
    pid_t* pid,
    uint32_t maxout,
    uint32_t intergral_limit,
    float 	kp, 
    float 	ki, 
    float 	kd);
    
void MOTOR_CAN2_RX(uint32_t canId, uint8_t *rxBuf);
void BOARD_CAN1_RX(uint32_t canId, uint8_t *rxBuf);
void BOARD_CAN2_RX(uint32_t canId, uint8_t *rxBuf);

void get_motor_measure(motor_measure_t *ptr);
void pid_calc(pid_t* pid, float fdb, float ref);
void PID_struct_init(pid_t* pid, uint32_t maxout, uint32_t intergral_limit,
    float 	kp, 
    float 	ki, 
    float 	kd);
		
extern pid_t pid_spd[2];
extern motor_measure_t  motor_measure[2];
#endif
