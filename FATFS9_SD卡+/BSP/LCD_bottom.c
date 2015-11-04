
#include "LCD_bottom.h"


//设置系统时钟，通过9倍频，将系统时钟设置为72MHz
void RCC_Configuration(void)
{   
  SystemInit();
}			


void Delay(__IO  uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}





