#include "WS2812.h"

extern TIM_HandleTypeDef htim1;

int16_t send_Buf_R[NUM_R] ;
int16_t send_Buf_move_R = 0;
int16_t cnt_time_R = 0;
int16_t Led_bite_cnt_R=0;


void WS_Load_R(void)
{
	HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)send_Buf_R, NUM_R);
}


void WS_CloseAll_R(void)
{
	uint16_t i;
	for (i = 0; i < (PIXEL_NUM_R) * 24; i++)
		send_Buf_R[i] = WS0; // 写入逻辑0的占空比
	for (i = PIXEL_NUM_R * 24; i < NUM_R; i++)
		send_Buf_R[i] = 0; // 占空比比为0，全为低电平
	
	
	WS_Load_R();
}

/*
	 大符中心R字体
*/
	void WS_Write_R(uint8_t n_R, uint8_t n_G, uint8_t n_B )
{
	uint16_t bit_24_low,bit_24_mid,bit_24_hig;
	uint8_t dat[24];
	// 将RGB数据进行转换
	for (bit_24_low = 0; bit_24_low < 8; bit_24_low++)
	{
		dat[bit_24_low] = ((n_G & 0x80) ? WS1 : WS0);
		n_G <<= 1;
		
		
	}
	for (bit_24_mid = 0; bit_24_mid < 8; bit_24_mid++)
	{
		dat[bit_24_mid + 8] = ((n_R & 0x80) ? WS1 : WS0);
		n_R <<= 1;
	}
	for (bit_24_hig = 0; bit_24_hig < 8; bit_24_hig++)
	{
		dat[bit_24_hig + 16] = ((n_B & 0x80) ? WS1 : WS0);
		n_B <<= 1;
	}
		for (int i = 0; i < PIXEL_NUM_R; i++)
	{
		for (int j = 0; j < 24; j++)
		{
			send_Buf_R[i * 24 + j] = dat[j];
		}
	}
	for(int i = PIXEL_NUM_R * 24; i < NUM_R; i++)
		send_Buf_R[i] = 0; // 占空比比为0，全为低电平
	
	WS_Load_R();
}
