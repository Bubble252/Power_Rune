/********************************************************************************
  * @file			usart_potocal.c
  * @version		V1.0.0
  * @date			
  * @brief   		
 *******************************************************************************/

/*
*	 usart ͨ�Žӿ�
*/

#include "usart_potocal.h"

#include <string.h>

extern UART_HandleTypeDef huart2;

/*
	 ���¼������
*/
void USART2_rxDataHandler(uint8_t *rxBuf)
{
 	Rc_Data(rxBuf);
	
}





