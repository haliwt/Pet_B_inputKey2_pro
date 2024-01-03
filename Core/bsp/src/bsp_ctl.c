#include "bsp_ctl.h"
#include "bsp.h"


uint8_t (*relay_tape_state)(void);
uint8_t (*relay_fan_state)(void);
uint8_t (*relay_kill_state)(void);
uint8_t (*relay_temp_state)(void);


static uint8_t relay_tape_fun(void);
static uint8_t relay_fan_fun(void);
static uint8_t relay_kill_fun(void);
static uint8_t relay_temp_fun(void);





touchpad_t tpd_t;
uint8_t led_on_of_number;

void bsp_ctl_init(void)
{
    Relay_Tape_Process(relay_tape_fun);
	Relay_Fan_Process(relay_fan_fun);
	Relay_Kill_Process(relay_kill_fun);
	Relay_Temp_Process(relay_temp_fun);

}


/***********************************************************
	*
	*Function Name:uint8_t relay_tape_fun(void)
	*Function: open of clouse relay
	*Input Ref: NO
	*Retrun Ref: 1->open 0->close
	*
***********************************************************/
static uint8_t relay_tape_fun(void)
{
     if(tpd_t.relay_tape_flag==1)return 1;
	 else return 0;

}
/***********************************************************
	*
	*Function Name:static uint8_t relay_kill_fun(void) 
	*Function: open of clouse relay
	*Input Ref: NO
	*Retrun Ref: 1->open 0->close
	*
***********************************************************/
static uint8_t relay_fan_fun(void)
{
   if(tpd_t.relay_fan_flag==1) return 1;
   else return 0;

}

/***********************************************************
	*
	*Function Name:static uint8_t relay_kill_fun(void) 
	*Function: open of clouse relay
	*Input Ref: NO
	*Retrun Ref: 1->open 0->close
	*
***********************************************************/
static uint8_t relay_kill_fun(void)
{
	if(tpd_t.relay_kill_flag==1) return 1;
	else return 0;

}
/***********************************************************
	*
	*Function Name: static uint8_t relay_temp_fun(void)
	*Function: open of clouse relay
	*Input Ref: NO
	*Retrun Ref: 1->open 0->close
	*
***********************************************************/
static uint8_t relay_temp_fun(void)
{
	if(pro_t.set_keey_temp_define_flag ==1){
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
	else{
		KEEP_HEAT_LED_OFF();
		RELAY_KEEP_TEMP_SetLow();
		KEY_FUN_CONFIRM_LED_ON() ;
		ADD_DEC_LED_OFF();

	}


}




/***********************************************************
	*
	*Function Name: void Relay_Tape_Process(uint8_t(*relay_tape_handler)(void))
	*Function: open of clouse relay
	*Input Ref: NO
	*Retrun Ref: 1->open 0->close
	*
***********************************************************/
void Relay_Tape_Process(uint8_t(*relay_tape_handler)(void))
{
   relay_tape_state = relay_tape_handler;
}
void Relay_Fan_Process(uint8_t(*relay_fan_handler)(void))
{
	relay_fan_state = relay_fan_handler;
}
void Relay_Kill_Process(uint8_t(*relay_kill_handler)(void))
{
   relay_kill_state = relay_kill_handler;

}
void Relay_Temp_Process(uint8_t(*relay_temp_handler)(void))
{
   relay_temp_state = relay_temp_handler;

}



