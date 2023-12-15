#include "bsp.h"

main_prcess_t pro_t;

static void Relay_Fun(uint8_t relay_id_led_flag);
static void Run_Display_Handler(uint8_t temp_value);




uint8_t relay_id_led,keep_temp_flag ;

/*
*********************************************************************************************************
*	函 数 名: bsp_Idle
*	功能说明: 空闲时执行的函数。一般主程序在for和while循环程序体中需要插入 CPU_IDLE() 宏来调用本函数。
*			 本函数缺省为空操作。用户可以添加喂狗、设置CPU进入休眠模式的功能。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Idle(void)
{
	/* --- 喂狗 */
    if(pro_t.gTimer_pro_feed_dog > 2){
	   pro_t.gTimer_pro_feed_dog=0;
	   Feed_Dog();

    }
		
	/* --- 让CPU进入休眠，由Systick定时中断唤醒或者其他中断唤醒 */
	
    /* 例如 emWin 图形库，可以插入图形库需要的轮询函数 */
	//GUI_Exec();
	
	/* 例如 uIP 协议，可以插入uip轮询函数 */
	//TOUCH_CapScan();
	


}
/*
*********************************************************************************************************
*	函 数 名: Key_Handler(uint8_t key_value)
*	功能说明: 中间层 
*			 
*	形    参: 输入按键的键值
*	返 回 值: 无
*********************************************************************************************************
*/
void Key_Handler(uint8_t key_value)
{
  static uint8_t set_temp_flag;
  switch(key_value){


     case fun_key:  //fun key 

         if(keep_temp_flag ==1){
		 	
		 	  tpd_t.gTimer_select_fun=0;
              tpd_t.digital_numbers++; //scope : 16~30度
			  if(tpd_t.digital_numbers>30) tpd_t.digital_numbers=30;
			  Run_Keep_Heat_Setup_Digital_Numbers();
			  

		 }
		 else{
         pro_t.key_fun++;
		 if(pro_t.key_fun > 4) pro_t.key_fun=1;
         switch(pro_t.key_fun){

		    case relay_a:
               KEY_FUN_CONFIRM_LED_ON() ;  
		       relay_id_led = relay_fan_led_on;
		       tpd_t.gTimer_select_fun=0;
			   

		    break;
		 
		    case relay_b:
				  KEY_FUN_CONFIRM_LED_ON() ; 
				relay_id_led = relay_tape_led_on;
				tpd_t.gTimer_select_fun=0;
				

		    break;

			case relay_c:
				  KEY_FUN_CONFIRM_LED_ON() ;  
				relay_id_led = relay_kill_led_on;
				tpd_t.gTimer_select_fun=0;
				
		    break;

			case relay_temp: //keep temperature value

			  KEY_FUN_CONFIRM_LED_ON() ;  
			relay_id_led = relay_keep_temp_led_on;
			tpd_t.gTimer_select_fun=0;
			 
			 
			//pro_t.key_fun=0;
		    break;

			default:

			break;

           }
		 }
    
     break;

	 case confirm_short_key: // confirm key

	   
	   if(relay_id_led == relay_keep_temp_led_on &&  keep_temp_flag ==0){ //"+" tempeature value 

	         
             if(tpd_t.relay_keep_temp_flag ==1){
                 tpd_t.relay_keep_temp_flag =0;
				 tpd_t.gTimer_select_fun =10;

			 }
             else{
			  
			  keep_temp_flag =1;
			  tpd_t.gTimer_select_fun=0;
	          ADD_DEC_LED_ON();

            }

	   }
	   else if(keep_temp_flag ==1){
	   
              tpd_t.gTimer_select_fun=0;
			  tpd_t.digital_numbers--; //scope : 16~30度
			  if(tpd_t.digital_numbers <16) tpd_t.digital_numbers=16;
			  Run_Keep_Heat_Setup_Digital_Numbers();
			  

		}
		else{
			tpd_t.gTimer_select_fun=0;
	    	pro_t.key_short_confirm_flag =1;
		}
		

	 break;

	 case confirm_long_key: //confirm long by pressed 

	    if(keep_temp_flag ==1){

	        tpd_t.gTimer_select_fun=20;
			keep_temp_flag =0;
		
			ADD_DEC_LED_OFF();
		
			
		    if(tpd_t.relay_keep_temp_flag ==0){
				tpd_t.relay_keep_temp_flag =1;
				
			   pro_t.set_keep_temp = 1;
		       pro_t.set_keep_tmep_value = tpd_t.digital_numbers;
			   if(pro_t.set_keep_tmep_value >= tpd_t.temperature_value ){
                   tpd_t.relay_keep_temp_flag =1;
			       KEEP_HEAT_LED_ON();
	               RELAY_KEEP_TEMP_SetHigh();

			  }
              else{
                  tpd_t.relay_keep_temp_flag =0;
			      KEEP_HEAT_LED_OFF();
	              RELAY_KEEP_TEMP_SetLow();

              }
			}
			else{
				 pro_t.set_keep_temp = 0;

				tpd_t.relay_keep_temp_flag =0;
			    KEEP_HEAT_LED_OFF();
	            RELAY_KEEP_TEMP_SetLow();


			}
		   

	   }

	 break;
  
  }


}
/*
*********************************************************************************************************
*	函 数 名: void Main_Process(void)
*	功能说明: App 层 
*			 
*	形    参: 输入按键的键值
*	返 回 值: 无
*********************************************************************************************************
*/
void Main_Process(void)
{
   
	 Relay_Fun(relay_id_led);
    

	if(pro_t.gTimer_pro_disp > 9){ //100ms
	    pro_t.gTimer_pro_disp =0;
		Run_Display_Handler(keep_temp_flag);
		

	}

}

