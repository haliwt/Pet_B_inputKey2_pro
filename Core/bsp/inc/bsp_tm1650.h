#ifndef __BSP_TM1650_H_
#define __BSP_TM1650_H_
#include "main.h"


//IO direction GPIOB PIN_9
 
#define SDA_IN()        {GPIOB->MODER&=0X0FFFFFFF;GPIOB->MODER &=~((uint32_t)1<<18);} //MODER:00
#define SDA_OUT()       {GPIOB->MODER&=0X0FFFFFFF;GPIOB->MODER |=(uint32_t)1<<18;}  //MODER:01 PB9

#define SetBit_Reverse(y,x)      (y ^=(1<<x)) //

#define  TM1650_ORDER           0x48        //数字指令 写数据
#define  TM1650_TURN_OFF        0x00

#define SCL_SetHigh()               HAL_GPIO_WritePin(SCL_GPIO_Port , SCL_Pin, GPIO_PIN_SET)
#define SCL_SetLow()                HAL_GPIO_WritePin(SCL_GPIO_Port , SCL_Pin, GPIO_PIN_RESET)

#define SDA_SetHigh()               HAL_GPIO_WritePin(SDA_GPIO_Port , SDA_Pin, GPIO_PIN_SET)
#define SDA_SetLow()                HAL_GPIO_WritePin(SDA_GPIO_Port , SDA_Pin, GPIO_PIN_RESET)

#define Read_SDA()                  HAL_GPIO_ReadPin(SDA_GPIO_Port , SDA_Pin)






void TM1650_Write_Data(uint8_t address,uint8_t data) ;


void Smg_Display_Temp_Degree_Handler(void);

void I2C_SDA_OUT(void);
void I2C_SDA_IN(void);//ÅäÖÃ³ÉÊä³ö
void Smg_Display_Digital_Numbers_Changed(uint8_t numbers);
void Repeat_Smg_Display_Digital_Numbers_Changed(uint8_t numbers);


#endif 







