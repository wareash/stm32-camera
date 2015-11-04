/****************************************************************************
* Copyright (C), 2013 奋斗嵌入式工作室 www.ourstm.net
*
* 本例程在 奋斗版STM32开发板V5上调试通过           
* QQ: 9191274, 旺旺：sun68, Email: sun68@163.com 
* 淘宝店铺：ourstm.taobao.com  
*
* 文件名: ov7670.c
* 内容简述:
*    OV7670的寄存器表，帧同步中断配置，以及OV7670+FIFO摄像头的引脚定义
		
*
* 文件历史:
* 版本号  日期       作者    说明
* v0.1    2013-01-27 sun68  创建该文件
*
*****************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "SCCB.h"
#include "OV7670.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_spi.h"
#include "misc.h"
extern void Delay(__IO uint32_t nCount);
/* Private variables ---------------------------------------------------------*/	
extern void Delay_ms(__IO uint32_t nTime);
uint8_t Vsync;	 /* 帧同步信号 */
#define RST   (1<<0)   				// PB14  
#define RST_SET(x) GPIOE->ODR=(GPIOE->ODR&~RST)|(x ? RST:0)  
const uint8_t OV7670_Reg[OV7670_REG_NUM][2]=
{	 
     /*以下为OV7670 QVGA RGB565参数  */
  {0x3a, 0x04},//
	{0x40, 0x10},
	{0x12, 0x14},
	{0x32, 0x80},
	{0x17, 0x16},
        
	{0x18, 0x04},//5
	{0x19, 0x02},
	{0x1a, 0x7b},//0x7a,
	{0x03, 0x06},//0x0a,
	{0x0c, 0x0c},
        {0x15, 0x02},
	{0x3e, 0x00},//10
	{0x70, 0x00},
	{0x71, 0x01},
	{0x72, 0x11},
	{0x73, 0x09},//
        
	{0xa2, 0x02},//15
	{0x11, 0x00},
	{0x7a, 0x20},
	{0x7b, 0x1c},
	{0x7c, 0x28},
        
	{0x7d, 0x3c},//20
	{0x7e, 0x55},
	{0x7f, 0x68},
	{0x80, 0x76},
	{0x81, 0x80},
        
	{0x82, 0x88},
	{0x83, 0x8f},
	{0x84, 0x96},
	{0x85, 0xa3},
	{0x86, 0xaf},
        
	{0x87, 0xc4},//30
	{0x88, 0xd7},
	{0x89, 0xe8},
	{0x13, 0xe0},
	{0x00, 0x00},//AGC
        
	{0x10, 0x00},
	{0x0d, 0x00},
	{0x14, 0x00},//0x38, limit the max gain
	{0xa5, 0x05},
	{0xab, 0x07},
        
	{0x24, 0x75},//40
	{0x25, 0x63},
	{0x26, 0xA5},
	{0x9f, 0x78},
	{0xa0, 0x68},
        
	{0xa1, 0x03},//0x0b,
	{0xa6, 0xdf},//0xd8,
	{0xa7, 0xdf},//0xd8,
	{0xa8, 0xf0},
	{0xa9, 0x90},
        
	{0xaa, 0x94},//50
	{0x13, 0xe5},
	{0x0e, 0x61},
	{0x0f, 0x4b},
	{0x16, 0x02},
        
	{0x1e, 0x37},//0x07,
	{0x21, 0x02},
	{0x22, 0x91},
	{0x29, 0x07},
	{0x33, 0x0b},
        
	{0x35, 0x0b},//60
	{0x37, 0x1d},
	{0x38, 0x71},
	{0x39, 0x2a},
	{0x3c, 0x78},
        
	{0x4d, 0x40},
	{0x4e, 0x20},
	{0x69, 0x5d},
	{0x6b, 0x40},//PLL
	{0x74, 0x19},
	{0x8d, 0x4f},
        
	{0x8e, 0x00},//70
	{0x8f, 0x00},
	{0x90, 0x00},
	{0x91, 0x00},
	{0x92, 0x00},//0x19,//0x66
        
	{0x96, 0x00},
	{0x9a, 0x80},
	{0xb0, 0x84},
	{0xb1, 0x0c},
	{0xb2, 0x0e},
        
	{0xb3, 0x82},//80
	{0xb8, 0x0a},
	{0x43, 0x14},
	{0x44, 0xf0},
	{0x45, 0x34},
        
	{0x46, 0x58},
	{0x47, 0x28},
	{0x48, 0x3a},
	{0x59, 0x88},
	{0x5a, 0x88},
        
	{0x5b, 0x44},//90
	{0x5c, 0x67},
	{0x5d, 0x49},
	{0x5e, 0x0e},
	{0x64, 0x04},
	{0x65, 0x20},
        
	{0x66, 0x05},
	{0x94, 0x04},
	{0x95, 0x08},
	{0x6c, 0x0a},
	{0x6d, 0x55},
        
        
	{0x4f, 0x80},
	{0x50, 0x80},
	{0x51, 0x00},
	{0x52, 0x22},
	{0x53, 0x5e},
	{0x54, 0x80},
        
	
        
        
	{0x6e, 0x11},//100
	{0x6f, 0x9f},//0x9e for advance AWB
    {0x55, 0x00},//亮度
    {0x56, 0x45},//对比度
    {0x57, 0x80},//0x40,  change according to Jim's request	   
};



