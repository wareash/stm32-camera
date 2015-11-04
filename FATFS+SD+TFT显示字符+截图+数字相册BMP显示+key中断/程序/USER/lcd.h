/***********************************************************
                       野火LCD命名规则
 默认为横屏，英文默认为8*16，汉字默认为16*16。字符串默认左对齐。

 各缩写:
 LCD	液晶		Liquid Crystal Display
 Char	字符		character						这里表示一个字的意思，显示一个汉字，也是用Char标识
 Str	字符串		string
 Num	数字		Number
 CH		中文		Chinese
 ENCH	中英混显	English-Chinese
 O		叠加效果	overlying
 P		竖屏		Portrait
 6x12	字体大小为6*12
 R		右对齐		right

 排列顺序:
 		类型		大小	   语言属性	   显示选项   对齐方式 	  屏幕选项
 LCD----Str 	----6x12	----CH		----O		----R		----P
        Char					ENCH	
 		Num
        XXX				

 这里XXX表示Point、Line、Rectangle等等。
 同一列表示同等优先顺序。
 部分参数可不要
***********************************************************/


/***********************************************************
 				野火LCD函数参数定义顺序规则
 (x,y)	----str		----其他未知属性	----(color,bkColor)
			char		如:len
			num
***********************************************************/

#ifndef __LCD_H
#define	__LCD_H

#include "stm32f10x.h"
#include "lcd_botton.h"
#include "sd_fs_app.h"
#include "asc_font.h"
#include <stdlib.h>

/******常用颜色*****/
#define BLACK 	0X0000	//纯黑
#define NAVY	  0X0010	//深蓝 
#define GREEN 	0X0400	//深绿
#define TEAL	  0X0410	//靛青
#define MAROON	0X8000	//深红
#define PURPLE	0X8010	//深紫
#define OLIVE 	0X8400	//褐黄
#define GRAY	  0X8410	//深灰
#define SILVER	0XC618	//浅灰
#define BLUE  	0X001F	//浅蓝
#define LIME	  0X07E0	//浅绿
#define AQUA	  0X07FF	//天蓝
#define RED		  0XF800	//大红
#define FUCHSIA	0XF81F	//品红
#define YELLOW	0XFFE0	//明黄
#define WHITE  	0XFFFF	//亮白

//3寸屏		   
#define LCD_XSIZE 400	   //定义LCD横向分辨率X
#define LCD_YSIZE 240		   //定义LCD横向分辨率Y	
#define LCD_PAGES 4		   //定义LCD横向分辨率Y


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


/***************  LCD控制函数 ***************/
//清屏
void LCD_clear(unsigned short a);
//LCD_WriteRAM_Prepare
void LCD_WriteRAM_Prepare(void);
//LCD_WriteRAM
void LCD_WriteRAM(u16 RGB_Code);
//设置屏幕座标
void lcd_SetCursor(u16 x,u16 y);
//设置窗口区域
void lcd_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY);
//获取指定座标的颜色值
u16 lcd_GetPoint(u16 x,u16 y);
//在指定座标画点
void lcd_SetPoint(u16 x,u16 y,u16 point);
//在指定座标显示一个8x16点阵的ascii字符
void lcd_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor);
//在指定坐标处显示8*16大小的字符串
void LCD_Str_O(u16 x, u16 y,const u8 *str,u16 Color,u16 bkColor);
//RRRRRGGGGGGBBBBB 改为 BBBBBGGGGGGRRRRR 格式
u16 lcd_BGR2RGB(u16 c);
//读取控制器数据
u16 lcd_ReadData(void);
//显示单个汉字字符
void LCD_Char_CH(u16 x,u16 y,const u8 *str,u16 Color,u16 bkColor);
//悬浮显示单个汉字字符
void LCD_Char_CH_O(u16 x,u16 y,const u8 *str,u16 Color) ;
//在指定坐标处显示16*16大小的指定颜色汉字字符串
void LCD_Str_CH(u16 x,u16 y,const u8 *str,u16 Color,u16 bkColor);  
//Lcd块选函数
void Lcd_SetBox(u16 xStart,u16 yStart,u16 W,u16 H);




/***************  LCD画图函数***************/
//将RGB颜色转换为16位颜色。
u16 GUI_Color565(u32 RGB);
//在指定座标显示字符串
void GUI_Text(u16 x, u16 y, u8 *str, u16 len,u16 Color, u16 bkColor);
//在指定座标画直线
void GUI_Line(u16 x0, u16 y0, u16 x1, u16 y1,u16 color);
//在指定座标画圆，可填充
void GUI_Circle(u16 cx,u16 cy,u16 r,u16 color,u8 fill);
//在指定区域画矩形，可填充颜色
void GUI_Rectangle(u16 x0, u16 y0, u16 x1, u16 y1,u16 color,u8 fill);
//在指定区域画正方形，可填充颜色
void  GUI_Square(u16 x0, u16 y0, u16 with, u16 color,u8 fill);


void Delay(__IO  uint32_t nCount);


#endif /* __LCD_H */

