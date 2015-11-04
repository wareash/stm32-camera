/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * 文件名  ：main.c
 * 描述    ：MicroSD卡(SDIO模式)测试实验，并将测试信息通过串口1在电脑的超级终端上
 *           打印出来         
 * 实验平台：野火STM32开发板
 * 库版本  ：ST3.5.0
 *
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
*********************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "BSP_TEST.h"

#include "fsmc_sram.h" 
#include "LCD_bottom.h"


int main(void)
{	

	test_config();
	SD_test();

	RCC_Configuration();   			//系统时钟配置为72MHz
	LCD_Init();								 //液晶初始化		
	
  LCD_test();
	
	
  while (1)
  {
		 LCD_test();
}
}

	  
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
