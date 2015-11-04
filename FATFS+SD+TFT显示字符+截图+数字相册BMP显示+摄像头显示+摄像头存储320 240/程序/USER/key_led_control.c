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
 * 函数名：LED_GPIO_Config
 * 描述  ：配置LED用到的I/O口
 * 输入  ：无
 * 输出  ：无  PB5--V6   PD6--V7   PD3--V8
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
  
  /* 配置P[A|B|C|D|E]5为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*
 * 函数名：EXTI_PE5_Config
 * 描述  ：配置 PE5 为线中断口，并设置中断优先级
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
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
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 // 上拉输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* EXTI line(PA5) mode config */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line5;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
}

/****************************************************************************
* 名    称：key_action(void)
* 功    能：键盘处理函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
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
		printf("字符串为: %c\n",pic_shoot_name[i]);
		printf("指针指向元素: %c\n",*point);
		printf("字符串2为: %c\n",pic_shoot_name_2[i]);
		printf("指针2指向元素: %c\n",*point_2);

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

