/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * 文件名  ：lcdbotton.c
 * 描述    ：lcd底层应用函数库         
 * 实验平台：野火STM32开发板
 * 硬件连接：-----------------------
 *          |见函数LCD_GPIO_Config()|
 *           -----------------------
 * 库版本  ：ST3.5.0
 *
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
******************************************************************************/
#include "lcd_botton.h"
#include "core_cm3.h"
#include "sysTick.h"

void Delay(__IO u32 nCount);



/*横竖屏标志位  0：GRAM指针扫描方向为横屏模式 1：GRAM指针扫描方向为竖屏模式*/
volatile u8 display_direction = 0;	 /*该标志位的作用方便用户识别当前究竟是处于哪种模式*/

#define LCD_ILI9341_CMD(index)       LCD_WR_REG(index)
#define LCD_ILI9341_Parameter(val)	 LCD_WR_Data(val)
									  
#define DEBUG_DELAY()    //Delay_us(500) 	 /*调节延时，如果不延时则直接注释掉即可*/

//写寄存器地址函数
void LCD_WR_REG(unsigned int index)
{
	*(__IO uint16_t *) (Bank1_LCD_C)= index;  

}

//写寄存器数据函数
//输入：dbw 数据位数，1为16位，0为8位。
void LCD_WR_CMD(unsigned int index,unsigned int val)
{	
	*(__IO uint16_t *) (Bank1_LCD_C)= index;	
	*(__IO uint16_t *) (Bank1_LCD_D)= val;
}

//读显示地址数据
unsigned int LCD_RD_data(void){
	unsigned int a=0;
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //空操作
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //读出的实际16位像素数据	  
	return(a);	
}


//写16位像素数据函数
void    LCD_WR_Data(unsigned int val)
{   
	*(__IO uint16_t *) (Bank1_LCD_D)= val; 	
}


/**********************************
 函数名：Set_direction
 函数功能：指定的坐标范围显示一副图片
 入口参数：StartX：行起始坐标，StartY：列起始坐标，Dir：图像显示方向，Pic：图片头指针 
 说    明：图片取模格式为水平扫描，16位颜色模式
*************************************/
void Set_direction(uint16_t StartX, uint16_t StartY, uint8_t Dir)
{
	
// 	uint16_t x = 0, y = 0;
// 	
// // 	x = ((uint16_t)(Pic[2] << 8) + Pic[3]) - 1;		//从图像数组里取出图像的宽度
// // 	y = ((uint16_t)(Pic[4] << 8) + Pic[5]) - 1;		//从图像数组里取出图像的高度

// 	x = 400;		//从图像数组里取出图像的宽度
// 	y = 240;		//从图像数组里取出图像的高度

	
	switch(Dir)
    {
    case 0:
			{	   /*竖屏*/
				LCD_WR_CMD(0x0003, 0x1030);			//图像显示方向为左上起，行递增，列增加
				LCD_WR_CMD(0x0210, 0);			//水平显示区起始地址 0
				LCD_WR_CMD(0x0211, 239);		//水平显示区结束地址 239
				LCD_WR_CMD(0x0212, 0);			//垂直显示区起始地址 0
				LCD_WR_CMD(0x0213, 399);		//垂直显示区结束地址 399

				LCD_WR_CMD(0x0200, 0);			//水平显示区地址 0
				LCD_WR_CMD(0x0201, 0);			//垂直显示区地址 0

				LCD_WR_REG(0x0202);				//写数据到显示区

        }break;
    case 1:
        {		/*横屏*/
					LCD_WR_CMD(0x0003, 0x1008);   				//图像显示方向为左下起，行递增，列递减
					LCD_WR_CMD(0x0210, 0); 				//水平显示区起始地址 0
					LCD_WR_CMD(0x0211, 239);           	//水平显示区结束地址 239
					LCD_WR_CMD(0x0212, 0);     			//垂直显示区起始地址 0
					LCD_WR_CMD(0x0213, 399);         	//垂直显示区结束地址 239
				
					LCD_WR_CMD(0x0200, 0);		          	//水平显示区地址
					LCD_WR_CMD(0x0201, 0);		    	//垂直显示区地址

					LCD_WR_REG(0x0202);				//写数据到显示区

        }break;
        
    }

}

