#include "bsp_led.h"
#include "bsp.h"

LED_T led_t;




//static void Fan_Led_Flicker(void);
//static void Tape_Led_Flicker(void);
//static void Sterilization_Led_Filcker(void);
//static void Keep_Heat_Led_Filcker(void);

static void Add_Dec_Led_Filcker(void);
//static void Keep_heat_SetUp_Led_Filcker(void);

/*************************************************************************
	*
	*Function Name: void Led_Display_Content_Fun(uint8_t selitem)
	*Function : flicker of fan led
	*Input Ref: wich of led flicker -selitem
	*Return Ref:NO
	*
*************************************************************************/
void Led_Display_Content_Fun(uint8_t selitem)
{

   switch(selitem){

   case LED_NULL:
     

   break;

   case FAN_LED:


		if(tpd_t.confirm_key_select_item_tape == tape_enable){
			TAPE_LED_ON();
		    
		    RELAY_TAPE_SetHigh()	;

		}
		else{
    		TAPE_LED_OFF();
    		RELAY_TAPE_SetLow();

		}

		if(tpd_t.confirm_key_select_item_sterilization == sterilization_enable ){
		    KILL_LED_ON();
		}
		else
		    KILL_LED_OFF();

		

		if(tpd_t.confirm_key_select_item_keep_heat == keep_heat_enable){
		  KEEP_HEAT_LED_ON();
          RELAY_KEEP_TEMP_SetHigh();
	  	}
	     else{ 
	  		KEEP_HEAT_LED_OFF();
            RELAY_KEEP_TEMP_SetLow();


           }

		
   	 
	 if(tpd_t.gTimer_select_fun < 6){
        Fan_Led_Flicker();
     }
     else{
         if(tpd_t.confirm_key_select_item_fan == confirm_disable){
          FAN_LED_OFF();
            }

     }


   break;

   case TAPE_LED:

      if(tpd_t.confirm_key_select_item_fan == fan_enable){
		FAN_LED_ON();//FAN_LED_ON();
		
		RELAY_FAN_SetHigh()	;
		}
		else{
		FAN_LED_OFF();//FAN_LED_OFF();
		RELAY_FAN_SetLow()	;

		}

        

		if(tpd_t.confirm_key_select_item_sterilization == sterilization_enable ){
            RELAY_KILL_SetHigh();
		    KILL_LED_ON();
		}
		else{
            RELAY_KILL_SetLow();
		    KILL_LED_OFF();//KILL_LED_OFF();

        }

		

		if(tpd_t.confirm_key_select_item_keep_heat == keep_heat_enable){
          RELAY_KEEP_TEMP_SetHigh();
		  KEEP_HEAT_LED_ON();
	  	}
	     else{
            RELAY_KEEP_TEMP_SetLow();
	  		KEEP_HEAT_LED_OFF();//KEEP_HEAT_LED_OFF();
          }
   	  
	 if(tpd_t.gTimer_select_fun < 6){
   	    Tape_Led_Flicker();
	 }
     else{
        if(tpd_t.confirm_key_select_item_tape == confirm_disable){

        TAPE_LED_OFF();
        }
     }

	 

   break;

   case STERILIZATION_LED:
      //FAN 
      if(tpd_t.confirm_key_select_item_fan == fan_enable){
		FAN_LED_ON();//FAN_LED_ON();
		RELAY_FAN_SetHigh()	;
		}
		else{
		FAN_LED_OFF();//FAN_LED_OFF();
		RELAY_FAN_SetLow()	;

		}

       //TAPE
		if(tpd_t.confirm_key_select_item_tape == tape_enable){
		TAPE_LED_ON();
		RELAY_TAPE_SetHigh()	;

		}
		else{
			TAPE_LED_OFF();//TAPE_LED_OFF();
		   RELAY_TAPE_SetLow()	;
		}
        //KEEP HEAT
		if(tpd_t.confirm_key_select_item_keep_heat == keep_heat_enable){
		  KEEP_HEAT_LED_ON();
          RELAY_KEEP_TEMP_SetHigh()	;
	  	}
	     else{
            RELAY_KEEP_TEMP_SetLow();
	  		KEEP_HEAT_LED_OFF();//KEEP_HEAT_LED_OFF();

          }


     //STERILIZATION
      if(tpd_t.gTimer_select_fun < 6){
   	     Sterilization_Led_Filcker();
       }
       else{
           if(tpd_t.confirm_key_select_item_sterilization == confirm_disable){

            KILL_LED_OFF();
            }

       }


   break;

   case KEEP_HEAT_LED:
        //FAN
       if(tpd_t.confirm_key_select_item_fan == fan_enable){
		FAN_LED_ON();
		RELAY_FAN_SetHigh();
		}
		else{
		FAN_LED_OFF();
		RELAY_FAN_SetLow();

		}

        //TAPE
		if(tpd_t.confirm_key_select_item_tape == tape_enable){
    		TAPE_LED_ON();
    		
    		RELAY_TAPE_SetHigh();

    	}
    	else{
    		TAPE_LED_OFF();//TAPE_LED_OFF();
    		
    		RELAY_TAPE_SetLow();
    		

		}
        //STERILIZATION 
		if(tpd_t.confirm_key_select_item_sterilization == sterilization_enable ){
		    KILL_LED_ON();
            RELAY_KILL_SetHigh()	;
            
		}
		else{
    		KILL_LED_OFF();//KILL_LED_OFF();
    		RELAY_KILL_SetLow()	;

         }

	
      //KEEP HEAT Display of LED 
       if(tpd_t.gTimer_select_fun < 6){
	   	   Keep_Heat_Led_Filcker();
		   tpd_t.keep_heat_run_flag= 1;
	       led_t.gTimer_keey_heat_flicker=0;
		   tpd_t.gTimer_keep_heat_fun=0;
		
       	}
        else{
           if(tpd_t.confirm_key_select_item_keep_heat == confirm_disable){
              KEEP_HEAT_LED_OFF();
            }
            else{

              KEEP_HEAT_LED_ON();

            }

        }
       

      
   break;

   case LED_ALL_OFF:

      FAN_LED_OFF();
	  TAPE_LED_OFF();
	  KILL_LED_OFF();
	  KEEP_HEAT_LED_OFF();
	  ADD_DEC_LED_OFF();   

   break;

   case KEY_NULL:

   break;

   

   default:

   break;



   }



}


