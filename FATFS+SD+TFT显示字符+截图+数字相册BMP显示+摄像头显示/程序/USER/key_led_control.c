#include "key_led_control.h"

#include "Sd_bmp.h"

unsigned char _it0=0,num=0;

unsigned char a=0,b=0,c=0;


uint8_t pic_shoot_name[512]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static uint8_t i=0;
unsigned char *point=0;  


// i = pic_shoot_name;
/****************************************************************************
* 名    称：void RCC_Configuration(void)
* 功    能：系统时钟配置为72MHZ， 外设时钟配置
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void RCC_Configuration(void)
{

  SystemInit(); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC 
  						| RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOE , ENABLE);
}
/****************************************************************************
* 名    称：void GPIO_Configuration(void)
* 功    能：LED控制口线及键盘设置
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
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

  GPIO_SetBits(GPIOB, GPIO_Pin_5);	    				 			//LED1 亮
  GPIO_SetBits(GPIOD, GPIO_Pin_6|GPIO_Pin_3 );	     	            //LED2，LED3亮
     
  /* K1 配置按键中断线PC5 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					    //输入上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* K2 配置按键中断线PC2 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					    //输入上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  /* K3 配置按键中断线PC3 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					    //输入上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  /* K4 配置按键中断线PE6 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					    //输入上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);	

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 			      //LCD背光控制
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_ResetBits(GPIOD, GPIO_Pin_13);			                  //LCD背光关闭	
	
}/*
 * 函数名：EXTI_PC3_Config--------K3
 * 描述  ：配置 PC3 为线中断口，并设置中断优先级
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */


void NVIC_Configuration_key(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  /* Configure one bit for preemption priority */
  /* 优先级组 说明了抢占优先级所用的位数，和响应优先级所用的位数   在这里是1， 3 
   0组：  抢占优先级占0位， 响应优先级占4位
   1组：  抢占优先级占1位， 响应优先级占3位
   2组：  抢占优先级占2位， 响应优先级占2位
   3组：  抢占优先级占3位， 响应优先级占1位
   4组：  抢占优先级占4位， 响应优先级占0位  */
    
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* Enable the EXTI9-5 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel =EXTI9_5_IRQn;				 //外部中断9-5
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	     //抢占优先级 0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			 //子优先级1  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 //使能
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;				 //外部中断2
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	     //抢占优先级 0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			 //子优先级2  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 //使能
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;				 //外部中断3
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	     //抢占优先级 0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			 //子优先级0  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 //使能
  NVIC_Init(&NVIC_InitStructure);

  //用于配置AFIO外部中断配置寄存器AFIO_EXTICR1，用于选择EXTI5外部中断的输入源是PC5。
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);     //外部中断配置AFIO--ETXI9-5
  //用于配置AFIO外部中断配置寄存器AFIO_EXTICR1，用于选择EXTI2外部中断的输入源是PC2。
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);     //外部中断配置AFIO--ETXI2
  //用于配置AFIO外部中断配置寄存器AFIO_EXTICR1，用于选择EXTI3外部中断的输入源是PC3。
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource3);     //外部中断配置AFIO--ETXI3

  EXTI_InitStructure.EXTI_Line = EXTI_Line5;						//PC5 作为键盘K1 检测状态
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			    //中断模式
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		    //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;						//PC2 作为键盘K2 检测状态
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			    //中断模式
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		    //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  EXTI_InitStructure.EXTI_Line = EXTI_Line3;						//PC3 作为键盘K3 检测状态
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			    //中断模式
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		    //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}



/****************************************************************************
* 名    称：void numm(void)
* 功    能：键盘动作标志函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void numm(void){
   num=0;
   if(_it0==1){			 									  //按键按下标志 
   	 if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)==0){		  //K1  
	  	Delay_1(0x3ffff);										  
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)==0){		  //按键消抖动
			while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)==0);//是否松开按键	 
			num=1; 											  //键值1  为K1按下
			goto n_exit;
		}
	  }		  
   }
   else if(_it0==2){			 							  //按键按下标志 
   	 if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)==0){		  //K2  
	  	Delay_1(0x3ffff);										  
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)==0){		  //按键消抖动
			while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)==0);//是否松开按键	 
			num=2; 											  //键值2  为K2按下
			goto n_exit;
		}
	  }		  
   }
   else if(_it0==3){			 							  //按键按下标志 
   	 if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)==0){		  //K3  
	  	Delay_1(0x3ffff);										  
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)==0){		  //按键消抖动
			while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)==0);//是否松开按键	 
			num=3; 											  //键值3  为K3按下
			goto n_exit;
		}
	  }		  
   }
   n_exit:;
   _it0=0; 
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
//  	i=pic_shoot_name;
  numm();																   //键盘扫描程序
	if(num==1&&a==0){GPIO_ResetBits(GPIOB, GPIO_Pin_5);a=1;}		       //K1 按下作处理
	else if(num==1&&a==1){GPIO_SetBits(GPIOB, GPIO_Pin_5);a=0;}

	if(num==2&&b==0){GPIO_ResetBits(GPIOD, GPIO_Pin_6);b=1;}			   //K2 按下作处理
	else if(num==2&&b==1){GPIO_SetBits(GPIOD, GPIO_Pin_6);b=0;}

	if(num==3&&c==0){GPIO_ResetBits(GPIOD, GPIO_Pin_3);c=1;}			   //K3 按下作处理
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
		printf("字符串为: %c\n",pic_shoot_name[i]);
		printf("指针指向元素: %c\n",*point);
 		Screen_shot(0, 0, 400,240 , point);

	}
}
	
void key_led_init(void)
{
  RCC_Configuration();	                           //系统时钟设置及外设时钟使能	        
  
  NVIC_Configuration_key();
    
  /*对控制3个LED指示灯的IO口进行了初始化，将3个端口配置为推挽上拉输出，
  口线速度为50Mhz。将中断线PC5,PC2,PC3配置为输入模式。
  在配置某个口线时，首先应对它所在的端口的时钟进行使能。否则无法配置成功，由于用到了端口B和端口D，C，E， 
  因此要对这4个端口的时钟进行使能，同时由于用到复用IO口功能用于配置外部中断。因此还要使能AFIO（复用功能IO）时钟。*/ 

  GPIO_Configuration_key();
}

	void Delay_1(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

