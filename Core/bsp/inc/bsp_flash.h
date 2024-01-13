#ifndef __BSP_FLASH_H
#define __BSP_FLASH_H
#include "main.h"

#define FLASH_USER_START_ADDR   (FLASH_BASE + (15 * FLASH_PAGE_SIZE)) //FLASH写入的起始地址 = 0x08000000+0x00007800 
#define FLASH_USER_END_ADDR     (FLASH_USER_START_ADDR + FLASH_PAGE_SIZE)//FLASH写入的结束地址 = 0x08000000+0x00007800 + 0x000000800= 0x08008000 (32K)

//#define DATA_64                 ((uint64_t)0x1234567812345678)//内容
//#define DATA_32                 ((uint32_t)0x12345678)



void STM32G030F6P6_Write_Flash_Data(uint64_t data);
uint32_t STM32G030F6P6_Read_Flash_Data(void);
void stmflash_read(uint32_t raddr, uint32_t *pbuf, uint16_t length);


#if 0

/* FLASH起始地址 */
#define STM32_FLASH_SIZE        0x8000           /* STM32G030F6P6 FLASH 总大小   32K */
#define STM32_FLASH_BASE        0x08000000      /* STM32 FLASH 起始地址 */

/* STM32G030F7P6 扇区大小 */
#define STM32_SECTOR_SIZE   2048                /* 扇区大小为2K字节 */




/* FLASH解锁键值 */
#define STM32_FLASH_KEY1        0X45670123
#define STM32_FLASH_KEY2        0XCDEF89AB

/* 静态函数(仅限stmflash.c调用) */
static void stmflash_unlock(void);                  /* 解锁STM32 内部FLASH */
static void stmflash_lock(void);                    /* 锁定STM32 内部FLASH */
static uint8_t stmflash_get_error_status(void);     /* 获取FLASH错误状态 */
static uint8_t stmflash_wait_done(uint32_t time);   /* 等待操作完成 */
static uint8_t stmflash_erase_sector(uint32_t saddr);                   /* 擦除扇区 */
static uint8_t stmflash_write_halfword(uint32_t faddr, uint16_t data);  /* FLASH写半字 */

/* 接口函数(外部可调用) */
uint16_t stmflash_read_halfword(uint32_t faddr);                        /* FLASH读半字 */
void stmflash_read(uint32_t raddr, uint16_t *pbuf, uint16_t length);    /* 从指定地址开始读出指定长度的数据 */
void stmflash_write(uint32_t waddr, uint16_t *pbuf, uint16_t length);   /* 在FLASH 指定位置, 写入指定长度的数据(自动擦除) */

/* 测试函数 */
void test_write(uint32_t waddr, uint16_t wdata);



#endif 


#endif 

