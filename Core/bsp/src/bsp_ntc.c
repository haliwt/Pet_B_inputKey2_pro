#include "bsp_ntc.h"
#include <stdlib.h>
#include "bsp.h"


#define Zero_Degree           5828    
#define ADC_Sample_Times      10

#define COMPENSATION_VALUE    0



uint16_t *pArray[23];
uint8_t find_out_temperature_value;
uint8_t temp_uint16_t_vlue;
uint8_t length_simple;

uint16_t temp_variable_value_1;

uint8_t  temp_degree;
uint8_t temp_decimal_point;

static uint16_t Read_NTC_Temperature_Voltage(void);

static int8_t  Binary_Search(const uint8_t *array ,uint8_t key,uint8_t length);

static int8_t  Binary_Uint16_t_Search(const uint16_t *array ,uint16_t key,uint8_t length);


static void Read_Ntc_Decimal_Point_Numbers(void);

static uint8_t Calculate_Display_Temperature_Value(const uint16_t *pt,uint16_t key,uint8_t length);

static void Display_Speicial_Temperature_Value(uint8_t temp);


uint8_t search_key;



//std voltage
static const uint16_t R10K_NTC[]={

    2993,2977,2960,2942,2923,  //-20 ~ -16
    2904,2884,2863,2842,2819,  //-15 ~ -11
    2796,2773,2748,2723,2697,  //-10 ~ -6
    2671,2644,2616,2587,2558,   //-5 ~ -1
    2528,2497,2466,2434,2402,	//0 ~ 4
    2369,2335,2301,2267,2232,	//5 ~ 9
    2197,2162,2126,2090,2054,	//10 ~ 14
    2017,1980,1944,1907,1870,	//15 ~ 19
    1833,1796,1759,1722,1686,	//20 ~ 24
    1650,1613,1577,1542,1506,	//25 ~ 29
    1471,1437,1402,1369,1335,	//30 ~ 34
    1302,1270,1238,1206,1175,	//35 ~ 39
    1145,1115,1085,1057,1028,	//40 ~ 44
    1001,974,947,921,896,		//45 ~ 49
    871,847,825,800,777,		//50 ~ 54
    756,734,713,693,673,		//55 ~ 59
    654,635,617,599,582,		//60 ~ 64
    565,549,533,518,503,		//65 ~ 69
    488,474,460,447,434, 		//70 ~ 74
    422,409,398,386,375,		//75 ~ 79
    364,354						//80 ~ 81
    
    
    
};
//0 degree to 80 degree
static const uint16_t R10K_NTC_81[]={

    2528,2497,2466,2434,2402,	//0 ~ 4
    2369,2335,2301,2267,2232,	//5 ~ 9
    2197,2162,2126,2090,2054,	//10 ~ 14
    2017,1980,1944,1907,1870,	//15 ~ 19
    1833,1796,1759,1722,1686,	//20 ~ 24
    1650,1613,1577,1542,1506,	//25 ~ 29
    1471,1437,1402,1369,1335,	//30 ~ 34
    1302,1270,1238,1206,1175,	//35 ~ 39
    1145,1115,1085,1057,1028,	//40 ~ 44
    1001,974,947,921,896,		//45 ~ 49
    871,847,825,800,777,		//50 ~ 54
    756,734,713,693,673,		//55 ~ 59
    654,635,617,599,582,		//60 ~ 64
    565,549,533,518,503,		//65 ~ 69
    488,474,460,447,434, 		//70 ~ 74
    422,409,398,386,375,		//75 ~ 79
    364,354						//80 ~ 81
    
};




typedef enum{

   degree_zero = 0,
   degree_one = 1,
   degree_two = 2,
   degree_three = 3,
   degree_four = 4,
   degree_five =5,
   degree_six = 6,
   degree_seven = 7,
   degree_eight = 8,
   degree_nine = 9,
   degree_ten = 10,
   degree_eleven = 11,
   degree_twelve = 12,
   degree_thirteen =13,
   degree_fourteen = 14,
   degree_fiveteen =15,
   degree_sixteen =16,
   degree_seventenn =17,
   degree_eighteen = 18,
   degree_nineteen = 19,
   degree_twenty = 20,
   degree_twenty_one = 21,
   degree_twenty_two =22
   
   


}degree_state;


