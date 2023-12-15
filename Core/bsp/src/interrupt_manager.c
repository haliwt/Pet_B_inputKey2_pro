#include "interrupt_manager.h"
#include "bsp.h"



/*******************************************************************************
  *
  * Function Name: void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
  * Function: Tim14 interrupt call back function
  * Tim3 timer :timing time 10ms
  * 
********************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

    static uint8_t tm0,tm1;
    if(htim->Instance==TIM14){
       tm0++;
       tm1++;
	     pro_t.gTimer_pro_key++;
         pro_t.gTimer_pro_disp++;
	    tpd_t.gTimer_smg_turn_on ++;
	   if(tm1>49){ //50 *10ms =500ms
	   	 tm1=0;
         led_t.gTimer_flicker ++;
	     led_t.gTimer_keey_heat_flicker++;
		 
	   	}
          

	  if(tm0>99){ //10ms * 100 =1000ms =1s
        tm0=0;
		pro_t.gTimer_pro_feed_dog++;
        tpd_t.gTimer_smg++;
		tpd_t.gTimer_read_adc++;
	    tpd_t.gTimer_display++;
		tpd_t.gTimer_select_fun++;
		tpd_t.gTimer_keep_heat_fun++;
		
		


	  }
	
		
	  
 	}
 }


