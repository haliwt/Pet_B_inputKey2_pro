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
static uint8_t relay_temp_flag_fun(void);
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
	 Relay_Temp_Flag_Handler(relay_temp_flag_fun);
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
	*Function Name:static uint8_t relay_temp_flag_fun(void)
	*Function: open of clouse relay
	*Input Ref: NO
	*Retrun Ref: 1->open 0->close
	*
***********************************************************/
static uint8_t relay_temp_flag_fun(void)
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
   
	static uint8_t set_value_off_flag;

	if(relay_temp_flag_state() ==1){
	  if(pro_t.set_keep_temp_value > ctl_t.temperature_value){
		  
		  if(ctl_t.temperature_value==29){
				ctl_t.relay_keep_temp_flag =0;
				  KEEP_HEAT_LED_OFF();
				  RELAY_KEEP_TEMP_SetLow();
				  KEY_FUN_CONFIRM_LED_ON() ;
				  ADD_DEC_LED_OFF();
			      set_value_off_flag =1;
			  
		   }
		   else{
			   ctl_t.relay_keep_temp_flag =1; //open keep temperature "relay_d" 
			   KEEP_HEAT_LED_ON();
			   RELAY_KEEP_TEMP_SetHigh();
			   KEY_FUN_CONFIRM_LED_ON() ;  
			   ADD_DEC_LED_OFF();
		   }

	    }
		else{
			
		  if(set_value_off_flag ==1 && (ctl_t.temperature_value < 28)){
			   set_value_off_flag =0;
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

         }
	}
	else{
		KEEP_HEAT_LED_OFF();
		RELAY_KEEP_TEMP_SetLow();
		KEY_FUN_CONFIRM_LED_ON() ;
		ADD_DEC_LED_OFF();
	}
}

void Default_TurnOff_Ptc(void)
{
	if(ctl_t.temperature_value > 29){
		KEEP_HEAT_LED_OFF();
		RELAY_KEEP_TEMP_SetLow();
		//KEY_FUN_CONFIRM_LED_ON() ;
		//ADD_DEC_LED_OFF();

	}



}

/***********************************************************************************
	*
	*Function Name: void Quantificat_FlashData_Handler(void)
	*Function:  tape =0x01,fan=0x02,kill= 0x04,keep_temp =0x08
	*Input Ref: NO
	*Retrun Ref: NO
	*
************************************************************************************/
void Quantificat_FlashData_Handler(void)
{

		if(relay_tape_state() == 1 && relay_fan_state() == 1 && relay_kill_state() == 1 && relay_temp_flag_state() ==1){//TAP+FAN+KILL+TEMP//
			ctl_t.relay_flag_flash_data = 0x0f;
		}
        else if(relay_tape_state() == 1 && relay_fan_state() == 1 && relay_kill_state() == 1 && relay_temp_flag_state() ==0){ //TAP+FAN+KILL
			ctl_t.relay_flag_flash_data= 0x07;
		}
        else if(relay_tape_state() == 1 && relay_fan_state() == 1  && relay_temp_flag_state() ==0 && relay_kill_state() == 0 ){ //TAP+FAN
				ctl_t.relay_flag_flash_data= 0x03;
		}
		else if(relay_tape_state() == 1 && relay_kill_state() == 1 && relay_temp_flag_state() ==0 && relay_fan_state() == 0 ){ //TAPE+ KILL
			ctl_t.relay_flag_flash_data= 0x05;
		}
		else if(relay_tape_state() == 1 &&  relay_temp_flag_state() ==1 && relay_fan_state() ==0 && relay_kill_state() == 0 ){ //TAP+TEMP
			ctl_t.relay_flag_flash_data = 0x09;
		}
		else if(relay_tape_state() == 1 && relay_fan_state() == 1 && relay_temp_flag_state() ==1 && relay_kill_state() == 0){ //TAPE+FAN+TEMP
			ctl_t.relay_flag_flash_data = 0x0B;
		}
		else if(relay_tape_state() == 1 && relay_kill_state() == 1 && relay_temp_flag_state() ==1 && relay_fan_state() == 0 ){ //TAPE+KILL+TEMP
			ctl_t.relay_flag_flash_data = 0x0D;
		}
		else if(relay_tape_state() == 1 && relay_kill_state() == 0 && relay_temp_flag_state() ==0 && relay_fan_state() == 0  ){ //TAPE
				ctl_t.relay_flag_flash_data= 0x01;
		}
        else if(relay_fan_state() == 1 && relay_kill_state() == 1 && relay_temp_flag_state() ==1 && relay_tape_state() == 0){ //FAN+KILL+TEMP

			ctl_t.relay_flag_flash_data= 0x0E;

       }
	   else if(relay_fan_state() == 1 && relay_kill_state() == 1  && relay_temp_flag_state() ==0 && relay_tape_state() == 0 ){ //FAN+KILL
		
			ctl_t.relay_flag_flash_data= 0x06;
		}
	   	else if(relay_fan_state() == 1 && relay_temp_flag_state() ==1 && relay_kill_state() ==0 && relay_tape_state() == 0){ //FAN+TEMP

			ctl_t.relay_flag_flash_data= 0x0A;

       }
	    else if(relay_fan_state() == 1 && relay_temp_flag_state() ==0 && relay_kill_state() ==0 && relay_tape_state() == 0){ //FAN
		
			ctl_t.relay_flag_flash_data= 0x02;
		}
		else if(relay_kill_state() == 1 && relay_temp_flag_state() ==1 && relay_fan_state() ==0 && relay_tape_state() == 0 ){//KILL+TEMP

			ctl_t.relay_flag_flash_data= 0x0C;

       }
	   else if(relay_kill_state() == 1 && relay_fan_state() ==0 && relay_keep_temp_state() ==0 && relay_tape_state() == 0){//KILL

			ctl_t.relay_flag_flash_data= 0x04;

       }
	  	else if(relay_temp_flag_state() ==1 && relay_fan_state() ==0 && relay_kill_state() ==0 && relay_tape_state() == 0){//KEEP TEMP 
		
			  ctl_t.relay_flag_flash_data= 0x08;
		}


//		//RELAY KEEP TEMP
//		if(relay_temp_flag_state()==1){
//
//		   
//           ctl_t.relay_keep_temp_data =pro_t.set_keep_tmep_value ;
//
//		}
//		

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



