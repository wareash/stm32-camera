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
// 	LCD_Init();										/* LCD 初始化*/		
// //   sd_fs_init();
// //   Sd_fs_test();
// 	
// 	Camera_RCC_Configuration();
// 	
// // 	
// // 	GUI_Text(20,30,"AAAAAAAAAAAAA ",sizeof("AAAAAAAAAAAAA "),0,0xffff);
// // LCD_Char_CH(200,100,"好",0,0xffff);
// // LCD_Str_CH(100,200,"呵呵，褚亭强你真牛逼",0,SILVER);	 

// // Screen_shot(0, 0, 400,240 , "/myScreen");

// // LCD_Str_O(50,60,str2,BLACK,TEAL);


// // Lcd_show_bmp(0,0,"/pic3.bmp",1);  	                     // 显示MicroSD卡里面的MYPIC1.bmp文件，图片分辨率1024*768

// // //LCD_WriteRAM_Prepare();

// // LCD_Str_O(50,80,str3,BLACK,SILVER);

// // //  ltk_list_file();
// // Lcd_show_bmp(0,0,"/1.bmp",1);  	                     // 显示MicroSD卡里面的MYPIC1.bmp文件，图片分辨率1024*768
// // Lcd_show_bmp(0,0,"/2.bmp",1);  	                     // 显示MicroSD卡里面的MYPIC1.bmp文件，图片分辨率1024*768


// /************************************For Camera******************************************/
//   lcd_wr_zf(40,32,280,32,0x0000,1,&zf3[0]);  //显示字符串	
// 	Delay_ms(1000);
// 	
// if (SysTick_Config(72000))		    	 //时钟节拍中断时1ms一次  用于定时 
//   { 
//     /* Capture error */ 
//      while (1);
//   }
// //   Delay_ms(1000);
//   FIFO_GPIO_Configuration();				 //摄像头引脚定义
//   RST_SET(0);          					     //xRST = 0   复位vs1003(); 
// //   Delay_ms(200);       					     //延时10ms

//   RST_SET(1);  
//   
// 		  
//   FIFO_WE_H();
//   while( 1 != Sensor_Init() );
//   
// 	
//   NVIC_Configuration();						 //帧同步检测中断源配置
// //   Delay_ms(50);
//   Vsync = 0;
//   LCD_WR_CMD(0x0003,0x1018);   			//图像显示方向为左下起  行递增  列递减
//   LCD_WR_CMD(0x0210, 0); 				//水平显示区起始地址 0-239
//   LCD_WR_CMD(0x0211, 239);           	//水平显示区结束地址 0-239
//   LCD_WR_CMD(0x0212, 40);     			//垂直显示区起始地址 0-399
//   LCD_WR_CMD(0x0213, 359);         		//垂直显示区结束地址 0-399
//   LCD_WR_CMD(0x200, 0);		          	//水平显示区起始地址
//   LCD_WR_CMD(0x201, 359);		      	//垂直显示区起始地址
//   LCD_WR_REG(0x0202);				    //写数据到显示区
//   /* Infinite loop */
//   while (1)
//   {	 
//     uint32_t count;
//     uint16_t CMOS_Data;
//   
//     if( Vsync == 2 )   //写图像数据到TFT显示区
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
// 		  *(__IO uint16_t *) (Bank1_LCD_D)= CMOS_Data; 		//将FIFO中的16位数据写入显示区 	
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
	LCD_Init();										/* LCD 初始化*/		
  sd_fs_init();
  Sd_fs_test();
	

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

	
	
	
	
  Camera_RCC_Configuration();	                     //系统时钟设置及外设时钟使能	        

  lcd_wr_zf(40,32,280,32,0x0000,1,&zf3[0]);  //显示字符串	
  
//   if (SysTick_Config(72000))		    	 //时钟节拍中断时1ms一次  用于定时 
//   { 
//     /* Capture error */ 
//      while (1);
//   }
//   Delay_ms(1000);
  FIFO_GPIO_Configuration();				 //摄像头引脚定义
  RST_SET(0);          					     //xRST = 0   复位vs1003(); 
//   Delay_ms(200);       					     //延时10ms

  RST_SET(1);  
  
		  
  FIFO_WE_H();
  while( 1 != Sensor_Init() );
  
	
  Camera_NVIC_Configuration();						 //帧同步检测中断源配置
//   Delay_ms(50);
	LCD_clear(BLACK);
  Vsync = 0;
  LCD_WR_CMD(0x0003,0x1018);   			//图像显示方向为左下起  行递增  列递减
  LCD_WR_CMD(0x0210, 0); 				//水平显示区起始地址 0-239
  LCD_WR_CMD(0x0211, 239);           	//水平显示区结束地址 0-239
  LCD_WR_CMD(0x0212, 40);     			//垂直显示区起始地址 0-399
  LCD_WR_CMD(0x0213, 359);         		//垂直显示区结束地址 0-399
  LCD_WR_CMD(0x200, 0);		          	//水平显示区起始地址
  LCD_WR_CMD(0x201, 359);		      	//垂直显示区起始地址
  LCD_WR_REG(0x0202);				    //写数据到显示区
  /* Infinite loop */
  while (1)
  {	 
    uint32_t count;
    uint16_t CMOS_Data;
  
    if( Vsync == 2 )   //写图像数据到TFT显示区
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
		  *(__IO uint16_t *) (Bank1_LCD_D)= CMOS_Data; 		//将FIFO中的16位数据写入显示区 	
         }
         Vsync = 0;
       }
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


/****************************************************************************
* 名    称：void Delay(__IO uint32_t nTime)
* 功    能：定时延时程序 1ms为单位
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/  
void Delay_ms(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