/*************************************************************************
	*
	*Function Name: static void Fan_Flicker(void)
	*Function : flicker of fan led
	*Input Ref: NO
	*Return Ref:NO
	*
*************************************************************************/
void Fan_Led_Flicker(void)
{
   
	if(led_t.gTimer_flicker < 1){ //500ms

	  FAN_LED_ON();

    }
	else if(led_t.gTimer_flicker >0 && led_t.gTimer_flicker <2){

	    
	  FAN_LED_OFF();


	}
	else{
	  led_t.gTimer_flicker=0;

    }

}


void Tape_Led_Flicker(void)
{
	if(led_t.gTimer_flicker < 1){ //500ms
	
		 TAPE_LED_ON()  ;
	
	   }
	   else if(led_t.gTimer_flicker >0 && led_t.gTimer_flicker <2){
	
		  TAPE_LED_OFF();
	
	
	   }
	   else{
		 led_t.gTimer_flicker=0;
	
	   }



}


void Sterilization_Led_Filcker(void)
{

	if(led_t.gTimer_flicker < 1){ //500ms
	
		 KILL_LED_ON();
	
	   }
	   else if(led_t.gTimer_flicker >0 && led_t.gTimer_flicker <2){
	
		  KILL_LED_OFF();
	
	
	   }
	   else{
		 led_t.gTimer_flicker=0;
	
	   }



}


void Keep_Heat_Led_Filcker(void)
{

	if(led_t.gTimer_flicker < 1){ //500ms
	
		KEEP_HEAT_LED_ON();
		
	
	   }
	   else if(led_t.gTimer_flicker >0 && led_t.gTimer_flicker <2){
	
		 
	     KEEP_HEAT_LED_OFF();
	
	   }
	   else{
		 led_t.gTimer_flicker=0;
	
	  }




}


static void Add_Dec_Led_Filcker(void)
{

  
	if(led_t.gTimer_flicker < 1){ //500ms

		ADD_DEC_LED_ON();	
		KEEP_HEAT_LED_ON();

	}
	else if(led_t.gTimer_flicker >0 && led_t.gTimer_flicker <2){


		ADD_DEC_LED_OFF();		

	}
	else{
		led_t.gTimer_flicker=0;

	}



}


