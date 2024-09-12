#include "bsp.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"



/*
**********************************************************************************************************
											宏定义
**********************************************************************************************************
*/
#define FUN_KEY_0	                            (1 << 0)
#define CONFIRM_KEY_1	                        (1 << 1)

#define FUN_LONGK_KEY_CHILD_LOCK_2                (1 << 2)
#define CONFIRM_LONG_KEY_3                      (1 << 3)


/*********************************************************************************************************
											函数声明
***********************************************************************************************************/
//static void vTaskTaskUserIF(void *pvParameters);
static void vTaskRunPro(void *pvParameters);
static void vTaskMsgPro(void *pvParameters);
static void vTaskStart(void *pvParameters);
static void AppTaskCreate (void);


/*
**********************************************************************************************************
											变量声明
**********************************************************************************************************
*/
//static TaskHandle_t xHandleTaskUserIF = NULL;
static TaskHandle_t xHandleTaskRunPro = NULL;
static TaskHandle_t xHandleTaskMsgPro = NULL;
static TaskHandle_t xHandleTaskStart = NULL;


uint32_t confirm_long_key_counter;
uint32_t fun_key_long_counter;


/**********************************************************************************************************
*	函 数 名: main
*	功能说明: 标准c程序入口。
*	形    参：无
*	返 回 值: 无
**********************************************************************************************************/
void freeRTOS_Handler(void)
{
	/* 创建任务 */
	AppTaskCreate();

	/* 创建任务通信机制 */
//	AppObjCreate();
	
    /* 启动调度，开始执行任务 */
    vTaskStartScheduler();
}

