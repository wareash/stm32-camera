
#include "LCD_bottom.h"


//����ϵͳʱ�ӣ�ͨ��9��Ƶ����ϵͳʱ������Ϊ72MHz
void RCC_Configuration(void)
{   
  SystemInit();
}			


void Delay(__IO  uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}





