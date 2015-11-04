#ifndef __LCD_DISPLAY_H__
#define __LCD_DISPLAY_H__

#include "stm32f10x.h"

#define Bank1_LCD_Data_Addr		((uint32_t)0x60020000)		//显示区数据地址
#define Bank1_LCD_Cmd_Addr		((uint32_t)0x60000000)		//显示区指令地址

#define Bank1_LCD_D				(*(__IO uint16_t *)(Bank1_LCD_Data_Addr))
#define Bank1_LCD_C				(*(__IO uint16_t *)(Bank1_LCD_Cmd_Addr))

void LCD_Init(void);				//3寸TFT（R61509）初始化函数
void Delay(__IO uint32_t nCnt);		//延时函数
void LCD_Display_Picture(void);	

#endif