/*
*********************************************************************************************************
*	函 数 名: vTaskRunPro
*	功能说明: 使用函数xTaskNotifyWait接收任务vTaskTaskUserIF发送的事件标志位设置
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 3  
*********************************************************************************************************
*/
static void vTaskRunPro(void *pvParameters)
{

  while(1)
  {
     bsp_Idle();
     exit_select_position_flag();
     Main_Process();
   
     vTaskDelay(40);//(40)
  }
	
}
/*
*********************************************************************************************************
*	函 数 名: vTaskMsgPro
*	功能说明: 使用函数xTaskNotifyWait接收任务vTaskTaskUserIF发送的事件标志位设置
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 3  
*********************************************************************************************************
*/
static void vTaskMsgPro(void *pvParameters)
{
    BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(50); /* 设置最大等待时间为300ms */
	uint32_t ulValue;
    static uint8_t fun_long_key_flag,confirm_long_key_flag;
   
	
    while(1)
    {
		/*
			第一个参数 ulBitsToClearOnEntry的作用（函数执行前）：
		          ulNotifiedValue &= ~ulBitsToClearOnEntry
		          简单的说就是参数ulBitsToClearOnEntry那个位是1，那么notification value
		          的那个位就会被清零。

		          这里ulBitsToClearOnEntry = 0x00000000就是函数执行前保留所有位。
		
		    第二个参数 ulBitsToClearOnExit的作用（函数退出前）：			
				  ulNotifiedValue &= ~ulBitsToClearOnExit
		          简单的说就是参数ulBitsToClearOnEntry那个位是1，那么notification value
		          的那个位就会被清零。

				  这里ulBitsToClearOnExi = 0xFFFFFFFF就是函数退出前清楚所有位。
		
		    注：ulNotifiedValue表示任务vTaskMsgPro的任务控制块里面的变量。		
		*/
		
		xResult = xTaskNotifyWait(0x00000000,      
						          0xFFFFFFFF,      
						          &ulValue,        /* 保存ulNotifiedValue到变量ulValue中 */
						          xMaxBlockTime);  /* 最大允许延迟时间   30*/
		
		if( xResult == pdPASS )
		{
			/* 接收到消息，检测那个位被按下 */
             
			if((ulValue & FUN_KEY_0) != 0)
			{
                 if(fun_long_key_flag != 1)
                   gpro_t.key_value = fun_key;
                 
                  fun_key_long_counter=0;
                
            }
            else if((ulValue & CONFIRM_KEY_1) !=0){ 

                 if(confirm_long_key_flag != 1)
                    gpro_t.key_value = confirm_short_key;

                 
                  confirm_long_key_counter=0;
                
            }
            else if((ulValue & FUN_LONGK_KEY_CHILD_LOCK_2) != 0){ //child lock function 
                
                   if(gpro_t.child_lock_flag ==0){
                       gpro_t.child_lock_flag = 1;
                    }
                    else{

                       gpro_t.child_lock_flag = 0;

                    }
                   fun_long_key_flag=1;
                   gpro_t.gTimer_pro_long_key_timer =0;
                  
            }
            else if((ulValue &  CONFIRM_LONG_KEY_3) != 0){

                 gpro_t.key_value  = confirm_long_key;
                 confirm_long_key_flag = 1;
                 gpro_t.gTimer_pro_long_key_timer =0;
             }
            
           
       }
	   else{


          if(gpro_t.child_lock_flag ==0){

           if(gpro_t.key_value == fun_key || gpro_t.key_value == confirm_short_key ||gpro_t.key_value  == confirm_long_key){
            
             Key_Handler(gpro_t.key_value);
            gpro_t.key_value =0xff;//confirm_long_key_flag = 1

           
          }

          }

          if(gpro_t.gTimer_pro_long_key_timer >1 && (confirm_long_key_flag ==1 ||  fun_long_key_flag ==1)){

                 if(confirm_long_key_flag ==1){
                   confirm_long_key_flag =0;

                 confirm_long_key_counter=0;

                 }  

                 if(fun_long_key_flag ==1){
                    fun_long_key_flag =0;
                    fun_key_long_counter=0;
                 } 

              }

          }
             
      }
 }     
 /**********************************************************************************************************
*	函 数 名: vTaskStart
*	功能说明: 启动任务，也就是最高优先级任务，这里用作按键扫描。
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 4  
**********************************************************************************************************/
static void vTaskStart(void *pvParameters)
{
  
    while(1)
    {
		/* 按键扫描 */
		//bsp_KeyScan();
       if(FUN_KEY_VALUE()== KEY_DOWN ){

             confirm_long_key_counter=0;

        while(FUN_KEY_VALUE()== KEY_DOWN && fun_key_long_counter < 2965000){ //child lock is funtion
               
               fun_key_long_counter++;
               if(fun_key_long_counter > 1000099){//1000099(1.5s)//999999(1.2s)//900099(1s)//800001 //1800001(2s)//2960000
                   fun_key_long_counter = 2965900;

                xTaskNotify(xHandleTaskMsgPro, /* 目标任务 */
                            FUN_LONGK_KEY_CHILD_LOCK_2,            /* 设置目标任务事件标志位bit0  */
                            eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/

                }

         }


         if(fun_key_long_counter < 2965900 ){

             
               xTaskNotify(xHandleTaskMsgPro, /* 目标任务 */
					 FUN_KEY_0,            /* 设置目标任务事件标志位bit0  */
					 eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/


         }
     }
	 else if(CONFIRM_KEY_VALUE() == KEY_DOWN ){
         
            fun_key_long_counter=0;
         while(CONFIRM_KEY_VALUE() == KEY_DOWN && confirm_long_key_counter < 2965000){

               confirm_long_key_counter++;
               if(confirm_long_key_counter > 900099){//999999(1.2s)//900099(1s) //800001//1800001(2s)//2960000
                   confirm_long_key_counter = 2965900;
               
               xTaskNotify(xHandleTaskMsgPro, /* 目标任务 */
                         CONFIRM_LONG_KEY_3,            /* 设置目标任务事件标志位bit0  */
                         eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/

                }

            


         }
         
         if(confirm_long_key_counter < 2965900 ){//
            
           xTaskNotify(xHandleTaskMsgPro, /* 目标任务 */
                         CONFIRM_KEY_1,            /* 设置目标任务事件标志位bit0  */
                         eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/

          }
       }


   
   
     
      vTaskDelay(20);
  }
}
/**********************************************************************************************************
*	函 数 名: AppTaskCreate
*	功能说明: 创建应用任务
*	形    参：无
*	返 回 值: 无
**********************************************************************************************************/
static void AppTaskCreate (void)
{

	xTaskCreate( vTaskRunPro,     		/* 任务函数  */
                 "vTaskRunPro",   		/* 任务名    */
                 128,             		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 1,               		/* 任务优先级*/
                 &xHandleTaskRunPro);  /* 任务句柄  */



    xTaskCreate( vTaskMsgPro,     		/* 任务函数  */
                 "vTaskMsgPro",   		/* 任务名    */
                 128,             		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 2,               		/* 任务优先级*/
                 &xHandleTaskMsgPro );  /* 任务句柄  */
	
	
	xTaskCreate( vTaskStart,     		/* 任务函数  */
                 "vTaskStart",   		/* 任务名    */
                 128,            		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 3,              		/* 任务优先级*/
                 &xHandleTaskStart );   /* 任务句柄  */
}