void Keep_heat_SetUp_Led_Filcker(void)
{

  
	if(led_t.gTimer_keey_heat_flicker < 1){ //500ms

		KEY_FUN_CONFIRM_LED_ON();
		KEEP_HEAT_LED_ON();	
	    
	  

	}
	else if(led_t.gTimer_keey_heat_flicker >0 && led_t.gTimer_keey_heat_flicker <2){


		KEY_FUN_CONFIRM_LED_OFF();
		KEEP_HEAT_LED_OFF();
	   
	    

	}
	else{
		led_t.gTimer_keey_heat_flicker=0;

	}






}

/*************************************************************************
	*
	*Function Name: void Key_Confirm_Handler(uint8_t selitem)
	*Function : flicker of fan led
	*Input Ref: wich of led flicker -selitem
	*Return Ref:NO
	*
*************************************************************************/
void Key_Confirm_Handler(uint8_t selitem)
{

   static uint8_t fan_confirm_flag,tape_confirm_flag,kill_confirm_flag,keep_heat_flag;

   switch(selitem){

   case LED_NULL:
   
	  

   break;

   case FAN_LED:
   	    if(tpd_t.confirm_key_select_item_tape == tape_enable){
	      TAPE_LED_ON();
		  
		  RELAY_TAPE_SetHigh()	;

	  }
	  else{
	      TAPE_LED_OFF();
		 
	      RELAY_TAPE_SetLow()	;

	  	}

   		
     if(tpd_t.confirm_key_select_item_sterilization == sterilization_enable ){
            RELAY_KILL_SetHigh();
            KILL_LED_ON();
          }
          else{
             RELAY_KILL_SetLow();
             KILL_LED_OFF();
          }
       
       
         if(tpd_t.confirm_key_select_item_keep_heat == keep_heat_enable){
             RELAY_KEEP_TEMP_SetHigh();
             KEEP_HEAT_LED_ON();
           }
          else{ 
               RELAY_KEEP_TEMP_SetLow();
               KEEP_HEAT_LED_OFF();
     
              }
	
	 // select 
	  fan_confirm_flag = fan_confirm_flag ^ 0x01;
       if(fan_confirm_flag==1){
    	  tpd_t.confirm_key_select_item_fan = fan_enable ;
          FAN_LED_ON(); 
    	  RELAY_FAN_SetHigh()	;

        }
        else{
            tpd_t.confirm_key_select_item_fan = confirm_disable ;
              FAN_LED_OFF(); 
             RELAY_FAN_SetLow() ;
        }
     
    
      tpd_t.run_process_tag=KEY_NULL;
   break;

   case TAPE_LED:
   	   if(tpd_t.confirm_key_select_item_fan == fan_enable){
   	     FAN_LED_ON();
		  RELAY_FAN_SetHigh()   ;
   	   }
	   else{
	     FAN_LED_OFF();
		 RELAY_FAN_SetLow()   ;

	   }

	    if(tpd_t.confirm_key_select_item_sterilization == sterilization_enable ){
         RELAY_KILL_SetHigh();
         KILL_LED_ON();
       }
	   else{
          RELAY_KILL_SetLow();
	      KILL_LED_OFF();
       }
	
	
	  if(tpd_t.confirm_key_select_item_keep_heat == keep_heat_enable){
          RELAY_KEEP_TEMP_SetHigh();
		  KEEP_HEAT_LED_ON();
	  	}
	   else{ 
            RELAY_KEEP_TEMP_SetLow();
	  		KEEP_HEAT_LED_OFF();

           }

	 
	  
	  //Select tape led fun
	  tape_confirm_flag = tape_confirm_flag ^ 0x01;
       if(tape_confirm_flag==1){
    	  tpd_t.confirm_key_select_item_tape = tape_enable;
    	 
       	  TAPE_LED_ON();
    	  RELAY_TAPE_SetHigh()	;
      }
      else{
          tpd_t.confirm_key_select_item_tape = confirm_disable ;

           TAPE_LED_OFF();
           RELAY_TAPE_SetLow() ;
       }
	  
    tpd_t.run_process_tag=KEY_NULL;
   break;

   case STERILIZATION_LED:
   	   if(tpd_t.confirm_key_select_item_fan == fan_enable){
   	     FAN_LED_ON();
		 RELAY_FAN_SetHigh()   ;
   	   }
	   else{
	     FAN_LED_OFF();
		 RELAY_FAN_SetLow()   ;
	   }

      if(tpd_t.confirm_key_select_item_tape == tape_enable){
	      TAPE_LED_ON();
		  
	  	  RELAY_TAPE_SetHigh()   ;

	  }
	  else{
	      TAPE_LED_OFF();
		
	       RELAY_TAPE_SetLow()   ;

	  }
	 
	   if(tpd_t.confirm_key_select_item_keep_heat == keep_heat_enable){
          RELAY_KEEP_TEMP_SetHigh();;
		  KEEP_HEAT_LED_ON();
	  	}
	   else{ 
            RELAY_KEEP_TEMP_SetLow();
	  		KEEP_HEAT_LED_OFF();

           }
	   
 
	  
	  //select sterilization fun
	   kill_confirm_flag = kill_confirm_flag ^ 0x01;
       if(kill_confirm_flag==1){
    	   tpd_t.confirm_key_select_item_sterilization = sterilization_enable ;
           KILL_LED_ON();
           RELAY_KILL_SetHigh();
       }
       else{
         tpd_t.confirm_key_select_item_sterilization = confirm_disable ;

          KILL_LED_OFF();
          RELAY_KILL_SetLow();


       }
   	   
    tpd_t.run_process_tag=KEY_NULL;
       
   break;

   case KEEP_HEAT_LED:

       if(tpd_t.confirm_key_select_item_fan == fan_enable){
   	     FAN_LED_ON();
		 RELAY_FAN_SetHigh()   ;
   	   }
	   else{
	     FAN_LED_OFF();
		 RELAY_FAN_SetLow()   ;

	   }

      if(tpd_t.confirm_key_select_item_tape == tape_enable){
	      TAPE_LED_ON();
		  
	  
	      RELAY_TAPE_SetHigh()   ;

	  }
	  else{
	      TAPE_LED_OFF();
		
	  
		  RELAY_TAPE_SetLow()	;
	  	}
   	    
       if(tpd_t.confirm_key_select_item_sterilization == sterilization_enable ){
         RELAY_KILL_SetHigh();
         KILL_LED_ON();
       }
	   else{
	      KILL_LED_OFF();
          RELAY_KILL_SetLow();

       }

     // tpd_t.keep_heat_flag = run_t.keep_heat_flag ^ 0x01;

     // if(run_t.keep_heat_flag==1){
     
	  if(tpd_t.keep_heat_run_flag==1){ //Confirm key of define key
           tpd_t.keep_heat_run_flag ++;
    
	  }
	  //select keep heat fun
	  switch(tpd_t.keep_heat_run_flag){

	   case 2:
			if(tpd_t.gTimer_keep_heat_fun< 11){
	
				tpd_t.keep_heat_fun_digital_numbers=1; //select keep heat item 
				ADD_DEC_LED_ON();  
				Keep_heat_SetUp_Led_Filcker();
           
				
			}
			else{
                if(tpd_t.confirm_key_select_item_keep_heat == confirm_disable){
				tpd_t.gTimer_keep_heat_fun= 12;
				
				KEEP_HEAT_LED_OFF();
				ADD_DEC_LED_OFF();  
			    
                }
                else {
                
                    KEEP_HEAT_LED_ON();
                }
				tpd_t.keep_heat_fun_digital_numbers=0;
                KEY_FUN_CONFIRM_LED_ON() ;
                tpd_t.run_process_tag=KEY_NULL; 
               


			}
		

	  break;

	  case 3:
	  	      tpd_t.confirm_key_select_item_keep_heat = keep_heat_enable;
	          KEEP_HEAT_LED_ON();
			  ADD_DEC_LED_OFF();  
			  KEY_FUN_CONFIRM_LED_ON() ;

              tpd_t.run_process_tag=KEY_NULL;
	  break;
    
	 }
    

   break;


   case LED_ALL_OFF:

      FAN_LED_OFF();
	  TAPE_LED_OFF();
	  KILL_LED_OFF();
	  KEEP_HEAT_LED_OFF();
	  ADD_DEC_LED_OFF();   

   break;

   case KEY_NULL:

      KEY_FUN_CONFIRM_LED_ON() ;
   break;

   default:

   break;



   }



}




