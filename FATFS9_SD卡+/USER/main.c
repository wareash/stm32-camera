/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * �ļ���  ��main.c
 * ����    ��MicroSD��(SDIOģʽ)����ʵ�飬����������Ϣͨ������1�ڵ��Եĳ����ն���
 *           ��ӡ����         
 * ʵ��ƽ̨��Ұ��STM32������
 * ��汾  ��ST3.5.0
 *
 * ����    ��wildfire team 
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
*********************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "BSP_TEST.h"

#include "fsmc_sram.h" 
#include "LCD_bottom.h"


int main(void)
{	

	test_config();
	SD_test();

	RCC_Configuration();   			//ϵͳʱ������Ϊ72MHz
	LCD_Init();								 //Һ����ʼ��		
	
  LCD_test();
	
	
  while (1)
  {
		 LCD_test();
}
}

	  
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