// /*******************************************
//  * 函数名：LCD_GPIO_Config
//  * 描述  ：根据FSMC配置LCD的I/O
//  * 输入  : 无 
//  * 输出  ：无
//  * 举例  ：无
//  * 注意  ：无
// /*******************************************
//  * 函数名：LCD_FSMC_Config
//  * 描述  ：LCD  FSMC 模式配置
//  * 输入  : 无 
//  * 输出  ：无
//  * 举例  ：无
//  * 注意  ：无
// *********************************************/ 
void FSMC_LCD_Init(void)
{
	
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  p;	
  GPIO_InitTypeDef GPIO_InitStructure;
  
  //使能FSMC外设时钟
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE); 

  
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
                         RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE , ENABLE); 	

  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 			  //LCD背光控制
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ; 	 		  //LCD复位
  GPIO_Init(GPIOE, &GPIO_InitStructure);  	
  
 	
  // 复用端口为FSMC接口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
                                GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Set PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10),
     PE.14(D11), PE.15(D12) as alternate function push pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_Init(GPIOE, &GPIO_InitStructure); 


  
  
  //FSMC NE1  LCD片选
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  //FSMC RS---LCD指令 指令/数据	切换
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ; 
  GPIO_Init(GPIOD, &GPIO_InitStructure); 

  GPIO_SetBits(GPIOD, GPIO_Pin_13);			           //LCD背光打开
  
  
  //FSMC接口特性配置
  p.FSMC_AddressSetupTime = 0x02;
  p.FSMC_AddressHoldTime = 0x00;
  p.FSMC_DataSetupTime = 0x05;
  p.FSMC_BusTurnAroundDuration = 0x00;
  p.FSMC_CLKDivision = 0x00;
  p.FSMC_DataLatency = 0x00;
  p.FSMC_AccessMode = FSMC_AccessMode_B;

 
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;	  

 
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 

  /* Enable FSMC Bank1_SRAM Bank */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  
}






/**********************************
 * 函数名：LCD_Rst
 * 描述  ：LCD 软件复位
 * 输入  : 无 
 * 输出  ：无
 * 举例  ：无
 * 注意  ：无
*************************************/
void LCD_Rst(void)
{			
		GPIO_ResetBits(GPIOE, GPIO_Pin_1);	      //PE1 为LCD 复位信号
    Delay(0xAFFFFf);					   
    GPIO_SetBits(GPIOE, GPIO_Pin_1 );		 	 
		Delay(0xAFFFFf);		
}



//  /*
//  * 函数名：Key1_GPIO_Config
//  * 描述  ：配置按键用到的I/O口
//  * 输入  ：无
//  * 输出  ：无
//  */
// void Key3_GPIO_Config(void)
// {
//   GPIO_InitTypeDef GPIO_InitStructure;

//   	/*开启按键端口（PB0）的时钟*/
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

//  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 

// 	GPIO_Init(GPIOC, &GPIO_InitStructure);
// }


//  /*
//  * 函数名：Key_Scan
//  * 描述  ：检测是否有按键按下
//  * 输入  ：GPIOx：x 可以是 A，B，C，D或者 E
//  *		     GPIO_Pin：待读取的端口位 	
//  * 输出  ：KEY_OFF(没按下按键)、KEY_ON（按下按键）
//  */
// u8 Key_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin)
// {			
// 		/*检测是否有按键按下 */
//    	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON ) 
// 	  {	   
// 	 	 /*延时消抖*/
// 	  	Delay_us(1000);		
// 	   		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
// 					{	 
// 						/*等待按键释放 */
// 						while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);   
// 			 			return 	KEY_ON;	 
// 					}
// 			else
// 					return KEY_OFF;
// 		}
// 	else
// 		return KEY_OFF;
// }






/**********************************
 * 函数名：Set_direction
 * 描述  ：设置ILI9341GRAM指针扫描方向
 * 输入  : 0： 横向扫描
 		   1： 纵向扫描
 * 输出  ：无
 * 举例  ：无
 * 注意  ：无
*************************************/
// void Set_direction(u8 option)
// {

//     switch(option)
//     {
//     case 0:
// 	{	    /*横屏*/
//         LCD_ILI9341_CMD(0x36); 
// 		LCD_ILI9341_Parameter(0x68);    //横屏
// 		LCD_ILI9341_CMD(0X2A); 
// 		LCD_ILI9341_Parameter(0x00);	//start 
// 		LCD_ILI9341_Parameter(0x00);
// 		LCD_ILI9341_Parameter(0x01);	//end
// 		LCD_ILI9341_Parameter(0x3F);
// 		
// 		LCD_ILI9341_CMD(0X2B); 
// 		LCD_ILI9341_Parameter(0x00);   //start
// 		LCD_ILI9341_Parameter(0x00);
// 		LCD_ILI9341_Parameter(0x00);   //end
// 		LCD_ILI9341_Parameter(0xEF);
//         display_direction = 0;		
//         }break;
//     case 1:
//         {		/*竖屏*/
//         LCD_ILI9341_CMD(0x36); 
// 	    LCD_ILI9341_Parameter(0x48);	//竖屏 
// 	    LCD_ILI9341_CMD(0X2A); 
// 		LCD_ILI9341_Parameter(0x00);
// 		LCD_ILI9341_Parameter(0x00);
// 		LCD_ILI9341_Parameter(0x00);
// 		LCD_ILI9341_Parameter(0xEF);	
// 		
// 		LCD_ILI9341_CMD(0X2B); 
// 		LCD_ILI9341_Parameter(0x00);
// 		LCD_ILI9341_Parameter(0x00);
// 		LCD_ILI9341_Parameter(0x01);
// 		LCD_ILI9341_Parameter(0x3F);
//         display_direction = 1;
//         }break;
//         
//     }

