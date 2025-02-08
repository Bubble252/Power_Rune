#ifndef __RC_H
#define __RC_H

#include "main.h"
#include "rp_device.h"
#include "rp_driver.h"
#include "rp_math.h"
//#define    RC_ONLINE             (rc.work_state == DEV_ONLINE)
#define    RC_ONLINE             (rc.work_state == DEV_OFFLINE)
#define    RC_OFFLINE            (rc.work_state == DEV_OFFLINE)
#define    RC_CH_VALUE_MIN       ((uint16_t)364 )
#define    RC_CH_VALUE_OFFSET    ((uint16_t)1024)
#define	   RC_CH_VALUE_MAX       ((uint16_t)1684)
#define	   RC_CH_VALUE_SIDE_WIDTH	((RC_CH_VALUE_MAX-RC_CH_VALUE_MIN)/2)

#define    CH_MIN       ((int16_t)-1000)//原来660
#define    CH_OFFSET    ((int16_t)   0)
#define	   CH_MAX       ((int16_t) 1000)

/* ----------------------- RC Switch Definition-------------------------------*/

#define    SW_UP              ((uint16_t)1)
#define    SW_MID             ((uint16_t)3)
#define    SW_DOWN            ((uint16_t)2)

/* ----------------------- Function Definition-------------------------------- */
/* 遥控摇杆通道偏移值 */
#define			SW1						(rc_info.s1)
#define			SW2						(rc_info.s2)
//#define			CH2_VALUE			(rc_info.ch2)
//#define			CH3_VALUE			(rc_info.ch3)
#define			CH2_VALUE			660
#define			CH3_VALUE			660
#define			CH0_VALUE			(rc_info.ch0)
#define			CH1_VALUE			(rc_info.ch1)
#define			TW_VALUE			(rc_info.thumbwheel)
#define			TW_CNT			  (rc_info.TW)
/*键盘值对应通道*/
#define			CH2_VALUE_K		(rc.info->Ch[2])
#define			CH3_VALUE_K		(rc.info->Ch[3])
#define			CH0_VALUE_K		(rc.info->Ch[0])
#define			CH1_VALUE_K		(rc.info->Ch[1])

/* 检测遥控器开关状态 */
#define     SW1_UP      (rc_info.s1 == SW_UP)
//#define     SW1_MID     (rc_info.s1 == SW_MID)
#define     SW1_MID     1
#define     SW1_DOWN    (rc_info.s1 == SW_DOWN)
#define     SW2_UP      (rc_info.s2 == SW_UP)
#define     SW2_MID     (rc_info.s2 == SW_MID)
#define     SW2_DOWN    (rc_info.s2 == SW_DOWN)


/*      遥控       */

typedef struct rc_sensor_info_struct {
	int16_t 	ch0;
	int16_t 	ch1;
	int16_t 	ch2;
	int16_t 	ch3;
	uint8_t  	s1;
	uint8_t  	s2;
	int16_t 	thumbwheel;
	int16_t   TW;
	
	struct
  {
      int16_t x;
      int16_t y;
      int16_t z;
      uint8_t press_l;
      uint8_t press_r;
  }mouse;	
	union 
	{
		uint16_t key_code;
		struct
		{
			uint16_t W : 1;//冒号表示位域，
			uint16_t S : 1;
			uint16_t A : 1;
			uint16_t D : 1;
			uint16_t SHIFT : 1;
			uint16_t CTRL : 1;
			uint16_t Q : 1;
			uint16_t E : 1;
			uint16_t R : 1;
			uint16_t F : 1;
			uint16_t G : 1;
			uint16_t Z : 1;
			uint16_t X : 1;
			uint16_t C : 1;
			uint16_t V : 1;
			uint16_t B : 1;
		} bit;
	} kb;
	
	int16_t		offline_cnt;
	int16_t		offline_max_cnt;
} rc_sensor_info_t;

typedef struct rc_sensor_struct {
	rc_sensor_info_t	*info;
	drv_uart_t			  *driver;
	void				     (*init)(struct rc_sensor_struct *self);
	void				     (*update)(struct rc_sensor_struct *self, uint8_t *rxBuf);
	void				     (*check)(struct rc_sensor_struct *self);	
	void				     (*heart_beat)(struct rc_sensor_struct *self);
	dev_work_state_t work_state;
	dev_errno_t			 errno;
	dev_id_t			   id;
} rc_sensor_t;




/************************************************************/
extern rc_sensor_info_t rc_info;
extern rc_sensor_t      rc;
void rc_sensor_check(rc_sensor_t *rc);
void rc_sensor_init(rc_sensor_t *rc_sensor);
void rc_sensor_update(rc_sensor_t *rc_sensor , uint8_t *rxBuf);
 void rc_sensor_heart_beat(rc_sensor_t *rc);
void RC_ResetData(rc_sensor_t *rc);
void Init_Rc(void);

void Rc_Data(uint8_t *rxBuf);


void Rc_Init(void);
#endif
