#ifndef __LCD_DISPLAY_H__
#define __LCD_DISPLAY_H__

#include "stm32f10x.h"

#define Bank1_LCD_Data_Addr		((uint32_t)0x60020000)		//��ʾ�����ݵ�ַ
#define Bank1_LCD_Cmd_Addr		((uint32_t)0x60000000)		//��ʾ��ָ���ַ

#define Bank1_LCD_D				(*(__IO uint16_t *)(Bank1_LCD_Data_Addr))
#define Bank1_LCD_C				(*(__IO uint16_t *)(Bank1_LCD_Cmd_Addr))

void LCD_Init(void);				//3��TFT��R61509����ʼ������
void Delay(__IO uint32_t nCnt);		//��ʱ����
void LCD_Display_Picture(void);	

#endif
