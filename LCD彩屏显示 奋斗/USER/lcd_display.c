#include "lcd_display.h"

extern unsigned char Picture1[];
extern unsigned char Picture2[];

static void LCD_WR_CMD(uint32_t cmd, uint32_t value);								//д�Ĵ������ݺ���
static void LCD_WR_Data(uint32_t value);											//д16λ�������ݺ���
static void LCD_WR_REG(uint32_t cmd);												//д�Ĵ�����ַ����
static void LCD_DrawPicture(uint16_t Sx, uint16_t Sy, uint8_t Dir, uint8_t *Pic);	//ָ�������귶Χ��ʾһ��ͼƬ

/*** 3��TFT��R61509����ʼ������ ***/
void LCD_Init(void)
{
	uint16_t i = 0;

	GPIO_ResetBits(GPIOE, GPIO_Pin_1);
	Delay(0x1AFFF);
	GPIO_SetBits(GPIOE, GPIO_Pin_1);
	Delay(0x1AFFF);
	
	LCD_WR_CMD(0x0000, 0x00000);
	LCD_WR_CMD(0x0000, 0x00000);
	LCD_WR_CMD(0x0000, 0x00000);
	LCD_WR_CMD(0x0000, 0x00000);
	
	LCD_WR_CMD(0x0400, 0x06200);
	LCD_WR_CMD(0x0008, 0x00808);
	LCD_WR_CMD(0x0300, 0x00C00);	//gamma
	LCD_WR_CMD(0x0301, 0x05A0B);
	LCD_WR_CMD(0x0302, 0x00906);
	LCD_WR_CMD(0x0303, 0x01017);
	LCD_WR_CMD(0x0304, 0x02300);
	LCD_WR_CMD(0x0305, 0x01700);
	LCD_WR_CMD(0x0306, 0x06309);
	LCD_WR_CMD(0x0307, 0x00C09);
	LCD_WR_CMD(0x0308, 0x0100C);
	LCD_WR_CMD(0x0309, 0x02232);
	LCD_WR_CMD(0x0010, 0x00016);	//69.5Hz
	LCD_WR_CMD(0x0011, 0x00101);
	LCD_WR_CMD(0x0012, 0x00000);
	LCD_WR_CMD(0x0013, 0x00001);
	LCD_WR_CMD(0x0100, 0x00330);	//BT,AP
	LCD_WR_CMD(0x0101, 0x00237);	//DC0,DC1,VC
	LCD_WR_CMD(0x0103, 0x00F00);	//VDV
	LCD_WR_CMD(0x0280, 0x06100);	//VCM
	LCD_WR_CMD(0x0102, 0x0C1B0);	//VRH,VCMR,PSON,PON

	LCD_WR_CMD(0x0001, 0x00100);
	LCD_WR_CMD(0x0002, 0x00100);
	LCD_WR_CMD(0x0003, 0x01030);
	LCD_WR_CMD(0x0009, 0x00001);
	LCD_WR_CMD(0x000C, 0x00000);
	LCD_WR_CMD(0x0090, 0x08000);
	LCD_WR_CMD(0x000F, 0x00000);
	LCD_WR_CMD(0x0210, 0x00000);
	LCD_WR_CMD(0x0211, 0x000EF);
	LCD_WR_CMD(0x0212, 0x00000);
	LCD_WR_CMD(0x0213, 0x0018F);	//432=1AF, 400=18F
	LCD_WR_CMD(0x0500, 0x00000);
	LCD_WR_CMD(0x0501, 0x00000);
	LCD_WR_CMD(0x0502, 0x0005F);
	LCD_WR_CMD(0x0401, 0x00001);
	LCD_WR_CMD(0x0404, 0x00000);

	LCD_WR_CMD(0x0007, 0x00100);	//BASEE

	LCD_WR_CMD(0x0200, 0x00000);
	LCD_WR_CMD(0x0201, 0x00000);
    LCD_WR_CMD(0x0200, 0x00000);
    LCD_WR_CMD(0x0201, 0x00000);
	
	Bank1_LCD_C = 0X0202;			//׼��д������ʾ��
	for (i=0; i<9600; i++)
	{
		LCD_WR_Data(0xFFFF);		//�ú�ɫ����
	}			
}

/*** д�Ĵ������ݺ��� ***/
static void LCD_WR_CMD(uint32_t cmd, uint32_t value)
{
	Bank1_LCD_C = cmd;
	Bank1_LCD_D = value;		
}