/*********************************************************************************************************
*	函 数 名: void Main_Process(void)
*	功能说明: App 层 
*			 
*	形    参: 输入按键的键值
*	返 回 值: 无
*********************************************************************************************************/
static void Relay_Fun(uint8_t relay_id_led_flag)
{

 
   switch(relay_id_led_flag){

    case relay_fan_led_on:


      if(tpd_t.gTimer_select_fun < 6 &&  pro_t.key_short_confirm_flag ==0){
        	Fan_Led_Flicker();
	
       }
       else{
	   	  tpd_t.gTimer_select_fun =10;

	      
		  if(pro_t.key_short_confirm_flag ==1){
		  	   pro_t.key_short_confirm_flag =0;
			   if(tpd_t.relay_fan_flag ==0){
			   	   tpd_t.relay_fan_flag = 1;
				  
				   FAN_LED_ON();
				   RELAY_FAN_SetHigh();
			   }
			   else {
				   tpd_t.relay_fan_flag = 0;
				  
				   FAN_LED_OFF();
				   RELAY_FAN_SetLow();

			   }
		  }
          else{


			 if(tpd_t.relay_fan_flag ==1){
					
				FAN_LED_ON();
				RELAY_FAN_SetHigh();
			}
			else if(tpd_t.relay_fan_flag ==0){
			 
			
			 FAN_LED_OFF();
			 RELAY_FAN_SetLow();
			  
			}

		  }
	
	  }
      

    if(pro_t.gTimer_pro_key > 1){//20ms
            pro_t.gTimer_pro_key =0;
		 if(tpd_t.relay_tape_flag ==1){
		 	 TAPE_LED_ON();
			 RELAY_TAPE_SetHigh();
			 
		 }
		 else{
		 	TAPE_LED_OFF();
            RELAY_TAPE_SetLow();

		 }

		 
	     if(tpd_t.relay_kill_flag ==1){
		 	 KILL_LED_ON();
			 RELAY_KILL_SetHigh();
	        
		 }
		 else{
		 	 KILL_LED_OFF();
			 RELAY_KILL_SetLow();
	
	
		 }

		 
	    if(tpd_t.relay_keep_temp_flag ==1){
			KEEP_HEAT_LED_ON();
	        RELAY_KEEP_TEMP_SetHigh();
		}
		else{
	        KEEP_HEAT_LED_OFF();
	        RELAY_KEEP_TEMP_SetLow();
	
		}

        }
    break;

	case relay_tape_led_on:

		if(tpd_t.gTimer_select_fun < 6 && pro_t.key_short_confirm_flag ==0){
			Tape_Led_Flicker();
		}
		else{

             tpd_t.gTimer_select_fun =10;
	
             if(pro_t.key_short_confirm_flag ==1){
                 pro_t.key_short_confirm_flag =0;

			   if(tpd_t.relay_tape_flag ==0){

				   tpd_t.relay_tape_flag=1;
				   TAPE_LED_ON();
			 	   RELAY_TAPE_SetHigh();

			   }
			   else{
				   tpd_t.relay_tape_flag=0;

				   TAPE_LED_OFF();
            		RELAY_TAPE_SetLow();
				  

			   }
		      }
			   else{

					if(tpd_t.relay_tape_flag ==1){
						TAPE_LED_ON();
			 			RELAY_TAPE_SetHigh();

					}
					else{
					 TAPE_LED_OFF();
            		 RELAY_TAPE_SetLow();


					}


			   }
			
		}
		



	    //
		 if(pro_t.gTimer_pro_key > 1){//20ms
      		pro_t.gTimer_pro_key =0;
         if(tpd_t.relay_fan_flag == 1){

			 FAN_LED_ON();//FAN_LED_ON();
					 
		     RELAY_FAN_SetHigh()	 ;

         }
         else{
		   FAN_LED_OFF();//FAN_LED_OFF();
		   RELAY_FAN_SetLow()	;


		 }
	
//         if(tpd_t.relay_tape_flag ==1){
//		 	 TAPE_LED_ON();
//			 RELAY_TAPE_SetHigh();
//			 
//		 }
//		 else{
//		 	TAPE_LED_OFF();
//            RELAY_TAPE_SetLow();
//
//		 }

		 
	     if(tpd_t.relay_kill_flag ==1){
		 	 KILL_LED_ON();
			 RELAY_KILL_SetHigh();
	        
		 }
		 else{
		 	 KILL_LED_OFF();
			 RELAY_KILL_SetLow();
	
	
		 }

		 
	    if(tpd_t.relay_keep_temp_flag ==1){
			KEEP_HEAT_LED_ON();
	        RELAY_KEEP_TEMP_SetHigh();
		}
		else{
	        KEEP_HEAT_LED_OFF();
	        RELAY_KEEP_TEMP_SetLow();
	
		}
	    }

    break;

	case relay_kill_led_on:

		//STERILIZATION
		if(tpd_t.gTimer_select_fun < 6 && pro_t.key_short_confirm_flag ==0 ){
			Sterilization_Led_Filcker();
		}
		else{
			 tpd_t.gTimer_select_fun =10;
			 
			if(pro_t.key_short_confirm_flag ==1){
				pro_t.key_short_confirm_flag =0;
				
				if(tpd_t.relay_kill_flag ==0){
					tpd_t.relay_kill_flag =1;
					KILL_LED_ON();
					RELAY_KILL_SetHigh();

				}
				else {

					tpd_t.relay_kill_flag =0;
					KILL_LED_OFF();
					RELAY_KILL_SetLow();

	           }
		   }
		   else{

				if(tpd_t.relay_kill_flag ==1){
				KILL_LED_ON();
				RELAY_KILL_SetHigh();

				}
				else{
				KILL_LED_OFF();
				RELAY_KILL_SetLow();


				}
			}

		}
  

		 if(pro_t.gTimer_pro_key > 1){//20ms
      		pro_t.gTimer_pro_key =0;
        if(tpd_t.relay_fan_flag == 1){

			 FAN_LED_ON();//FAN_LED_ON();
					 
		     RELAY_FAN_SetHigh()	 ;

         }
         else{
		   FAN_LED_OFF();//FAN_LED_OFF();
		   RELAY_FAN_SetLow()	;


		 }
	
         if(tpd_t.relay_tape_flag ==1){
		 	 TAPE_LED_ON();
			 RELAY_TAPE_SetHigh();
			 
		 }
		 else{
		 	TAPE_LED_OFF();
            RELAY_TAPE_SetLow();

		 }

		 
//	     if(tpd_t.relay_kill_flag ==1){
//		 	 KILL_LED_ON();
//			 RELAY_KILL_SetHigh();
//	        
//		 }
//		 else{
//		 	 KILL_LED_OFF();
//			 RELAY_KILL_SetLow();
//	
//	
//		 }

		 
	    if(tpd_t.relay_keep_temp_flag ==1){
			KEEP_HEAT_LED_ON();
	        RELAY_KEEP_TEMP_SetHigh();
		}
		else{
	        KEEP_HEAT_LED_OFF();
	        RELAY_KEEP_TEMP_SetLow();
	
		}
        }
      

    break;

	case relay_keep_temp_led_on: //keep temperature be set up value 16~30 degree

	    //KEEP HEAT Display of LED 
       if(tpd_t.gTimer_select_fun < 12 && pro_t.key_long_confirm_flag ==0){

	       if(keep_temp_flag ==0){
		   	
	   	      Keep_Heat_Led_Filcker();
	       }
	       else{

			 Keep_heat_SetUp_Led_Filcker();
		  }
		  
		
       	}
        else{
			tpd_t.gTimer_select_fun=20;
            keep_temp_flag =0;
		    ADD_DEC_LED_OFF();
       
			if(tpd_t.relay_keep_temp_flag ==1){
				KEEP_HEAT_LED_ON();
				RELAY_KEEP_TEMP_SetHigh();
				}
				else{
				KEEP_HEAT_LED_OFF();
				RELAY_KEEP_TEMP_SetLow();

			  }

        }
          

      

		

		//
		if(pro_t.gTimer_pro_key > 1){//20ms
           pro_t.gTimer_pro_key =0;
	     if(tpd_t.relay_fan_flag == 1){

			 FAN_LED_ON();//FAN_LED_ON();
					 
		     RELAY_FAN_SetHigh()	 ;

         }
         else{
		   FAN_LED_OFF();//FAN_LED_OFF();
		   RELAY_FAN_SetLow()	;


		 }
	
         if(tpd_t.relay_tape_flag ==1){
		 	 TAPE_LED_ON();
			 RELAY_TAPE_SetHigh();
			 
		 }
		 else{
		 	TAPE_LED_OFF();
            RELAY_TAPE_SetLow();

		 }

		 
	     if(tpd_t.relay_kill_flag ==1){
		 	 KILL_LED_ON();
			 RELAY_KILL_SetHigh();
	        
		 }
		 else{
		 	 KILL_LED_OFF();
			 RELAY_KILL_SetLow();
	
	
		 }

		 
//	    if(tpd_t.relay_keep_temp_flag ==1){
//			KEEP_HEAT_LED_ON();
//	        RELAY_KEEP_TEMP_SetHigh();
//		}
//		else{
//	        KEEP_HEAT_LED_OFF();
//	        RELAY_KEEP_TEMP_SetLow();
//	
//		}
		}

    break;

    }




}


