#include "bsp.h"

main_prcess_t pro_t;

static void Relay_Tunr_OnOff_Fun(uint8_t relay_id_led_flag);






uint8_t relay_id_led ;
uint8_t display_keep_temp_value;
uint8_t  disp_keep_temp_value ;

uint64_t write_flash_datta;

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

   if(pro_t.gTimer_pro_det_dog > 10 ){
 		 pro_t.gTimer_pro_det_dog =0;
 		 Feed_Dog();
		
	}
	
	/* 例如 uIP 协议，可以插入uip轮询函数 */
	//TOUCH_CapScan();
	//IWDG_Detected_Times();
	
}

/*
*********************************************************************************************************
*	函 数 名: Key_Handler(uint8_t pro_t.key_value)
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


     case fun_key:  //fun key 

        switch(pro_t.key_as_numbers_input_flag){

		 case 1: //keep temperature value that as inupt number of key
             pro_t.iwdg_detected_times=0;
			 
			ctl_t.gTimer_select_fun=0;
			disp_keep_temp_value =0xff;
			pro_t.gTimer_pro_disp_temp=0; //display set keep temperature value
			//
			ctl_t.digital_numbers++; //scope : 16~30度
			if(ctl_t.digital_numbers <16)ctl_t.digital_numbers =16;
			if(ctl_t.digital_numbers>30) ctl_t.digital_numbers=30;
			Run_Keep_Heat_Setup_Digital_Numbers(ctl_t.digital_numbers);
			  
			break;

		 case 0: // relay of switch form A to D 
		 pro_t.iwdg_detected_times=0;
		 KEY_FUN_CONFIRM_LED_ON() ; 
         pro_t.key_fun++;
		 
			 
		 if(pro_t.key_fun > 4){
		 	pro_t.key_fun=1;

		 }
         switch(pro_t.key_fun){

		    case relay_a_tape_led:

		        relay_id_led = relay_tape_led_on;
				ctl_t.gTimer_select_fun=0;
				pro_t.fun_key_be_pressing_flag=1;
		 

		   break;
		 
		    case relay_b_fan_led:
				relay_id_led = relay_fan_led_on;
				ctl_t.gTimer_select_fun=0;
				pro_t.key_short_confirm_flag=0;
				pro_t.fun_key_be_pressing_flag=1;
				

		    break;

			case relay_c_kill_led:  // 
				relay_id_led = relay_kill_led_on;
				ctl_t.gTimer_select_fun=0;
				pro_t.key_short_confirm_flag=0;
				 pro_t.fun_key_be_pressing_flag=1;
				
		    break;

			case relay_keep_temp: //keep temperature value

			   
				relay_id_led = relay_keep_temp_led_on;
				ctl_t.gTimer_select_fun=0;
				pro_t.key_short_confirm_flag=0;//WT.EDIT 2023.12.20
				pro_t.fun_key_be_pressing_flag=1;
			 
		    break;

           }
		  break;
		
        }

     
     break;
		
     //confirm key 
	case confirm_short_key: // confirm key

      pro_t.iwdg_detected_times=0;
	  if(pro_t.fun_key_be_pressing_flag==0){ //if don't be pressed "select key(fun key)",display set temp value 
        
	   if( pro_t.set_temp_value_success_flag == 1){ //display has been set keep heat temperatur value .exmalpe "28"
		pro_t.gTimer_pro_disp_temp=0;
        disp_keep_temp_value =1;

		}
		else{ //display "00:00"
		  pro_t.gTimer_pro_disp_temp=0;
		
		  disp_keep_temp_value =2;
		

		}


     }
	 else{// confrim key be used to "confrm" key done .

      switch(relay_id_led){

		 case relay_tape_led_on:

		      if(ctl_t.relay_tape_flag ==0){
				 ctl_t.relay_tape_flag =1;
                  pro_t.key_fun=0;
				  TAPE_LED_ON();
				  RELAY_TAPE_SetHigh();
			  } 
			  else{
				  ctl_t.relay_tape_flag =0;
                   pro_t.key_fun=0;
				   TAPE_LED_OFF(); 
				  RELAY_TAPE_SetLow();
			  } 
		      pro_t.fun_key_be_pressing_flag=0;

		   break;
		 
		    case relay_fan_led_on:
				if(ctl_t.relay_fan_flag==0){
					ctl_t.relay_fan_flag=1;
                     pro_t.key_fun=0;
					 FAN_LED_ON();
					RELAY_FAN_SetHigh();
				}
				else{
					ctl_t.relay_fan_flag=0;
                     pro_t.key_fun=0;
					FAN_LED_OFF(); 
		            RELAY_FAN_SetLow();
				}
				 pro_t.fun_key_be_pressing_flag=0;
			break;

			case relay_kill_led_on:  // 
				if(ctl_t.relay_kill_flag==0){
					ctl_t.relay_kill_flag=1;
                     pro_t.key_fun=0;
					KILL_LED_ON();
		            RELAY_KILL_SetHigh();
				}else{
					ctl_t.relay_kill_flag=0;
                     pro_t.key_fun=0;
					 KILL_LED_OFF(); 
		             RELAY_KILL_SetLow();
				}
				 pro_t.fun_key_be_pressing_flag=0;
				
				
		    break;

			case relay_keep_temp_led_on: //keep temperature value by confirm done.

			  switch(pro_t.set_temp_value_success_flag){

			   case 1: //normal -> cancle 
                 pro_t.key_fun=0; //WT.EDIT 2024.07.11
			     ctl_t.set_keep_heat_tempeature_flag=0; //WT.EDIT.2024.05.17
			 	 pro_t.set_temp_value_success_flag=0;
           
             
				 ctl_t.gTimer_select_fun =10;
				 pro_t.key_short_confirm_flag =1;
			
				 KEY_FUN_CONFIRM_LED_OFF() ;  
			     pro_t.fun_key_be_pressing_flag=0;
				  KEEP_HEAT_LED_OFF();
	              RELAY_KEEP_TEMP_SetLow();
				  KEY_FUN_CONFIRM_LED_ON() ;
				  ADD_DEC_LED_OFF();

			 
			  break;

			 case 0: //confirm key as input numbers key

			  if(pro_t.key_as_numbers_input_flag ==0){
				  pro_t.key_as_numbers_input_flag =1;
				  ctl_t.gTimer_select_fun=0;
		          ADD_DEC_LED_ON();

			  }
			  else{

			    ctl_t.gTimer_select_fun=0;
				pro_t.gTimer_pro_disp_temp=0;
				disp_keep_temp_value = 0xff;
                if(the_first_dec_key==0){
                     the_first_dec_key++;
                     ctl_t.digital_numbers--; //scope : 16~30度
				    if(ctl_t.digital_numbers <16) ctl_t.digital_numbers=30;

                }
                else{
				ctl_t.digital_numbers--; //scope : 16~30度
				if(ctl_t.digital_numbers <16) ctl_t.digital_numbers=16;
                }
				Run_Keep_Heat_Setup_Digital_Numbers(ctl_t.digital_numbers);
			  }
			  
			  break;

		     }
              

		  break;
      }

      
	 }
        
	  break;
	  
     //function key long be pressed 
	 case confirm_long_key: //confirm long by pressed 
         pro_t.iwdg_detected_times=0;
	    if(pro_t.key_as_numbers_input_flag ==1){

	        ctl_t.gTimer_select_fun=20;
			pro_t.key_as_numbers_input_flag =0;
		
			ADD_DEC_LED_OFF();
		
		   pro_t.set_temp_value_success_flag = 1; //set keep temperature is complete.
		   ctl_t.set_keep_heat_tempeature_flag = 1;   //WT.EIDT .2024.05.17 new add item .
		   ctl_t.again_open_relay_ptc=0;  //WT.EDIT .2024.05.20 the first times at once open PTC .
            
		   pro_t.long_key_flag =0; //repeat by pressed key_confirm .
		   disp_keep_temp_value =0;
		   ctl_t.gTimer_read_adc =20;
          
		
		   pro_t.set_keep_temp_value = ctl_t.digital_numbers;
		   if(pro_t.set_keep_temp_value > ctl_t.temperature_value ){
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
		   pro_t.fun_key_be_pressing_flag=0;
		}




	 break;

     case fun_long_key :
        
       SetRelay_TurnOff_Fun();

     


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
   
    
    Relay_Tunr_OnOff_Fun(relay_id_led);

	switch(disp_keep_temp_value){

	   case 0: //works normal temperature value 
			if((ctl_t.gTimer_read_adc >4) && ctl_t.thefirst_detected_temp_falg==1){
			  ctl_t.gTimer_read_adc =0;
		     
			    Read_NTC_Temperature_Value_Handler();
               ctl_t.disp_ntc_res_liner_temp_value = Disp_NtcRes_LinearValue(ctl_t.temperature_value);
			    Smg_Display_Temp_Degree_Handler(ctl_t.disp_ntc_res_liner_temp_value);
		    }

            if(ctl_t.thefirst_detected_temp_falg == 0 && ctl_t.gTimer_read_adc < 6){
                ctl_t.thefirst_detected_temp_falg ++ ;  
                Read_NTC_Temperature_Init_Handler();
               
            }
             
			
           if(pro_t.gTimer_display_relay_led > 3){
			   pro_t.gTimer_display_relay_led =0;
			   Relay_Confirm_Turn_OnOff_Fun();   
			 
          	}

		break;

	   case 1: //
    
		if(pro_t.gTimer_pro_disp_temp <2){

		   Repeat_Keep_Heat_Setup_Digital_Numbers(pro_t.set_keep_temp_value);
		   
		 }
		else{
		   disp_keep_temp_value =0;
		   ctl_t.gTimer_read_adc  =20; //at once return NTC read tempeerature
		   ctl_t.disp_ntc_res_liner_temp_value = Disp_NtcRes_LinearValue(ctl_t.temperature_value);
		   Smg_Display_Temp_Degree_Handler(ctl_t.disp_ntc_res_liner_temp_value);
		}

	   break;

	   case 2: //don't set up keep tempeature is 00 

	       if(pro_t.gTimer_pro_disp_temp <2){
	   	
	          Repeat_Keep_Heat_Setup_Digital_Numbers(0);
	   
		   }
		   else{
			  disp_keep_temp_value =0;
			  ctl_t.gTimer_read_adc  =50;
			 // Smg_Display_Temp_Degree_Handler(ctl_t.temperature_value );
			    ctl_t.disp_ntc_res_liner_temp_value = Disp_NtcRes_LinearValue(ctl_t.temperature_value);
			    Smg_Display_Temp_Degree_Handler(ctl_t.disp_ntc_res_liner_temp_value);
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
static void Relay_Tunr_OnOff_Fun(uint8_t relay_id_led_flag)
{


   switch(relay_id_led_flag){

    
	case relay_tape_led_on:

         //relay_a_tape
		if(ctl_t.gTimer_select_fun < 6 && pro_t.fun_key_be_pressing_flag==1){
		
			Tape_Led_Filcker();
			//Relay_Tape_State();

			Relay_Fan_State();

			Relay_Kill_State();

		
			relay_keep_temperature_led_on_off();

            ctl_t.select_fun_led_blink_flag = 1;
			
			
		}
		else{
            pro_t.key_fun=0; //WT.EDIT 2024.07.11
			pro_t.fun_key_be_pressing_flag=0;
		    pro_t.key_as_numbers_input_flag =0;
		    pro_t.gTimer_pro_key=50; //at once to switch normal relay display led 
		    ctl_t.select_fun_led_blink_flag = 0;
		    //Relay_Tape_State();
		}
		
		if(pro_t.gTimer_pro_key > 20){//200ms
 			pro_t.gTimer_pro_key=0;	
			Relay_Confirm_Turn_OnOff_Fun();
		}
	
		break;


   

    case relay_fan_led_on:


      if(ctl_t.gTimer_select_fun < 6 &&  pro_t.fun_key_be_pressing_flag ==1){
        	Fan_Led_Flicker();//Tape_Led_Filcker();
        	Relay_Tape_State();

			//Relay_Fan_State();

			Relay_Kill_State();

	
			relay_keep_temperature_led_on_off();
            ctl_t.select_fun_led_blink_flag = 1;
	
       }
       else{
           pro_t.key_fun=0; //WT.EDIT 2024.07.11
	   	  ctl_t.select_fun_led_blink_flag = 0;
		  pro_t.fun_key_be_pressing_flag=0;
		  pro_t.key_as_numbers_input_flag =0;
	      pro_t.gTimer_pro_key=50; //at once to switch normal relay display led 
	   	//  Relay_Fan_State();
	   }
	   if(pro_t.gTimer_pro_key > 20){//300ms
	       pro_t.gTimer_pro_key=0;
		  Relay_Confirm_Turn_OnOff_Fun();
	    }		
		break;

	
	case relay_kill_led_on:

		//relay_a_tape
		if(ctl_t.gTimer_select_fun < 6 && pro_t.fun_key_be_pressing_flag==1){
			Sterilization_Led_Filcker();//Fan_Led_Flicker();
			Relay_Tape_State();

			Relay_Fan_State();

			//Relay_Kill_State();

			
			relay_keep_temperature_led_on_off();
            ctl_t.select_fun_led_blink_flag =1;
			
		}
		else{
            pro_t.key_fun=0; //WT.EDIT 2024.07.11
            ctl_t.select_fun_led_blink_flag = 0;
			pro_t.fun_key_be_pressing_flag=0;
			pro_t.key_as_numbers_input_flag =0;
		     pro_t.gTimer_pro_key=50; //at once to switch normal relay display led 
			
		}
		if(pro_t.gTimer_pro_key > 20){//200ms
 			pro_t.gTimer_pro_key=0;
            Relay_Confirm_Turn_OnOff_Fun();
		}
				
      

    break;

	case relay_keep_temp_led_on: //keep temperature be set up value 16~30 degree

	    //KEEP HEAT Display of LED 
        if(ctl_t.gTimer_select_fun < 6 && pro_t.fun_key_be_pressing_flag ==1){

	       if(pro_t.key_as_numbers_input_flag ==0){
		   	
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
            pro_t.key_fun=0; //WT.EDIT 2024.07.11
            ctl_t.select_fun_led_blink_flag = 0;
			pro_t.fun_key_be_pressing_flag =0;
			pro_t.key_as_numbers_input_flag =0;
		    pro_t.gTimer_pro_key=50; //at once to switch normal relay display led 
			ADD_DEC_LED_OFF();
		    KEY_FUN_CONFIRM_LED_ON() ;
			Relay_Keep_Temp_State();
		  }
		  
		if(pro_t.gTimer_pro_key > 20){//300ms
			pro_t.gTimer_pro_key=0;
             Relay_Confirm_Turn_OnOff_Fun();
		}
				
       

    break;

    }

}


