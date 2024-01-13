#include "bsp_flash.h"
#include "bsp.h"


uint32_t FirstPage = 0, NbOfPages = 0;//页管理变量
uint32_t Address = 0, PageError = 0;//地址管理变量
__IO uint64_t data32 = 0 ;//状态变量
__IO uint64_t data32_data2 = 0 ;
static FLASH_EraseInitTypeDef EraseInitStruct;

static uint32_t GetPage(uint32_t Address);//获取页
static uint32_t stmflash_read_halfword(uint32_t faddr);


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
       Tape_Led_Filcker();      //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
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
uint32_t STM32G030F6P6_Read_Flash_Data(void)
{
 //  static uint8_t i=0;
	 Address = FLASH_USER_START_ADDR;


	 while (Address < FLASH_USER_END_ADDR)
	 {
	   
    
       data32 = *(__IO uint64_t *)Address;//读取数据
      
	   Address = Address + 8;
	   data32_data2 =*(uint64_t *)(Address);//读取数据
		 
		
     
	 }
	
	 /*Check if there is an issue to program data*/
	
	
	
   
   return (uint32_t)data32;
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



/**
 * @brief       从指定地址读取一个半字 (16位数据)
 * @param       faddr   : 读取地址 (此地址必须为2的倍数!!)
 * @retval      读取到的数据 (16位)
 */
static uint32_t stmflash_read_halfword(uint32_t faddr)
{
    return *(volatile uint32_t *)faddr;
}

/**
 * @brief       从指定地址开始读出指定长度的数据
 * @param       raddr : 起始地址
 * @param       pbuf  : 数据指针
 * @param       length: 要读取的半字(16位)数,即2个字节的整数倍
 * @retval      无
 */
void stmflash_read(uint32_t raddr, uint32_t *pbuf, uint16_t length)
{
    uint16_t i;

    for (i = 0; i < length; i++)
    {
        pbuf[i] = stmflash_read_halfword(raddr);    /* 读取2个字节 */
        //raddr += 2; /* 偏移2个字节  16bit*/
        raddr += 4; /* 偏移4个字节 32bit */
    }
}




