#include "key_led_control.h"

#include "Sd_bmp.h"

unsigned char _it0=0,num=0;

unsigned char a=0,b=0,c=0;


uint8_t pic_shoot_name[512]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
uint8_t pic_shoot_name_2[512]="123456789012345678901234567890";

static uint8_t i=0;
unsigned char *point=0;  
unsigned char *point_2=0;  

/*
 * ��������LED_GPIO_Config
 * ����  ������LED�õ���I/O��
 * ����  ����
 * ���  ����  PB5--V6   PD6--V7   PD3--V8
 */
void LED_GPIO_Config(void)   
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE);

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_6 ;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_SetBits(GPIOD, GPIO_Pin_3 | GPIO_Pin_6);	 // turn off all led
	
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_SetBits(GPIOB, GPIO_Pin_5);	 // turn off all led
}

static void Key_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* ����P[A|B|C|D|E]5Ϊ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*
 * ��������EXTI_PE5_Config
 * ����  ������ PE5 Ϊ���жϿڣ��������ж����ȼ�
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void EXTI_PA5_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* config the extiline(PE5) clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);
												
	/* config the NVIC(PE5) */
	Key_NVIC_Configuration();

	/* EXTI line gpio config(PA5) */	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 // ��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* EXTI line(PA5) mode config */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line5;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
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

	if(i==24)
	{
		i=0;
	}
	else
	{
		i++;
		point=&pic_shoot_name[i];
		point_2=&pic_shoot_name_2[i];
		printf("�ַ���Ϊ: %c\n",pic_shoot_name[i]);
		printf("ָ��ָ��Ԫ��: %c\n",*point);
		printf("�ַ���2Ϊ: %c\n",pic_shoot_name_2[i]);
		printf("ָ��2ָ��Ԫ��: %c\n",*point_2);

		LED2(ON);
 		Screen_shot(0, 0, 400,240 , point);
		LED2(OFF);
		LED3(ON);
 		Camera_shot(0, 0, 320,240 , point_2);
		LED3(OFF);
	}
}
	
void key_led_init(void)
{
	LED_GPIO_Config();
	LED1( ON );
	
	/* exti line config */
	EXTI_PA5_Config(); 

}

	void Delay_1(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

