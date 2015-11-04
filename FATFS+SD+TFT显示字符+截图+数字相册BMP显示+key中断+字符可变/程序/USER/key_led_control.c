#include "key_led_control.h"

#include "Sd_bmp.h"

unsigned char _it0=0,num=0;

unsigned char a=0,b=0,c=0;


uint8_t pic_shoot_name[512]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static uint8_t i=0;
unsigned char *point=0;  


// i = pic_shoot_name;
/****************************************************************************
* ��    �ƣ�void RCC_Configuration(void)
* ��    �ܣ�ϵͳʱ������Ϊ72MHZ�� ����ʱ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void RCC_Configuration(void)
{

  SystemInit(); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC 
  						| RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOE , ENABLE);
}
/****************************************************************************
* ��    �ƣ�void GPIO_Configuration(void)
* ��    �ܣ�LED���ƿ��߼���������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void GPIO_Configuration_key(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				    		 //LED1  V6
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_3;		 		//LED2, LED3	 V7 V8
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_SetBits(GPIOB, GPIO_Pin_5);	    				 			//LED1 ��
  GPIO_SetBits(GPIOD, GPIO_Pin_6|GPIO_Pin_3 );	     	            //LED2��LED3��
     
  /* K1 ���ð����ж���PC5 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					    //��������
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* K2 ���ð����ж���PC2 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					    //��������
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  /* K3 ���ð����ж���PC3 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					    //��������
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  /* K4 ���ð����ж���PE6 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					    //��������
  GPIO_Init(GPIOE, &GPIO_InitStructure);	

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 			      //LCD�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_ResetBits(GPIOD, GPIO_Pin_13);			                  //LCD����ر�	
	
}/*
 * ��������EXTI_PC3_Config--------K3
 * ����  ������ PC3 Ϊ���жϿڣ��������ж����ȼ�
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */


void NVIC_Configuration_key(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  /* Configure one bit for preemption priority */
  /* ���ȼ��� ˵������ռ���ȼ����õ�λ��������Ӧ���ȼ����õ�λ��   ��������1�� 3 
   0�飺  ��ռ���ȼ�ռ0λ�� ��Ӧ���ȼ�ռ4λ
   1�飺  ��ռ���ȼ�ռ1λ�� ��Ӧ���ȼ�ռ3λ
   2�飺  ��ռ���ȼ�ռ2λ�� ��Ӧ���ȼ�ռ2λ
   3�飺  ��ռ���ȼ�ռ3λ�� ��Ӧ���ȼ�ռ1λ
   4�飺  ��ռ���ȼ�ռ4λ�� ��Ӧ���ȼ�ռ0λ  */
    
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* Enable the EXTI9-5 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel =EXTI9_5_IRQn;				 //�ⲿ�ж�9-5
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	     //��ռ���ȼ� 0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			 //�����ȼ�1  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 //ʹ��
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;				 //�ⲿ�ж�2
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	     //��ռ���ȼ� 0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			 //�����ȼ�2  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 //ʹ��
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;				 //�ⲿ�ж�3
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	     //��ռ���ȼ� 0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			 //�����ȼ�0  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 //ʹ��
  NVIC_Init(&NVIC_InitStructure);

  //��������AFIO�ⲿ�ж����üĴ���AFIO_EXTICR1������ѡ��EXTI5�ⲿ�жϵ�����Դ��PC5��
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);     //�ⲿ�ж�����AFIO--ETXI9-5
  //��������AFIO�ⲿ�ж����üĴ���AFIO_EXTICR1������ѡ��EXTI2�ⲿ�жϵ�����Դ��PC2��
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);     //�ⲿ�ж�����AFIO--ETXI2
  //��������AFIO�ⲿ�ж����üĴ���AFIO_EXTICR1������ѡ��EXTI3�ⲿ�жϵ�����Դ��PC3��
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource3);     //�ⲿ�ж�����AFIO--ETXI3

  EXTI_InitStructure.EXTI_Line = EXTI_Line5;						//PC5 ��Ϊ����K1 ���״̬
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			    //�ж�ģʽ
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		    //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;						//PC2 ��Ϊ����K2 ���״̬
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			    //�ж�ģʽ
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		    //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  EXTI_InitStructure.EXTI_Line = EXTI_Line3;						//PC3 ��Ϊ����K3 ���״̬
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			    //�ж�ģʽ
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		    //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}



