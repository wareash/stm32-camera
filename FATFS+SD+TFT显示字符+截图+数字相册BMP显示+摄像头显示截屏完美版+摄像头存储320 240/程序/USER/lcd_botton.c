/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * �ļ���  ��lcdbotton.c
 * ����    ��lcd�ײ�Ӧ�ú�����         
 * ʵ��ƽ̨��Ұ��STM32������
 * Ӳ�����ӣ�-----------------------
 *          |������LCD_GPIO_Config()|
 *           -----------------------
 * ��汾  ��ST3.5.0
 *
 * ����    ��wildfire team 
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
******************************************************************************/
#include "lcd_botton.h"
#include "core_cm3.h"
#include "sysTick.h"

void Delay(__IO u32 nCount);



/*��������־λ  0��GRAMָ��ɨ�跽��Ϊ����ģʽ 1��GRAMָ��ɨ�跽��Ϊ����ģʽ*/
volatile u8 display_direction = 0;	 /*�ñ�־λ�����÷����û�ʶ��ǰ�����Ǵ�������ģʽ*/

#define LCD_ILI9341_CMD(index)       LCD_WR_REG(index)
#define LCD_ILI9341_Parameter(val)	 LCD_WR_Data(val)
									  
#define DEBUG_DELAY()    //Delay_us(500) 	 /*������ʱ���������ʱ��ֱ��ע�͵�����*/

//д�Ĵ�����ַ����
void LCD_WR_REG(unsigned int index)
{
	*(__IO uint16_t *) (Bank1_LCD_C)= index;  

}

//д�Ĵ������ݺ���
//���룺dbw ����λ����1Ϊ16λ��0Ϊ8λ��
void LCD_WR_CMD(unsigned int index,unsigned int val)
{	
	*(__IO uint16_t *) (Bank1_LCD_C)= index;	
	*(__IO uint16_t *) (Bank1_LCD_D)= val;
}

//����ʾ��ַ����
unsigned int LCD_RD_data(void){
	unsigned int a=0;
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //�ղ���
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //������ʵ��16λ��������	  
	return(a);	
}


//д16λ�������ݺ���
void    LCD_WR_Data(unsigned int val)
{   
	*(__IO uint16_t *) (Bank1_LCD_D)= val; 	
}


/**********************************
 ��������Set_direction
 �������ܣ�ָ�������귶Χ��ʾһ��ͼƬ
 ��ڲ�����StartX������ʼ���꣬StartY������ʼ���꣬Dir��ͼ����ʾ����Pic��ͼƬͷָ�� 
 ˵    ����ͼƬȡģ��ʽΪˮƽɨ�裬16λ��ɫģʽ
*************************************/
void Set_direction(uint16_t StartX, uint16_t StartY, uint8_t Dir)
{
	
// 	uint16_t x = 0, y = 0;
// 	
// // 	x = ((uint16_t)(Pic[2] << 8) + Pic[3]) - 1;		//��ͼ��������ȡ��ͼ��Ŀ��
// // 	y = ((uint16_t)(Pic[4] << 8) + Pic[5]) - 1;		//��ͼ��������ȡ��ͼ��ĸ߶�

// 	x = 400;		//��ͼ��������ȡ��ͼ��Ŀ��
// 	y = 240;		//��ͼ��������ȡ��ͼ��ĸ߶�

	
	switch(Dir)
    {
    case 0:
			{	   /*����*/
				LCD_WR_CMD(0x0003, 0x1030);			//ͼ����ʾ����Ϊ�������е�����������
				LCD_WR_CMD(0x0210, 0);			//ˮƽ��ʾ����ʼ��ַ 0
				LCD_WR_CMD(0x0211, 239);		//ˮƽ��ʾ��������ַ 239
				LCD_WR_CMD(0x0212, 0);			//��ֱ��ʾ����ʼ��ַ 0
				LCD_WR_CMD(0x0213, 399);		//��ֱ��ʾ��������ַ 399

				LCD_WR_CMD(0x0200, 0);			//ˮƽ��ʾ����ַ 0
				LCD_WR_CMD(0x0201, 0);			//��ֱ��ʾ����ַ 0

				LCD_WR_REG(0x0202);				//д���ݵ���ʾ��

        }break;
    case 1:
        {		/*����*/
					LCD_WR_CMD(0x0003, 0x1008);   				//ͼ����ʾ����Ϊ�������е������еݼ�
					LCD_WR_CMD(0x0210, 0); 				//ˮƽ��ʾ����ʼ��ַ 0
					LCD_WR_CMD(0x0211, 239);           	//ˮƽ��ʾ��������ַ 239
					LCD_WR_CMD(0x0212, 0);     			//��ֱ��ʾ����ʼ��ַ 0
					LCD_WR_CMD(0x0213, 399);         	//��ֱ��ʾ��������ַ 239
				
					LCD_WR_CMD(0x0200, 0);		          	//ˮƽ��ʾ����ַ
					LCD_WR_CMD(0x0201, 0);		    	//��ֱ��ʾ����ַ

					LCD_WR_REG(0x0202);				//д���ݵ���ʾ��

        }break;
        
    }

}