/*******************************************************************************
* Function Name  : FIFO_GPIO_Configuration
* Description    : FIFO GPIO Configuration 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/  
void FIFO_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;  

 
  /*--适合奋斗V5板的摄像头插座 -------------------------------
  模块引脚名称    | 描述            |     STM32端引脚连接      |
  ------------------------------------------------------------
 3-SCCB_SCL       : SCCB时钟        : 3-PE2   I2C2_SCL
 4-SCCB_SDA       : SCCB数据        : 4-PE3   I2C2_SDA
 5-CAM_VSYNC      : 帧同步          : 5-PE4   外部中断4
  CAM_HREF（未用）: 行同步       
 7-CAM_WEN        : FIFO写允许      : 7-PE5   
   XCLK（未用）   : CMOS传感器主时钟:        
 9-CAM_RRST       : FIFO读地址复位  : 9-PE6   
 10-CAM_REN       : FIFO片选        : GND   
 11-CAM_RCLK      : FIFO读时钟      : 11-PD12
---------------------------------------------------------
  FIFO D0~D7      : FIFO数据输出    : PC0-PC7 
  13-D0							      13-PC0
  14-D1							      14-PC1
  15-D2							      15-PC2
  16-D3							      16-PC3
  17-D4							      17-PC4
  18-D5							      18-PC5
  19-D6							      19-PC6
  20-D7							      20-PC7	
  1-3.3V 						      1
  2,12-GND						      2,12
  -----------------------------------------------------*/
  /* FIFO_RCLK : PD12 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* FIFO_RRST : PE6 */	 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	     
  GPIO_Init(GPIOE, &GPIO_InitStructure);

//  /* FIFO_CS : PE0 */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* FIFO_WEN : PE5 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	
  /* FIFO D[0-7] */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  /* FIFO_VSYNC : PE4 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  /* FVS1003 XRST : PE0 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

}


/****************************************************************************
* 名    称：void NVIC_Configuration(void)
* 功    能：中断源配置
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void Camera_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);							
 
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn ;				 //外部中断4
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	     //抢占优先级 0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			 //子优先级0  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 //使能
  NVIC_Init(&NVIC_InitStructure);

  //用于配置AFIO外部中断配置寄存器AFIO_EXTICR1，用于选择EXTI4外部中断的输入源是PE4。
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);     //外部中断配置AFIO--ETXI4
  
  EXTI_InitStructure.EXTI_Line = EXTI_Line4;					  //PE4 作为摄像头帧同步检测 检测一帧FIFO接收完成
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			  //中断模式
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		  //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
  
}
/*******************************************************************************
* Function Name  : Sensor_Init
* Description    : Sensor初始化
* Input          : None
* Output         : None
* Return         : None
* Attention		 : 返回1成功，返回0失败
*******************************************************************************/ 
int Sensor_Init(void)
{
  uint16_t i=0;
  uint8_t Sensor_IDCode = 0;

  I2C_Configuration();
  if( 0 == I2C_WriteByte ( 0x12, 0x80 , ADDR_OV7670 ) )             /* Reset SCCB */
  {
     return 0 ;
  }

  Delay(1500);

  if( 0 == I2C_ReadByte( &Sensor_IDCode, 1, 0x0b,  ADDR_OV7670 ) )	 /* read ID */
  {
	 return 0;	                              /* error*/
  }

  if(Sensor_IDCode == OV7670)				  /* ID = OV7670 */
  {
  	 for( i=0 ; i < OV7670_REG_NUM ; i++ )
  	 {												
	 	if( 0 == I2C_WriteByte(  OV7670_Reg[i][0], OV7670_Reg[i][1] , ADDR_OV7670 ) )
	    {
			return 0;
	 	}
	 }
  }
  else										  /* NO ID */
  {
      return 0;
  }

  return 1; 
} 
/******************* (C) COPYRIGHT 2013 奋斗STM32 *****END OF FILE****/