/****************************************************************************
* ��    �ƣ�void numm(void)
* ��    �ܣ����̶�����־����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void numm(void){
   num=0;
   if(_it0==1){			 									  //�������±�־ 
   	 if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)==0){		  //K1  
	  	Delay_1(0x3ffff);										  
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)==0){		  //����������
			while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)==0);//�Ƿ��ɿ�����	 
			num=1; 											  //��ֵ1  ΪK1����
			goto n_exit;
		}
	  }		  
   }
   else if(_it0==2){			 							  //�������±�־ 
   	 if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)==0){		  //K2  
	  	Delay_1(0x3ffff);										  
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)==0){		  //����������
			while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)==0);//�Ƿ��ɿ�����	 
			num=2; 											  //��ֵ2  ΪK2����
			goto n_exit;
		}
	  }		  
   }
   else if(_it0==3){			 							  //�������±�־ 
   	 if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)==0){		  //K3  
	  	Delay_1(0x3ffff);										  
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)==0){		  //����������
			while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)==0);//�Ƿ��ɿ�����	 
			num=3; 											  //��ֵ3  ΪK3����
			goto n_exit;
		}
	  }		  
   }
   n_exit:;
   _it0=0; 
}
/****************************************************************************
* ��    �ƣ�key_action(void)
* ��    �ܣ����̴�����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
 void key_action(void)
{
//  	i=pic_shoot_name;
  numm();																   //����ɨ�����
	if(num==1&&a==0){GPIO_ResetBits(GPIOB, GPIO_Pin_5);a=1;}		       //K1 ����������
	else if(num==1&&a==1){GPIO_SetBits(GPIOB, GPIO_Pin_5);a=0;}

	if(num==2&&b==0){GPIO_ResetBits(GPIOD, GPIO_Pin_6);b=1;}			   //K2 ����������
	else if(num==2&&b==1){GPIO_SetBits(GPIOD, GPIO_Pin_6);b=0;}

	if(num==3&&c==0){GPIO_ResetBits(GPIOD, GPIO_Pin_3);c=1;}			   //K3 ����������
	else if(num==3&&c==1){GPIO_SetBits(GPIOD, GPIO_Pin_3);c=0;}

// 	if(i==pic_shoot_name+sizeof(pic_shoot_name)/sizeof(pic_shoot_name[0]))
	if(i==24)
	{
		i=0;
	}
	else
	{
		i++;
		point=&pic_shoot_name[i];
		printf("�ַ���Ϊ: %c\n",pic_shoot_name[i]);
		printf("ָ��ָ��Ԫ��: %c\n",*point);
 		Screen_shot(0, 0, 400,240 , point);

	}
}
	
void key_led_init(void)
{
  RCC_Configuration();	                           //ϵͳʱ�����ü�����ʱ��ʹ��	        
  
  NVIC_Configuration_key();
    
  /*�Կ���3��LEDָʾ�Ƶ�IO�ڽ����˳�ʼ������3���˿�����Ϊ�������������
  �����ٶ�Ϊ50Mhz�����ж���PC5,PC2,PC3����Ϊ����ģʽ��
  ������ĳ������ʱ������Ӧ�������ڵĶ˿ڵ�ʱ�ӽ���ʹ�ܡ������޷����óɹ��������õ��˶˿�B�Ͷ˿�D��C��E�� 
  ���Ҫ����4���˿ڵ�ʱ�ӽ���ʹ�ܣ�ͬʱ�����õ�����IO�ڹ������������ⲿ�жϡ���˻�Ҫʹ��AFIO�����ù���IO��ʱ�ӡ�*/ 

  GPIO_Configuration_key();
}

	void Delay_1(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

