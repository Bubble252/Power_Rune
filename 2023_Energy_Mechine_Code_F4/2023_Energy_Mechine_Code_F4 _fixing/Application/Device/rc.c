/********************************************************************************
  * @file			rc.c
	* @author      RobotPilots@2022
  * @version		V1.0.0
  * @date			2022
  * @brief   		遥控以及键盘
 *******************************************************************************/

#include "rc.h"

/****************************实体化*******************************/
static void rc_sensor_check(rc_sensor_t *rc);
void rc_sensor_heart_beat(rc_sensor_t *rc);

uint8_t Channel_Lock;


/* 遥控信息 */
rc_sensor_info_t rc_info = {
	.offline_max_cnt = 60,
};
/* 遥控驱动 */
drv_uart_t rc_sensor_driver = {
	.type    = DRV_UART2,
	.tx_byte = NULL,
};


rc_sensor_t rc = {
  .info       = &rc_info,
	.driver     = &rc_sensor_driver,
  .init       = rc_sensor_init,
  .update     = rc_sensor_update,
  .check      = rc_sensor_check,
	.heart_beat = rc_sensor_heart_beat,
	.work_state = DEV_OFFLINE,
	.id         = DEV_ID_RC,
};


/*


*  遥控数据清空，失联时使用


*/
void RC_ResetData(rc_sensor_t *rc)
{
	rc->info->ch0 = 0;           // 通道值强行设置成中间值(不拨动摇杆的状态)
	rc->info->ch1 = 0;
	rc->info->ch2 = 0;
	rc->info->ch3 = 0;	
	rc->info->s1  = 4;      
	rc->info->s2  = 4;
	
	rc->info->mouse.x        = 0;// 鼠标
	rc->info->mouse.y        = 0;
	rc->info->mouse.z        = 0;
	rc->info->mouse.press_l  = 0;
	rc->info->mouse.press_r  = 0;	
	rc->info->kb.key_code    = 0;// 键盘	
	rc->info->thumbwheel     = 0;// 左拨轮
	rc->info->TW             = 0;
	Channel_Lock = 0;
}

/*

	State
	状态切换初始化

*/
void Rc_Init(void)
{
	rc.info->ch0 = 0;           // 通道值强行设置成中间值(不拨动摇杆的状态)
	rc.info->ch1 = 0;
	rc.info->ch2 = 0;
	rc.info->ch3 = 0;	
	rc.info->thumbwheel     = 0;// 左拨轮
	rc.info->TW             = 0;
	
	rc.info->mouse.x        = 0;// 鼠标
	rc.info->mouse.y        = 0;
	rc.info->mouse.z        = 0;
	rc.info->mouse.press_l  = 0;
	rc.info->mouse.press_r  = 0;	
	rc.info->kb.key_code    = 0;// 键盘	
	rc.info->thumbwheel     = 0;// 左拨轮

	
}

/*状态切换初始化end*/






void Init_Rc(void)
{
	rc.init(&rc);
}

_Bool rc_offline_check(void)
{
	rc.heart_beat(&rc);
	if(rc.work_state == DEV_OFFLINE)
		return 1;
	
	else
		return 0 ;
}




/*以下为结构体函数 begin*/


void rc_sensor_init(rc_sensor_t *rc_sensor)
{
	rc_sensor->info->offline_cnt = rc_sensor->info->offline_max_cnt + 1;
	rc_sensor->work_state = DEV_OFFLINE;
	RC_ResetData(rc_sensor);
	if(rc_sensor->id == DEV_ID_RC)
		rc_sensor->errno = NONE_ERR;
	else
		rc_sensor->errno = DEV_ID_ERR;
}







/*更新数据 在接收中断中使用*/
void rc_sensor_update(rc_sensor_t *rc_sensor , uint8_t *rxBuf)
{
	rc_sensor->info->ch0 = (rxBuf[0] | rxBuf[1] << 8) & 0x07FF;//0x07FF刚好11位，移位后，将11位后的数与去
	rc_sensor->info->ch1 = (rxBuf[1]>>3 | rxBuf[2] << 5) & 0x07FF;
	rc_sensor->info->ch2 = (rxBuf[2]>>6 | rxBuf[3] << 2 | rxBuf[4] << 10) & 0x07FF;
	rc_sensor->info->ch3 = (rxBuf[4]>>1 | rxBuf[5] << 7) & 0x07FF;
	rc_sensor->info->s2  = (rxBuf[5]>>4) & 0x0003;
	rc_sensor->info->s1  = (rxBuf[5]>>6) & 0x0003;
	
	rc_sensor->info->ch0 -= 1024;
	rc_sensor->info->ch1 -= 1024;
	rc_sensor->info->ch2 -= 1024;
	rc_sensor->info->ch3 -= 1024;
	
	rc_sensor->info->thumbwheel  = ((int16_t)rxBuf[16] | ((int16_t)rxBuf[17] << 8)) & 0x07ff;
	rc_sensor->info->thumbwheel -= 1024;	

	if(rc_sensor->info->thumbwheel > 600)rc_sensor->info->TW  = 5;
	if(rc_sensor->info->thumbwheel <-600)rc_sensor->info->TW  = 0;

	rc_sensor->info->offline_cnt = 0;
	

}


	/*检查数据 在接收中断中使用*/
static void rc_sensor_check(rc_sensor_t *rc)
{
	rc_sensor_info_t *rc_info = rc->info;
	
	if(abs1(rc_info->ch0) > 660 ||
	   abs1(rc_info->ch1) > 660 ||
	   abs1(rc_info->ch2) > 660 ||
	   abs1(rc_info->ch3) > 660)
	{
		rc->errno = DEV_DATA_ERR;
		rc_info->ch0 = 0;
		rc_info->ch1 = 0;
		rc_info->ch2 = 0;
		rc_info->ch3 = 0;		
		rc_info->s1  = SW_MID;
		rc_info->s2  = SW_MID;
		rc_info->thumbwheel = 0;
	}
	else
	{
		rc->errno = NONE_ERR;
		
	}		
}
		
	/*心跳包 在失联检查函数中调用*/
 void rc_sensor_heart_beat(rc_sensor_t *rc)
{
	
	rc_sensor_info_t *rc_info = rc->info;

	rc_info->offline_cnt++;
	if(rc_info->offline_cnt > rc_info->offline_max_cnt)//每次等待一段时间后自动离线
	{
		rc_info->offline_cnt = rc_info->offline_max_cnt;
		rc->work_state = DEV_OFFLINE;
			
	} 
	else //每次接收成功就清空计数
	{
		/* 离线->在线 */
		if(rc->work_state == DEV_OFFLINE)
		{
			rc->work_state = DEV_ONLINE;
			RC_ResetData(rc);  //重新连接后复位所有遥控数据
		}
	}
}	
						/*以上为结构体函数 end*/

/*
	更新检查数据

*/
void Rc_Data(uint8_t *rxBuf)
{
	rc.update(&rc, rxBuf);
	rc.check(&rc);
	rc.info->offline_cnt= 0;
}

