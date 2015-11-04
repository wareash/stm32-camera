/**************************************************
*                                                 *
*                 作者：gchs2012                  *
*                 日期：2013-07-20                *
*                                                 *
*     奋斗版STM32开发板 TFT（R61509）的LCD实验    *
*                                                 *
***************************************************/

#include "lcd_display.h"

static void RCC_Configuration(void);	//系统时钟配置函数
static void GPIO_Configuration(void);	//GPIO配置函数
static void FSMC_Configuration(void);	//FSMC配置函数

int main(void)
{
	RCC_Configuration();
	GPIO_Configuration();
	FSMC_Configuration();
	LCD_Init();	

	while(1)
	{
		LCD_Display_Picture();	
	}
}

/*** 系统时钟配置函数 ***/
static void RCC_Configuration(void)
{
	SystemInit();
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);
}

/*** GPIO配置函数 ***/
static void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*** PD13（LCD背光控制） ***/
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/*** PE1（LCD复位） ***/
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/*** FSMC的复用端口D0 --- D15 ***/
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
									GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 |
									GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/*** PD4(NOE) and PD5(NWE) and PD7(CS) and PD11(RS) ***/
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/*** LCD背光打开 ***/
	GPIO_SetBits(GPIOD, GPIO_Pin_13); 
}

/*** FSMC配置函数 ***/
static void FSMC_Configuration(void)
{
	FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef FSMC_NORSRAMTimingInitStructure;
	
	/*** FSMC Configuration：FSMC_Bank1_NORSRAM1 Configuration ***/
	FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime      = 0x02;			//地址建立时间（1 ~ 16个HCLK）
	FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime       = 0x00;			//地址保存时间（1 ~ 16个HCLK）
	FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime         = 0x05;			//数据建立时间（0：保留，1：2个HCLK，0xFF：256个HCLK）
	FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0x00;			//总线恢复时间（1 ~ 16个HCLK）
	FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision           = 0x00;			//时钟分频比（定义CLK时钟输出信号的周期，0：保留，1-15表示1个CLK相当于2-16个HCLK）
	FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency           = 0x00;			//数据保持时间（2 - 17个周期）
	FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode            = FSMC_AccessMode_B;	//模式B（一共有4种模式：A，B，C，D）
	
	/*** Color LCD configuration ***/
	FSMC_NORSRAMInitStructure.FSMC_Bank                  = FSMC_Bank1_NORSRAM1;						//Bank1_NORSRAM1
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux        = FSMC_DataAddressMux_Disable;				//地址和数据复用失能
	FSMC_NORSRAMInitStructure.FSMC_MemoryType            = FSMC_MemoryType_NOR;						//连接到相应存储块的外部存储器类型
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth       = FSMC_MemoryDataWidth_16b;				//存储器数据总线宽度16bit
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode       = FSMC_BurstAccessMode_Disable;			//使能或关闭同步NOR闪存存储器的突发访问模式设置是否使用迸发访问模式
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity    = FSMC_WaitSignalPolarity_Low;				//设置WAIT信号的有效电平
	FSMC_NORSRAMInitStructure.FSMC_WrapMode              = FSMC_WrapMode_Disable;					//设置是否使用环回模式
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive      = FSMC_WaitSignalActive_BeforeWaitState;	//设置WAIT信号有效时机
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation        = FSMC_WriteOperation_Enable;				//设定是否使能写操作
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal            = FSMC_WaitSignal_Disable;					//设定是否使用WAIT信号
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode          = FSMC_ExtendedMode_Disable;				//使能或关闭扩展模式，扩展模式用于访问具有不同读写操作时序的存储器，设定是否使用单独的写时序
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst            = FSMC_WriteBurst_Disable;					//设定是否使用迸发写模式
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;		//设定读写时序
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct     = &FSMC_NORSRAMTimingInitStructure;		//设定读写时序
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

	/*** Enable FSMC Bank1_SRAM Bank ****/
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}