int8_t left_point =0;
int8_t right_point ;
int8_t length = sizeof(R10K_NTC_81)/(sizeof(R10K_NTC_81[0]));
int8_t mid_value;
//拆分成23个数组，mid =11
static const uint16_t R10K_0_0[2]={2558,2528};
static const uint16_t R10K_1_4[4]={2497,2466,2434,2402};
static const uint16_t R10K_5_7[3]={2369,2335,2301};
static const uint16_t R10K_8_9[2]={2267,2232};
static const uint16_t R10K_10_12[3]={2197,2162,2126};

static const uint16_t R10K_13_15[3]={2090,2054,2017};
static const uint16_t R10K_16_18[3]={1980,1944,1907};
static const uint16_t R10K_19_20[2]={1870, 1833};
static const uint16_t R10K_21_23[3]={1796,1759,1722};
static const uint16_t R10K_24_26[3]={1686,1650,1613};

static const uint16_t R10K_27_29[3]={1577,1542,1506};
static const uint16_t R10K_30_32[3]={1471,1437,1402};
static const uint16_t R10K_33_35[3]={1369,1335,1302};
static const uint16_t R10K_36_38[3]={1270,1238,1206};
static const uint16_t R10K_39_41[3]={1175,1145,1115};

static const uint16_t R10K_42_45[4]={1085,1057,1028,1001};
static const uint16_t R10K_46_48[3]={974,947,921};
static const uint16_t R10K_49_53[5]={896,871,847,825,800};
static const uint16_t R10K_54_57[4]={777,756,734,713};
static const uint16_t R10K_58_62[5]={693,673,654,635,617};

static const uint16_t R10K_63_69[7]={599,582,565,549,533,518,503};
static const uint16_t R10K_70_76[7]={488,474,460,447,434,422,409};
static const uint16_t R10K_77_81[5]={398,386,375,364,354};

static const uint8_t R10K_Init_0_81_simple[23]={

    25,24,23,22,21,
	20,19,18,17,16,
	15,14,13,12,11,
	10,9,8,7,6,
	5,4,3
};


static uint16_t Read_NTC_Temperature_Voltage_Power_On(void)
{
      uint16_t read_ntc_value;
	 
	  read_ntc_value = Get_Adc_Voltage_Value(2);
	
      

	  return read_ntc_value;
}



static uint16_t Read_NTC_Temperature_Voltage(void)
{
      uint16_t read_ntc_value;
	 
	  read_ntc_value = Get_Adc_Voltage_Value(ADC_Sample_Times);
	
      

	  return read_ntc_value;
}


/*************************************************************************
	*
	*Functin Name: static int8_t  Binary_Search(uint8_t *R10K_NTC_81 ,uint8_t key)
	*Funtion: binary search arithmetic
	*
	*
	*
*************************************************************************/
static int8_t  Binary_Search(const uint8_t *array ,uint8_t key,uint8_t length)
{
     left_point =0 ;
	right_point = length -1;
   while(left_point <= right_point){

        mid_value = (left_point + right_point)/2;

        if(key == array[mid_value]){

		    return   mid_value;

		}
        else if(array[mid_value] >   key ){ //right ->small

             left_point = mid_value +1;

		}
		else if(array[mid_value] < key){ //left -> big number

          right_point = mid_value -1;

       }


      } 

   
}
   
