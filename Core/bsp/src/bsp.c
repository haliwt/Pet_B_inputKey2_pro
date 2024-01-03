#include "bsp.h"

main_prcess_t pro_t;

static void Relay_Tunr_OnOff_Fun(uint8_t relay_id_led_flag);
//static void Run_Display_Handler(uint8_t temp_value);




uint8_t relay_id_led ;
uint8_t fun_key_counter,display_keep_temp_value;
uint8_t  disp_keep_temp_value ;
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
    if(pro_t.gTimer_pro_feed_dog > 3 && pro_t.key_value ==0 ){
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
*	函 数 名: Key_Handler(uint8_t pro_t.key_value)
*	功能说明: 中间层 
*			 
*	形    参: 输入按键的键值
*	返 回 值: 无
*********************************************************************************************************
*/
void Key_Handler(uint8_t key_value)
{
 
  switch(key_value){


     case fun_key:  //fun key 

        switch(pro_t.key_as_numbers_input_flag){

		 case 1:

			tpd_t.gTimer_select_fun=0;
			disp_keep_temp_value =0xff;
			pro_t.gTimer_pro_disp_temp=0; //display set keep temperature timer timing 
			tpd_t.digital_numbers++; //scope : 16~30度
			if(tpd_t.digital_numbers <16)tpd_t.digital_numbers =16;
			if(tpd_t.digital_numbers>30) tpd_t.digital_numbers=30;
			Run_Keep_Heat_Setup_Digital_Numbers(tpd_t.digital_numbers);
			  
			break;

		 case 0:
		
		 KEY_FUN_CONFIRM_LED_ON() ; 
         pro_t.key_fun++;
		 if(pro_t.key_fun > 4) pro_t.key_fun=1;
         switch(pro_t.key_fun){

		    case relay_a_tape_led:

		        relay_id_led = relay_tape_led_on;
				tpd_t.gTimer_select_fun=0;
				pro_t.key_short_confirm_flag=0;//WT.EDIT 2023.12.20
				fun_key_counter=1;
		 

		   break;
		 
		    case relay_b_fan:
				relay_id_led = relay_fan_led_on;
				tpd_t.gTimer_select_fun=0;
				pro_t.key_short_confirm_flag=0;
				 fun_key_counter=1;
				

		    break;

			case relay_c_kill:  // 
				relay_id_led = relay_kill_led_on;
				tpd_t.gTimer_select_fun=0;
				pro_t.key_short_confirm_flag=0;
				 fun_key_counter=1;
				
		    break;

			case relay_keep_temp: //keep temperature value

			   
				relay_id_led = relay_keep_temp_led_on;
				tpd_t.gTimer_select_fun=0;
				pro_t.key_short_confirm_flag=0;//WT.EDIT 2023.12.20
				fun_key_counter=1;
			 
		    break;

			default:

			break;

           }
		   break;
		
        }
     break;
		
     //confirm key 
	 case confirm_short_key: // confirm key

	  switch(fun_key_counter){

	     case 1:

	     //special key of fun relay D "keep heap temperature be set up " 
         if(relay_id_led == relay_keep_temp_led_on &&  pro_t.key_as_numbers_input_flag ==0){ //"+" tempeature value 

		     switch(pro_t.set_keey_temp_define_flag){

			   case 1:
  
			
			 	 pro_t.set_keey_temp_define_flag=0;
                 tpd_t.relay_keep_temp_flag =0;
				 tpd_t.gTimer_select_fun =10;
				 pro_t.key_short_confirm_flag =1;
			
				 KEY_FUN_CONFIRM_LED_OFF() ;  

			 
			  break;

			 case 0:
         
			 // tpd_t.relay_keep_temp_flag =1;
			  pro_t.key_as_numbers_input_flag =1;
			  tpd_t.gTimer_select_fun=0;
	          ADD_DEC_LED_ON();
		      fun_key_counter =1;
			  

          
			  break;

		     }

			

	   }


	   if(pro_t.key_as_numbers_input_flag ==1 && relay_id_led == relay_keep_temp_led_on){
	   
          tpd_t.gTimer_select_fun=0;
		  pro_t.gTimer_pro_disp_temp=0;
          disp_keep_temp_value = 0xff;
		  tpd_t.digital_numbers--; //scope : 16~30度
		  if(tpd_t.digital_numbers <16) tpd_t.digital_numbers=16;
		  Run_Keep_Heat_Setup_Digital_Numbers(tpd_t.digital_numbers);
		}
		else{
			tpd_t.gTimer_select_fun=0;
	    	pro_t.key_short_confirm_flag =1;
		}
	

	  break;

	  case 0: //don't touch fun key and the first touch confirm key
	 

          if( pro_t.set_keey_temp_define_flag == 1){ //display has been set keep heat temperatur value .
		  	  pro_t.gTimer_pro_disp_temp=0;
		      disp_keep_temp_value =1;

          }
		  else{ //display "00:00"
		  	  pro_t.gTimer_pro_disp_temp=0;
			 disp_keep_temp_value =2;

		  }


	    break;

	    }

	 break;
     //function key long be pressed 
	 case confirm_long_key: //confirm long by pressed 

	    if(pro_t.key_as_numbers_input_flag ==1){

	        tpd_t.gTimer_select_fun=20;
			pro_t.key_as_numbers_input_flag =0;
		
			ADD_DEC_LED_OFF();
		
		   pro_t.set_keey_temp_define_flag = 1; //set keep temperature is complete.
		   pro_t.long_key_flag =0; //repeat by pressed key_confirm .
		   disp_keep_temp_value =0;
		   tpd_t.gTimer_read_adc =20;
		
		   pro_t.set_keep_tmep_value = tpd_t.digital_numbers;
			   if(pro_t.set_keep_tmep_value >= tpd_t.temperature_value ){
                   tpd_t.relay_keep_temp_flag =1; //open keep temperature "relay_d" 
			       KEEP_HEAT_LED_ON();
	               RELAY_KEEP_TEMP_SetHigh();
				   KEY_FUN_CONFIRM_LED_ON() ;  
				   ADD_DEC_LED_OFF();

			  }
              else{
                  tpd_t.relay_keep_temp_flag =0;
			      KEEP_HEAT_LED_OFF();
	              RELAY_KEEP_TEMP_SetLow();
				  KEY_FUN_CONFIRM_LED_ON() ;
				   ADD_DEC_LED_OFF();

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
   
	Relay_Tunr_OnOff_Fun(relay_id_led);

	switch(disp_keep_temp_value){

	   case 0:
			if(tpd_t.gTimer_read_adc >12 ){
			  tpd_t.gTimer_read_adc =0;
		     
			    Read_NTC_Temperature_Value_Handler();
				Smg_Display_Temp_Degree_Handler();
		    }
			
			if(tpd_t.gTimer_display > 15 ){
		      tpd_t.gTimer_display=0; 
			
		       Smg_Display_Temp_Degree_Handler();
				
			}

		break;

	   case 1: //
    
		if(pro_t.gTimer_pro_disp_temp < 3){
		    Repeat_Keep_Heat_Setup_Digital_Numbers(pro_t.set_keep_tmep_value);

	    }
		else{
		   disp_keep_temp_value =0;
		   tpd_t.gTimer_read_adc  =20;
			Smg_Display_Temp_Degree_Handler();
		}

	   break;

	   case 2: //don't set up keep tempeature is 00 
	   	
	       if(pro_t.gTimer_pro_disp_temp < 3){
			  Repeat_Keep_Heat_Setup_Digital_Numbers(0);
	   
		   }
		   else{
			  disp_keep_temp_value =0;
			   tpd_t.gTimer_read_adc  =20;
			   Smg_Display_Temp_Degree_Handler();
		   }


	   break;

	   }

}



/*********************************************************************************************************
*	函 数 名: void Main_Process(void)
*	功能说明: App 层 
*			 
*	形    参: 输入按键的键值
*	返 回 值: 无
*********************************************************************************************************/
static void Relay_Tunr_OnOff_Fun(uint8_t relay_id_led_flag)
{


   switch(relay_id_led_flag){

    
	case relay_tape_led_on:

         //relay_a_tape
		if(tpd_t.gTimer_select_fun < 6 && pro_t.key_short_confirm_flag ==0 ){
			if(tpd_t.relay_keep_temp_flag ==0){
				KEEP_HEAT_LED_OFF();
				RELAY_KEEP_TEMP_SetLow();
		
			}
			Tape_Led_Filcker();
			
		}
		else{
			 tpd_t.gTimer_select_fun =10;
			 fun_key_counter=0;
			 pro_t.key_as_numbers_input_flag =0;
			 
		  if(pro_t.key_short_confirm_flag ==1){
				pro_t.key_short_confirm_flag =0;
				
				if(tpd_t.relay_tape_flag ==0){
					tpd_t.relay_tape_flag =1;
					TAPE_LED_ON();
					RELAY_TAPE_SetHigh();

				}
				else {

					tpd_t.relay_tape_flag =0;
					TAPE_LED_OFF(); 
					RELAY_TAPE_SetLow();

	           }
		   }
		   else{
             if(pro_t.gTimer_pro_key > 20){//30ms){
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
		}

		//
	    if(pro_t.gTimer_pro_key > 20){//20ms
				pro_t.gTimer_pro_key =0;
			 if(tpd_t.relay_fan_flag == 1){
	
				FAN_LED_ON(); 
						 
		
				RELAY_FAN_SetHigh();
				
	
			 }
			 else{
			  FAN_LED_OFF();
			
			  RELAY_FAN_SetLow();
			  
	
	
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


   

    case relay_fan_led_on:


      if(tpd_t.gTimer_select_fun < 6 &&  pro_t.key_short_confirm_flag ==0){
        	Fan_Led_Flicker();//Tape_Led_Filcker();
	
       }
       else{
	   	  tpd_t.gTimer_select_fun =10;
		  fun_key_counter=0;
		  pro_t.key_as_numbers_input_flag =0;


		
				
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

			 if(pro_t.gTimer_pro_key > 20){//30ms
			 if(tpd_t.relay_fan_flag ==1){
					
				FAN_LED_ON();
				RELAY_FAN_SetHigh();
			}
			else if(tpd_t.relay_fan_flag ==0){
			 
			
			 FAN_LED_OFF() ;
			 RELAY_FAN_SetLow();
			  
			}
			}
          }
		}
     
	
      

    if(pro_t.gTimer_pro_key > 20){//300ms
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

	
	case relay_kill_led_on:

		//relay_a_tape
		if(tpd_t.gTimer_select_fun < 6 && pro_t.key_short_confirm_flag ==0){
			Sterilization_Led_Filcker();//Fan_Led_Flicker();
		}
		else{

			tpd_t.gTimer_select_fun =10;
			fun_key_counter=0;
			pro_t.key_as_numbers_input_flag =0;


			if(pro_t.key_short_confirm_flag ==1){
				pro_t.key_short_confirm_flag =0;

				if(tpd_t.relay_kill_flag==0){

					tpd_t.relay_kill_flag=1;
					KILL_LED_ON();//FAN_LED_ON();
					RELAY_KILL_SetHigh()	 ;//RELAY_FAN_SetHigh();

				}
				else{
					tpd_t.relay_kill_flag=0;

					KILL_LED_OFF();//FAN_LED_OFF();
					RELAY_KILL_SetLow()	 ;//RELAY_FAN_SetLow();


				}
			}
			else{
				if(pro_t.gTimer_pro_key > 20){//200ms
				if(tpd_t.relay_kill_flag ==1){
					KILL_LED_ON();
					RELAY_KILL_SetHigh()	 ;//RELAY_FAN_SetHigh();

					}
					else{
						KILL_LED_OFF();
						RELAY_KILL_SetLow()	 ;//RELAY_FAN_SetLow();


					}
				}
			 }

	   }
			
		if(pro_t.gTimer_pro_key > 20){//200ms
			pro_t.gTimer_pro_key =0;
		if(tpd_t.relay_fan_flag == 1){

			FAN_LED_ON(); 
			RELAY_FAN_SetHigh();


		}
		else{
			FAN_LED_OFF();
		
			RELAY_FAN_SetLow();
		}

		if(tpd_t.relay_tape_flag ==1){
			TAPE_LED_ON();
		
			RELAY_TAPE_SetHigh();

		}
		else{
			TAPE_LED_OFF();
			
			RELAY_TAPE_SetLow();

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

	case relay_keep_temp_led_on: //keep temperature be set up value 16~30 degree

	    //KEEP HEAT Display of LED 
       if(tpd_t.gTimer_select_fun < 6 && pro_t.key_short_confirm_flag ==0){

	       if(pro_t.key_as_numbers_input_flag ==0){
		   	
	   	      Keep_Heat_Led_Filcker();
	       }
	       else{

			 Keep_heat_SetUp_Led_Filcker();
		  }
		  
		
       	}
        else{
			tpd_t.gTimer_select_fun=20;
            pro_t.key_as_numbers_input_flag =0;
		    fun_key_counter=0;
		
			
			pro_t.long_key_flag =0;
		    pro_t.key_short_confirm_flag=0;
		    ADD_DEC_LED_OFF();
		    KEY_FUN_CONFIRM_LED_ON() ;
			
			if(pro_t.gTimer_pro_select > 20){
				pro_t.gTimer_pro_select=0;

				if(tpd_t.relay_keep_temp_flag ==1){
					KEEP_HEAT_LED_ON();
					RELAY_KEEP_TEMP_SetHigh();
				}
				else if(tpd_t.relay_keep_temp_flag ==0){
					KEEP_HEAT_LED_OFF();
					RELAY_KEEP_TEMP_SetLow();

			  }
		  }

        }
          

      

		

		//
		if(pro_t.gTimer_pro_key > 20){//20ms
           pro_t.gTimer_pro_key =0;

		//fan_led_relay
	   	 if(tpd_t.relay_fan_flag == 1){
	
				FAN_LED_ON(); //TAPE_LED_ON();//TAPE_LED_ON();
						 
				// RELAY_KILL_SetHigh()	 ;
				RELAY_FAN_SetHigh();
				
	
			 }
			 else{
			  FAN_LED_OFF();// TAPE_LED_OFF();//TAPE_LED_OFF();
			  // RELAY_KILL_SetLow()	;
			  RELAY_FAN_SetLow();
			  
	
	
			 }
       //tape_led_relay
		 if(tpd_t.relay_tape_flag ==1){
			 TAPE_LED_ON();//FAN_LED_ON();
			// RELAY_FAN_SetHigh();
			RELAY_TAPE_SetHigh();
			 
		 }
		 else{
			TAPE_LED_OFF();//FAN_LED_OFF();
		   // RELAY_FAN_SetLow();
		   RELAY_TAPE_SetLow();
		 
	   }

		//kill_led_relay
		  if(tpd_t.relay_kill_flag ==1){
		 	 KILL_LED_ON();
			// RELAY_TAPE_SetHigh();
			RELAY_KILL_SetHigh();
	        
		 }
		 else{
		 	 KILL_LED_OFF();
			// RELAY_TAPE_SetLow();
			RELAY_KILL_SetLow();
	
	
		 }

		 
	   
	 }

    break;

    }

}





