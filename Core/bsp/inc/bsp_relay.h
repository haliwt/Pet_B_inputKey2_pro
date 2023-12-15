#ifndef __BSP_RELAY_H
#define __BSP_RELAY_H
#include "main.h"



//relay output 
#define RELAY_FAN_SetHigh()		HAL_GPIO_WritePin(RELAY_A_GPIO_Port, RELAY_A_Pin, GPIO_PIN_SET)
#define RELAY_FAN_SetLow()		HAL_GPIO_WritePin(RELAY_A_GPIO_Port, RELAY_A_Pin, GPIO_PIN_RESET)



#define RELAY_TAPE_SetHigh()		HAL_GPIO_WritePin(RELAY_B_GPIO_Port , RELAY_B_Pin, GPIO_PIN_SET)
#define RELAY_TAPE_SetLow()		HAL_GPIO_WritePin(RELAY_B_GPIO_Port , RELAY_B_Pin, GPIO_PIN_RESET)

#define RELAY_KILL_SetHigh()		HAL_GPIO_WritePin(RELAY_C_GPIO_Port, RELAY_C_Pin, GPIO_PIN_SET)
#define RELAY_KILL_SetLow()		HAL_GPIO_WritePin(RELAY_C_GPIO_Port, RELAY_C_Pin, GPIO_PIN_RESET)

//special GPIO output realay D
#define RELAY_KEEP_TEMP_SetHigh()		HAL_GPIO_WritePin(RELAY_D_GPIO_Port , RELAY_D_Pin , GPIO_PIN_RESET) //special of gpio has MMBT3904
#define RELAY_KEEP_TEMP_SetLow()		HAL_GPIO_WritePin(RELAY_D_GPIO_Port , RELAY_D_Pin , GPIO_PIN_SET)


#endif 
