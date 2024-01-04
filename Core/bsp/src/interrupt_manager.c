#include "interrupt_manager.h"
#include "bsp.h"



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
	static uint8_t tm2,tm1;
    if(htim->Instance==TIM14){
       tm0++;
       tm1++;
	    tpd_t.gTimer_smg_turn_on ++;
	   if(tm2 > 9){
		 	 tm2=0;
		     pro_t.gTimer_pro_key++;
	         pro_t.gTimer_pro_disp++;
		    
			pro_t.gTimer_pro_select++;
	     }
		
	   if(tm1>149){ //19 *10ms =190ms
	   	 tm1=0;
         led_t.gTimer_flicker ++;
	     led_t.gTimer_keey_heat_flicker++;
		}
          

	  if(tm0>999){ //10ms * 100 =1000ms =1s
        tm0=0;
		pro_t.gTimer_pro_feed_dog++;
        tpd_t.gTimer_smg++;
		tpd_t.gTimer_read_adc++;
	    tpd_t.gTimer_display++;
		tpd_t.gTimer_select_fun++;
		tpd_t.gTimer_keep_heat_fun++;
		pro_t.gTimer_pro_disp_temp++;
		}
	}
 }


