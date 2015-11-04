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

#include "key_led_control.h"

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

void Camera2LCD_prepare(void);



void Camera_RCC_Configuration(void){

  SystemInit(); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOE , ENABLE);
}
/********************************/







int main(void)
{ 
	
   /* USART1 config */
	USART1_Config();
	SysTick_Init();
	LCD_Init();										/* LCD ��ʼ��*/		
  sd_fs_init();
  Sd_fs_test();
	key_led_init();
	

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

	
	Camera2LCD_prepare();	
	
  while (1)
  {	 
				
// switch (state)
// 		{
//         case 1:
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
								
//         case 2:								 
								 
//         case 3:printf("Wednesday\n");
//         case 4:printf("Thursday\n");
//         case 5:printf("Friday\n");
//         case 6:printf("Saturday\n");
//         case 7:printf("Sunday\n");
//         default:printf("error\n");
//     }
//     return 0;
// 		
		
	}
}



// int Camera_shot(unsigned short int x, unsigned short int y, unsigned short int Width, unsigned short int Height, unsigned char *filename)
// {
//     unsigned char header[54] =
//     {
//         0x42, 0x4d, 0, 0, 0, 0, 
//         0, 0, 0, 0, 54, 0, 
//         0, 0, 40,0, 0, 0, 
//         0, 0, 0, 0, 0, 0, 
//         0, 0, 1, 0, 24, 0, 
//         0, 0, 0, 0, 0, 0, 
//         0, 0, 0, 0, 0, 
//         0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 
//         0, 0, 0
//     };
//     int i;
//     int j;
//     long file_size;     
//     long width;
//     long height;
//     unsigned short int tmp_rgb;
//     unsigned char r,g,b;	
//     unsigned char tmp_name[30];
//     unsigned int mybw;
//     char kk[4]={0,0,0,0};
//     
//     
//    // if(!(Width%4))
//     //    file_size = (long)Width * (long)Height * 3 + 54;
//     //else
//     file_size = (long)Width * (long)Height * 3 + Height*(Width%4) + 54;		//��*�� +������ֽ� + ͷ����Ϣ
//     
//     header[2] = (unsigned char)(file_size &0x000000ff);
//     header[3] = (file_size >> 8) & 0x000000ff;
//     header[4] = (file_size >> 16) & 0x000000ff;
//     header[5] = (file_size >> 24) & 0x000000ff;
//     
//     
//     width=Width;
//     header[18] = width & 0x000000ff;
//     header[19] = (width >> 8) &0x000000ff;
//     header[20] = (width >> 16) &0x000000ff;
//     header[21] = (width >> 24) &0x000000ff;
//     
//     height = Height;
//     header[22] = height &0x000000ff;
//     header[23] = (height >> 8) &0x000000ff;
//     header[24] = (height >> 16) &0x000000ff;
//     header[25] = (height >> 24) &0x000000ff;
//     
//     sprintf((char*)tmp_name,"0:%s.bmp",filename);
//     f_mount(0, &bmpfs[0]);
//     bmpres = f_open( &bmpfsrc , (char*)tmp_name, FA_CREATE_NEW | FA_WRITE);
// 		
// 		
// 		
// 		
//     
//     if ( bmpres == FR_OK )
//     {    
//         bmpres = f_write(&bmpfsrc, header,sizeof(unsigned char)*54, &mybw);
// 			

// /***********************READ Carmera Preparation******************************************/
// 							uint32_t count;
// 							uint16_t CMOS_Data;
// 						
// 							if( Vsync == 2 )   //дͼ�����ݵ�TFT��ʾ��
// 									{	
// 											
// 								 FIFO_RRST_L(); 
// 								 FIFO_RCLK_L();
// 						 
// 								 FIFO_RCLK_H();
// 								 FIFO_RRST_H();
// 								 FIFO_RCLK_L();
// 							
// 								 FIFO_RCLK_H();
// /*********************************************************************/

