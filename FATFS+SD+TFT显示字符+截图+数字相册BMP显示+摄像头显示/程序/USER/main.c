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

#include "OV7670.h"

// #include "key_led_control.h"

uint8_t str1[512]="AAD";
uint8_t str2[512]="FUCK YOU!";
uint8_t str3[512]="FUCK YOU AGAIN AGAINNNNN!";

void pic_display(void);

// unsigned char 	_key_push = 0;
/***********For Camera***************/
#define RST   (1<<0)   				// PB14  
#define RST_SET(x) GPIOE->ODR=(GPIOE->ODR&~RST)|(x ? RST:0) 

int32_t TimingDelay;
void Delay_ms(__IO uint32_t nTime);

void Camera_RCC_Configuration(void){

  SystemInit(); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOE , ENABLE);
}
/********************************/







// int main(void)
// {		
//    /* USART1 config */	
// // 	USART1_Config();
// // 	Key3_GPIO_Config();
// 	
// // 	key_led_init();	
// 	LCD_Init();										/* LCD ��ʼ��*/		
// //   sd_fs_init();
// //   Sd_fs_test();
// 	
// 	Camera_RCC_Configuration();
// 	
// // 	
// // 	GUI_Text(20,30,"AAAAAAAAAAAAA ",sizeof("AAAAAAAAAAAAA "),0,0xffff);
// // LCD_Char_CH(200,100,"��",0,0xffff);
// // LCD_Str_CH(100,200,"�Ǻǣ���ͤǿ����ţ��",0,SILVER);	 

// // Screen_shot(0, 0, 400,240 , "/myScreen");

// // LCD_Str_O(50,60,str2,BLACK,TEAL);


// // Lcd_show_bmp(0,0,"/pic3.bmp",1);  	                     // ��ʾMicroSD�������MYPIC1.bmp�ļ���ͼƬ�ֱ���1024*768

// // //LCD_WriteRAM_Prepare();

// // LCD_Str_O(50,80,str3,BLACK,SILVER);

// // //  ltk_list_file();
// // Lcd_show_bmp(0,0,"/1.bmp",1);  	                     // ��ʾMicroSD�������MYPIC1.bmp�ļ���ͼƬ�ֱ���1024*768
// // Lcd_show_bmp(0,0,"/2.bmp",1);  	                     // ��ʾMicroSD�������MYPIC1.bmp�ļ���ͼƬ�ֱ���1024*768


// /************************************For Camera******************************************/
//   lcd_wr_zf(40,32,280,32,0x0000,1,&zf3[0]);  //��ʾ�ַ���	
// 	Delay_ms(1000);
// 	
// if (SysTick_Config(72000))		    	 //ʱ�ӽ����ж�ʱ1msһ��  ���ڶ�ʱ 
//   { 
//     /* Capture error */ 
//      while (1);
//   }
// //   Delay_ms(1000);
//   FIFO_GPIO_Configuration();				 //����ͷ���Ŷ���
//   RST_SET(0);          					     //xRST = 0   ��λvs1003(); 
// //   Delay_ms(200);       					     //��ʱ10ms

//   RST_SET(1);  
//   
// 		  
//   FIFO_WE_H();
//   while( 1 != Sensor_Init() );
//   
// 	
//   NVIC_Configuration();						 //֡ͬ������ж�Դ����
// //   Delay_ms(50);
//   Vsync = 0;
//   LCD_WR_CMD(0x0003,0x1018);   			//ͼ����ʾ����Ϊ������  �е���  �еݼ�
//   LCD_WR_CMD(0x0210, 0); 				//ˮƽ��ʾ����ʼ��ַ 0-239
//   LCD_WR_CMD(0x0211, 239);           	//ˮƽ��ʾ��������ַ 0-239
//   LCD_WR_CMD(0x0212, 40);     			//��ֱ��ʾ����ʼ��ַ 0-399
//   LCD_WR_CMD(0x0213, 359);         		//��ֱ��ʾ��������ַ 0-399
//   LCD_WR_CMD(0x200, 0);		          	//ˮƽ��ʾ����ʼ��ַ
//   LCD_WR_CMD(0x201, 359);		      	//��ֱ��ʾ����ʼ��ַ
//   LCD_WR_REG(0x0202);				    //д���ݵ���ʾ��
//   /* Infinite loop */
//   while (1)
//   {	 
//     uint32_t count;
//     uint16_t CMOS_Data;
//   
//     if( Vsync == 2 )   //дͼ�����ݵ�TFT��ʾ��
//     {	
//     
//        FIFO_RRST_L(); 
//        FIFO_RCLK_L();
//    
//        FIFO_RCLK_H();
//        FIFO_RRST_H();
//        FIFO_RCLK_L();
//     
//        FIFO_RCLK_H();
//        
// 	   for( count = 0; count < 76800; count++ )//320*240=76800
//         {
//           FIFO_RCLK_L(); 
//           CMOS_Data = ((GPIOC->IDR<<8) & 0xff00);	  
//           FIFO_RCLK_H();
//           FIFO_RCLK_L();
//           CMOS_Data |= (((GPIOC->IDR) & 0x00ff));
//           FIFO_RCLK_H();  
// 		  *(__IO uint16_t *) (Bank1_LCD_D)= CMOS_Data; 		//��FIFO�е�16λ����д����ʾ�� 	
//          }
//          Vsync = 0;
//        }
// 	}
// /***********************************************************************/
// 	
// }

