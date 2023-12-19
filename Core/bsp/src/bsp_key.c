#include "bsp_key.h"
#include "bsp.h"

key_types key_t;

 uint16_t  K1=0;
 uint32_t  K2=0;
  uint8_t 	 	value1 ;
  uint8_t   	value2 ;
uint8_t cnt;
/***********************************************************
 *  *
    *Function Name: INT8U ReadKey(void)
    *Function: ReCharge battery be detected
    *Input Ref: NO
    *Return Ref: 1 -battery recharge   0-no recharge
    * 
***********************************************************/
uint8_t ReadKey(void)
{


 // static uint16_t  K1=0;
 // static uint16_t  K2=0;

//  static uint8_t cnt;
  //uint8_t 	 	value1 = 0;
 // uint8_t   	value2 = 0;

//	if(!T1msFlag)  //10ms check once 
//		return value1;
//	T1msFlag = 0;
	
  if(FUN_KEY_VALUE() ==KEY_DOWN && CONFIRM_KEY_VALUE()==KEY_UP){ //KEY1 =POWER_KEY ,KEY2 = MODES
		cnt = 0;
		pro_t.long_key_flag =0;
		K1++;	 //Fun_key press 
  }
  else if( CONFIRM_KEY_VALUE()==KEY_DOWN && pro_t.long_key_flag ==0){
		cnt = 0;
		K2++;   //Confirm_key press
		if(pro_t.keep_temp_flag ==1){
           if(K2 > 199000){
              K2=0;
			  cnt = 0;
		      K1 = 0;
			  value1=0;
			  value2=0;
			  pro_t.long_key_flag =1;
			  
			  return 0x82;
            }


		}
  }
  else if(FUN_KEY_VALUE()==0 && CONFIRM_KEY_VALUE()==0 && pro_t.long_key_flag ==0){ //oneself key 
		cnt++;
		if(cnt<70){ //按键松开消抖,一定要大于短按键次数 > 20
		    return 0; 

		}
		
		cnt = 0;//
		if(K1>60){ //KEY_FUN
			value1 = 0x01;	//short time power press ---power on 
		}
		else{
			value1 = 0;

		}

		//KEY_CONFIRM 
		if(K2>60 ){//short time modes press 
            value2 = 0x02;

		}
//		else if(K2>60000){
//			value2 = 0x82;  //long time power press
//		}
		else{ 
			value2 = 0;
		}
		 	
		
		K1 = 0;
		K2 = 0;		

		return (value1+value2);
	}

   //judge key combination
    #if 0
	if((K1==500) && (K2<50)) //combination key 
		value1 = 0x03;
	else if(K2==500 && K1<50)
		value2 = 0x30;
	else if(K1==100 && K2>100)
		value1 = 0x44;
	else if(K1>100 && K2==100)
		value1 = 0x44;
	

  return (value1+value2);
  #endif 
	
   return 0;
}





/***********************************************************
 *  *
    *Function Name: INT8U ReadKey(void)
    *Function: ReCharge battery be detected
    *Input Ref: NO
    *Return Ref: 1 -battery recharge   0-no recharge
    * 
***********************************************************/
uint8_t KEY_Scan(void)
{

   uint8_t  reval = 0;
 
  key_t.read = _KEY_ALL_OFF; //0xFF 
  
  
    if(FUN_KEY_VALUE()==KEY_DOWN )// high level
    {
        key_t.read &= ~0x01; // 0xff & 0xfe =  0xFE
    }
    else if(CONFIRM_KEY_VALUE()==KEY_DOWN )
	{
		  key_t.read &= ~0x02; // 0xFf & 0xfd =  0xFd
	}
   
    
   

    switch(key_t.state )
	{
		case start:
		{
			if(key_t.read != _KEY_ALL_OFF)
			{
				key_t.buffer   = key_t.read; //??:key.buffer = 0xFE  POWER KEY 
				key_t.state    = first;
				key_t.on_time  = 0;
				key_t.off_time = 0;
			//	key_t.value=0; //WT.EDIT 2023.12.15
                
			}
			break;
		}
		case first:
		{
			if(key_t.read == key_t.buffer) // adjust key be down ->continunce be pressed key
			{

			 if(++key_t.on_time>1000 ){

					key_t.value = key_t.buffer^_KEY_ALL_OFF; // key.value = 0xFE ^ 0xFF = 0x01
					key_t.on_time = 0;                        //key .value = 0xEF ^ 0XFF = 0X10
                   
					key_t.state   = second;


				 }
			}
			else
			{
				key_t.on_time = 0; 
				key_t.state   = start;
			}
			break;
		}
		case second:
		{
			if(key_t.read == key_t.buffer) //again adjust key if be pressed down 
			{
				if(++key_t.on_time>20000 )// 500 long key be down
				{
					
					key_t.value = key_t.value|0x80; //key.value(power_on) = 0x01 | 0x80  =0x81  
					key_t.on_time = 0;               //key.vaule(model_key) = 0x01 | 0x80 = 0x82
					key_t.state   = finish; 
                   
				}
			}
			else if(key_t.read == _KEY_ALL_OFF)  // loose hand 
				{
					if(++key_t.off_time>10) //8 //30 don't holding key dithering
					{
						key_t.value = key_t.buffer^_KEY_ALL_OFF; // key.value = 0x1E ^ 0x1f = 0x01
						
						key_t.state   = finish; // loose hand
					}
					
				}
		   
			break;
		}
		case finish:
		{
			
			reval = key_t.value; // is short time  TIMER_KEY = 0x01  2. long times TIMER_KEY = 0X81
			key_t.state   = end;
         
			break;
		}
		case end: //4
		{
			if(key_t.read == _KEY_ALL_OFF)
			{
				if(++key_t.off_time>10)//50 //100
				{
					key_t.state   = start;
                   
                  
				}
				
			}
			break;
		}
		default:
		{
			key_t.state   = start;
            
			break;
		}
	}
	return  reval;


}





