#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

//#include "Sd_bmp.h"


void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration_key(void);
void numm(void);
void Delay_1(__IO uint32_t nCount);
void key_led_init(void);
 void key_action(void);


