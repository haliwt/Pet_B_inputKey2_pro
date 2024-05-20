#include "bsp_ctl.h"
#include "bsp.h"


uint8_t (*relay_tape_state)(void);
uint8_t (*relay_fan_state)(void);
uint8_t (*relay_kill_state)(void);
uint8_t (*relay_temp_flag_state)(void);
uint8_t (*relay_keep_temp_state)(void);


static uint8_t relay_tape_fun(void);
static uint8_t relay_fan_fun(void);
static uint8_t relay_kill_fun(void);
static uint8_t relay_set_temp_flag_fun(void);
static uint8_t relay_keep_temp_fun(void);


touchpad_t ctl_t;
uint8_t led_on_of_number;

/***********************************************************
	*
	*Function Name:void Relay_LED_ON_OFF_Handler(void)
	*Function: open of clouse relay
	*Input Ref: NO
	*Retrun Ref: 1->open 0->close
	*
***********************************************************/
void bsp_ctl_init(void)
{
    Relay_Tape_Process(relay_tape_fun);
	Relay_Fan_Process(relay_fan_fun);
	Relay_Kill_Process(relay_kill_fun);
	 Relay_Temp_Flag_Handler(relay_set_temp_flag_fun);
	Relay_Keep_Temp_Process(relay_keep_temp_fun);

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
     if(ctl_t.relay_tape_flag==1)return 1;
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
   if(ctl_t.relay_fan_flag==1) return 1;
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
	if(ctl_t.relay_kill_flag==1) return 1;
	else return 0;

}

/***********************************************************
	*
	*Function Name:static uint8_t relay_set_temp_flag_fun(void)
	*Function: open of clouse relay
	*Input Ref: NO
	*Retrun Ref: 1->open 0->close
	*
***********************************************************/
static uint8_t relay_set_temp_flag_fun(void)
{
   if(pro_t.set_keep_temp_fun_flag==1) return 1;
   else return 0;

}

/***********************************************************
	*
	*Function Name: static uint8_t relay_keep_temp_fun(void)
	*Function: open of clouse relay
	*Input Ref: NO
	*Retrun Ref: 1->open 0->close
	*
***********************************************************/
static uint8_t relay_keep_temp_fun(void)
{
   
	
  //  ctl_t.disp_net_temp_value = Disp_NtcRes_LinearValue(ctl_t.temperature_value);
    
	if(relay_temp_flag_state() ==1){ //has been set up temperature value 
	 
         if(pro_t.set_keep_temp_value > ctl_t.disp_net_temp_value ){

                   if(ctl_t.again_open_relay_ptc != 2){ //open
                    
                       ctl_t.again_open_relay_ptc++;
                       ctl_t.open_has_been_open = 1;
                       ctl_t.again_open=1;
                       ctl_t.temperature_decimal_point_value =0;
    			       KEEP_HEAT_LED_ON();  // ptc open 
    	               RELAY_KEEP_TEMP_SetHigh();
    				   KEY_FUN_CONFIRM_LED_ON() ;  
    				   ADD_DEC_LED_OFF();

                    }
                    else{
                      
                        if((pro_t.set_keep_temp_value -3)> ctl_t.disp_net_temp_value &&  ctl_t.open_has_been_open ==0){ //WT.EDIT 2024.05.18

                           ctl_t.again_open=1;
                           ctl_t.temperature_decimal_point_value =0;
                           KEEP_HEAT_LED_ON();  // ptc open 
                           RELAY_KEEP_TEMP_SetHigh(); //open ptc heat relay .
                           KEY_FUN_CONFIRM_LED_ON() ;  
                           ADD_DEC_LED_OFF();



                        }

                      
                       
                   }

                   if( ctl_t.again_open==0){


                        Set_KeepTempValue_DispLed();


                     }

			  }
              else{
               if(ctl_t.set_keep_heat_tempeature_flag == 1){
                 if(ctl_t.again_open_relay_ptc==0){
                    ctl_t.again_open_relay_ptc++;
                  }
                  else if(ctl_t.again_open_relay_ptc==2){
                      ctl_t.open_has_been_open = 0;

                 }
                 Set_KeepTempValue_DispLed();
                
                  ctl_t.again_open=0;
                  
                  
			   }
			   else{
                 
					KEEP_HEAT_LED_OFF();
					RELAY_KEEP_TEMP_SetLow();
					KEY_FUN_CONFIRM_LED_ON() ;
					ADD_DEC_LED_OFF();
			   }

              }


	 }
	else{
        
		KEEP_HEAT_LED_OFF();
		RELAY_KEEP_TEMP_SetLow();
		KEY_FUN_CONFIRM_LED_ON() ;
		ADD_DEC_LED_OFF();
	}
}




/*
*********************************************************************************************************
*	函 数 名: BEBufToUint16
*	功能说明: 将2字节数组(大端Big Endian次序，高字节在前)转换为16位整数 big 
*	形    参: _pBuf : 数组
*	返 回 值: 16位整数值
*
*   大端(Big Endian)与小端(Little Endian)
*********************************************************************************************************
*/
uint16_t LEBufToUint16(uint8_t *_pBuf)
{
    return (((uint16_t)_pBuf[1] << 8) | _pBuf[0]);
}


/*
*********************************************************************************************************
*	函 数 名: LEBufToUint16
*	功能说明: 将2字节数组(小端Little Endian，低字节在前)转换为16位整数
*	形    参: _pBuf : 数组
*	返 回 值: 16位整数值
*********************************************************************************************************
*/
uint16_t BEBufToUint16(uint8_t *_pBuf)
{
    return (((uint16_t)_pBuf[0] << 8) | _pBuf[1]);
}


/*
*********************************************************************************************************
*	函 数 名: BEBufToUint32
*	功能说明: 将4字节数组(大端Big Endian次序，高字节在前)转换为16位整数
*	形    参: _pBuf : 数组
*	返 回 值: 16位整数值
*
*   大端(Big Endian)与小端(Little Endian)
*********************************************************************************************************
*/
uint32_t BEBufToUint32(uint8_t *_pBuf)
{
 return (((uint32_t)_pBuf[3] << 24) | ((uint32_t)_pBuf[2] << 16) | ((uint32_t)_pBuf[1] << 8) | _pBuf[0]);
}

/***********************************************************************************
	*
	*Function Name: void Relay_Tape_Process(uint8_t(*relay_tape_handler)(void))
	*Function: open of clouse relay
	*Input Ref: NO
	*Retrun Ref: 1->open 0->close
	*
************************************************************************************/
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
void Relay_Keep_Temp_Process(uint8_t(*relay_temp_handler)(void))
{
   relay_keep_temp_state = relay_temp_handler;

}
void Relay_Temp_Flag_Handler(uint8_t(*relay_temp_flag_handler)(void))
{

	relay_temp_flag_state = relay_temp_flag_handler;

}



