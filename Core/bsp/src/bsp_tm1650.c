#include "bsp_tm1650.h"
#include "bsp.h"


/**************************************************
	*Smg LED Display Data Data:2020.07.23

	    //                a
        //            ---------
        //           |         |
        //         f |         | b
        //           |    g    |
        //            ---------
        //           |         |
        //         e |         | c
        //           |    d    |
        //            ---------   . <- h
        //
**************************************************/


#define   seg_a  0x01       //seg_a   //seg_e = 0x10
#define   seg_b  0x02      //seg_b , //seg_f = 0x20
#define   seg_c  0x04      //seg_c , //seg_g = 0x40
#define   seg_d  0x08      //seg_d , //seg_d = 0x08
#define   seg_e  0x10      //seg_e , //seg_c = 0x04
#define   seg_f  0x20      //seg_f   //seg_a = 0x01
#define   seg_g  0x40      //seg_g   //seg_b = 0x02 
#define   seg_h  0x80      //


const unsigned char segNumber[]={
         seg_a+seg_b+seg_c+seg_d+seg_e+seg_f,        // char "0"  0x00
         seg_b+seg_c,                                // char "1"  0x01
         seg_a+seg_b+seg_d+seg_e+seg_g,              // char "2"  0x02
         seg_a+seg_b+seg_c+seg_d+seg_g,              // char "3"  0x03
         seg_b+seg_c+seg_f+seg_g,                    // char "4"  0x04
         seg_a+seg_c+seg_d+seg_f+seg_g,              // char "5"  0x05
         seg_a+seg_c+seg_d+seg_e+seg_f+seg_g,        // char "6"  0x06
         seg_a+seg_b+seg_c+seg_f,                    // char "7"  0x07
         seg_a+seg_b+seg_c+seg_d+seg_e+seg_f+seg_g,  // char "8"  0x08
         seg_a+seg_b+seg_c+seg_d+seg_f+seg_g,        // char "9"  0x09
         seg_h,                                      // char "."  0x0A
         0                                           // Dont't display 0x0b
         
};






//static void IIC_Init_TM1650(void);
static void IIC_Start_TM1650(void);
static void IIC_Stop_TM1650(void);
static void IIC_Ack_TM1650(void);
static void IIC_NAck_TM1650(void);
static uint8_t IIC_Wait_Ack_TM1650(void);
static void IIC_WrByte_TM1650(uint8_t number);






/******************************************************************************
 ** 
 ** Function Name: void IIC_Init_TM1650(void)
 ** Function:          	
 ** Input Ref:
 ** Return Ref: 
 **  
 ******************************************************************************/

/******************************************************************************
 ** 
 ** Function Name: void IIC_Start_TM1650(void)
 ** Function:          	
 ** Input Ref:
 ** Return Ref: 
 **  
 ******************************************************************************/
static void IIC_Start_TM1650(void)
{
	
	  // SDA 1->0 while SCL High
    //SCL高电平期间，SDA出现一个下降沿表示起始信号
     I2C_SDA_OUT();//SDA_OUT();
   
    SDA_SetHigh();//SDA = 1;      //数据线先保持为高，起始信号要该口的下降沿 
   
    SCL_SetHigh() ;//= 1;        //时钟线保持为高            
     // asm("nop"); ;//有一个大概5us的延时具体以器件而定    
     delay_us(5);   
     SDA_SetLow();//SDA = 0;        //数据线拉低出现下降沿           
     delay_us(4);
     SCL_SetLow();//SCL = 0;        //拉低时钟线，保证接下来数据线允许改变    
        
}

/******************************************************************************
 ** 
 ** Function Name: void I2C_Stop_TM1650(void)
 ** Function:          	
 ** Input Ref:
 ** Return Ref: 
 **  
 ******************************************************************************/
