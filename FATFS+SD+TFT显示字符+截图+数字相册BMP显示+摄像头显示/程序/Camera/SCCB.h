
#ifndef __SCCB_H
#define __SCCB_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private define ------------------------------------------------------------*/
#define SCL_H         GPIOE->BSRR = GPIO_Pin_2	 
#define SCL_L         GPIOE->BRR  = GPIO_Pin_2 
   
#define SDA_H         GPIOE->BSRR = GPIO_Pin_3	 
#define SDA_L         GPIOE->BRR  = GPIO_Pin_3	 

#define SCL_read      GPIOE->IDR  & GPIO_Pin_2	 
#define SDA_read      GPIOE->IDR  & GPIO_Pin_3	


#define ADDR_OV7670   0x42

/* Private function prototypes -----------------------------------------------*/
void I2C_Configuration(void);
int I2C_WriteByte( uint16_t WriteAddress , uint8_t SendByte , uint8_t DeviceAddress);
int I2C_ReadByte(uint8_t* pBuffer,   uint16_t length,   uint8_t ReadAddress,  uint8_t DeviceAddress);

#endif 
/******************* (C) COPYRIGHT 2013 ·Ü¶·STM32 *****END OF FILE****/