// }




/**********************************
 * 函数名：LCD_open_windows
 * 描述  ：开窗(以x,y为坐标起点，长为len,高为wid)
 * 输入  : -x    窗户起点
           -y	   窗户起点
           -len  窗户长 
           -wid 窗户宽
 * 输出  ：无
 * 举例  ：无
 * 注意  ：无
*************************************/
// void LCD_open_windows(u16 x,u16 y,u16 len,u16 wid)
// {                    

//     if(display_direction == 0)		/*如果是横屏选项*/
//     {

// 		LCD_ILI9341_CMD(0X2A); 
// 		LCD_ILI9341_Parameter(x>>8);	//start 
// 		LCD_ILI9341_Parameter(x-((x>>8)<<8));
// 		LCD_ILI9341_Parameter((x+len-1)>>8);	//end
// 		LCD_ILI9341_Parameter((x+len-1)-(((x+len-1)>>8)<<8));
// 		
// 		LCD_ILI9341_CMD(0X2B); 
// 		LCD_ILI9341_Parameter(y>>8);   //start
// 		LCD_ILI9341_Parameter(y-((y>>8)<<8));
// 		LCD_ILI9341_Parameter((y+wid-1)>>8);   //end
// 		LCD_ILI9341_Parameter((y+wid-1)-(((y+wid-1)>>8)<<8));
//      
//     }
//     else
//     {
// 		LCD_ILI9341_CMD(0X2B); 
// 		LCD_ILI9341_Parameter(x>>8);
// 		LCD_ILI9341_Parameter(x-((x>>8)<<8));
// 		LCD_ILI9341_Parameter((x+len-1)>>8);
// 		LCD_ILI9341_Parameter((x+len-1)-(((x+len-1)>>8)<<8));

// 	    LCD_ILI9341_CMD(0X2A); 
// 		LCD_ILI9341_Parameter(y>>8);
// 		LCD_ILI9341_Parameter(y-((y>>8)<<8));
// 		LCD_ILI9341_Parameter((y+wid-1)>>8);
// 		LCD_ILI9341_Parameter((y+wid-1)-(((y+wid-1)>>8)<<8));	
//         
//     }

//       LCD_ILI9341_CMD(0x2c);     
// }



// void Lcd_data_start(void)
// {
//          LCD_WR_REG(0x2C);//开始写
// }




