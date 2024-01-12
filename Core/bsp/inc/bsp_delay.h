#ifndef __BSP_DELAY_H_
#define __BSP_DELAY_H_
#include "main.h"


/* 开关全局中断的宏 */
#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */


void delay_init(uint16_t SYSCLK);

void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);


#endif
