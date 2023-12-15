#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__
#include "main.h"





#define FUN_KEY_VALUE()                                 HAL_GPIO_ReadPin(INPUT_KEY_FUN_GPIO_Port ,INPUT_KEY_FUN_Pin )

#define CONFIRM_KEY_VALUE()                             HAL_GPIO_ReadPin(INPUT_KEY_CONFIRM_GPIO_Port,INPUT_KEY_CONFIRM_Pin)







#define		_KEY_ALL_OFF				 0XFF

//normal times be pressed key
#define FUNCTION_KEY_PRES 	0x01
#define CONFIRM_KEY_PRES	0x02




//long times be pressed
#define	   _KEY_CONT_1_POWER     	     0x81    //??????--???
#define	   KEY_LONG_PRES_CONFIRM_MODE   		     0x82    //??????--???







#define HIGH_LEVEL                   1  /* 根据原理图设计，KEY1按下时引脚为低电平，所以这里设置为0 */


typedef enum
{
  KEY_UP   = 0,
  KEY_DOWN = 1,
}KEYState_TypeDef;




typedef  struct  _state_
{
 unsigned char         read;
 unsigned char         buffer;
 unsigned long          value;
 unsigned char        off_time;
 unsigned long        on_time;

 enum{
  start  = 0,
  first  = 1,
  second = 2,
  cont   = 3,
  end    = 4,
  finish = 5,
 }state;
}key_types;



uint8_t ReadKey(void);

uint8_t KEY_Scan(void);










#endif 