static void IIC_Stop_TM1650(void)
{

	// SDA 0->1 while SCL High
    //SCL高电平期间，SDA产生一个上升沿 表示停止
    I2C_SDA_OUT();//SDA_OUT();//SET_SDA_OUT_TM1650();
    SCL_SetLow();//SCL= 0;
    SDA_SetLow();//SDA = 0;    //保证数据线为低电平
    delay_us(4);
    SCL_SetHigh();//SCL= 1;   //先保证时钟线为高电平
    delay_us(1);    //延时 以得到一个可靠的电平信号            
    SDA_SetHigh();//SDA = 1;        //数据线出现上升沿           
    delay_us(4);    //延时 保证一个可靠的高电平   
}
/******************************************************************************
 ** 
 ** Function Name: void IIC_Ack_TM1650(void)
 ** Function:          	
 ** Input Ref:
 ** Return Ref: 
 **  
 ******************************************************************************/
static void IIC_Ack_TM1650(void)
{
 //数据线一直保持为低电平，时钟线出现上升沿即为应答
  SCL_SetLow();
   I2C_SDA_OUT();//SDA_OUT();//SET_SDA_OUT_TM1650();
  SDA_SetLow();//SDA = 0;
  delay_us(2);
  SCL_SetHigh();//SCL= 0;
  delay_us(2);
  SCL_SetLow(); //应答完成后 将时钟线拉低 允许数据修改
    
}
/******************************************************************************
 ** 
 ** Function Name: void IIC_NAck_TM1650(void)
 ** Function:          	
 ** Input Ref:
 ** Return Ref: 
 **  
 ******************************************************************************/
static void IIC_NAck_TM1650(void)
{
    //非应答即相反 与应答区别即为数据线保持高电平即可
  SCL_SetLow();
  I2C_SDA_OUT();//SDA_OUT();//SET_SDA_OUT_TM1650();
  SDA_SetHigh();  //SDA = 1;
  delay_us(2);
  SCL_SetHigh();//SCL= 1;
  delay_us(2);
  SCL_SetLow();//最后要将时钟线拉低 允许数据变化
   
}

/******************************************************************************
 ** 
 ** Function Name: IIC_Wait_Ack_TM1650(void)
 ** Function:   等待应答信号    	
 ** Input Ref:
 ** Return Ref: 0:应答 1：无应答
 **  
 ******************************************************************************/
static uint8_t IIC_Wait_Ack_TM1650(void)
{
    //应答等待计数
    uint8_t ackTime = 0;
    
    I2C_SDA_IN();//SDA_IN();//SET_SDA_IN_TM1650();//切换为输入模式
  
    //时钟线拉高
    SCL_SetHigh();//SCL = 1;
    delay_us(2);
    //等待数据线拉低应答
    while(Read_SDA()){
        //如果在该时间内仍未拉低
        ackTime ++;
        if(ackTime > 250)
        {
            //认为非应答 停止信号
            IIC_Stop_TM1650();
            return 1;
        }
    }
    SCL_SetLow();//SCL = 0;
    return 0 ;
}
/******************************************************************************
 ** 
 ** Function Name: void IIC_WrByte_TM1650(uint8_t number)
 ** Function:     	
 ** Input Ref:
 ** Return Ref:
 ** 
 ******************************************************************************/
static void IIC_WrByte_TM1650(uint8_t oneByte)
{
    //定义一个计数变量
    uint8_t i;
    SCL_SetLow();//SCL =0; //将时钟线拉低允许数据改变
    delay_us(1);
    I2C_SDA_OUT();//SDA_OUT();//SET_SDA_OUT_TM1650();
   
    //按位发送数据
    for(i = 0;i < 8; i ++)
    {
    
    if((oneByte&0x80))            //高位在前发送数据
        SDA_SetHigh();//SDA=1;
    else                     //  TM1650_DIO_L;
        SDA_SetLow();//SDA= 0;
          
        //delay_us(1);
        SCL_SetHigh();//SCL=1;
        delay_us(1);
        SCL_SetLow();//SCL=0;//TM1650_CLK_L;
       // delay_us(1);
        oneByte<<=1;        

    }
   //  delay_us(1);
}
/******************************************************************************
 ** 
 ** Function Name: static void TM1650_Write_Data(uint8_t address,uint8_t data) 
 ** Function:     数码管显示	
 ** Input Ref:
 ** Return Ref:
 ** 
 ******************************************************************************/
