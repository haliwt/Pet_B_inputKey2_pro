#include "bsp.h"

main_prcess_t gpro_t;









uint8_t display_keep_temp_value;
uint8_t  disp_keep_temp_value ;

uint64_t write_flash_datta;

uint8_t keep_heat_temp_number_flag;

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
  #if OLDER_VERSION
   static uint8_t iwdg_times;
   static uint8_t parse_data;
   #endif 
   /* --- 喂狗 */

   if(gpro_t.gTimer_pro_det_dog > 10 ){
 		 gpro_t.gTimer_pro_det_dog =0;
 		 Feed_Dog();
		
	}
	
	/* 例如 uIP 协议，可以插入uip轮询函数 */
	//TOUCH_CapScan();
	//IWDG_Detected_Times();
	
}

/*
*********************************************************************************************************
*	函 数 名: Key_Handler(uint8_t gpro_t.key_value)
*	功能说明: 中间层 
*			 
*	形    参: 输入按键的键值
*	返 回 值: 无
*********************************************************************************************************
*/
void Key_Handler(uint8_t key_value)
{
 
  static uint8_t the_first_dec_key;


  switch(key_value){


     case fun_key:  //fun -sekect key 

        switch(gpro_t.key_as_numbers_input_flag){

		 case 1: //keep temperature value that as inupt number of key
             gpro_t.iwdg_detected_times=0;
			 
			ctl_t.gTimer_select_fun=0;
			disp_keep_temp_value =0xff;
			gpro_t.gTimer_pro_disp_temp=0; //display set keep temperature value
			//
			ctl_t.set_digital_numbers++; //scope : 16~30度
			if(ctl_t.set_digital_numbers <16)ctl_t.set_digital_numbers =16;
			if(ctl_t.set_digital_numbers>30) ctl_t.set_digital_numbers=30;

            gpro_t.gTimer_counter_exit_select_fun =0;
            keep_heat_temp_number_flag=1;
			Run_Keep_Heat_Setup_Digital_Numbers(ctl_t.set_digital_numbers);
			  
			break;

		 case 0: // relay by select  from A to D --normal function is selection 
		 
		 gpro_t.iwdg_detected_times=0;
		 KEY_FUN_CONFIRM_LED_ON() ; 
         gpro_t.key_fun++;
		 
			 
		 if(gpro_t.key_fun > 4){
		 	gpro_t.key_fun=1;

		 }
         switch(gpro_t.key_fun){

		    case relay_a_tape_led:

		        gpro_t.relay_id_led = relay_tape_led_on;
				ctl_t.gTimer_select_fun=0;
          
                gpro_t.gTimer_counter_exit_select_fun =0;
				gpro_t.fun_key_be_pressing_flag=1;
		 

		   break;
		 
		    case relay_b_fan_led:
				gpro_t.relay_id_led = relay_fan_led_on;
				ctl_t.gTimer_select_fun=0;
			
                gpro_t.gTimer_counter_exit_select_fun =0;
				gpro_t.fun_key_be_pressing_flag=1;
				

		    break;

			case relay_c_kill_led:  // 
				gpro_t.relay_id_led = relay_kill_led_on;
				ctl_t.gTimer_select_fun=0;
			
                gpro_t.gTimer_counter_exit_select_fun =0;
				 gpro_t.fun_key_be_pressing_flag=1;
				
		    break;

			case relay_keep_temp: //keep temperature value

			   
				gpro_t.relay_id_led = relay_keep_temp_led_on;
				ctl_t.gTimer_select_fun=0;
			
				gpro_t.gTimer_counter_exit_select_fun =0;
				gpro_t.fun_key_be_pressing_flag=1;
			 
		    break;

           }
		  break;
		
        }

     
     break;
		
     //confirm key 
	case confirm_short_key: // confirm key

      gpro_t.iwdg_detected_times=0;
	  if(gpro_t.fun_key_be_pressing_flag==0){ //if don't be pressed "select key(fun key)",display set temp value 
        
	   if(gpro_t.set_temp_value_success_flag == 1){ //display has been set keep heat temperatur value .exmalpe "28"
		gpro_t.gTimer_pro_disp_temp=0;
        disp_keep_temp_value =1;

		}
		else{ //display "00:00"
		  gpro_t.gTimer_pro_disp_temp=0;
		
		  disp_keep_temp_value =2;
		

		}


     }
	 else{// confrim key be used to "confrm" key done .---> confirm wich led of by sure done.

      switch(gpro_t.relay_id_led){

		 case relay_tape_led_on:

		      if(ctl_t.relay_tape_flag ==0){
				 ctl_t.relay_tape_flag =1;
               
				  TAPE_LED_ON();
				  RELAY_TAPE_SetHigh();
			  } 
			  else{
				  ctl_t.relay_tape_flag =0;
				   TAPE_LED_OFF(); 
				  RELAY_TAPE_SetLow();
			  } 
		      gpro_t.fun_key_be_pressing_flag=0;
              gpro_t.gTimer_counter_exit_select_fun =0;

		   break;
		 
		    case relay_fan_led_on:
				if(ctl_t.relay_fan_flag==0){
					ctl_t.relay_fan_flag=1;
					 FAN_LED_ON();
					RELAY_FAN_SetHigh();
				}
				else{
					ctl_t.relay_fan_flag=0;
               
					FAN_LED_OFF(); 
		            RELAY_FAN_SetLow();
				}
				 gpro_t.fun_key_be_pressing_flag=0;
                 gpro_t.gTimer_counter_exit_select_fun =0;
			break;

			case relay_kill_led_on:  // 
				if(ctl_t.relay_kill_flag==0){
					ctl_t.relay_kill_flag=1;
                     
					KILL_LED_ON();
		            RELAY_KILL_SetHigh();
				}else{
					ctl_t.relay_kill_flag=0;
                     
					 KILL_LED_OFF(); 
		             RELAY_KILL_SetLow();
				}
				 gpro_t.fun_key_be_pressing_flag=0;
				 gpro_t.gTimer_counter_exit_select_fun =0;
				
		    break;

			case relay_keep_temp_led_on: //keep temperature value by confirm done.

			  switch(gpro_t.set_temp_value_success_flag){


			   case 1: //normal -> cancle has been set temperature value .
                
			     ctl_t.set_keep_heat_tempeature_flag=0; //WT.EDIT.2024.05.17
			 	 gpro_t.set_temp_value_success_flag=0;
           
             
				 ctl_t.gTimer_select_fun =10;
				
			
				 KEY_FUN_CONFIRM_LED_OFF() ;  
			     gpro_t.fun_key_be_pressing_flag=0;
				  KEEP_HEAT_LED_OFF();
	              RELAY_KEEP_TEMP_SetLow();
				  KEY_FUN_CONFIRM_LED_ON() ;
				  ADD_DEC_LED_OFF();
                  gpro_t.gTimer_counter_exit_select_fun =0;
			 
			  break;

			 case 0: //confirm key as input numbers key

			  if(gpro_t.key_as_numbers_input_flag ==0){
				  gpro_t.key_as_numbers_input_flag =1;
				  ctl_t.gTimer_select_fun=0;
                  gpro_t.gTimer_counter_exit_select_fun =0;
		          ADD_DEC_LED_ON();

			  }
			  else{

			    ctl_t.gTimer_select_fun=0;
				gpro_t.gTimer_pro_disp_temp=0;
                gpro_t.gTimer_counter_exit_select_fun =0;
				disp_keep_temp_value = 0xff;
                if(the_first_dec_key==0){
                     the_first_dec_key++;
                     ctl_t.set_digital_numbers--; //scope : 16~30度
				    if(ctl_t.set_digital_numbers <16) ctl_t.set_digital_numbers=30;

                }
                else{
    				ctl_t.set_digital_numbers--; //scope : 16~30度
    				if(ctl_t.set_digital_numbers <16) ctl_t.set_digital_numbers=16;
                }
                keep_heat_temp_number_flag=2;
				Run_Keep_Heat_Setup_Digital_Numbers(ctl_t.set_digital_numbers);
			  }

             
			  
			  break;

		     }
              

		  break;
      }

      
	 }
        
	  break;
	  
	 case confirm_long_key: //confirm long by pressed 
         gpro_t.iwdg_detected_times=0;
	    if(gpro_t.key_as_numbers_input_flag ==1){

	        ctl_t.gTimer_select_fun=20;
			gpro_t.key_as_numbers_input_flag =0;
		
			ADD_DEC_LED_OFF();
		
		   gpro_t.set_temp_value_success_flag = 1; //set keep temperature is complete.
		   ctl_t.set_keep_heat_tempeature_flag = 1;   //WT.EIDT .2024.05.17 new add item .
		   ctl_t.again_open_relay_ptc=0;  //WT.EDIT .2024.05.20 the first times at once open PTC .
            
		   gpro_t.long_key_flag =0; //repeat by pressed key_confirm .
		   disp_keep_temp_value =0;
		   ctl_t.gTimer_read_adc =20;
          
		   if(keep_heat_temp_number_flag > 0){ //WT.EDIT 2024.09.12
                keep_heat_temp_number_flag=0;
    		    gpro_t.set_keep_temp_value = ctl_t.set_digital_numbers;
            }
            else{

               if(ctl_t.disp_ntc_res_liner_temp_value > 30)ctl_t.disp_ntc_res_liner_temp_value =30;
               else if(ctl_t.disp_ntc_res_liner_temp_value <16 )ctl_t.disp_ntc_res_liner_temp_value =16;
               
               gpro_t.set_keep_temp_value = ctl_t.disp_ntc_res_liner_temp_value;

            }

           
		   if(gpro_t.set_keep_temp_value > ctl_t.temperature_value ){
                   KEEP_HEAT_LED_ON();
	               RELAY_KEEP_TEMP_SetHigh();
				   KEY_FUN_CONFIRM_LED_ON() ;  
				   ADD_DEC_LED_OFF();

			}
            else{
              
			      KEEP_HEAT_LED_OFF();
	              RELAY_KEEP_TEMP_SetLow();
				  KEY_FUN_CONFIRM_LED_ON() ;
				  ADD_DEC_LED_OFF();

            }
		   gpro_t.fun_key_be_pressing_flag=0;

           gpro_t.gTimer_counter_exit_select_fun = 0;
		}




	 break;

     case fun_long_key_child_lock : //all function be shut off
        
      SetRelay_All_TurnOff_Fun();

      gpro_t.gTimer_counter_exit_select_fun = 40; //at once to switch form raly A number: 1.


     break;

     case mixture_long_key: //mixture_long_key

        set_all_on_or_off_fun();

       gpro_t.gTimer_counter_exit_select_fun = 40; //at once to switch form raly A number: 1.

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
   switch(disp_keep_temp_value){

	   case 0: //works normal temperature value 

          
           
			if((ctl_t.gTimer_read_adc >2) && ctl_t.thefirst_detected_temp_falg==1){
			  ctl_t.gTimer_read_adc =0;
		     
               Read_NTC_Temperature_Value_Handler();
               ctl_t.disp_ntc_res_liner_temp_value = Disp_NtcRes_LinearValue(ctl_t.temperature_value);
                Smg_Display_Temp_Degree_Handler(ctl_t.disp_ntc_res_liner_temp_value);
                 
		    }

            if(ctl_t.thefirst_detected_temp_falg ==0 ){
                ctl_t.thefirst_detected_temp_falg ++ ;  
                Read_NTC_Temperature_Init_Handler();
               ctl_t.gTimer_read_adc =30;
            }


           if(gpro_t.gTimer_display_relay_led > 3){
			   gpro_t.gTimer_display_relay_led =0;
			   Relay_Confirm_Turn_OnOff_Fun();   
			 
          	}

		break;

	   case 1: //
    
		if(gpro_t.gTimer_pro_disp_temp <2){

		   disp_set_temp_value_led_blink(gpro_t.set_keep_temp_value);
		   
		 }
		else{
		   disp_keep_temp_value =0;
		   ctl_t.gTimer_read_adc  =20; //at once return NTC read tempeerature
		   ctl_t.disp_ntc_res_liner_temp_value = Disp_NtcRes_LinearValue(ctl_t.temperature_value);
           if(gpro_t.child_lock_flag ==0){
		      Smg_Display_Temp_Degree_Handler(ctl_t.disp_ntc_res_liner_temp_value);
            }
            else{
               Smg_Display_Temp_Degree_And_Char_L_Handler(ctl_t.disp_ntc_res_liner_temp_value);
            }
		}

	   break;

	   case 2: //don't set up keep tempeature is 00 

	       if(gpro_t.gTimer_pro_disp_temp <2){
	   	
	          disp_set_temp_value_led_blink(0);
	   
		   }
		   else{
			  disp_keep_temp_value =0;
			  ctl_t.gTimer_read_adc  =50;
			 // Smg_Display_Temp_Degree_Handler(ctl_t.temperature_value );
			    ctl_t.disp_ntc_res_liner_temp_value = Disp_NtcRes_LinearValue(ctl_t.temperature_value);
                if(gpro_t.child_lock_flag ==0){
			      Smg_Display_Temp_Degree_Handler(ctl_t.disp_ntc_res_liner_temp_value);
                }
                else{

                    Smg_Display_Temp_Degree_And_Char_L_Handler(ctl_t.disp_ntc_res_liner_temp_value);


                }
		   }


	   break;

       

	   }

}



/********************************************************************************
*
*	函 数 名: void Main_Process(void)
*	功能说明: App 层 
*			 
*	形    参: 输入按键的键值
*	返 回 值: 无
*
*********************************************************************************/
void Relay_Tunr_OnOff_Fun(uint8_t relay_id_led_flag)
{


   switch(relay_id_led_flag){

    
	case relay_tape_led_on:

         //relay_a_tape
		if(ctl_t.gTimer_select_fun < 6 && gpro_t.fun_key_be_pressing_flag==1){
		
			Tape_Led_Filcker();
			//Relay_Tape_State();

			Relay_Fan_State();

			Relay_Kill_State();

		
			relay_keep_temperature_led_on_off();

            ctl_t.select_fun_led_blink_flag = 1;
			
			
		}
		else{

			gpro_t.fun_key_be_pressing_flag=0;
		    gpro_t.key_as_numbers_input_flag =0;
		    gpro_t.gTimer_pro_key=50; //at once to switch normal relay display led 
		    ctl_t.select_fun_led_blink_flag = 0;
		    
		}
		
		if(gpro_t.gTimer_pro_key > 20 && ctl_t.select_fun_led_blink_flag == 0){//200ms
 			gpro_t.gTimer_pro_key=0;	
			Relay_Confirm_Turn_OnOff_Fun();
		}
	
		break;


   

    case relay_fan_led_on:


      if(ctl_t.gTimer_select_fun < 6 &&  gpro_t.fun_key_be_pressing_flag ==1){
        	Fan_Led_Flicker();//Tape_Led_Filcker();
        	Relay_Tape_State();

			//Relay_Fan_State();

			Relay_Kill_State();

	
			relay_keep_temperature_led_on_off();
            ctl_t.select_fun_led_blink_flag = 1;
	
       }
       else{
 
	   	  ctl_t.select_fun_led_blink_flag = 0;
		  gpro_t.fun_key_be_pressing_flag=0;
		  gpro_t.key_as_numbers_input_flag =0;
	      gpro_t.gTimer_pro_key=50; //at once to switch normal relay display led 
	   	  
	   }
       
	   if(gpro_t.gTimer_pro_key > 20 && ctl_t.select_fun_led_blink_flag == 0){//300ms
	       gpro_t.gTimer_pro_key=0;
		  Relay_Confirm_Turn_OnOff_Fun();
	    }		
		break;

	
	case relay_kill_led_on:

		//relay_a_tape
		if(ctl_t.gTimer_select_fun < 6 && gpro_t.fun_key_be_pressing_flag==1){
			Sterilization_Led_Filcker();//Fan_Led_Flicker();
			Relay_Tape_State();

			Relay_Fan_State();

			//Relay_Kill_State();

			
			relay_keep_temperature_led_on_off();
            ctl_t.select_fun_led_blink_flag =1;
			
		}
		else{
           
            ctl_t.select_fun_led_blink_flag = 0;
			gpro_t.fun_key_be_pressing_flag=0;
			gpro_t.key_as_numbers_input_flag =0;
		     gpro_t.gTimer_pro_key=50; //at once to switch normal relay display led 
			
		}
		if(gpro_t.gTimer_pro_key > 20 && ctl_t.select_fun_led_blink_flag == 0){//200ms
 			gpro_t.gTimer_pro_key=0;
            Relay_Confirm_Turn_OnOff_Fun();
		}
				
      

    break;

	case relay_keep_temp_led_on: //keep temperature be set up value 16~30 degree

	    //KEEP HEAT Display of LED 
        if(ctl_t.gTimer_select_fun < 6 && gpro_t.fun_key_be_pressing_flag ==1){

	       if(gpro_t.key_as_numbers_input_flag ==0){
		   	
	   	      Keep_Heat_Led_Filcker();
			  Relay_Tape_State();

			  Relay_Fan_State();

			 Relay_Kill_State();
             ctl_t.select_fun_led_blink_flag = 1;
			

			  
	       }
	       else{
             
			 Keep_heat_SetUp_Led_Filcker();
			 Relay_Tape_State();

			Relay_Fan_State();

			Relay_Kill_State();
            ctl_t.select_fun_led_blink_flag = 1;

			
		
		  }
		  
		
       	}
	    else{
          
            ctl_t.select_fun_led_blink_flag = 0;
			gpro_t.fun_key_be_pressing_flag =0;
			gpro_t.key_as_numbers_input_flag =0;
		    gpro_t.gTimer_pro_key=50; //at once to switch normal relay display led 
			ADD_DEC_LED_OFF();
		    KEY_FUN_CONFIRM_LED_ON() ;
			Relay_Keep_Temp_State();
		  }
		  
		if(gpro_t.gTimer_pro_key > 20 && ctl_t.select_fun_led_blink_flag == 0){//300ms
			gpro_t.gTimer_pro_key=0;
             Relay_Confirm_Turn_OnOff_Fun();
		}
				
       

    break;

    }

}


void exit_select_position_flag(void)
{
    if(gpro_t.gTimer_counter_exit_select_fun > 29){
        gpro_t.key_fun=0;    

    }


}

void confirm_key_long_fun(void)
{
    gpro_t.iwdg_detected_times=0;
	    if(gpro_t.key_as_numbers_input_flag ==1){

	        ctl_t.gTimer_select_fun=20;
			gpro_t.key_as_numbers_input_flag =0;
		
			ADD_DEC_LED_OFF();
		
		   gpro_t.set_temp_value_success_flag = 1; //set keep temperature is complete.
		   ctl_t.set_keep_heat_tempeature_flag = 1;   //WT.EIDT .2024.05.17 new add item .
		   ctl_t.again_open_relay_ptc=0;  //WT.EDIT .2024.05.20 the first times at once open PTC .
            
		   gpro_t.long_key_flag =0; //repeat by pressed key_confirm .
		   disp_keep_temp_value =0;
		   ctl_t.gTimer_read_adc =20;
          
		   if(keep_heat_temp_number_flag > 0){ //WT.EDIT 2024.09.12
                keep_heat_temp_number_flag=0;
    		    gpro_t.set_keep_temp_value = ctl_t.set_digital_numbers;
            }
            else{

               if(ctl_t.disp_ntc_res_liner_temp_value > 30)ctl_t.disp_ntc_res_liner_temp_value =30;
               else if(ctl_t.disp_ntc_res_liner_temp_value <16 )ctl_t.disp_ntc_res_liner_temp_value =16;
               
               gpro_t.set_keep_temp_value = ctl_t.disp_ntc_res_liner_temp_value;

            }

           
		   if(gpro_t.set_keep_temp_value > ctl_t.temperature_value ){
                   KEEP_HEAT_LED_ON();
	               RELAY_KEEP_TEMP_SetHigh();
				   KEY_FUN_CONFIRM_LED_ON() ;  
				   ADD_DEC_LED_OFF();

			}
            else{
              
			      KEEP_HEAT_LED_OFF();
	              RELAY_KEEP_TEMP_SetLow();
				  KEY_FUN_CONFIRM_LED_ON() ;
				  ADD_DEC_LED_OFF();

            }
		   gpro_t.fun_key_be_pressing_flag=0;

           gpro_t.gTimer_counter_exit_select_fun = 0;
		}

}

