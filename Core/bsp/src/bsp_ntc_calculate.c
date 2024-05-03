#include "bsp.h"


/************************************************************************************************************
	*
	*Function Name: static uint8_t Calculate_Display_Temperature_Value(const uin16_t *pt,uint8_t length)
	*Function :calculate special value
	*Input Ref: speical transmit display temperature value 
	*Return Ref:NO
	*
*************************************************************************************************************/
uint8_t Calculate_Display_Temperature_19_21_Value(const uint16_t *pt,uint16_t key,uint8_t length)
{
      uint8_t i;
	
	
	  uint16_t temp_temperature_value;
      for(i=0;i<length;i++){

	   		 if(i==0){
               if(*(pt+0) < key && key < *(pt+1))){ //*(pt+0)= 19 ,*(pt+1)=20,*(pt+2)=21

			    if(key- *(pt+0) >=10){

				   ctl_t.temperature_rectify_value =0;//-1;
				  

				}
                else{
					ctl_t.temperature_rectify_value =0;
                }
				
				    temp_decimal_point = key - *(pt+i);

			        temp_decimal_point = temp_decimal_point +9;

	   		       ctl_t.temperature_decimal_point_value =  temp_decimal_point/10  ;

               }

              if(*(pt+0) < key){
			  	
			    temp_temperature_value  = i;
				return temp_temperature_value ;

               }


			}
	 	    else if(*(pt+i) >  key && (*(pt+i+1) < key)){ //high temperature degree is number is smaller

            if(key- (*(pt+i+1)) >=10){ //10
                 ctl_t.temperature_rectify_value =2;//1;
				 
            }
			else
		      ctl_t.temperature_rectify_value =1;
			
		   temp_decimal_point = *(pt+i) -key;

		   temp_decimal_point = temp_decimal_point +9;
           ctl_t.temperature_decimal_point_value =  temp_decimal_point/10 ;

            
				temp_temperature_value =  i;
				return temp_temperature_value ;

            

		 }
		 else if(i==(length-1)){

               if(*(pt+i) >  key){


			   if((*(pt+i) - key) >=10){
                 ctl_t.temperature_rectify_value =2;
				 
               }
			   else
		      	ctl_t.temperature_rectify_value =1;

			    temp_decimal_point = *(pt+i)-key; //小数点

		        temp_decimal_point = temp_decimal_point +9;


   		       ctl_t.temperature_decimal_point_value =  temp_decimal_point/10 ;

                temp_temperature_value  = i;

				return temp_temperature_value ;
			   	

               }

		 }
		
	  }
	  return 0;

} 
  

