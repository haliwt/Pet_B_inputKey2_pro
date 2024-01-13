#include "bsp.h"

main_prcess_t pro_t;

static void Relay_Tunr_OnOff_Fun(uint8_t relay_id_led_flag);

static void IWDG_Detected_Times(void);
static void Parse_Flash_Read_Data(uint32_t data);




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
   static uint8_t iwdg_times;
   static uint8_t parse_data;
   /* --- 喂狗 */
    if((pro_t.gTimer_pro_feed_dog > 3 && FUN_KEY_VALUE()==KEY_UP && CONFIRM_KEY_VALUE()==KEY_UP) ||(iwdg_times < 4 && pro_t.gTimer_pro_feed_dog > 3 )){
		pro_t.gTimer_pro_feed_dog=0;
		pro_t.gTimer_pro_det_dog =0;
		iwdg_times ++;
	   
	   Feed_Dog();

    }
	else{

	  if(pro_t.gTimer_pro_det_dog > 3 &&  pro_t.iwdg_detected_times < 6){
		 pro_t.gTimer_pro_det_dog =0;
		 Feed_Dog();
		 pro_t.iwdg_detected_times ++;

	    }
	}
		
	/* --- 让CPU进入休眠，由Systick定时中断唤醒或者其他中断唤醒 */
	
    /* 例如 emWin 图形库，可以插入图形库需要的轮询函数 */
	//GUI_Exec();
	
	/* 例如 uIP 协议，可以插入uip轮询函数 */
	//TOUCH_CapScan();
	IWDG_Detected_Times();
	if(parse_data ==0){
		parse_data =1;
		Parse_Flash_Read_Data(pro_t.read_flash_data);
	}
}
/*
*********************************************************************************************************
*	函 数 名: static void IWDG_Detected_Times(void)
*	功能说明: uint64_t write_flash_data: 0x01-relay_tape, 0x02--relay_fan,0x04--relay kill
*			   0x08---relay keep temp . 
*	形    参: 输入按键的键值
*	返 回 值: 无
*********************************************************************************************************
*/
static void IWDG_Detected_Times(void)
{
   
	 static uint8_t  iwdg_flag;
	 uint16_t temp_value;
     if(pro_t.iwdg_detected_times  > 5  && iwdg_flag ==0){
		pro_t.gTimer_pro_feed_dog=0;
		iwdg_flag ++;
		Feed_Dog();
		Quantificat_FlashData_Handler();
		
        if(relay_temp_flag_state()==1){
          // ctl_t.relay_keep_temp_data;// =30;//pro_t.set_keep_tmep_value ;
           ctl_t.relay_keep_temp_data =(uint64_t) pro_t.set_keep_tmep_value ;
		   ctl_t.relay_keep_temp_data = ctl_t.relay_keep_temp_data <<8;
		  write_flash_datta =(ctl_t.relay_flag_flash_data | ctl_t.relay_keep_temp_data);

		}
		else
		  write_flash_datta =(uint64_t)(ctl_t.relay_flag_flash_data);
		
		STM32G030F6P6_Write_Flash_Data(write_flash_datta);
		HAL_Delay(100);
		 
	}

	if(iwdg_flag==1)pro_t.gTimer_pro_feed_dog=0;

    

}
/***********************************************************************************
	*
	*Function Name: static void Parse_Flash_Read_Data(uint32_t data)
	*Function:  tape =0x01,fan=0x02,kill= 0x04,keep_temp =0x08
	*Input Ref: NO
	*Retrun Ref: NO
	*
************************************************************************************/
static void Parse_Flash_Read_Data(uint32_t data)
{
    static uint16_t read_data;
	static uint8_t read_temp_data,read_flash_relay_data;
	if(data > 0){
       
	   read_data = (uint16_t)data;
       read_temp_data = (data >> 8);
	   read_flash_relay_data = (data & 0x00FF);

	   switch(read_flash_relay_data){

	   case 0x01:
	   	ctl_t.relay_tape_flag=1;

	   break;

	   case 0x02:
	   	ctl_t.relay_fan_flag=1;

	   break;

	   case 0x04:
	   	 ctl_t.relay_kill_flag=1;

	   break;

       case 0x08:
	   	
	     pro_t.set_keey_temp_define_flag=1; //0x08
		 pro_t.set_keep_tmep_value = read_temp_data;

	   break;

	   case 0x0F:
	      ctl_t.relay_tape_flag=1;
	      ctl_t.relay_fan_flag=1;
	      ctl_t.relay_kill_flag=1;
		  
		   pro_t.set_keey_temp_define_flag=1; //0x08
		  pro_t.set_keep_tmep_value = read_temp_data;


	   break;

	   case 0x07:
	   	  ctl_t.relay_tape_flag=1; //0x01
	      ctl_t.relay_fan_flag=1;  //0x02
	      ctl_t.relay_kill_flag=1; //0x04
       break;

	    case 0x0B:
	   	
		 ctl_t.relay_tape_flag=1; //0x01
		 ctl_t.relay_fan_flag=1;  //0x02
		
		  pro_t.set_keey_temp_define_flag=1; //0x08
		 
		 pro_t.set_keep_tmep_value = read_temp_data;

	   break;

	   case 0x0D:
	   	 ctl_t.relay_tape_flag=1; //0x01
		 ctl_t.relay_kill_flag=1;  //0x04
		
		  pro_t.set_keey_temp_define_flag=1; //0x08
		 
		 
		 pro_t.set_keep_tmep_value = read_temp_data;

	   break;

	   case 0x03:
	   	 ctl_t.relay_tape_flag=1;
	     ctl_t.relay_fan_flag=1;

	   break;

	   case 0x05:
	   	ctl_t.relay_tape_flag=1; //
	    ctl_t.relay_kill_flag=1;

	   break;

	   case 0x09:
	   	  ctl_t.relay_tape_flag=1; //0x01
	     
		  pro_t.set_keey_temp_define_flag=1; //0x08
		  pro_t.set_keep_tmep_value = read_temp_data;

	   break;
       
	  
       //FAN -RELAY
	   case 0x0E:
	   		 
		  ctl_t.relay_fan_flag=1;  //0x02

		  ctl_t.relay_kill_flag=1;  //0x04
		
		  pro_t.set_keey_temp_define_flag=1; //0x08
		 
		 
		 pro_t.set_keep_tmep_value = read_temp_data;


	   break;

	   case 0x06:
	   	  ctl_t.relay_fan_flag=1;  //0x02

		  ctl_t.relay_kill_flag=1;  //0x04

	   break;

	   case 0x0A:

	     ctl_t.relay_fan_flag=1;  //0x02

		 pro_t.set_keey_temp_define_flag=1; //0x08
		 
		pro_t.set_keep_tmep_value = read_temp_data;

	   break;
       //KILL RELAY 
	   case 0x0C:
	  

		  ctl_t.relay_kill_flag=1;  //0x04
		  

	      pro_t.set_keey_temp_define_flag=1; //0x08
		
		  pro_t.set_keep_tmep_value = read_temp_data;

	   break;

	   }
	   
	}

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
             pro_t.iwdg_detected_times=0;
			 
			ctl_t.gTimer_select_fun=0;
			disp_keep_temp_value =0xff;
			pro_t.gTimer_pro_disp_temp=0; //display set keep temperature timer timing 
			ctl_t.digital_numbers++; //scope : 16~30度
			if(ctl_t.digital_numbers <16)ctl_t.digital_numbers =16;
			if(ctl_t.digital_numbers>30) ctl_t.digital_numbers=30;
			Run_Keep_Heat_Setup_Digital_Numbers(ctl_t.digital_numbers);
			  
			break;

		 case 0:
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
				pro_t.fun_key_counter=1;
		 

		   break;
		 
		    case relay_b_fan_led:
				relay_id_led = relay_fan_led_on;
				ctl_t.gTimer_select_fun=0;
				pro_t.key_short_confirm_flag=0;
				pro_t.fun_key_counter=1;
				

		    break;

			case relay_c_kill_led:  // 
				relay_id_led = relay_kill_led_on;
				ctl_t.gTimer_select_fun=0;
				pro_t.key_short_confirm_flag=0;
				 pro_t.fun_key_counter=1;
				
		    break;

			case relay_keep_temp: //keep temperature value

			   
				relay_id_led = relay_keep_temp_led_on;
				ctl_t.gTimer_select_fun=0;
				pro_t.key_short_confirm_flag=0;//WT.EDIT 2023.12.20
				pro_t.fun_key_counter=1;
			 
		    break;

           }
		  break;
		
        }
     break;
		
     //confirm key 
	case confirm_short_key: // confirm key

      pro_t.iwdg_detected_times=0;
	  if(pro_t.fun_key_counter==0){
        
		if( pro_t.set_keey_temp_define_flag == 1){ //display has been set keep heat temperatur value .
		pro_t.gTimer_pro_disp_temp=0;
		 //pro_t.disp_temp_value=1;
		disp_keep_temp_value =1;

		}
		else{ //display "00:00"
		  pro_t.gTimer_pro_disp_temp=0;
		//  pro_t.disp_temp_value=1;
		  disp_keep_temp_value =2;
		

		}


     }
	 else{

      switch(relay_id_led){

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
		      pro_t.fun_key_counter=0;

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
				 pro_t.fun_key_counter=0;
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
				 pro_t.fun_key_counter=0;
				
				
		    break;

			case relay_keep_temp_led_on: //keep temperature value

			  switch(pro_t.set_keey_temp_define_flag){

			   case 1:
  
			
			 	 pro_t.set_keey_temp_define_flag=0;
                 ctl_t.relay_keep_temp_flag =0;
				 ctl_t.gTimer_select_fun =10;
				 pro_t.key_short_confirm_flag =1;
			
				 KEY_FUN_CONFIRM_LED_OFF() ;  
			     pro_t.fun_key_counter=0;
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
				ctl_t.digital_numbers--; //scope : 16~30度
				if(ctl_t.digital_numbers <16) ctl_t.digital_numbers=16;
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
		
		   pro_t.set_keey_temp_define_flag = 1; //set keep temperature is complete.
		   pro_t.long_key_flag =0; //repeat by pressed key_confirm .
		   disp_keep_temp_value =0;
		   ctl_t.gTimer_read_adc =20;
		
		   pro_t.set_keep_tmep_value = ctl_t.digital_numbers;
			   if(pro_t.set_keep_tmep_value >= ctl_t.temperature_value ){
                   ctl_t.relay_keep_temp_flag =1; //open keep temperature "relay_d" 
			       KEEP_HEAT_LED_ON();
	               RELAY_KEEP_TEMP_SetHigh();
				   KEY_FUN_CONFIRM_LED_ON() ;  
				   ADD_DEC_LED_OFF();

			  }
              else{
                  ctl_t.relay_keep_temp_flag =0;
			      KEEP_HEAT_LED_OFF();
	              RELAY_KEEP_TEMP_SetLow();
				  KEY_FUN_CONFIRM_LED_ON() ;
				   ADD_DEC_LED_OFF();

              }
			  pro_t.fun_key_counter=0;
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
   
    static uint8_t p_disp;
    Relay_Tunr_OnOff_Fun(relay_id_led);

	switch(disp_keep_temp_value){

	   case 0:
			if((ctl_t.gTimer_read_adc >14) ||(p_disp < 3 )){
			  ctl_t.gTimer_read_adc =0;
		      p_disp++;
			    Read_NTC_Temperature_Value_Handler();
				Smg_Display_Temp_Degree_Handler();
		    }
			
			if(ctl_t.gTimer_display > 10 ){
		      ctl_t.gTimer_display=0; 
			
		       Smg_Display_Temp_Degree_Handler();
			   
			}

			if(pro_t.gTimer_display_relay_led > 3){
			   pro_t.gTimer_display_relay_led =0;
			   Relay_Confirm_Turn_OnOff_Fun();
          	}

		break;

	   case 1: //
    
		if(pro_t.gTimer_pro_disp_temp <2){

		   Repeat_Keep_Heat_Setup_Digital_Numbers(pro_t.set_keep_tmep_value);
		   
		 }
		else{
		   disp_keep_temp_value =0;
		   ctl_t.gTimer_read_adc  =20; //at once return NTC read tempeerature
		   Smg_Display_Temp_Degree_Handler();
		}

	   break;

	   case 2: //don't set up keep tempeature is 00 

	       if(pro_t.gTimer_pro_disp_temp <2){
	   	
	          Repeat_Keep_Heat_Setup_Digital_Numbers(0);
	   
		   }
		   else{
			  disp_keep_temp_value =0;
			  ctl_t.gTimer_read_adc  =50;
			  Smg_Display_Temp_Degree_Handler();
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
		if(ctl_t.gTimer_select_fun < 6 && pro_t.fun_key_counter==1){
		
			Tape_Led_Filcker();
			//Relay_Tape_State();

			Relay_Fan_State();

			Relay_Kill_State();

			Relay_Temp_State();
			
			
		}
		else{
			pro_t.fun_key_counter=0;
		    pro_t.key_as_numbers_input_flag =0;
		    pro_t.gTimer_pro_key=50; //at once to switch normal relay display led 
		    //Relay_Tape_State();
		}
		
		if(pro_t.gTimer_pro_key > 20){//200ms
 			pro_t.gTimer_pro_key=0;	
			Relay_Confirm_Turn_OnOff_Fun();
		}
	
		break;


   

    case relay_fan_led_on:


      if(ctl_t.gTimer_select_fun < 6 &&  pro_t.fun_key_counter ==1){
        	Fan_Led_Flicker();//Tape_Led_Filcker();
        	Relay_Tape_State();

			//Relay_Fan_State();

			Relay_Kill_State();

			Relay_Temp_State();
	
       }
       else{
	   	 
		  pro_t.fun_key_counter=0;
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
		if(ctl_t.gTimer_select_fun < 6 && pro_t.fun_key_counter==1){
			Sterilization_Led_Filcker();//Fan_Led_Flicker();
			Relay_Tape_State();

			Relay_Fan_State();

			//Relay_Kill_State();

			Relay_Temp_State();
			
		}
		else{
			pro_t.fun_key_counter=0;
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
        if(ctl_t.gTimer_select_fun < 6 && pro_t.fun_key_counter ==1){

	       if(pro_t.key_as_numbers_input_flag ==0){
		   	
	   	      Keep_Heat_Led_Filcker();
			  Relay_Tape_State();

			  Relay_Fan_State();

			 Relay_Kill_State();
			

			  
	       }
	       else{

			 Keep_heat_SetUp_Led_Filcker();
			 Relay_Tape_State();

			Relay_Fan_State();

			Relay_Kill_State();

			//Relay_Temp_State();
		
		  }
		  
		
       	}
	    else{
			pro_t.fun_key_counter =0;
			pro_t.key_as_numbers_input_flag =0;
		    pro_t.gTimer_pro_key=50; //at once to switch normal relay display led 
			ADD_DEC_LED_OFF();
		    KEY_FUN_CONFIRM_LED_ON() ;
			Relay_Temp_State();
		  }
		  
		if(pro_t.gTimer_pro_key > 20){//300ms
			pro_t.gTimer_pro_key=0;
             Relay_Confirm_Turn_OnOff_Fun();
		}
				
       

    break;

    }

}


