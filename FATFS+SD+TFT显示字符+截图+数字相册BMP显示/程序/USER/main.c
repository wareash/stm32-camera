/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * �ļ���  ��main.c
 * ����    ��LCD��ʾʵ�飬����Ӣ�����ʾ������ʾBMP��ʽ��ͼƬ���ɽ�ͼ��
 *           ��ͼ������MicroSD���У��ļ�����ΪmyScreen.bmp��
 *           ����ʾ��BMPͼƬ���ֿ�����MicroSD���С�         
 * ʵ��ƽ̨��Ұ��STM32������
 * ��汾  ��ST3.5.0
 *
 * ����    ��wildfire team 
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
**********************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "SysTick.h"
#include "usart1.h"	
#include "lcd.h"
#include "sd_fs_app.h"
#include "Sd_bmp.h"

uint8_t str1[512]="AAD";
uint8_t str2[512]="FUCK YOU!";
uint8_t str3[512]="FUCK YOU AGAIN AGAINNNNN!";

void pic_display(void);



int main(void)
{
   /* USART1 config */
	USART1_Config();
	Key3_GPIO_Config();
	SysTick_Init();
	LCD_Init();										/* LCD ��ʼ��*/		
  sd_fs_init();
  Sd_fs_test();
	

GUI_Text(20,30,"AAAAAAAAAAAAA ",sizeof("AAAAAAAAAAAAA "),0,0xffff);
LCD_Char_CH(200,100,"��",0,0xffff);
LCD_Str_CH(100,200,"�Ǻǣ���ͤǿ����ţ��",0,SILVER);	 

Screen_shot(0, 0, 400,240 , "/myScreen");

LCD_Str_O(50,60,str2,BLACK,TEAL);


Lcd_show_bmp(0,0,"/pic3.bmp",1);  	                     // ��ʾMicroSD�������MYPIC1.bmp�ļ���ͼƬ�ֱ���1024*768

//LCD_WriteRAM_Prepare();

LCD_Str_O(50,80,str3,BLACK,SILVER);

//  ltk_list_file();
Lcd_show_bmp(0,0,"/1.bmp",1);  	                     // ��ʾMicroSD�������MYPIC1.bmp�ļ���ͼƬ�ֱ���1024*768
Lcd_show_bmp(0,0,"/2.bmp",1);  	                     // ��ʾMicroSD�������MYPIC1.bmp�ļ���ͼƬ�ֱ���1024*768
  while (1)
  {
		pic_display();		
	}
	
}

void pic_display(void)
{
	Lcd_show_bmp(0,0,"/pic3.bmp",1);  	                     // ��ʾMicroSD�������MYPIC1.bmp�ļ���ͼƬ�ֱ���
	Delay_us(300000);
	Lcd_show_bmp(0,0,"/1.bmp",1);  	                     // ��ʾMicroSD�������MYPIC1.bmp�ļ���ͼƬ�ֱ���
	Delay_us(300000);
	Lcd_show_bmp(0,0,"/2.bmp",1);  	                     // ��ʾMicroSD�������MYPIC1.bmp�ļ���ͼƬ�ֱ�
	Delay_us(300000);
	Lcd_show_bmp(0,0,"/3.bmp",1);  	                     // ��ʾMicroSD�������MYPIC1.bmp�ļ���ͼƬ�ֱ���
	Delay_us(300000);
	Lcd_show_bmp(0,0,"/4.bmp",1);  	                     // ��ʾMicroSD�������MYPIC1.bmp�ļ���ͼƬ�ֱ���
	Delay_us(300000);
	Lcd_show_bmp(0,0,"/5.bmp",1);  	                     // ��ʾMicroSD�������MYPIC1.bmp�ļ���ͼƬ�ֱ�
	Delay_us(300000);
	Lcd_show_bmp(0,0,"/6.bmp",1);  	                     // ��ʾMicroSD�������MYPIC1.bmp�ļ���ͼƬ�ֱ���
	Delay_us(300000);
	Lcd_show_bmp(0,0,"/myScreen.bmp",1);  	                     // ��ʾMicroSD�������MYPIC1.bmp�ļ���ͼƬ�ֱ���
	Delay_us(300000);

}
	  
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
