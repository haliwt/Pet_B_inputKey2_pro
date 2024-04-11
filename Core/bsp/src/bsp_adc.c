/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "bsp_adc.h"
#include "bsp.h"

/* USER CODE BEGIN 0 */
uint16_t temp_value ;
static uint16_t Get_Adc_Hex_Value(void);
static uint16_t Get_Adc_Hex_Average(uint8_t times);

/* USER CODE END 0 */



/* USER CODE BEGIN 1 */
/*****************************************************************
*
	*Function Name: static uint16_t Get_Adc(uint32_t ch)  
	*Function ADC input channel be selected "which one channe"
	*Input Ref: which one ? AC_Channel_?
	*Return Ref: No
	*
	*
*****************************************************************/
static uint16_t Get_Adc_Hex_Value(void)
{
	
    HAL_ADC_Start(&hadc1);                
    
    //temp_value=(uint16_t)ADC1->DR;
   // return temp_value;
	return (uint16_t)HAL_ADC_GetValue(&hadc1);	  
}



static uint16_t Get_Adc_Hex_Average(uint8_t times)
{
	uint32_t temp_val=0;
	uint8_t t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc_Hex_Value();
		//delay_ms(5);
	}
	return temp_val/times;
} 


uint16_t Get_Adc_Voltage_Value(uint8_t times) {
    // 定义一个变量，用于存储计算出的电压值
    uint16_t temp_voltage_value;
    // 定义一个变量，用于存储读取到的模拟电压的十六进制数
    uint16_t temp_hex_value;
    // 读取times次，计算平均值
    temp_hex_value = Get_Adc_Hex_Average(times);
    // 使用公式计算出实际的电压值
    temp_voltage_value = (temp_hex_value * 3300) / 4096; // amplification -> 1000 multiple
    // 返回实际的电压值
    return temp_voltage_value;
}

/* USER CODE END 1 */