/*************************************************************************
	*
	*Functin Name: static int8_t  Binary_Uint16_t_Search(const uint16_t *array ,uint16_t key,uint8_t length)
	*Funtion: binary search arithmetic
	*
	*
	*
*************************************************************************/
static int8_t  Binary_Uint16_t_Search(const uint16_t *array ,uint16_t key,uint8_t length)
{
   
   
	 left_point =0 ;
	right_point = length -1;
   while(left_point <= right_point){

        mid_value = left_point + (right_point-left_point)/2;

        if(key == array[mid_value]){

		
            tpd_t.temperature_value = mid_value;
		    return   mid_value;

		}
        else if(array[mid_value] >   key ){ //right ->small

             left_point = mid_value +1;

		}
		else if(array[mid_value] < key){ //left -> big number

          right_point = mid_value -1;

       }

	   
       
		
	   if(right_point < 0){

	       return -1 ; //don't find key of value 


	   }
	   else if(left_point >=length){

	       return -1;
       }
       else{
		 if((R10K_NTC_81[left_point])  -key >=0 && (R10K_NTC_81[right_point]-key) >=0){

		       tpd_t.temperature_value = ((R10K_NTC_81[left_point]-key) > (R10K_NTC_81[right_point]-key)) ? right_point:left_point;
				
               return right_point ;
		 }

		  
		  	
      }

     }

    return -1;

}
/*********************************************************************
    *
*Funtion Name:static void Analysis_Read_Ntc_Votalge(uint8_t readvalue)
    *
    *
*********************************************************************/
static void Read_Ntc_Decimal_Point_Numbers(void)
{
   
   uint8_t temp_decimal_point;
   temp_decimal_point = R10K_NTC_81[tpd_t.temperature_value] - R10K_NTC_81[tpd_t.temperature_value +1];

   temp_decimal_point = temp_decimal_point +5;

   tpd_t.temperature_decimal_point_value =  temp_decimal_point/10 ;
   
}
/******************************************************************************
	*
	*Function Name: Read_NTC_Temperature_Value_Handler(void)
	*
	*
	*
******************************************************************************/
void Read_NTC_Temperature_Power_On(void)
{
    
  
	 #if 1
	 tpd_t.ntc_voltage_value=Read_NTC_Temperature_Voltage_Power_On(); //Read_NTC_Temperature_Voltage();
      temp_uint16_t_vlue= tpd_t.ntc_voltage_value /100;
	 length_simple = sizeof(R10K_Init_0_81_simple)/sizeof(R10K_Init_0_81_simple[0]);
    
   	 tpd_t.temp_degree = Binary_Search(R10K_Init_0_81_simple,temp_uint16_t_vlue,length_simple);

	 Display_Speicial_Temperature_Value(tpd_t.temp_degree);

	#endif 
    #if 0
    tpd_t.ntc_voltage_value= Read_NTC_Temperature_Voltage();

	temp_uint16_t_vlue   =Binary_Uint16_t_Search(R10K_NTC_81,tpd_t.ntc_voltage_value,length);
	if(tpd_t.ntc_voltage_value !=0)
         Read_Ntc_Decimal_Point_Numbers();
    #endif 

}

/******************************************************************************
	*
	*Function Name: Read_NTC_Temperature_Value_Handler(void)
	*
	*
	*
******************************************************************************/
void Read_NTC_Temperature_Value_Handler(void)
{
    
  
	 #if 1
	 tpd_t.ntc_voltage_value= Read_NTC_Temperature_Voltage();
     temp_uint16_t_vlue= tpd_t.ntc_voltage_value /100;
	 length_simple = sizeof(R10K_Init_0_81_simple)/sizeof(R10K_Init_0_81_simple[0]);
    
   	 tpd_t.temp_degree = Binary_Search(R10K_Init_0_81_simple,temp_uint16_t_vlue,length_simple);

	 Display_Speicial_Temperature_Value(tpd_t.temp_degree);

	#endif 
    #if 0
    tpd_t.ntc_voltage_value= Read_NTC_Temperature_Voltage();

	temp_uint16_t_vlue   =Binary_Uint16_t_Search(R10K_NTC_81,tpd_t.ntc_voltage_value,length);
	if(tpd_t.ntc_voltage_value !=0)
         Read_Ntc_Decimal_Point_Numbers();
    #endif 

}

