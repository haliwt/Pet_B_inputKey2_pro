#include "bsp_flash.h"


uint32_t FirstPage = 0, NbOfPages = 0;//页管理变量
uint32_t Address = 0, PageError = 0;//地址管理变量
__IO uint32_t data32 = 0 , MemoryProgramStatus = 0;//状态变量
static FLASH_EraseInitTypeDef EraseInitStruct;

static uint32_t GetPage(uint32_t Address);//获取页

/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
*/

void STM32G030F6P6_Write_Flash_Data(uint64_t data)
{
  /* Unlock the Flash to enable the flash control register access *************/
  HAL_FLASH_Unlock();//FLASH解锁

  FirstPage = GetPage(FLASH_USER_START_ADDR);//获取起始地址所在页
  NbOfPages = 1;//1页
  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;//擦除
  EraseInitStruct.Page        = FirstPage;
  EraseInitStruct.NbPages     = NbOfPages;
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)//擦除FLASH
  {
    while (1)//如果擦除失败，LED闪烁
    {
       //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
	   //HAL_Delay(1000);//1s等待
    }
  }
  Address = FLASH_USER_START_ADDR;
  while (Address < FLASH_USER_END_ADDR)
  {
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, data) == HAL_OK)//把0x1234567812345678写入0x08007800开始的地址
    {
      Address = Address + 8;
    }
   else
    {
      while (1)//如果写入失败，LED闪烁
      {
             //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
			 //HAL_Delay(1000);//1s等待
      }
    }
  }

  HAL_FLASH_Lock();//FLASH锁
}

/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
**/
uint8_t STM32G030F6P6_Read_Flash_Data_Handler(void)
{
  
	 Address = FLASH_USER_START_ADDR;
	 MemoryProgramStatus = 0x0;
	 while (Address < FLASH_USER_END_ADDR)
	 {
	   data32 = *(__IO uint32_t *)Address;//读取数据
	
	   if (data32 != DATA_32)//如果数据不一致，计数器加1
	   {
		 MemoryProgramStatus++;
	   }
	   Address = Address + 4;
	 }
	
	 /*Check if there is an issue to program data*/
	 if (MemoryProgramStatus == 0)//数据全都一致
	 {
	   /* No error detected. Switch on LED0*/
	 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
	 }
	 else
	 {
	   /* Error detected. LED0 will blink with 1s period */
	   while (1)//如果写入跟读出来的不一致，LED闪烁
	   {
		 HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
		 HAL_Delay(1000);
	   }
	 }

   return (uint8_t)data32;
}



}

/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
static uint32_t GetPage(uint32_t Addr)
{
  return (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;;
}







