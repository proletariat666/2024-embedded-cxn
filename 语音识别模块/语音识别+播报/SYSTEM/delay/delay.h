#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f4xx.h"

void Delay_Init(void);
void Delay_us(uint16_t ctr);
void Delay_ms(uint16_t ctr);
void Delay_xms(u16 nms);

#endif