int main(void)
{ 
	
   /* USART1 config */
	USART1_Config();
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

	
	
	
	
  Camera_RCC_Configuration();	                     //ϵͳʱ�����ü�����ʱ��ʹ��	        

  lcd_wr_zf(40,32,280,32,0x0000,1,&zf3[0]);  //��ʾ�ַ���	
  
//   if (SysTick_Config(72000))		    	 //ʱ�ӽ����ж�ʱ1msһ��  ���ڶ�ʱ 
//   { 
//     /* Capture error */ 
//      while (1);
//   }
//   Delay_ms(1000);
  FIFO_GPIO_Configuration();				 //����ͷ���Ŷ���
  RST_SET(0);          					     //xRST = 0   ��λvs1003(); 
//   Delay_ms(200);       					     //��ʱ10ms

  RST_SET(1);  
  
		  
  FIFO_WE_H();
  while( 1 != Sensor_Init() );
  
	
  Camera_NVIC_Configuration();						 //֡ͬ������ж�Դ����
//   Delay_ms(50);
	LCD_clear(BLACK);
  Vsync = 0;
  LCD_WR_CMD(0x0003,0x1018);   			//ͼ����ʾ����Ϊ������  �е���  �еݼ�
  LCD_WR_CMD(0x0210, 0); 				//ˮƽ��ʾ����ʼ��ַ 0-239
  LCD_WR_CMD(0x0211, 239);           	//ˮƽ��ʾ��������ַ 0-239
  LCD_WR_CMD(0x0212, 40);     			//��ֱ��ʾ����ʼ��ַ 0-399
  LCD_WR_CMD(0x0213, 359);         		//��ֱ��ʾ��������ַ 0-399
  LCD_WR_CMD(0x200, 0);		          	//ˮƽ��ʾ����ʼ��ַ
  LCD_WR_CMD(0x201, 359);		      	//��ֱ��ʾ����ʼ��ַ
  LCD_WR_REG(0x0202);				    //д���ݵ���ʾ��
  /* Infinite loop */
  while (1)
  {	 
    uint32_t count;
    uint16_t CMOS_Data;
  
    if( Vsync == 2 )   //дͼ�����ݵ�TFT��ʾ��
    {	
    
       FIFO_RRST_L(); 
       FIFO_RCLK_L();
   
       FIFO_RCLK_H();
       FIFO_RRST_H();
       FIFO_RCLK_L();
    
       FIFO_RCLK_H();
       
	   for( count = 0; count < 76800; count++ )//320*240=76800
        {
          FIFO_RCLK_L(); 
          CMOS_Data = ((GPIOC->IDR<<8) & 0xff00);	  
          FIFO_RCLK_H();
          FIFO_RCLK_L();
          CMOS_Data |= (((GPIOC->IDR) & 0x00ff));
          FIFO_RCLK_H();  
		  *(__IO uint16_t *) (Bank1_LCD_D)= CMOS_Data; 		//��FIFO�е�16λ����д����ʾ�� 	
         }
         Vsync = 0;
       }
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

// void screen_shoot_func(void)
// {
//   LED_GPIO_Config();

// 	EXTI_PB0_Config(); 
// }


/****************************************************************************
* ��    �ƣ�void Delay(__IO uint32_t nTime)
* ��    �ܣ���ʱ��ʱ���� 1msΪ��λ
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/  
void Delay_ms(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
