/***********************************************************
                       Ұ��LCD��������
 Ĭ��Ϊ������Ӣ��Ĭ��Ϊ8*16������Ĭ��Ϊ16*16���ַ���Ĭ������롣

 ����д:
 LCD	Һ��		Liquid Crystal Display
 Char	�ַ�		character						�����ʾһ���ֵ���˼����ʾһ�����֣�Ҳ����Char��ʶ
 Str	�ַ���		string
 Num	����		Number
 CH		����		Chinese
 ENCH	��Ӣ����	English-Chinese
 O		����Ч��	overlying
 P		����		Portrait
 6x12	�����СΪ6*12
 R		�Ҷ���		right

 ����˳��:
 		����		��С	   ��������	   ��ʾѡ��   ���뷽ʽ 	  ��Ļѡ��
 LCD----Str 	----6x12	----CH		----O		----R		----P
        Char					ENCH	
 		Num
        XXX				

 ����XXX��ʾPoint��Line��Rectangle�ȵȡ�
 ͬһ�б�ʾͬ������˳��
 ���ֲ����ɲ�Ҫ
***********************************************************/


/***********************************************************
 				Ұ��LCD������������˳�����
 (x,y)	----str		----����δ֪����	----(color,bkColor)
			char		��:len
			num
***********************************************************/

#ifndef __LCD_H
#define	__LCD_H

#include "stm32f10x.h"
#include "lcd_botton.h"
#include "sd_fs_app.h"
#include "asc_font.h"
#include <stdlib.h>

/******������ɫ*****/
#define RED	  0XF800
#define GREEN 0X07E0
#define BLUE  0X001F  
#define BRED  0XF81F
#define GRED  0XFFE0
#define GBLUE 0X07FF
#define BLACK 0X0000
#define WHITE 0XFFFF


void LCD_Init(void);
void Delay(__IO uint32_t nCount);
void lcd_wr_zf(u16 StartX, u16 StartY, u16 X, u16 Y, u16 Color, u8 Dir, u8 *chr);
void LCD_test(void);
unsigned char *num_pub(unsigned  int a);

extern unsigned char zm9[];
extern unsigned char zm8[];
extern unsigned char zm7[];
extern unsigned char zm6[];
extern unsigned char zm5[];
extern unsigned char zm4[];
extern unsigned char zm3[];
extern unsigned char zm2[];
extern unsigned char zm1[];
extern unsigned char zm0[];
extern unsigned char a1[];

extern unsigned char zf2[];
extern unsigned char zf3[];	
extern unsigned char pic[];
extern unsigned char a2[];
extern unsigned char a3[];


/***************  LCD���ƺ��� ***************/
//������Ļ����
void lcd_SetCursor(u16 x,u16 y);
//���ô�������
void lcd_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY);
//��ȡָ���������ɫֵ
u16 lcd_GetPoint(u16 x,u16 y);
//��ָ�����껭��
void lcd_SetPoint(u16 x,u16 y,u16 point);
//��ָ��������ʾһ��8x16�����ascii�ַ�
void lcd_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor);
//��ָ�����괦��ʾ8*16��С���ַ���
void LCD_Str_O(u16 x, u16 y,const u8 *str,u16 Color,u16 bkColor);
//RRRRRGGGGGGBBBBB ��Ϊ BBBBBGGGGGGRRRRR ��ʽ
u16 lcd_BGR2RGB(u16 c);
//��ȡ����������
u16 lcd_ReadData(void);
//��ʾ���������ַ�
void LCD_Char_CH(u16 x,u16 y,const u8 *str,u16 Color,u16 bkColor);
//������ʾ���������ַ�
void LCD_Char_CH_O(u16 x,u16 y,const u8 *str,u16 Color) ;
//��ָ�����괦��ʾ16*16��С��ָ����ɫ�����ַ���
void LCD_Str_CH(u16 x,u16 y,const u8 *str,u16 Color,u16 bkColor);  





/***************  LCD��ͼ����***************/
//��RGB��ɫת��Ϊ16λ��ɫ��
u16 GUI_Color565(u32 RGB);
//��ָ��������ʾ�ַ���
void GUI_Text(u16 x, u16 y, u8 *str, u16 len,u16 Color, u16 bkColor);
//��ָ�����껭ֱ��
void GUI_Line(u16 x0, u16 y0, u16 x1, u16 y1,u16 color);
//��ָ�����껭Բ�������
void GUI_Circle(u16 cx,u16 cy,u16 r,u16 color,u8 fill);
//��ָ�����򻭾��Σ��������ɫ
void GUI_Rectangle(u16 x0, u16 y0, u16 x1, u16 y1,u16 color,u8 fill);
//��ָ�����������Σ��������ɫ
void  GUI_Square(u16 x0, u16 y0, u16 with, u16 color,u8 fill);


void Delay(__IO  uint32_t nCount);


#endif /* __LCD_H */