/*********************************************************************************************************
*	函 数 名: static void Run_Display_Handler(uint8_t temp_value)
*	功能说明: App 层 
*			 
*	形    参: 输入按键的键值
*	返 回 值: 无
*********************************************************************************************************/
static void Run_Display_Handler(uint8_t temp_value)
{

    static uint8_t first_power_on;

	switch(temp_value){

	 case 0:
		
	if(tpd_t.gTimer_read_adc >12 || tpd_t.power_on_times < 50){
	  tpd_t.gTimer_read_adc =0;
      if(tpd_t.power_on_times < 10){
           Read_NTC_Temperature_Power_On();
      }
	  else	
	      Read_NTC_Temperature_Value_Handler();
    }
	
	if(tpd_t.gTimer_display > 6 || tpd_t.power_on_times < 50){
      tpd_t.gTimer_display=0; 
	  tpd_t.power_on_times++;
        
         Smg_Display_Temp_Degree_Handler();
		 
		 if( pro_t.set_keep_temp == 1){ //
		      
             if(pro_t.set_keep_tmep_value >= tpd_t.temperature_value ){
				  tpd_t.relay_keep_temp_flag =1;
			       KEEP_HEAT_LED_ON();
	               RELAY_KEEP_TEMP_SetHigh();

			 }
             else{
                  tpd_t.relay_keep_temp_flag =0;
			      KEEP_HEAT_LED_OFF();
	              RELAY_KEEP_TEMP_SetLow();

            }

		 }
        
    }

	if(first_power_on==0){
		first_power_on++;
       KEY_FUN_CONFIRM_LED_ON();

	}

    break;

	case 1:
      Run_Keep_Heat_Setup_Digital_Numbers();
			

	break;
   }

}


