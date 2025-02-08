/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *  2023 RobotPiolts
  *  能量机关大主控代码
  *  作者：zs
	*  邮件：szhangszu@163.com
	*  QQ:   1273890169
  *
  * 
  * 
  * 
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "CAN_receive.h"
#include "can_protocal.h"
#include "uart_drv.h"
#include "rc.h"
#include "motor.h"
#include <math.h>
#include "Flag_State.h"
#include "WS2812.h"
#include <stdlib.h>
#include <time.h>
#include "cmsis_armcc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
float motor_speed;
uint8_t  ucSec;
extern float usTms; //毫秒计时
extern float ms_2500; //毫秒计时


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BOARD_NUMBER 0
#define TIME_CNT_CLEAR 255

#define ON  1
#define OFF 0
#define BLUE  1
#define RED 0
#define COLOR (BLUE)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
float speed_rate;
uint8_t color;
uint8_t rand_set;
uint8_t rand_num_0;
uint8_t rand_num_1;
uint8_t rand_num_2;
uint8_t rand_num_3;
uint8_t rand_num_4;
uint8_t motor_control;
uint8_t s2_down_now;
uint8_t s2_down_last;
uint8_t s1_down_now;
uint8_t s1_down_last;
float a_x_1000;
float a_x_1;
float w_x_1000;
float w_x_1;
float b;
int32_t last_time = 0;
int32_t now_time = 0;
char speed1;
char speed2;
float sin_value;
char Manual;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
	CAN1_Init();
	CAN2_Init();		
	USART2_Init();
	Init_Rc();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	WS_CloseAll_R();
	/*点亮R*/
	WS_Write_R(0,0,255);
	
	/* 标志位初始化 */	
	Board_Order_info_Init(Board_Order_info);
	Big_Fu_info_Init(Big_Fu_info);
	//记录睡眠时间
  uint64_t sleep_time = 0;
	
	speed_rate = 907;
	color = COLOR;
	
	/*初始五个随机数生成*/
	rand_num();		
	a_x_1000 = rand()%265 + 780;
	a_x_1 = a_x_1000/1000.0f;
					
	w_x_1000 = rand()%116 + 1884;
	w_x_1 = w_x_1000/1000.0f;
					
	b = 2.09f - a_x_1;	
	last_time = 0;
	
	/* PID初始化设置 */	
	PID_struct_init(&pid_spd[0],12000,5000,20.0f,0.6f,0.0f);  
	
  while (1)
 {			
//	      WS_Write_R(0,0,255);
//		
//		    Board_Order_info[0].Set_Color = color;
//				Board_Order_info[1].Set_Color = color;
//				Board_Order_info[2].Set_Color = color;
//				Board_Order_info[3].Set_Color = color;
//				Board_Order_info[4].Set_Color = color;
//		    rand_num();
//			/*扇叶做好击打准备*/			
//				Board_Order_info[rand_num_0].Board_Work  = ON;

//						if(Big_Fu_info[rand_num_0].Single_Hit_State == OFF)
//						{
//								if((ms_2500 >= 2400 && Big_Fu_info[rand_num_0].Single_Hit_State == OFF)
//									|| (Big_Fu_info[rand_num_1].Single_Hit_State
//									||  Big_Fu_info[rand_num_2].Single_Hit_State
//									||	Big_Fu_info[rand_num_3].Single_Hit_State
//									||	Big_Fu_info[rand_num_4].Single_Hit_State))
//								{
//										Board_Order_info[0].Board_Work = OFF;
//										Board_Order_info[1].Board_Work = OFF;
//										Board_Order_info[2].Board_Work = OFF;
//										Board_Order_info[3].Board_Work = OFF;
//										Board_Order_info[4].Board_Work = OFF;

//										rand_num();
//								}		
//						}			
//								
//				if(Big_Fu_info[rand_num_0].Single_Hit_State == ON)
//				{				
//					Board_Order_info[rand_num_1].Board_Work  = ON;
//	   	  }
//				if(Big_Fu_info[rand_num_1].Single_Hit_State == ON)
//				{
//					Board_Order_info[rand_num_2].Board_Work  = ON;
//				}
//				if(Big_Fu_info[rand_num_2].Single_Hit_State == ON)
//				{
//					Board_Order_info[rand_num_3].Board_Work  = ON;
//				}
//				if(Big_Fu_info[rand_num_3].Single_Hit_State == ON)
//				{
//					Board_Order_info[rand_num_4].Board_Work  = ON;

//				}
//				
//				
//				sin_value = sin(w_x_1*usTms/1000.0f);
//				motor_speed = (a_x_1 * sin_value + b)*speed_rate;
//				pid_calc(&pid_spd[0],  motor_measure[0].speed_rpm, motor_speed);				
//				/*CAN发送电流驱动电机*/
//				CAN_cmd_motor( 0, 0,pid_spd[0].pos_out, 0);
//				
//				motor_speed = 950;
//				pid_calc(&pid_spd[0],  motor_measure[0].speed_rpm, motor_speed);				
//				/*CAN发送电流驱动电机*/
//				CAN_cmd_motor( 0, 0,pid_spd[0].pos_out, 0);
//				
//		

////    /* USER CODE END WHILE */

////    /* USER CODE BEGIN 3 */
//		/* 遥控器心跳包 */
		rc_sensor_heart_beat(&rc);
//		
//		/* 遥控器离线/在线处理 */
		if(RC_OFFLINE)
	  {
      Board_Info_Tx();
			if(HAL_GetTick() - sleep_time < 200)
			{
				RC_ResetData(&rc);
			  pid_spd[0].pos_out = 0;
				Board_Order_info[0].Board_Work = OFF;
				Board_Order_info[1].Board_Work = OFF;
				Board_Order_info[2].Board_Work = OFF;
				Board_Order_info[3].Board_Work = OFF;
				Board_Order_info[4].Board_Work = OFF;
			}
			else if(HAL_GetTick()- sleep_time<500)
			{			
				/*底盘电机停转卸力	*/
				CAN_cmd_motor(0,0,0,0);	
			}			

		}
    /* 大符速度设置 */	
	
				//颜色设置
				Board_Order_info[0].Set_Color = color;
				Board_Order_info[1].Set_Color = color;
				Board_Order_info[2].Set_Color = color;
				Board_Order_info[3].Set_Color = color;
				Board_Order_info[4].Set_Color = color;
		    		    
	/*扇叶做好击打准备*/			
				Board_Order_info[rand_num_0].Board_Work  = ON;
				if(	!CH0_VALUE && !CH1_VALUE)
				{
						if(Big_Fu_info[rand_num_0].Single_Hit_State == OFF)
						{
								if((ms_2500 >= 2400 && Big_Fu_info[rand_num_0].Single_Hit_State == OFF)
									|| (Big_Fu_info[rand_num_1].Single_Hit_State
									||  Big_Fu_info[rand_num_2].Single_Hit_State
									||	Big_Fu_info[rand_num_3].Single_Hit_State
									||	Big_Fu_info[rand_num_4].Single_Hit_State))
								{
										Board_Order_info[0].Board_Work = OFF;
										Board_Order_info[1].Board_Work = OFF;
										Board_Order_info[2].Board_Work = OFF;
										Board_Order_info[3].Board_Work = OFF;
										Board_Order_info[4].Board_Work = OFF;

										rand_num();
								}		
						}			
					}				
				if(Big_Fu_info[rand_num_0].Single_Hit_State == ON)
				{				
					Board_Order_info[rand_num_1].Board_Work  = ON;
	   	  }
				if(Big_Fu_info[rand_num_1].Single_Hit_State == ON)
				{
					Board_Order_info[rand_num_2].Board_Work  = ON;
				}
				if(Big_Fu_info[rand_num_2].Single_Hit_State == ON)
				{
					Board_Order_info[rand_num_3].Board_Work  = ON;
				}
				if(Big_Fu_info[rand_num_3].Single_Hit_State == ON)
				{
					Board_Order_info[rand_num_4].Board_Work  = ON;

				}


				
	if(RC_ONLINE)
	{			
			HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
			s2_down_now = rc.info->s2;
			if(s2_down_last != s2_down_now && rc.info->s2 == 2)
			{	
				motor_control =! motor_control;
			}	
			s2_down_last = s2_down_now;
		
			if(motor_control == 0)
			{
				pid_spd[0].pos_out = 0;
				CAN_cmd_motor(0,0,0,0);	
			}
			
			if(CH0_VALUE <= -600 && CH1_VALUE >= 600)
			{
				speed1 = 1;
				speed2 = 0;			
			}
			if(CH2_VALUE >= 600 && CH3_VALUE >= 600)
			{
				speed1 = 0;
				speed2 = 1;			
			}
		
			
				if(CH0_VALUE <= -600  && CH1_VALUE >= 600 && CH2_VALUE>= 600 && CH3_VALUE >= 600)
			{
				 __set_FAULTMASK(1); //关闭所有中断
				NVIC_SystemReset(); //进行软件复位		
			}
		
			/*电机旋转速度*/
				if(speed1==1)
			{
        sin_value = sin(w_x_1*usTms/1000.0f);
				motor_speed = (a_x_1 * sin_value + b)*speed_rate;
			}
			if(speed2==1)
			{
				motor_speed = 950;
			}
			if(motor_control == 1)
			{
				pid_calc(&pid_spd[0],  motor_measure[0].speed_rpm, motor_speed);				
					
				/*CAN发送电流驱动电机*/
				CAN_cmd_motor( 0, 0,pid_spd[0].pos_out, 0);
			}
			
	
			if(SW2_MID)
			{
				color = BLUE;	
        WS_Write_R(0,0,150);				
			}
			
			if(SW2_UP)
			{
				color = RED;		
        WS_Write_R(150,0,0);				
			}
			
			if(SW1_UP && !CH2_VALUE && !CH3_VALUE)
			{
					Board_Order_info[0].Board_Work = OFF;
					Board_Order_info[1].Board_Work = OFF;
					Board_Order_info[2].Board_Work = OFF;
					Board_Order_info[3].Board_Work = OFF;
					Board_Order_info[4].Board_Work = OFF;
				  Manual = 0;				
			}

			if(CH0_VALUE >= 600  && CH1_VALUE <= -600 )
			{
				Board_Order_info[0].Board_Work = OFF;
				Board_Order_info[1].Board_Work = OFF;
				Board_Order_info[2].Board_Work = OFF;
				Board_Order_info[3].Board_Work = OFF;
				Board_Order_info[4].Board_Work = OFF;
				
				Board_Order_info[0].RainBow_Flag = 1;
				Board_Order_info[1].RainBow_Flag = 1;
				Board_Order_info[2].RainBow_Flag = 1;
				Board_Order_info[3].RainBow_Flag = 1;
				Board_Order_info[4].RainBow_Flag = 1;
			}
			if(CH2_VALUE <= -600  && CH3_VALUE <= -600 )
			{
				Board_Order_info[0].RainBow_Flag = 0;
				Board_Order_info[1].RainBow_Flag = 0;
				Board_Order_info[2].RainBow_Flag = 0;
				Board_Order_info[3].RainBow_Flag = 0;
				Board_Order_info[4].RainBow_Flag = 0;
				
				Board_Order_info[0].Board_Work = OFF;
				Board_Order_info[1].Board_Work = OFF;
				Board_Order_info[2].Board_Work = OFF;
				Board_Order_info[3].Board_Work = OFF;
				Board_Order_info[4].Board_Work = OFF;
			}
			
		
			
			
			if(SW1_DOWN && !CH0_VALUE && !CH1_VALUE)
			{					
				  	rand_num();
						a_x_1000 = rand()%265 + 780;
						a_x_1 = a_x_1000/1000.0f;
					
						w_x_1000 = rand()%116 + 1884;
						w_x_1 = w_x_1000/1000.0f;
					
					  b = 2.09f - a_x_1;						
			}						
	}
		Board_Info_Tx();
		HAL_Delay(100);
	}	
}

void rand_num(void)
{
	rand_num_0 = rand()%5;
	rand_num_1 = rand()%5;
	while(rand_num_1 == rand_num_0)
	{
		rand_num_1 = rand()%5;
	}
	rand_num_2 = rand()%5;
	while(rand_num_2 == rand_num_0 || rand_num_2 == rand_num_1)
	{
		rand_num_2 = rand()%5;
	}
	rand_num_3 = rand()%5;
	while(rand_num_3 == rand_num_0 || rand_num_3 == rand_num_1 || rand_num_3 == rand_num_2)
	{
		rand_num_3 = rand()%5;
	}
		rand_num_4 = rand()%5;
	while(rand_num_4 == rand_num_0 || rand_num_4 == rand_num_1 || rand_num_4 == rand_num_2 || rand_num_4 == rand_num_3 )
	{
		rand_num_4 = rand()%5;
	}
}
/* USER CODE END 3 */
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