void TM1650_Write_Data(uint8_t address,uint8_t data) 
{
	//写显存必须从高地址开始写
	IIC_Start_TM1650();
  	IIC_WrByte_TM1650(address); // display of address 
	IIC_Ack_TM1650();
	IIC_WrByte_TM1650(data);  //display of data
	IIC_Ack_TM1650();
	IIC_Stop_TM1650();
}

/******************************************************************************
 ** 
 ** Function Name: void Smg_Display_Temp_Degree_Handler(void)
 ** Function:    
 ** Input Ref: number :display times numbers 
 ** Return Ref:
 ** 
 ******************************************************************************/
void Smg_Display_Temp_Degree_Handler(void)
{
      uint8_t decade_temp=0,uint_temp=0,decimal_point;


      decade_temp  = ctl_t.temperature_value / 10;
      uint_temp =   ctl_t.temperature_value  % 10;
	

	  decimal_point =   ctl_t.temperature_decimal_point_value ;
   
 
  
	TM1650_Write_Data(0x48,0x71);//初始化为7级灰度，开显示,power off TM1650_Set(0x48,0x30);
	TM1650_Write_Data(0x68,segNumber[decade_temp]);//bit one  
   

	TM1650_Write_Data(0x6A,segNumber[uint_temp]+seg_h);//bit two


    TM1650_Write_Data(0x6C,segNumber[decimal_point]);//decimal_point
	
}


void I2C_SDA_IN(void) 	//ÅäÖÃ³ÉÊäÈë  
{  
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = SDA_Pin 	;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);
} 
void I2C_SDA_OUT(void)//ÅäÖÃ³ÉÊä³ö
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = SDA_Pin 	;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);
	
}


/******************************************************************************
 ** 
 ** Function Name: void Smg_Display_Digital_Numbers_Changed(uint8_t numbers)
 ** Function:    
 ** Input Ref: number :display times numbers 
 ** Return Ref:
 ** 
 ******************************************************************************/
void Smg_Display_Digital_Numbers_Changed(uint8_t numbers)
{

    uint8_t decade_temp=0,uint_temp=0,decimal_point;


      decade_temp  = numbers / 10;
      uint_temp =   numbers  % 10;

	  decimal_point =   0;//ctl_t.temperature_decimal_point_value ;
   
 
//   if(ctl_t.gTimer_smg_turn_on < 100){ //20*10 =200ms
	     TM1650_Write_Data(0x48,0x71);//初始化为7级灰度，开显示,power off TM1650_Set(0x48,0x30);
	     
		
		TM1650_Write_Data(0x68,segNumber[decade_temp]);//bit one  
	   

		TM1650_Write_Data(0x6A,segNumber[uint_temp]+seg_h);//bit two


	    TM1650_Write_Data(0x6C,segNumber[decimal_point]);//decimal_point



}



/******************************************************************************
 ** 
 ** Function Name: void Smg_Display_Digital_Numbers_Changed(uint8_t numbers)
 ** Function:    
 ** Input Ref: number :display times numbers 
 ** Return Ref:
 ** 
 ******************************************************************************/
void Repeat_Smg_Display_Digital_Numbers_Changed(uint8_t numbers)
{

    uint8_t decade_temp=0,uint_temp=0,decimal_point;


      decade_temp  = numbers / 10;
      uint_temp =   numbers  % 10;

	  decimal_point =   0;//ctl_t.temperature_decimal_point_value ;
   
 
   if(ctl_t.gTimer_smg_turn_on < 300){ //20*10 =200ms
	     TM1650_Write_Data(0x48,0x71);//初始化为7级灰度，开显示,power off TM1650_Set(0x48,0x30);
	     
		
		TM1650_Write_Data(0x68,segNumber[decade_temp]);//bit one  
	   

		TM1650_Write_Data(0x6A,segNumber[uint_temp]+seg_h);//bit two


	    TM1650_Write_Data(0x6C,segNumber[decimal_point]);//decimal_point


   	}
	else if(ctl_t.gTimer_smg_turn_on >299  && ctl_t.gTimer_smg_turn_on < 601){

	     TM1650_Write_Data(0x48,0x00);
    }
	else{
	  ctl_t.gTimer_smg_turn_on =0;
	 

	}

	
	

}





