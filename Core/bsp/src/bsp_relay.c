#include "bsp_relay.h"
#include "bsp.h"




void Relay_Init(void)
{
    ctl_t.relay_tape_flag =0;
    ctl_t.relay_fan_flag =0;
    ctl_t.relay_kill_flag =0;

   // ctl_t.set_keep_heat_tempeature_flag =0 ; //WT.EIDT 2024.05.17
    gpro_t.set_temp_value_success_flag=0;

}

void Relay_Tape_State(void)
{
     if(relay_tape_state() == 1){
        TAPE_LED_ON();
		RELAY_TAPE_SetHigh();

	 }
	 else{
        TAPE_LED_OFF(); 
		RELAY_TAPE_SetLow();
    }

}

void Relay_Fan_State(void)
{

     if(relay_fan_state() == 1){
        FAN_LED_ON();
		RELAY_FAN_SetHigh();

	 }
	 else{
        FAN_LED_OFF(); 
		RELAY_FAN_SetLow();
    }
}

void Relay_Kill_State(void)
{
 if(relay_kill_state() == 1){
        KILL_LED_ON();
		RELAY_KILL_SetHigh();

	 }
	 else{
        KILL_LED_OFF(); 
		RELAY_KILL_SetLow();
    }
}

void Relay_Keep_Temp_State(void)
{
   relay_keep_temp_run_fun();
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
void Relay_Confirm_Turn_OnOff_Fun(void)
{

        Relay_Tape_State();
    
       
        Relay_Fan_State();
   
      
        Relay_Kill_State();
   
       
      //  Relay_Keep_Temp_State();  ////检查设置的温度值和实际检测到的温度值对比
    
    
}


/***********************************************************
 *
 *Function Name : relay_keep_temperature_led_on_off(void)
 *
 *
 *
 *
***********************************************************/
void relay_keep_temperature_led_on_off(void)
{
     if(ctl_t.relay_keep_temp_on_off_flag == keep_temp_open && relay_settemp_flag_state() ==1){
        KEEP_HEAT_LED_ON();  // ptc open 
    	RELAY_KEEP_TEMP_SetHigh();

     }
     else if(ctl_t.relay_keep_temp_on_off_flag == keep_temp_close){
        KEEP_HEAT_LED_OFF();
		RELAY_KEEP_TEMP_SetLow();

     }


}


/***********************************************************************
*
*Function Name : void SetRelay_All_TurnOff_Fun(void)
*
*
*
************************************************************************/
void SetRelay_All_TurnOff_Fun(void)
{

   ctl_t.gTimer_select_fun = 10;
   Relay_Init();

   Relay_Confirm_Turn_OnOff_Fun();

   gpro_t.set_temp_value_success_flag = 0;
   ctl_t.relay_keep_temp_on_off_flag =0;

 

   //keep temperature is turn off 
   KEEP_HEAT_LED_OFF();
   RELAY_KEEP_TEMP_SetLow();
   KEY_FUN_CONFIRM_LED_ON() ;
   ADD_DEC_LED_OFF();

   
}

void SetRelay_TurnOn_Fan_Tape_Fun(void)
{

    ctl_t.gTimer_select_fun = 10;
    ctl_t.relay_tape_flag=1;
    ctl_t.relay_fan_flag=1;

   
  
    ctl_t.relay_kill_flag =0;

   // ctl_t.set_keep_heat_tempeature_flag =0 ; //WT.EIDT 2024.05.17
    gpro_t.set_temp_value_success_flag=0;

     Relay_Confirm_Turn_OnOff_Fun();

    
    gpro_t.set_temp_value_success_flag = 0;
   ctl_t.relay_keep_temp_on_off_flag =0;

   //keep temperature is turn off 
   KEEP_HEAT_LED_OFF();
   RELAY_KEEP_TEMP_SetLow();
   KEY_FUN_CONFIRM_LED_ON() ;
   ADD_DEC_LED_OFF();

	 

}


/***********************************************************
 *
 *Function Name : relay_keep_temperature_led_on_off(void)
 *data: 2024.09.11 add new item 
 *
 *
 *
***********************************************************/
void set_all_on_or_off_fun(void)
{

    static uint8_t dc_power_on_first_set_mode;
    //adjust all switch is on or off,this is call "smart mode"
     #if 0
     if(ctl_t.relay_tape_flag ==1 && ctl_t.relay_fan_flag==1 && ctl_t.relay_kill_flag==1 && gpro_t.set_temp_value_success_flag ==1 ){

           gpro_t.master_switch_control_flag = set_all_off_mode;

     }
     else if(ctl_t.relay_tape_flag ==0 && ctl_t.relay_fan_flag==0 && ctl_t.relay_kill_flag==0 && gpro_t.set_temp_value_success_flag ==0 ){

           gpro_t.master_switch_control_flag = set_on_mode;

     }
     else{

     #endif 

     switch(gpro_t.master_switch_control_flag){


          case other_mode:


           break;

           case set_all_off_mode:

             if(ctl_t.relay_tape_flag ==0 && ctl_t.relay_fan_flag==0 && ctl_t.relay_kill_flag==0 && gpro_t.set_temp_value_success_flag ==0 ){

                 SetRelay_TurnOn_Fan_Tape_Fun();

             }
             else{
                
                 SetRelay_All_TurnOff_Fun();


             }
           
             

           break;

           case set_on_mode:  //turn on only two item "opne fan" + "onpen tape led "

                if(dc_power_on_first_set_mode==0){
                   dc_power_on_first_set_mode ++;
                     SetRelay_TurnOn_Fan_Tape_Fun();


                 }
                 else if(ctl_t.relay_tape_flag ==1 && ctl_t.relay_fan_flag==1 && ctl_t.relay_kill_flag==1 && gpro_t.set_temp_value_success_flag ==1 ){

                     SetRelay_All_TurnOff_Fun();

                 }
                 else{

                      SetRelay_TurnOn_Fan_Tape_Fun();


                 }

           break;

          

     }


 }

