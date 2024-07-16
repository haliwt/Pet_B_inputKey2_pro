#ifndef __BSP_H_
#define __BSP_H_
#include "main.h"

//core
#include "adc.h"
#include "gpio.h"
#include "iwdg.h"
#include "tim.h"

//bsp
#include "bsp_ctl.h"
#include "bsp_key.h"
#include "bsp_led.h"
#include "bsp_ntc.h"
#include "bsp_tm1650.h"
#include "bsp_smg.h"
#include "bsp_delay.h"
#include "bsp_relay.h"
#include "bsp_adc.h"
#include "bsp_flash.h"
#include "bsp_ntc_calculate.h"
#include "bsp_freertos.h"


#define OLDER_VERSION   0

#define __STM32G0F3P6_BSP_VERSION		"1.0"



#define  USE_FreeRTOS      1

#if USE_FreeRTOS == 1
    #include "FreeRTOS.h"
	#include "task.h"

	#define DISABLE_INT()    taskENTER_CRITICAL()
	#define ENABLE_INT()     taskEXIT_CRITICAL()
#else
	/* ¿ª¹ØÈ«¾ÖÖÐ¶ÏµÄºê */
	#define ENABLE_INT()	__set_PRIMASK(0)	/* Enable global interrupt  */
	#define DISABLE_INT()	__set_PRIMASK(1)	/* ½ûÖ¹È«¾ÖÖÐ¶Ï */
#endif



typedef enum{

    fun_key =0x01,
	confirm_short_key,
	confirm_long_key =0x82,
	fun_long_key = 0x83


}key_id_state;


typedef enum{

    relay_a_tape_led = 0x01 ,
	relay_b_fan_led ,
	relay_c_kill_led ,
	relay_keep_temp 

}relay_id_state;

typedef enum{

    relay_tape_led_on=0x01,
	relay_fan_led_on ,
	relay_kill_led_on,
	relay_keep_temp_led_on,
	
	
}reelay_led_state;


typedef struct{


   uint8_t key_fun;
   uint8_t key_value;
   uint8_t fun_key_be_pressing_flag;
 
   uint8_t key_long_confirm_flag;
   uint8_t set_temp_value_success_flag;
   uint8_t set_keep_temp_value;
   uint8_t key_as_numbers_input_flag;
   uint8_t long_key_flag;
   uint8_t disp_temp_value;
   uint8_t iwdg_detected_times;
   uint8_t key_detected_flag;
   
   uint8_t gTimer_pro_long_key_timer;
   uint8_t gTimer_pro_key;
   uint8_t gTimer_pro_disp;
   uint8_t gTimer_pro_disp_temp;
   uint8_t gTimer_pro_select;
   uint8_t gTimer_display_relay_led ;
   uint8_t gTimer_pro_det_dog;
   uint8_t gTimer_counter_exit_select_fun;
   
   
 }main_prcess_t;


extern main_prcess_t pro_t;


void bsp_Idle(void);
void Key_Handler(uint8_t key_value);

void Main_Process(void);


void exit_select_position_flag(void);

#endif 