// /*******************************************
//  * ��������LCD_GPIO_Config
//  * ����  ������FSMC����LCD��I/O
//  * ����  : �� 
//  * ���  ����
//  * ����  ����
//  * ע��  ����
// /*******************************************
//  * ��������LCD_FSMC_Config
//  * ����  ��LCD  FSMC ģʽ����
//  * ����  : �� 
//  * ���  ����
//  * ����  ����
//  * ע��  ����
// *********************************************/ 
void FSMC_LCD_Init(void)
{
	
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  p;	
  GPIO_InitTypeDef GPIO_InitStructure;
  
  //ʹ��FSMC����ʱ��
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE); 

  
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
                         RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE , ENABLE); 	

  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 			  //LCD�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ; 	 		  //LCD��λ
  GPIO_Init(GPIOE, &GPIO_InitStructure);  	
  
 	
  // ���ö˿�ΪFSMC�ӿ�
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


  
  
  //FSMC NE1  LCDƬѡ
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  //FSMC RS---LCDָ�� ָ��/����	�л�
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ; 
  GPIO_Init(GPIOD, &GPIO_InitStructure); 

  GPIO_SetBits(GPIOD, GPIO_Pin_13);			           //LCD�����
  
  
  //FSMC�ӿ���������
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
 * ��������LCD_Rst
 * ����  ��LCD �����λ
 * ����  : �� 
 * ���  ����
 * ����  ����
 * ע��  ����
*************************************/
void LCD_Rst(void)
{			
		GPIO_ResetBits(GPIOE, GPIO_Pin_1);	      //PE1 ΪLCD ��λ�ź�
    Delay(0xAFFFFf);					   
    GPIO_SetBits(GPIOE, GPIO_Pin_1 );		 	 
		Delay(0xAFFFFf);		
}



//  /*
//  * ��������Key1_GPIO_Config
//  * ����  �����ð����õ���I/O��
//  * ����  ����
//  * ���  ����
//  */
// void Key3_GPIO_Config(void)
// {
//   GPIO_InitTypeDef GPIO_InitStructure;

//   	/*���������˿ڣ�PB0����ʱ��*/
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

//  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 

// 	GPIO_Init(GPIOC, &GPIO_InitStructure);
// }


//  /*
//  * ��������Key_Scan
//  * ����  ������Ƿ��а�������
//  * ����  ��GPIOx��x ������ A��B��C��D���� E
//  *		     GPIO_Pin������ȡ�Ķ˿�λ 	
//  * ���  ��KEY_OFF(û���°���)��KEY_ON�����°�����
//  */
// u8 Key_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin)
// {			
// 		/*����Ƿ��а������� */
//    	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON ) 
// 	  {	   
// 	 	 /*��ʱ����*/
// 	  	Delay_us(1000);		
// 	   		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
// 					{	 
// 						/*�ȴ������ͷ� */
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
 * ��������Set_direction
 * ����  ������ILI9341GRAMָ��ɨ�跽��
 * ����  : 0�� ����ɨ��
 		   1�� ����ɨ��
 * ���  ����
 * ����  ����
 * ע��  ����
*************************************/
// void Set_direction(u8 option)
// {

//     switch(option)
//     {
//     case 0:
// 	{	    /*����*/
//         LCD_ILI9341_CMD(0x36); 
// 		LCD_ILI9341_Parameter(0x68);    //����
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
//         {		/*����*/
//         LCD_ILI9341_CMD(0x36); 
// 	    LCD_ILI9341_Parameter(0x48);	//���� 
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
 * ��������LCD_open_windows
 * ����  ������(��x,yΪ������㣬��Ϊlen,��Ϊwid)
 * ����  : -x    �������
           -y	   �������
           -len  ������ 
           -wid ������
 * ���  ����
 * ����  ����
 * ע��  ����
*************************************/
// void LCD_open_windows(u16 x,u16 y,u16 len,u16 wid)
// {                    

//     if(display_direction == 0)		/*����Ǻ���ѡ��*/
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
//          LCD_WR_REG(0x2C);//��ʼд
// }




// /**********************************
//  * ��������Lcd_init_conf
//  * ����  ��ILI9341 LCD�Ĵ�����ʼ����
//  * ����  : ��
//  * ���  ����
//  * ����  ����
//  * ע��  ����
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
// 	LCD_ILI9341_Parameter(0x68);    /*����  ���Ͻǵ�(���)�����½�(�յ�)ɨ�跽ʽ*/
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
// 	Set_direction(0);	  //����0-->��ʼ��Ĭ��Ϊ������ʽ
// 	LCD_ILI9341_CMD(0x2c); 
// }
// /******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/

