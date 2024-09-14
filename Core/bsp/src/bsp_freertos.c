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
//static void vTaskRunPro(void *pvParameters);
static void vTaskMsgPro(void *pvParameters);
static void vTaskStart(void *pvParameters);
static void AppTaskCreate (void);


/*
**********************************************************************************************************
											变量声明
**********************************************************************************************************
*/
//static TaskHandle_t xHandleTaskUserIF = NULL;
//static TaskHandle_t xHandleTaskRunPro = NULL;
static TaskHandle_t xHandleTaskMsgPro = NULL;
static TaskHandle_t xHandleTaskStart = NULL;




typedef struct _KEY_STATE{

    uint8_t  fun_key_flag;
    uint8_t  ok_key_flag;
    uint8_t  fun_key_long_flag;
    uint8_t  ok_key_long_flag; 

    
    uint8_t confirm_long_key_counter;
    uint8_t fun_key_long_counter;



}KEY_STATE_REF;

KEY_STATE_REF  g_ks;


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
//static void vTaskRunPro(void *pvParameters)
//{
//
//  while(1)
//  {
//     bsp_Idle();
//     exit_select_position_flag();
//     Main_Process();
//   
//     vTaskDelay(10);//(40)
//  }
//	
//}
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
  
   while(1)
    {
	   if(g_ks.fun_key_flag == 1){

          if(FUN_KEY_VALUE()== KEY_UP){
             g_ks.fun_key_flag++;

           if(g_ks.fun_key_long_flag == 1){

//             if(gpro_t.child_lock_flag ==0){
//                 gpro_t.child_lock_flag = 1;
//                 Smg_Display_Temp_Degree_And_Char_L_Handler(ctl_t.disp_ntc_res_liner_temp_value);
//                      
//             }
//             else{
//                      
//               gpro_t.child_lock_flag = 0;
//               Smg_Display_Temp_Degree_Handler(ctl_t.disp_ntc_res_liner_temp_value);
//            }
                g_ks.fun_key_long_counter =0;
                gpro_t.gTimer_pro_long_key_timer =0;

           }
           else  if(gpro_t.child_lock_flag ==0){
              g_ks.fun_key_long_counter=0;
              gpro_t.key_value = fun_key ;

           }

          }

        }
        else if(g_ks.ok_key_flag ==1){

            if(CONFIRM_KEY_VALUE() == KEY_UP){
               g_ks.ok_key_flag++;

               if( g_ks.ok_key_long_flag ==1){

                    g_ks.confirm_long_key_counter=0;
                   gpro_t.gTimer_pro_long_key_timer =0;

               }
               else{
                 g_ks.confirm_long_key_counter=0;
                 gpro_t.key_value = confirm_short_key;

               }
              
            }

         }
		
         if(gpro_t.child_lock_flag ==0){

           if(gpro_t.key_value == fun_key || gpro_t.key_value == confirm_short_key){
             g_ks.fun_key_long_counter=0;
             g_ks.confirm_long_key_counter=0;
             Key_Handler(gpro_t.key_value);
            gpro_t.key_value =0xff;//confirm_long_key_flag = 1

           }
           }
           else{

              if(FUN_KEY_VALUE() == KEY_UP &&   gpro_t.child_lock_flag ==1 && g_ks.fun_key_long_flag ==0){
                     g_ks.fun_key_long_counter=0;
              }
           }

          if(gpro_t.gTimer_pro_long_key_timer >1 && (g_ks.fun_key_long_flag ==1 ||g_ks.ok_key_long_flag ==1  )){

                 if(g_ks.ok_key_long_flag ==1){
                     g_ks.ok_key_long_flag =0;

                 }  

                 if(g_ks.fun_key_long_flag ==1){
                    g_ks.fun_key_long_flag =0;
                    
                 } 
          }

           
            Relay_Tunr_OnOff_Fun(gpro_t.relay_id_led);
           
            Main_Process();
            exit_select_position_flag();
            bsp_Idle();

       

       vTaskDelay(20);
             
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

             g_ks.confirm_long_key_counter=0;
             g_ks.fun_key_long_counter++;

          if(g_ks.fun_key_long_counter > 80){ //child lock is funtion
               g_ks.fun_key_long_counter=0;
               g_ks.fun_key_long_flag = 1;
               
               if(gpro_t.child_lock_flag ==0){
                     gpro_t.child_lock_flag = 1;
                     Smg_Display_Temp_Degree_And_Char_L_Handler(ctl_t.disp_ntc_res_liner_temp_value);
                          
                 }
                 else{  
                   gpro_t.child_lock_flag = 0;
                   Smg_Display_Temp_Degree_Handler(ctl_t.disp_ntc_res_liner_temp_value);
                }
                gpro_t.gTimer_pro_long_key_timer =0;

         }
       
         

         if(gpro_t.child_lock_flag ==0)
              g_ks.fun_key_flag = 1;

     }
	 else if(CONFIRM_KEY_VALUE() == KEY_DOWN ){

           g_ks.fun_key_long_counter=0;
        
         if(gpro_t.child_lock_flag ==0){
            g_ks.confirm_long_key_counter++;
     
            if( g_ks.confirm_long_key_counter > 80){
                g_ks.confirm_long_key_counter=0;
                g_ks.ok_key_long_flag =1;
                confirm_key_long_fun();
                gpro_t.gTimer_pro_long_key_timer =0;
            }
         
            g_ks.ok_key_flag =1;

        }
      }


   
   
     
      vTaskDelay(40);
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

//	xTaskCreate( vTaskRunPro,     		/* 任务函数  */
//                 "vTaskRunPro",   		/* 任务名    */
//                 128,             		/* 任务栈大小，单位word，也就是4字节 */
//                 NULL,           		/* 任务参数  */
//                 1,               		/* 任务优先级*/
//                 &xHandleTaskRunPro);  /* 任务句柄  */



    xTaskCreate( vTaskMsgPro,     		/* 任务函数  */
                 "vTaskMsgPro",   		/* 任务名    */
                 128,             		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 1,               		/* 任务优先级*/
                 &xHandleTaskMsgPro );  /* 任务句柄  */
	
	
	xTaskCreate( vTaskStart,     		/* 任务函数  */
                 "vTaskStart",   		/* 任务名    */
                 128,            		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 2,              		/* 任务优先级*/
                 &xHandleTaskStart );   /* 任务句柄  */
}





