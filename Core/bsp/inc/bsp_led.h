#ifndef __BSP_LED_H__
#define __BSP_LED_H__
#include "main.h"




#define KILL_LED_OFF()			HAL_GPIO_WritePin(KILL_LED_GPIO_Port , KILL_LED_Pin, GPIO_PIN_SET)
#define KILL_LED_ON()			HAL_GPIO_WritePin(KILL_LED_GPIO_Port , KILL_LED_Pin, GPIO_PIN_RESET)


#define TAPE_LED_OFF()			    HAL_GPIO_WritePin(FAN_LED_GPIO_Port, FAN_LED_Pin, GPIO_PIN_SET)
#define TAPE_LED_ON()					HAL_GPIO_WritePin(FAN_LED_GPIO_Port, FAN_LED_Pin, GPIO_PIN_RESET)

#define KEEP_HEAT_LED_OFF()				HAL_GPIO_WritePin(KEEP_TEMP_LED_GPIO_Port , KEEP_TEMP_LED_Pin , GPIO_PIN_SET)
#define KEEP_HEAT_LED_ON()				HAL_GPIO_WritePin(KEEP_TEMP_LED_GPIO_Port , KEEP_TEMP_LED_Pin , GPIO_PIN_RESET)


#define FAN_LED_OFF()			       HAL_GPIO_WritePin(TAPE_LED_GPIO_Port, TAPE_LED_Pin , GPIO_PIN_SET)
#define FAN_LED_ON()			       HAL_GPIO_WritePin(TAPE_LED_GPIO_Port, TAPE_LED_Pin, GPIO_PIN_RESET)


#define ADD_DEC_LED_OFF()			   HAL_GPIO_WritePin(ADD_DEC_LED_GPIO_Port , ADD_DEC_LED_Pin, GPIO_PIN_SET)
#define ADD_DEC_LED_ON()			   HAL_GPIO_WritePin(ADD_DEC_LED_GPIO_Port , ADD_DEC_LED_Pin, GPIO_PIN_RESET)

#define KEY_FUN_CONFIRM_LED_OFF()   			  	HAL_GPIO_WritePin(FUN_CONFIRM_LED_GPIO_Port, FUN_CONFIRM_LED_Pin, GPIO_PIN_SET)
#define KEY_FUN_CONFIRM_LED_ON()     				HAL_GPIO_WritePin(FUN_CONFIRM_LED_GPIO_Port, FUN_CONFIRM_LED_Pin, GPIO_PIN_RESET)




//circle led
  




typedef enum{

    LED_NULL,
	FAN_LED ,
    TAPE_LED,
    STERILIZATION_LED,
    KEEP_HEAT_LED,
    ADD_DEC_LED,
    LED_ALL_OFF,
    KEY_NULL
   
}led_content;

typedef enum{

   confirm_disable=0,
   fan_enable=0x01,
   tape_enable,
   sterilization_enable,
   keep_heat_enable,
   add_dec_enable,
   


}confirm_item;


typedef struct _led_t_{

    uint8_t gTimer_flicker;
	uint8_t gTimer_keey_heat_flicker;




}LED_T;

extern LED_T led_t;


void Led_Display_Content_Fun(uint8_t selitem);



void Led_Test_Fun(void);

void Tape_Led_Filcker(void);
void Fan_Led_Flicker(void);
void Sterilization_Led_Filcker(void);
void Keep_Heat_Led_Filcker(void);

void Keep_heat_SetUp_Led_Filcker(void);


#endif 


