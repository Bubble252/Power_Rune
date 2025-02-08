#ifndef _WS2812_H
#define _WS2812_H


#include "main.h"

#define PIXEL_NUM_R  64
#define NUM_R (24*PIXEL_NUM_R + 224)        // Reset 280us / 1.25us = 224

#define WS1  70
#define WS0  35


void WS_Write_R(uint8_t n_R, uint8_t n_G, uint8_t n_B );
void WS_Load_R(void);
void WS_CloseAll_R(void);

#endif
