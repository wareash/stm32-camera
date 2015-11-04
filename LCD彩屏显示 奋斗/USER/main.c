/**************************************************
*                                                 *
*                 ���ߣ�gchs2012                  *
*                 ���ڣ�2013-07-20                *
*                                                 *
*     �ܶ���STM32������ TFT��R61509����LCDʵ��    *
*                                                 *
***************************************************/

#include "lcd_display.h"

static void RCC_Configuration(void);	//ϵͳʱ�����ú���
static void GPIO_Configuration(void);	//GPIO���ú���
static void FSMC_Configuration(void);	//FSMC���ú���

int main(void)
{
	RCC_Configuration();
	GPIO_Configuration();
	FSMC_Configuration();
	LCD_Init();	

	while(1)
	{
		LCD_Display_Picture();	
	}
}

/*** ϵͳʱ�����ú��� ***/
static void RCC_Configuration(void)
{
	SystemInit();
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);
}

/*** GPIO���ú��� ***/
static void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*** PD13��LCD������ƣ� ***/
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/*** PE1��LCD��λ�� ***/
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/*** FSMC�ĸ��ö˿�D0 --- D15 ***/
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
									GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 |
									GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/*** PD4(NOE) and PD5(NWE) and PD7(CS) and PD11(RS) ***/
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/*** LCD����� ***/
	GPIO_SetBits(GPIOD, GPIO_Pin_13); 
}

/*** FSMC���ú��� ***/
static void FSMC_Configuration(void)
{
	FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef FSMC_NORSRAMTimingInitStructure;
	
	/*** FSMC Configuration��FSMC_Bank1_NORSRAM1 Configuration ***/
	FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime      = 0x02;			//��ַ����ʱ�䣨1 ~ 16��HCLK��
	FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime       = 0x00;			//��ַ����ʱ�䣨1 ~ 16��HCLK��
	FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime         = 0x05;			//���ݽ���ʱ�䣨0��������1��2��HCLK��0xFF��256��HCLK��
	FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0x00;			//���߻ָ�ʱ�䣨1 ~ 16��HCLK��
	FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision           = 0x00;			//ʱ�ӷ�Ƶ�ȣ�����CLKʱ������źŵ����ڣ�0��������1-15��ʾ1��CLK�൱��2-16��HCLK��
	FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency           = 0x00;			//���ݱ���ʱ�䣨2 - 17�����ڣ�
	FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode            = FSMC_AccessMode_B;	//ģʽB��һ����4��ģʽ��A��B��C��D��
	
	/*** Color LCD configuration ***/
	FSMC_NORSRAMInitStructure.FSMC_Bank                  = FSMC_Bank1_NORSRAM1;						//Bank1_NORSRAM1
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux        = FSMC_DataAddressMux_Disable;				//��ַ�����ݸ���ʧ��
	FSMC_NORSRAMInitStructure.FSMC_MemoryType            = FSMC_MemoryType_NOR;						//���ӵ���Ӧ�洢����ⲿ�洢������
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth       = FSMC_MemoryDataWidth_16b;				//�洢���������߿��16bit
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode       = FSMC_BurstAccessMode_Disable;			//ʹ�ܻ�ر�ͬ��NOR����洢����ͻ������ģʽ�����Ƿ�ʹ�ñŷ�����ģʽ
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity    = FSMC_WaitSignalPolarity_Low;				//����WAIT�źŵ���Ч��ƽ
	FSMC_NORSRAMInitStructure.FSMC_WrapMode              = FSMC_WrapMode_Disable;					//�����Ƿ�ʹ�û���ģʽ
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive      = FSMC_WaitSignalActive_BeforeWaitState;	//����WAIT�ź���Чʱ��
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation        = FSMC_WriteOperation_Enable;				//�趨�Ƿ�ʹ��д����
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal            = FSMC_WaitSignal_Disable;					//�趨�Ƿ�ʹ��WAIT�ź�
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode          = FSMC_ExtendedMode_Disable;				//ʹ�ܻ�ر���չģʽ����չģʽ���ڷ��ʾ��в�ͬ��д����ʱ��Ĵ洢�����趨�Ƿ�ʹ�õ�����дʱ��
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst            = FSMC_WriteBurst_Disable;					//�趨�Ƿ�ʹ�ñŷ�дģʽ
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;		//�趨��дʱ��
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct     = &FSMC_NORSRAMTimingInitStructure;		//�趨��дʱ��
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

	/*** Enable FSMC Bank1_SRAM Bank ****/
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}
