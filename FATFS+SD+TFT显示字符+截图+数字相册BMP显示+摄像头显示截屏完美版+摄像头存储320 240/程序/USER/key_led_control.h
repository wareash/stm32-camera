#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

//#include "Sd_bmp.h"

void LED_GPIO_Config(void);
void EXTI_PA5_Config(void);
static void Key_NVIC_Configuration(void);

void Delay_1(__IO uint32_t nCount);
void key_led_init(void);
void key_action(void);


 //PB5--V6   PD6--V7   PD3--V8
#define ON  0
#define OFF 1

#define LED1(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_5);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_5)

#define LED2(a)	if (a)	\
					GPIO_SetBits(GPIOD,GPIO_Pin_6);\
					else		\
					GPIO_ResetBits(GPIOD,GPIO_Pin_6)

#define LED3(a)	if (a)	\
					GPIO_SetBits(GPIOD,GPIO_Pin_3);\
					else		\
					GPIO_ResetBits(GPIOD,GPIO_Pin_3)

