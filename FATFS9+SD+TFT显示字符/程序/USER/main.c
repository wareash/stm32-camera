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


int main(void)
{
   /* USART1 config */
	USART1_Config();

	SysTick_Init();
	LCD_Init();										/* LCD ��ʼ��*/		
  sd_fs_init();
  Sd_fs_test();
	
// 	/*��ʾͼ��*/
//     Lcd_show_bmp(0, 0,"/pic3.bmp");   
//     Lcd_show_bmp( 40,200,"/pic2.bmp");
//     Lcd_show_bmp( 100,100,"/pic1.bmp");	
//  
//     /*������ʾ*/
//     LCD_Str_O(20, 10, "lCD_DEMO",0); 	  
//     LCD_Str_CH(20,30,"��Ī��̳Ұ��ר��",0,0xffff);	  
//     LCD_Str_CH_O(20,50,"��Ī��̳Ұ��ר��",0);
//     LCD_Num_6x12_O(20, 70, 65535, BLACK);
//     LCD_Str_6x12_O(20, 90,"LOVE STM32", BLACK);	   

//     /*������ʾ*/     
//     LCD_Str_O_P(300, 10, "Runing", 0);	
//     LCD_Str_CH_P(280,10,"��Ī��̳Ұ��ר����ӭ��",0xff,0xffff);		   
//     LCD_Str_CH_O_P(260,10,"��Ī��̳Ұ��ר��",0);
//     LCD_Str_6x12_O_P(240, 10,"LOVE STM32", 0);    
//     LCD_Str_ENCH_O_P(220,10,"��ӭʹ��Ұ��stm32������",0);
//     
// 	/*��ͼ*/
//     LCD_Str_CH(20,150,"���ڽ�ͼ",0,0xffff);
//  	Screen_shot(0, 0, 240,320 , "/myScreen");
//     LCD_Str_CH(20,150,"��ͼ���",0,0xffff);
//     
//  
//  LCD_test();

//	lcd_PutChar(50,30,'A',BLACK,WHITE);


GUI_Text(20,30,"AAAAAAAAAAAAA",sizeof("AAAAAAAAAAAAA"),0,0xffff);
LCD_Char_CH(200,100,"��",0,0xffff);
LCD_Str_CH(100,200,"�Ǻǣ���ͤǿ����ţ��",0,BLUE);	 

LCD_Str_O(50,60,str2,BLACK,BLUE);

  while (1)
  {
//  LCD_test();
	}
	
}

	  
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