/*** д16λ�������ݺ��� ***/
static void LCD_WR_Data(uint32_t value)
{
	Bank1_LCD_D = value;	
}

/*** ����ʼ���� ***/
void LCD_Display_Picture(void)
{
	LCD_DrawPicture(0, 0, 0, Picture1);		//��ʾͼƬ1��ͼƬ1�ĳߴ�Ҫ��240*400��Χ��
	Delay(0x1AFFFFF);

	LCD_DrawPicture(0, 0, 1, Picture2);		//��ʾͼƬ2��ͼƬ2�ĳߴ�Ҫ��400*240��Χ��
	Delay(0x1AFFFFF);	
}

/*** �������ܣ�ָ�������귶Χ��ʾһ��ͼƬ ***/
/*** ��ڲ�����StartX������ʼ���꣬StartY������ʼ���꣬Dir��ͼ����ʾ����Pic��ͼƬͷָ�� ***/
/*** ˵    ����ͼƬȡģ��ʽΪˮƽɨ�裬16λ��ɫģʽ��ȡģ���img2LCD ***/
static void LCD_DrawPicture(uint16_t StartX, uint16_t StartY, uint8_t Dir, uint8_t *Pic)
{
	uint16_t temp;
	uint16_t x = 0, y = 0;
	uint32_t i = 8, len = 0;
	
	x = ((uint16_t)(Pic[2] << 8) + Pic[3]) - 1;		//��ͼ��������ȡ��ͼ��Ŀ��
	y = ((uint16_t)(Pic[4] << 8) + Pic[5]) - 1;		//��ͼ��������ȡ��ͼ��ĸ߶�
	
	if (Dir == 0)
	{
		LCD_WR_CMD(0x0003, 0x1030);			//ͼ����ʾ����Ϊ�������е�����������
		LCD_WR_CMD(0x0210, StartX);			//ˮƽ��ʾ����ʼ��ַ 0
		LCD_WR_CMD(0x0211, StartX + x);		//ˮƽ��ʾ��������ַ 239
		LCD_WR_CMD(0x0212, StartY);			//��ֱ��ʾ����ʼ��ַ 0
		LCD_WR_CMD(0x0213, StartY + y);		//��ֱ��ʾ��������ַ 399

		LCD_WR_CMD(0x0200, StartX);			//ˮƽ��ʾ����ַ 0
  	LCD_WR_CMD(0x0201, StartY);			//��ֱ��ʾ����ַ 0
	}
	else if (Dir == 1)
	{
		LCD_WR_CMD(0x0003, 0x1018);   				//ͼ����ʾ����Ϊ�������е������еݼ�
	    LCD_WR_CMD(0x0210, StartY); 				//ˮƽ��ʾ����ʼ��ַ 0
	  	LCD_WR_CMD(0x0211, StartY + y);           	//ˮƽ��ʾ��������ַ 239
	  	LCD_WR_CMD(0x0212, StartX);     			//��ֱ��ʾ����ʼ��ַ 0
	  	LCD_WR_CMD(0x0213, StartX + x);         	//��ֱ��ʾ��������ַ 239
	  
	  	LCD_WR_CMD(0x0200, StartY);		          	//ˮƽ��ʾ����ַ
	  	LCD_WR_CMD(0x0201, StartX + x);		    	//��ֱ��ʾ����ַ
	}
	
	LCD_WR_REG(0x0202);				//д���ݵ���ʾ��

	len = 2 * (x + 1) * (y + 1);	//�����ͼ����ռ���ֽ���
	while (i < (len + 8))			//��ͼ������ĵ�9λ��ʼ����
	{
		temp = (uint16_t)((Pic[i] << 8) + Pic[i+1]);	//16λ���ߣ���Ҫһ�η���2���ֽڵ�����
		LCD_WR_Data(temp);								//��ȡ����16λ��������������ʾ��
		i += 2;											//ȡģλ�ü�2����Ϊ��ȡ��һ����������
	}	
}

/*** д�Ĵ�����ַ���� ***/
static void LCD_WR_REG(uint32_t cmd)
{
	Bank1_LCD_C = cmd;
}

/*** ��ʱ���� ***/
void Delay(__IO uint32_t nCnt)
{
	for (; nCnt>0; nCnt--);
}
