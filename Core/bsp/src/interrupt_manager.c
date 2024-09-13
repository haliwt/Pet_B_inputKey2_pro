#include "interrupt_manager.h"
#include "bsp.h"

#if 0
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{

   if(GPIO_Pin == INPUT_KEY_CONFIRM_Pin){
	 if(CONFIRM_KEY_VALUE() == KEY_DOWN){

       gpro_t.iwdg_detected_times =0;

	 }


   }

   if(GPIO_Pin == INPUT_KEY_FUN_Pin){
	 if(CONFIRM_KEY_VALUE() == KEY_DOWN){

       gpro_t.iwdg_detected_times =0;

	 }


   }



}
#endif 

/*******************************************************************************
  *
  * Function Name: void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
  * Function: Tim14 interrupt call back function
  * Tim3 timer :timing time 1ms
  * 
********************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

    static uint16_t tm0;
	static uint8_t tm2,tm1,tm3;
    if(htim->Instance==TIM14){
       tm0++;
       tm1++;
	   tm2++;
	   tm3++;
	   ctl_t.gTimer_smg_turn_on ++;
	   
	   if(tm2 > 9){
		 	 tm2=0;
		     gpro_t.gTimer_pro_key++;
	         gpro_t.gTimer_pro_disp++;
		     gpro_t.gTimer_pro_select++;
             
            
	     }
	   
		
	   if(tm1>250){//149 //19 *10ms =190ms
	   	 tm1=0;
         led_t.gTimer_flicker ++;
	     led_t.gTimer_keey_heat_flicker++;
         
		}
          

	  if(tm0>999){ //10ms * 100 =1000ms =1s
        tm0=0;

        ctl_t.gTimer_smg++;
		ctl_t.gTimer_read_adc++;
	    ctl_t.gTimer_display++;
		ctl_t.gTimer_select_fun++;
		ctl_t.gTimer_keep_heat_fun++;
        ctl_t.gTimer_again_open_ptc++;
        ctl_t.gTimer_keep_heat_led++;
        
		gpro_t.gTimer_pro_disp_temp++;
		gpro_t.gTimer_display_relay_led ++;
		gpro_t.gTimer_pro_det_dog++;
        gpro_t.gTimer_pro_long_key_timer++;
        gpro_t.gTimer_counter_exit_select_fun++;
        
       
		}
	}
 }


