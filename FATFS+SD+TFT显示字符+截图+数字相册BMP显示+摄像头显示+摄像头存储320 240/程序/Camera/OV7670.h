
#ifndef __OV7670_H
#define __OV7670_H 

/* Includes ------------------------------------------------------------------*/	   
#include "stm32f10x.h"
#define Bank1_LCD_D    ((uint32_t)0x60020000)    //显示区数据地址	  
#define Bank1_LCD_C    ((uint32_t)0x60000000)	 //显示区指令地址

/* Private define ------------------------------------------------------------*/

#define FIFO_CS_PIN     GPIO_Pin_0   /* FIFO片选 */ 
#define FIFO_RRST_PIN   GPIO_Pin_6   /* FIFO读地址复位 */ 
#define FIFO_RCLK_PIN   GPIO_Pin_12   /* FIFO读时钟 */ 
#define FIFO_WE_PIN     GPIO_Pin_5   /* FIFO写允许 */

#define FIFO_CS_H()     GPIOE->BSRR =FIFO_CS_PIN	 
#define FIFO_CS_L()     GPIOE->BRR  =FIFO_CS_PIN

#define FIFO_RRST_H()   GPIOE->BSRR =FIFO_RRST_PIN	
#define FIFO_RRST_L()   GPIOE->BRR  =FIFO_RRST_PIN

#define FIFO_RCLK_H()   GPIOD->BSRR =FIFO_RCLK_PIN
#define FIFO_RCLK_L()   GPIOD->BRR  =FIFO_RCLK_PIN

#define FIFO_WE_H()     GPIOE->BSRR =FIFO_WE_PIN
#define FIFO_WE_L()     GPIOE->BRR  =FIFO_WE_PIN	

#define OV7670							   0x73
#define OV7670_REG_NUM                     114
#define PORT_VSYNC_CMOS                    GPIOE
#define RCC_APB2Periph_PORT_VSYNC_CMOS     RCC_APB2Periph_GPIOE
#define PIN_VSYNC_CMOS                     GPIO_Pin_4
#define EXTI_LINE_VSYNC_CMOS               EXTI_Line4
#define PORT_SOURCE_VSYNC_CMOS             GPIO_PortSourceGPIOE
#define PIN_SOURCE_VSYNC_CMOS              GPIO_PinSource4

/* Private variables ---------------------------------------------------------*/	
extern uint8_t Vsync;

/* Private function prototypes -----------------------------------------------*/
int Sensor_Init(void);
void FIFO_GPIO_Configuration(void);
// void FSMC_LCD_Init(void);							 //FSMC总线配置		  
// void LCD_Init(void);								 //液晶初始化		  
void Camera_NVIC_Configuration(void );						 //帧同步检测中断源配置
// void LCD_Init(void);
// void LCD_WR_REG(unsigned int index);
// void LCD_WR_CMD(unsigned int index,unsigned int val);
// void LCD_WR_Data(unsigned int val);

// void LCD_test(void);
// void LCD_clear(unsigned int p);
// void lcd_wr_zf(u16 StartX, u16 StartY, u16 X, u16 Y, u16 Color, u8 Dir, u8 *chr); 
unsigned char *num_pub(unsigned int a);

#endif
/******************* (C) COPYRIGHT 2013 奋斗STM32 *****END OF FILE****/

