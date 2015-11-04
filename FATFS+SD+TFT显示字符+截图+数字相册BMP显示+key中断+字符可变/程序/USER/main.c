/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * 文件名  ：main.c
 * 描述    ：LCD显示实验，可中英混合显示，可显示BMP格式的图片，可截图，
 *           截图保存在MicroSD卡中，文件名存为myScreen.bmp，
 *           需显示的BMP图片和字库存放在MicroSD卡中。         
 * 实验平台：野火STM32开发板
 * 库版本  ：ST3.5.0
 *
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
**********************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "SysTick.h"
#include "usart1.h"	
#include "lcd.h"
#include "sd_fs_app.h"
#include "Sd_bmp.h"
#include "key_led_control.h"

uint8_t str1[512]="AAD";
uint8_t str2[512]="FUCK YOU!";
uint8_t str3[512]="FUCK YOU AGAIN AGAINNNNN!";

void pic_display(void);

unsigned char 	_key_push = 0;

int main(void)
{
   /* USART1 config */
	USART1_Config();
// 	Key3_GPIO_Config();
	SysTick_Init();
	
	key_led_init();	
	LCD_Init();										/* LCD 初始化*/		
  sd_fs_init();
  Sd_fs_test();
	
// 	/*显示图像*/
//     Lcd_show_bmp(0, 0,"/pic3.bmp");   
//     Lcd_show_bmp( 40,200,"/pic2.bmp");
//     Lcd_show_bmp( 100,100,"/pic1.bmp");	
//  
//     /*横屏显示*/
//     LCD_Str_O(20, 10, "lCD_DEMO",0); 	  
//     LCD_Str_CH(20,30,"阿莫论坛野火专区",0,0xffff);	  
//     LCD_Str_CH_O(20,50,"阿莫论坛野火专区",0);
//     LCD_Num_6x12_O(20, 70, 65535, BLACK);
//     LCD_Str_6x12_O(20, 90,"LOVE STM32", BLACK);	   

//     /*竖屏显示*/     
//     LCD_Str_O_P(300, 10, "Runing", 0);	
//     LCD_Str_CH_P(280,10,"阿莫论坛野火专区欢迎你",0xff,0xffff);		   
//     LCD_Str_CH_O_P(260,10,"阿莫论坛野火专区",0);
//     LCD_Str_6x12_O_P(240, 10,"LOVE STM32", 0);    
//     LCD_Str_ENCH_O_P(220,10,"欢迎使用野火stm32开发板",0);
//     
// 	/*截图*/
//     LCD_Str_CH(20,150,"正在截图",0,0xffff);
//  	Screen_shot(0, 0, 240,320 , "/myScreen");
//     LCD_Str_CH(20,150,"截图完成",0,0xffff);
//     
//  
//  LCD_test();

//	lcd_PutChar(50,30,'A',BLACK,WHITE);


GUI_Text(20,30,"AAAAAAAAAAAAA ",sizeof("AAAAAAAAAAAAA "),0,0xffff);
LCD_Char_CH(200,100,"好",0,0xffff);
LCD_Str_CH(100,200,"呵呵，褚亭强你真牛逼",0,SILVER);	 

Screen_shot(0, 0, 400,240 , "/myScreen");

LCD_Str_O(50,60,str2,BLACK,TEAL);


Lcd_show_bmp(0,0,"/pic3.bmp",1);  	                     // 显示MicroSD卡里面的MYPIC1.bmp文件，图片分辨率1024*768

//LCD_WriteRAM_Prepare();

LCD_Str_O(50,80,str3,BLACK,SILVER);

//  ltk_list_file();
Lcd_show_bmp(0,0,"/1.bmp",1);  	                     // 显示MicroSD卡里面的MYPIC1.bmp文件，图片分辨率1024*768
Lcd_show_bmp(0,0,"/2.bmp",1);  	                     // 显示MicroSD卡里面的MYPIC1.bmp文件，图片分辨率1024*768
  while (1)
  {

 
			pic_display();

	}
	
}

void pic_display(void)
{
	Lcd_show_bmp(0,0,"/pic3.bmp",1);  	                     // 显示MicroSD卡里面的MYPIC1.bmp文件，图片分辨率
	Delay_us(300000);
	Lcd_show_bmp(0,0,"/1.bmp",1);  	                     // 显示MicroSD卡里面的MYPIC1.bmp文件，图片分辨率
	Delay_us(300000);
	Lcd_show_bmp(0,0,"/2.bmp",1);  	                     // 显示MicroSD卡里面的MYPIC1.bmp文件，图片分辨
	Delay_us(300000);
	Lcd_show_bmp(0,0,"/3.bmp",1);  	                     // 显示MicroSD卡里面的MYPIC1.bmp文件，图片分辨率
	Delay_us(300000);
	Lcd_show_bmp(0,0,"/4.bmp",1);  	                     // 显示MicroSD卡里面的MYPIC1.bmp文件，图片分辨率
	Delay_us(300000);
	Lcd_show_bmp(0,0,"/5.bmp",1);  	                     // 显示MicroSD卡里面的MYPIC1.bmp文件，图片分辨
	Delay_us(300000);
	Lcd_show_bmp(0,0,"/6.bmp",1);  	                     // 显示MicroSD卡里面的MYPIC1.bmp文件，图片分辨率
	Delay_us(300000);
	Lcd_show_bmp(0,0,"/myScreen.bmp",1);  	                     // 显示MicroSD卡里面的MYPIC1.bmp文件，图片分辨率
	Delay_us(300000);
}

// void screen_shoot_func(void)
// {
//   LED_GPIO_Config();

// 	EXTI_PB0_Config(); 
// }




/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
