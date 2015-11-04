/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "sdio_sdcard.h"	
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "OV7670.h"
#include "key_led_control.h"

extern void TimingDelay_Decrement(void);
extern void Delay_us(__IO u32 nTime);

extern unsigned char _it0,_it1,_it2,_it3;
extern char _key_push;
/********************Camera *******************************************/
extern uint8_t Vsync;
/****************************************************************/
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	TimingDelay_Decrement();  /* 系统滴答中断处理函数 */
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

// /****************************************************************************
// * 名    称：void EXTI9_5_IRQHandler(void)
// * 功    能：EXTI9-5中断处理程序
// * 入口参数：无
// * 出口参数：无
// * 说    明：
// * 调用方法：无 
// ****************************************************************************/
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5) != RESET) //确保是否产生了EXTI Line中断
	{
		// LED1 取反		
		GPIO_WriteBit(GPIOB, GPIO_Pin_5, 
			(BitAction)((1-GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5))));
		
		key_action();
		
		EXTI_ClearITPendingBit(EXTI_Line5);     //清除中断标志位
	}  
}
// /****************************************************************************
// * 名    称：void EXTI2_IRQHandler(void)
// * 功    能：EXTI2中断处理程序
// * 入口参数：无
// * 出口参数：无
// * 说    明：
// * 调用方法：无 
// ****************************************************************************/
// void EXTI2_IRQHandler(void)
// {
//   if(EXTI_GetITStatus(EXTI_Line2) != RESET)				  //判别是否有键按下
//   {
// 	_it0=2;	    										  //按键中断标志
// 	key_action();

//     EXTI_ClearITPendingBit(EXTI_Line2);					  //清除中断请求标志
//   }
// }

// /****************************************************************************
// * 名    称：void EXTI3_IRQHandler(void)
// * 功    能：EXTI3中断处理程序
// * 入口参数：无
// * 出口参数：无
// * 说    明：
// * 调用方法：无 
// ****************************************************************************/
// void EXTI3_IRQHandler(void)
// {
//   if(EXTI_GetITStatus(EXTI_Line3) != RESET)				  //判别是否有键按下
//   {
// 	_it0=3;	    										  //按键中断标志	

// 	key_action();
// 		
//     EXTI_ClearITPendingBit(EXTI_Line3);					   //清除中断请求标志
//   }
// }



/*******************************************************************************
* Function Name  : EXTI0_IRQHandler
* Description    : This function handles External lines 0 interrupt request.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void EXTI4_IRQHandler(void)
{
  if ( EXTI_GetITStatus(EXTI_LINE_VSYNC_CMOS) != RESET ) 
  {
     
	 if( Vsync == 0 )
     {
        FIFO_WE_H();
        Vsync = 1;  
        FIFO_WE_H();
     }
     else if( Vsync == 1 )
     {
          FIFO_WE_L();
          Vsync = 2;  
     }					

	 EXTI_ClearITPendingBit(EXTI_LINE_VSYNC_CMOS);
	}	
}
/**
  * @}
  */ 
/*
 * 函数名：SDIO_IRQHandler
 * 描述  ：在SDIO_ITConfig(）这个函数开启了sdio中断	，
 			数据传输结束时产生中断
 * 输入  ：无		 
 * 输出  ：无
 */
void SDIO_IRQHandler(void) 
{
  /* Process All SDIO Interrupt Sources */
  SD_ProcessIRQSrc();
}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
