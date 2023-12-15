/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
//
#define SCL_Pin 					GPIO_PIN_7
#define SCL_GPIO_Port 				GPIOB

#define SDA_Pin 					GPIO_PIN_9
#define SDA_GPIO_Port 				GPIOB

//Relay Output
#define RELAY_D_Pin 							GPIO_PIN_15
#define RELAY_D_GPIO_Port 						GPIOC


#define RELAY_A_Pin 							GPIO_PIN_2
#define RELAY_A_GPIO_Port 						GPIOA

#define RELAY_B_Pin 							GPIO_PIN_3
#define RELAY_B_GPIO_Port 						GPIOA

#define RELAY_C_Pin GPIO_PIN_12
#define RELAY_C_GPIO_Port GPIOA

//Led display
//ke of led 
#define FUN_CONFIRM_LED_Pin 					GPIO_PIN_1
#define FUN_CONFIRM_LED_GPIO_Port 				GPIOA

#define ADD_DEC_LED_Pin 						GPIO_PIN_11
#define ADD_DEC_LED_GPIO_Port 					GPIOA


//outp led of display
#define FAN_LED_Pin 							GPIO_PIN_4
#define FAN_LED_GPIO_Port 						GPIOA

#define TAPE_LED_Pin 							GPIO_PIN_5
#define TAPE_LED_GPIO_Port 						GPIOA

#define KILL_LED_Pin 							GPIO_PIN_6
#define KILL_LED_GPIO_Port 						GPIOA

#define KEEP_TEMP_LED_Pin 						GPIO_PIN_7
#define KEEP_TEMP_LED_GPIO_Port 				GPIOA


//touch key input 

#define INPUT_KEY_FUN_Pin GPIO_PIN_0
#define INPUT_KEY_FUN_GPIO_Port GPIOB

#define INPUT_KEY_CONFIRM_Pin GPIO_PIN_3
#define INPUT_KEY_CONFIRM_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