// /**********************************
//  * 函数名：Lcd_init_conf
//  * 描述  ：ILI9341 LCD寄存器初始配置
//  * 输入  : 无
//  * 输出  ：无
//  * 举例  ：无
//  * 注意  ：无
// *************************************/
// void Lcd_init_conf(void)
// {
// 	DEBUG_DELAY();
// 	LCD_ILI9341_CMD(0xCF);
// 	LCD_ILI9341_Parameter(0x00);
// 	LCD_ILI9341_Parameter(0x81);
// 	LCD_ILI9341_Parameter(0x30);
// 	
// 	DEBUG_DELAY();
// 	LCD_ILI9341_CMD(0xED);
// 	LCD_ILI9341_Parameter(0x64);
// 	LCD_ILI9341_Parameter(0x03);
// 	LCD_ILI9341_Parameter(0x12);
// 	LCD_ILI9341_Parameter(0x81);
// 	
// 	DEBUG_DELAY();
// 	LCD_ILI9341_CMD(0xE8);
// 	LCD_ILI9341_Parameter(0x85);
// 	LCD_ILI9341_Parameter(0x10);
// 	LCD_ILI9341_Parameter(0x78);
// 	
// 	DEBUG_DELAY();
// 	LCD_ILI9341_CMD(0xCB);
// 	LCD_ILI9341_Parameter(0x39);
// 	LCD_ILI9341_Parameter(0x2C);
// 	LCD_ILI9341_Parameter(0x00);
// 	LCD_ILI9341_Parameter(0x34);
// 	LCD_ILI9341_Parameter(0x02);
// 	
// 	DEBUG_DELAY();
// 	LCD_ILI9341_CMD(0xF7);
// 	LCD_ILI9341_Parameter(0x20);
// 	
// 	DEBUG_DELAY();
// 	LCD_ILI9341_CMD(0xEA);
// 	LCD_ILI9341_Parameter(0x00);
// 	LCD_ILI9341_Parameter(0x00);
// 	
// 	DEBUG_DELAY();
// 	LCD_ILI9341_CMD(0xB1);
// 	LCD_ILI9341_Parameter(0x00);
// 	LCD_ILI9341_Parameter(0x1B);
// 	
// 	DEBUG_DELAY();
// 	LCD_ILI9341_CMD(0xB6);
// 	LCD_ILI9341_Parameter(0x0A);
// 	LCD_ILI9341_Parameter(0xA2);
// 	
// 	DEBUG_DELAY();
// 	LCD_ILI9341_CMD(0xC0);
// 	LCD_ILI9341_Parameter(0x35);
// 	
// 	DEBUG_DELAY();
// 	LCD_ILI9341_CMD(0xC1);
// 	LCD_ILI9341_Parameter(0x11);
// 	
// 	LCD_ILI9341_CMD(0xC5);
// 	LCD_ILI9341_Parameter(0x45);
// 	LCD_ILI9341_Parameter(0x45);
// 	
// 	LCD_ILI9341_CMD(0xC7);
// 	LCD_ILI9341_Parameter(0xA2);
// 	
// 	LCD_ILI9341_CMD(0xF2);
// 	LCD_ILI9341_Parameter(0x00);
// 	
// 	LCD_ILI9341_CMD(0x26);
// 	LCD_ILI9341_Parameter(0x01);
// 	DEBUG_DELAY();
// 	LCD_ILI9341_CMD(0xE0); //Set Gamma
// 	LCD_ILI9341_Parameter(0x0F);
// 	LCD_ILI9341_Parameter(0x26);
// 	LCD_ILI9341_Parameter(0x24);
// 	LCD_ILI9341_Parameter(0x0B);
// 	LCD_ILI9341_Parameter(0x0E);
// 	LCD_ILI9341_Parameter(0x09);
// 	LCD_ILI9341_Parameter(0x54);
// 	LCD_ILI9341_Parameter(0xA8);
// 	LCD_ILI9341_Parameter(0x46);
// 	LCD_ILI9341_Parameter(0x0C);
// 	LCD_ILI9341_Parameter(0x17);
// 	LCD_ILI9341_Parameter(0x09);
// 	LCD_ILI9341_Parameter(0x0F);
// 	LCD_ILI9341_Parameter(0x07);
// 	LCD_ILI9341_Parameter(0x00);
// 	LCD_ILI9341_CMD(0XE1); //Set Gamma
// 	LCD_ILI9341_Parameter(0x00);
// 	LCD_ILI9341_Parameter(0x19);
// 	LCD_ILI9341_Parameter(0x1B);
// 	LCD_ILI9341_Parameter(0x04);
// 	LCD_ILI9341_Parameter(0x10);
// 	LCD_ILI9341_Parameter(0x07);
// 	LCD_ILI9341_Parameter(0x2A);
// 	LCD_ILI9341_Parameter(0x47);
// 	LCD_ILI9341_Parameter(0x39);
// 	LCD_ILI9341_Parameter(0x03);
// 	LCD_ILI9341_Parameter(0x06);
// 	LCD_ILI9341_Parameter(0x06);
// 	LCD_ILI9341_Parameter(0x30);
// 	LCD_ILI9341_Parameter(0x38);
// 	LCD_ILI9341_Parameter(0x0F);
// 	DEBUG_DELAY();
// 	LCD_ILI9341_CMD(0x36); 
// 	LCD_ILI9341_Parameter(0x68);    /*横屏  左上角到(起点)到右下角(终点)扫描方式*/
// 	DEBUG_DELAY();
// 	LCD_ILI9341_CMD(0X2A); 
// 	LCD_ILI9341_Parameter(0x00);
// 	LCD_ILI9341_Parameter(0x00);
// 	LCD_ILI9341_Parameter(0x01);
// 	LCD_ILI9341_Parameter(0x3F);
// 	
// 	DEBUG_DELAY();
// 	LCD_ILI9341_CMD(0X2B); 
// 	LCD_ILI9341_Parameter(0x00);
// 	LCD_ILI9341_Parameter(0x00);
// 	LCD_ILI9341_Parameter(0x00);
// 	LCD_ILI9341_Parameter(0xEF);

// 	DEBUG_DELAY();
// 	LCD_ILI9341_CMD(0x3a); // Memory Access Control
// 	LCD_ILI9341_Parameter(0x55);
// 	LCD_ILI9341_CMD(0x11); //Exit Sleep
// 	Delay_us(1200);
// 	LCD_ILI9341_CMD(0x29); //display on
//     
// 	Set_direction(0);	  //传入0-->初始化默认为横屏方式
// 	LCD_ILI9341_CMD(0x2c); 
// }
// /******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/

