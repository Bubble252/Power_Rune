/********************************************************************************
  * @file			usart_potocal.c
  * @version		V1.0.0
  * @date			
  * @brief   		
 *******************************************************************************/

/*
*	 usart 通信接口
*/

#include "usart_potocal.h"

#include <string.h>

extern UART_HandleTypeDef huart2;

/*
	 更新检查数据
*/
void USART2_rxDataHandler(uint8_t *rxBuf)
{
 	Rc_Data(rxBuf);
	
}





