#ifndef __BSP_NTC_H_
#define __BSP_NTC_H_
#include "main.h"


#define DISP_VALUE            10


typedef enum{
   decimals,
   integer

}read_ntc_value;


extern uint8_t disp_ntc_value[DISP_VALUE];



void Read_NTC_Temperature_Power_On(void);

void Read_NTC_Temperature_Value_Handler(void);
void Display_Speicial_Temperature_Value(uint8_t temp);

uint8_t Disp_NtcRes_LinearValue(uint8_t ntc_value);

void Set_KeepTempValue_DispLed(void);
#endif 