/******************************************************************************
	*
	*Function Name: static void Display_Speicial_Temperature_Value(uint8_t temp)
	*Function :calculate special value
	*Input Ref: speical transmit display temperature value 
	*Return Ref:NO
	*
******************************************************************************/
static void Display_Speicial_Temperature_Value(uint8_t temp)
{
   static uint8_t array_subscript;

   switch(temp){

    case degree_zero: //zero degree

         // zero_d =2;
        array_subscript =  Calculate_Display_Temperature_Value(R10K_0_0,tpd_t.ntc_voltage_value,2);
        HAL_Delay(200);
        tpd_t.temperature_value = tpd_t.temperature_rectify_value ;

    break;

	
   case degree_one :
   	    array_subscript =  Calculate_Display_Temperature_Value(R10K_1_4,tpd_t.ntc_voltage_value,4);
		HAL_Delay(200);
        switch(array_subscript){

		 case 0:
		 	tpd_t.temperature_value = 1 + tpd_t.temperature_rectify_value +COMPENSATION_VALUE;

		 break;

		 case 1:
		 	tpd_t.temperature_value = 2 + tpd_t.temperature_rectify_value +COMPENSATION_VALUE;

		 break;

		 case 2:
		 	tpd_t.temperature_value = 3 + tpd_t.temperature_rectify_value +COMPENSATION_VALUE;

		 break;

		 case 3:
		 	tpd_t.temperature_value = 4 + tpd_t.temperature_rectify_value +COMPENSATION_VALUE;

		 break;

        }

   break;
   
   case degree_two:
   	   array_subscript =  Calculate_Display_Temperature_Value(R10K_5_7,tpd_t.ntc_voltage_value,3);
	   HAL_Delay(200);
	   switch(array_subscript){

		 case 0:
		 	tpd_t.temperature_value = 5 + tpd_t.temperature_rectify_value + COMPENSATION_VALUE;

		 break;

		 case 1:
		 	tpd_t.temperature_value = 6 + tpd_t.temperature_rectify_value +COMPENSATION_VALUE;

		 break;

		 case 2:
		 	tpd_t.temperature_value = 7 + tpd_t.temperature_rectify_value +COMPENSATION_VALUE;

		 break;

		

        }

   break;

   
   case degree_three :

         array_subscript =  Calculate_Display_Temperature_Value(R10K_8_9,tpd_t.ntc_voltage_value,2);
		 
  		 HAL_Delay(200);
		 switch(array_subscript){
   
		   case 0:
			  tpd_t.temperature_value = 8 + tpd_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  tpd_t.temperature_value = 9 + tpd_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  
   
		  }

   break;
   
   case degree_four:

         array_subscript =  Calculate_Display_Temperature_Value(R10K_10_12,tpd_t.ntc_voltage_value,3);
		  HAL_Delay(200);
		 switch(array_subscript){
   
		   case 0:
			  tpd_t.temperature_value = 10 + tpd_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  tpd_t.temperature_value = 11 + tpd_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  tpd_t.temperature_value = 12 + tpd_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  }


   break;

   case degree_five :
		 array_subscript =  Calculate_Display_Temperature_Value(R10K_13_15,tpd_t.ntc_voltage_value,3);
		  HAL_Delay(200);
		 switch(array_subscript){
   
		   case 0:
			  tpd_t.temperature_value = 13 + tpd_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  tpd_t.temperature_value = 14 + tpd_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  tpd_t.temperature_value = 15 + tpd_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  }
   break;
   
   case degree_six:
   	     array_subscript =  Calculate_Display_Temperature_Value(R10K_16_18,tpd_t.ntc_voltage_value,3);
		  HAL_Delay(200);
		 switch(array_subscript){
   
		   case 0:
			  tpd_t.temperature_value = 16 + tpd_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  tpd_t.temperature_value = 17 + tpd_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  tpd_t.temperature_value = 18 + tpd_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  }

   break;
   
   case degree_seven :
   	     array_subscript =  Calculate_Display_Temperature_Value(R10K_19_20,tpd_t.ntc_voltage_value,2);
		  HAL_Delay(200);
		 switch(array_subscript){
   
		   case 0:
			  tpd_t.temperature_value = 19 + tpd_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  tpd_t.temperature_value = 20 + tpd_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
   
		  }

   break;

  
   case degree_eight :
   	     array_subscript =  Calculate_Display_Temperature_Value(R10K_21_23,tpd_t.ntc_voltage_value,3);
		  HAL_Delay(200);
		 switch(array_subscript){
   
		   case 0:
			  tpd_t.temperature_value = 21 + tpd_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  tpd_t.temperature_value = 22 + tpd_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  tpd_t.temperature_value = 23 + tpd_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  }

   break;

   
   case degree_nine :
   	    array_subscript =  Calculate_Display_Temperature_Value(R10K_24_26,tpd_t.ntc_voltage_value,3);
		 HAL_Delay(200);
		 switch(array_subscript){
   
		   case 0:
			  tpd_t.temperature_value = 24 + tpd_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  tpd_t.temperature_value = 25 + tpd_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  tpd_t.temperature_value = 26 + tpd_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  }

   break;
   
   case degree_ten :
   	 array_subscript =  Calculate_Display_Temperature_Value(R10K_27_29,tpd_t.ntc_voltage_value,3);
	  HAL_Delay(200);
		 switch(array_subscript){
   
		   case 0:
			  tpd_t.temperature_value = 27 + tpd_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  tpd_t.temperature_value = 28 + tpd_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  tpd_t.temperature_value = 29 + tpd_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  }

   break;
   
   case  degree_eleven :
   		 array_subscript =  Calculate_Display_Temperature_Value(R10K_30_32,tpd_t.ntc_voltage_value,3);
		  HAL_Delay(200);
		 switch(array_subscript){
   
		   case 0:
			  tpd_t.temperature_value = 30 + tpd_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  tpd_t.temperature_value = 31 + tpd_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  tpd_t.temperature_value = 32 + tpd_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  }

   break;
   
   case degree_twelve :
   	   	 array_subscript =  Calculate_Display_Temperature_Value(R10K_33_35,tpd_t.ntc_voltage_value,3);
		  HAL_Delay(200);
		 switch(array_subscript){
   
		   case 0:
			  tpd_t.temperature_value = 33 + tpd_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  tpd_t.temperature_value = 34 + tpd_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  tpd_t.temperature_value = 35 + tpd_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  }

   break;

   case degree_thirteen :
   		 array_subscript =  Calculate_Display_Temperature_Value(R10K_36_38,tpd_t.ntc_voltage_value,3);
		  HAL_Delay(200);
		 switch(array_subscript){
   
		   case 0:
			  tpd_t.temperature_value = 36 + tpd_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  tpd_t.temperature_value = 37 + tpd_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  tpd_t.temperature_value = 38 + tpd_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  }

   break;
   
   case degree_fourteen : //40 degree
       array_subscript =  Calculate_Display_Temperature_Value(R10K_39_41,tpd_t.ntc_voltage_value,3);
	    HAL_Delay(200);
		 switch(array_subscript){
   
		   case 0:
			  tpd_t.temperature_value = 39 + tpd_t.temperature_rectify_value + COMPENSATION_VALUE;
   
		   break;
   
		   case 1:
			  tpd_t.temperature_value = 40 + tpd_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		   case 2:
			  tpd_t.temperature_value = 41 + tpd_t.temperature_rectify_value +COMPENSATION_VALUE;
   
		   break;
   
		  }


   break;
   


}
}
/******************************************************************************
	*
	*Function Name: static uint8_t Calculate_Display_Temperature_Value(const uin16_t *pt,uint8_t length)
	*Function :calculate special value
	*Input Ref: speical transmit display temperature value 
	*Return Ref:NO
	*
******************************************************************************/
static uint8_t Calculate_Display_Temperature_Value(const uint16_t *pt,uint16_t key,uint8_t length)
{
      uint8_t i;
	  static uint8_t  rectify_value;
	
	  uint16_t temp_temperature_value;
      for(i=0;i<length;i++){

	   		 if(i==0){
               if(*(pt+0) < key){

			    temp_decimal_point = key - *(pt+i);

		        temp_decimal_point = temp_decimal_point +5;

   		       tpd_t.temperature_decimal_point_value =  temp_decimal_point/10 ;

                temp_temperature_value  = i;

				return temp_temperature_value ;


			   }


			}
	 	    else if(*(pt+i) > key && (*(pt+i+1) < key)){

            if(key- (*(pt+i+1)) >=20){ //10
                 tpd_t.temperature_rectify_value =1;
				 
            }
			else
		      tpd_t.temperature_rectify_value =0;
			
		   temp_decimal_point = *(pt+i) -*(pt+i+1);

		   temp_decimal_point = temp_decimal_point +5;
           tpd_t.temperature_decimal_point_value =  temp_decimal_point/10 ;

			temp_temperature_value =  i;
			return temp_temperature_value ;

		 }
		 else if(i==(length-1)){

               if(*(pt+i) >  key){


			   if(key- (*(pt+i+1)) >=10){
                 tpd_t.temperature_rectify_value =1;
				 
               }
			   else
		      tpd_t.temperature_rectify_value =0;

			    temp_decimal_point = key - *(pt+i);

		        temp_decimal_point = temp_decimal_point +5;


   		       tpd_t.temperature_decimal_point_value =  temp_decimal_point/10 ;

                temp_temperature_value  = i;

				return temp_temperature_value ;
			   	

               	}

		 }
		

		 
		
	
      }
	  
  }

