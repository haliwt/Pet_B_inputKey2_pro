#ifndef __BSP_RELAY_H
#define __BSP_RELAY_H
#include "main.h"



//relay output 
#define RELAY_KILL_SetHigh()		HAL_GPIO_WritePin(RELAY_A_GPIO_Port, RELAY_A_Pin, GPIO_PIN_SET)
#define RELAY_KILL_SetLow()		HAL_GPIO_WritePin(RELAY_A_GPIO_Port, RELAY_A_Pin, GPIO_PIN_RESET)



#define RELAY_FAN_SetHigh()		HAL_GPIO_WritePin(RELAY_B_GPIO_Port , RELAY_B_Pin, GPIO_PIN_SET)
#define RELAY_FAN_SetLow()		HAL_GPIO_WritePin(RELAY_B_GPIO_Port , RELAY_B_Pin, GPIO_PIN_RESET)

#define RELAY_TAPE_SetHigh()		HAL_GPIO_WritePin(RELAY_C_GPIO_Port, RELAY_C_Pin, GPIO_PIN_SET)
#define RELAY_TAPE_SetLow()		HAL_GPIO_WritePin(RELAY_C_GPIO_Port, RELAY_C_Pin, GPIO_PIN_RESET)

//special GPIO output realay D
#define RELAY_KEEP_TEMP_SetHigh()		HAL_GPIO_WritePin(RELAY_D_GPIO_Port , RELAY_D_Pin , GPIO_PIN_RESET) //special of gpio has MMBT3904
#define RELAY_KEEP_TEMP_SetLow()		HAL_GPIO_WritePin(RELAY_D_GPIO_Port , RELAY_D_Pin , GPIO_PIN_SET)


void Relay_Init(void);

void Relay_Confirm_Turn_OnOff_Fun(void);

void Relay_Tape_State(void);

void Relay_Fan_State(void);

void Relay_Kill_State(void);

void Relay_Keep_Temp_State(void);

void Quantificat_FlashData_Handler(void);


#endif 
