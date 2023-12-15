#include "bsp_ctl.h"
#include "bsp.h"


uint8_t (*relay_a_state)(void);
uint8_t (*relay_b_state)(void);
uint8_t (*relay_c_state)(void);
uint8_t (*relay_d_state)(void);


static uint8_t relay_a_fun(void);
static uint8_t relay_b_fun(void);
static uint8_t relay_c_fun(void);
static uint8_t relay_d_fun(void);





touchpad_t tpd_t;
uint8_t led_on_of_number;

void bsp_ctl_init(void)
{
    Relay_A_Process(relay_a_fun);
	Relay_B_Process(relay_b_fun);
	Relay_C_Process(relay_c_fun);
	Relay_D_Process(relay_d_fun);

}


/***********************************************************
	*
	*Function Name: void Run_InputKey_Model(uint8_t keyvalue)
	*Function: parse of input key value
	*Input Ref: input key of value 
	*Retrun Ref:NO
	*
***********************************************************/
void Run_InputKey_Model(uint8_t keyvalue)
{
   
    static uint8_t confirm_flag;
    switch(tpd_t.read_key_value){
			
	

		case FUNCTION_KEY_PRES: //FUN

	    if(tpd_t.keep_heat_fun_digital_numbers ==1){
              tpd_t.gTimer_keep_heat_fun=0;
			  tpd_t.digital_numbers++; //scope : 0~40度
			  if(tpd_t.digital_numbers>40) tpd_t.digital_numbers=0;
			   Key_Confirm_Handler(KEEP_HEAT_LED);
			   Run_Keep_Heat_Setup_Digital_Numbers();
			   tpd_t.run_process_tag= KEY_FUNCTION_ITEM;

		}
		else{

	     tpd_t.run_process_tag= KEY_FUNCTION_ITEM;
	     if(led_on_of_number == KEEP_HEAT_LED)led_on_of_number =0;
         if(led_on_of_number ==KEY_NULL)led_on_of_number =0;
	     led_on_of_number++;
		 led_t.gTimer_flicker=0;
		 tpd_t.gTimer_select_fun=0;
		   
		  
		}
		break;
	
		case CONFIRM_KEY_PRES: //CONFIRM_KEY

		if(tpd_t.keep_heat_fun_digital_numbers ==1){
              tpd_t.gTimer_keep_heat_fun=0;
			   Key_Confirm_Handler(KEEP_HEAT_LED);
			  tpd_t.digital_numbers--; //scope : 0~40度--; //scope : 0 ~40 degree
			  if(tpd_t.digital_numbers < 0)  tpd_t.digital_numbers=40; //scope : 0~80度=80;
			  Run_Keep_Heat_Setup_Digital_Numbers();
               tpd_t.run_process_tag= KEY_CONFIRM_ITEM;
			  
		}
		else{
             
		      tpd_t.run_process_tag= KEY_CONFIRM_ITEM;
           }
	
		break;

		case KEY_LONG_PRES_CONFIRM_MODE : //long key 
			tpd_t.keep_heat_fun_digital_numbers=0;
            tpd_t.keep_heat_run_flag=3;
			Key_Confirm_Handler(KEEP_HEAT_LED);

		break;

	    
		}

}

/***********************************************************
	*
	*Function Name: void Run_BoardCommand_Handler(void)
	*Function: contiunce of process 
	*Input Ref: NO
	*Retrun Ref:NO
	*
***********************************************************/
void Run_BoardCommand_Handler(void)
{

     static uint8_t keep_heat_confirm;
	 switch(tpd_t.run_process_tag){
         

	 case KEY_FUNCTION_ITEM:
	 
	    Led_Display_Content_Fun(led_on_of_number);

     break;

	 case KEY_CONFIRM_ITEM:
             if(tpd_t.confirm_key_select_item_keep_heat == keep_heat_enable && led_on_of_number ==KEEP_HEAT_LED){
                 tpd_t.confirm_key_select_item_keep_heat= confirm_disable;
                 RELAY_KEEP_TEMP_SetLow();
                 KEEP_HEAT_LED_OFF();
                 tpd_t.keep_heat_fun_digital_numbers =0;
                 led_on_of_number =KEY_NULL;
                 Key_Confirm_Handler(led_on_of_number);

              }
              else if(tpd_t.keep_heat_fun_digital_numbers ==1){
                   
			  	Key_Confirm_Handler(KEEP_HEAT_LED);

			 
			 }
			 else 
               Key_Confirm_Handler(led_on_of_number);
	 

	 break;

	
     }

}




/***********************************************************
	*
	*Function Name: void Relay_A_Process(uint8_t (*relay_a_handler)(void))
	*Function: open of clouse relay
	*Input Ref: NO
	*Retrun Ref: 1->open 0->close
	*
***********************************************************/
void Relay_A_Process(uint8_t (*relay_a_handler)(void))
{
     relay_a_state = relay_a_handler;
}

static uint8_t relay_a_fun(void)
{
   if(tpd_t.relay_fan_flag ==1)
   	return open;
   else 
   	return close;

}

/***********************************************************
	*
	*Function Name: void Relay_A_Process(uint8_t (*relay_a_handler)(void))
	*Function: open of clouse relay
	*Input Ref: NO
	*Retrun Ref: 1->open 0->close
	*
***********************************************************/
void Relay_B_Process(uint8_t (*relay_b_handler)(void))
{
     relay_b_state = relay_b_handler;
}

static uint8_t relay_b_fun(void)
{
   if(tpd_t.relay_tape_flag ==1)
   	return open;
   else 
   	return close;

}

/***********************************************************
	*
	*Function Name: void Relay_A_Process(uint8_t (*relay_a_handler)(void))
	*Function: open of clouse relay
	*Input Ref: NO
	*Retrun Ref: 1->open 0->close
	*
***********************************************************/
void Relay_C_Process(uint8_t (*relay_c_handler)(void))
{
     relay_c_state = relay_c_handler;
}

static uint8_t relay_c_fun(void)
{
   if(tpd_t.relay_kill_flag ==1)
   	return open;
   else 
   	return close;

}

/***********************************************************
	*
	*Function Name: void Relay_A_Process(uint8_t (*relay_a_handler)(void))
	*Function: open of clouse relay
	*Input Ref: NO
	*Retrun Ref: 1->open 0->close
	*
***********************************************************/
void Relay_D_Process(uint8_t (*relay_d_handler)(void))
{
     relay_d_state = relay_d_handler;
}

static uint8_t relay_d_fun(void)
{
   if(tpd_t.relay_keep_temp_flag ==1)
   	return open;
   else 
   	return close;

}