// 										
// 			for(i=0;i<Height;i++)						//��
//         {
//             if(!(Width%4))
//             {
//                 for(j=0;j<Width;j++)  	//��
//                 { 
//                     
// /************************READ Carmera DATA*****************************/		
// 									FIFO_RCLK_L(); 
// 									CMOS_Data = ((GPIOC->IDR<<8) & 0xff00);	  
// 									FIFO_RCLK_H();
// 									FIFO_RCLK_L();
// 									CMOS_Data |= (((GPIOC->IDR) & 0x00ff));
// 									FIFO_RCLK_H();  
// // 								*(__IO uint16_t *) (Bank1_LCD_D)= CMOS_Data; 		//��FIFO�е�16λ����д����ʾ�� 	
// 									tmp_rgb = CMOS_Data;
// /**********************************************************/									
// 									
// 									
// 									
// //                     tmp_rgb = lcd_GetPoint(j+y,Height-i+x-1);
//                     
//                     r =  GETR_FROM_RGB16(tmp_rgb);
//                     g =  GETG_FROM_RGB16(tmp_rgb);
//                     b =  GETB_FROM_RGB16(tmp_rgb);
//                     
// 									
// 									
// 									
//                     bmpres = f_write(&bmpfsrc, &b,sizeof(unsigned char), &mybw);
//                     bmpres = f_write(&bmpfsrc, &g,sizeof(unsigned char), &mybw);
//                     bmpres = f_write(&bmpfsrc, &r,sizeof(unsigned char), &mybw);
//                     
//                 }
//                 
//             }
//             else
//             {
// 														
// 							
//                 for(j=0;j<Width;j++)
//                 { 
//                     
//                     
// /***********************READ Carmera DATA*****************************/		
// 									FIFO_RCLK_L(); 
// 									CMOS_Data = ((GPIOC->IDR<<8) & 0xff00);	  
// 									FIFO_RCLK_H();
// 									FIFO_RCLK_L();
// 									CMOS_Data |= (((GPIOC->IDR) & 0x00ff));
// 									FIFO_RCLK_H();  
// // 								*(__IO uint16_t *) (Bank1_LCD_D)= CMOS_Data; 		//��FIFO�е�16λ����д����ʾ�� 	
// 									tmp_rgb = CMOS_Data;
// /*********************************************************************/									
// 									
// //                     tmp_rgb =lcd_GetPoint(j+y,Height-i+x-1);
//                     r =  GETR_FROM_RGB16(tmp_rgb);
//                     g =  GETG_FROM_RGB16(tmp_rgb);
//                     b =  GETB_FROM_RGB16(tmp_rgb);
//                     
//                     bmpres = f_write(&bmpfsrc, &b,sizeof(unsigned char), &mybw);
//                     bmpres = f_write(&bmpfsrc, &g,sizeof(unsigned char), &mybw);
//                     bmpres = f_write(&bmpfsrc, &r,sizeof(unsigned char), &mybw);
//                     
//                 }
//                 
//                 bmpres = f_write(&bmpfsrc, kk,sizeof(unsigned char)*(Width%4), &mybw);                
// 		
//             }	
//         }

// /***********************Carmera Reset*****************************/		

// 						 Vsync = 0;
// 				
// /*********************************************************************/									

//         f_close(&bmpfsrc); 
//         return 0;
//     }
//     else if ( bmpres == FR_EXIST )  //����ļ��Ѿ�����
//     {
//         return FR_EXIST;	 					//8
//     }
//     
//     else
//     {
//         return -1;
//     }    
// }


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

void Camera2LCD_prepare(void)
{
  Camera_RCC_Configuration();	                     //ϵͳʱ�����ü�����ʱ��ʹ��	        

//   lcd_wr_zf(40,32,280,32,0x0000,1,&zf3[0]);  //��ʾ�ַ���	
  
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
}

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
