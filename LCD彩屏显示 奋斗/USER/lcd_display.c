#include "lcd_display.h"

extern unsigned char Picture1[];
extern unsigned char Picture2[];

static void LCD_WR_CMD(uint32_t cmd, uint32_t value);								//写寄存器数据函数
static void LCD_WR_Data(uint32_t value);											//写16位像素数据函数
static void LCD_WR_REG(uint32_t cmd);												//写寄存器地址函数
static void LCD_DrawPicture(uint16_t Sx, uint16_t Sy, uint8_t Dir, uint8_t *Pic);	//指定的坐标范围显示一副图片

/*** 3寸TFT（R61509）初始化函数 ***/
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
	
	Bank1_LCD_C = 0X0202;			//准备写数据显示区
	for (i=0; i<9600; i++)
	{
		LCD_WR_Data(0xFFFF);		//用黑色清屏
	}			
}

/*** 写寄存器数据函数 ***/
static void LCD_WR_CMD(uint32_t cmd, uint32_t value)
{
	Bank1_LCD_C = cmd;
	Bank1_LCD_D = value;		
}

/*** 写16位像素数据函数 ***/
static void LCD_WR_Data(uint32_t value)
{
	Bank1_LCD_D = value;	
}

/*** 程序开始运行 ***/
void LCD_Display_Picture(void)
{
	LCD_DrawPicture(0, 0, 0, Picture1);		//显示图片1，图片1的尺寸要在240*400范围内
	Delay(0x1AFFFFF);

	LCD_DrawPicture(0, 0, 1, Picture2);		//显示图片2，图片2的尺寸要在400*240范围内
	Delay(0x1AFFFFF);	
}

/*** 函数功能：指定的坐标范围显示一副图片 ***/
/*** 入口参数：StartX：行起始坐标，StartY：列起始坐标，Dir：图像显示方向，Pic：图片头指针 ***/
/*** 说    明：图片取模格式为水平扫描，16位颜色模式，取模软件img2LCD ***/
static void LCD_DrawPicture(uint16_t StartX, uint16_t StartY, uint8_t Dir, uint8_t *Pic)
{
	uint16_t temp;
	uint16_t x = 0, y = 0;
	uint32_t i = 8, len = 0;
	
	x = ((uint16_t)(Pic[2] << 8) + Pic[3]) - 1;		//从图像数组里取出图像的宽度
	y = ((uint16_t)(Pic[4] << 8) + Pic[5]) - 1;		//从图像数组里取出图像的高度
	
	if (Dir == 0)
	{
		LCD_WR_CMD(0x0003, 0x1030);			//图像显示方向为左上起，行递增，列增加
		LCD_WR_CMD(0x0210, StartX);			//水平显示区起始地址 0
		LCD_WR_CMD(0x0211, StartX + x);		//水平显示区结束地址 239
		LCD_WR_CMD(0x0212, StartY);			//垂直显示区起始地址 0
		LCD_WR_CMD(0x0213, StartY + y);		//垂直显示区结束地址 399

		LCD_WR_CMD(0x0200, StartX);			//水平显示区地址 0
  	LCD_WR_CMD(0x0201, StartY);			//垂直显示区地址 0
	}
	else if (Dir == 1)
	{
		LCD_WR_CMD(0x0003, 0x1018);   				//图像显示方向为左下起，行递增，列递减
	    LCD_WR_CMD(0x0210, StartY); 				//水平显示区起始地址 0
	  	LCD_WR_CMD(0x0211, StartY + y);           	//水平显示区结束地址 239
	  	LCD_WR_CMD(0x0212, StartX);     			//垂直显示区起始地址 0
	  	LCD_WR_CMD(0x0213, StartX + x);         	//垂直显示区结束地址 239
	  
	  	LCD_WR_CMD(0x0200, StartY);		          	//水平显示区地址
	  	LCD_WR_CMD(0x0201, StartX + x);		    	//垂直显示区地址
	}
	
	LCD_WR_REG(0x0202);				//写数据到显示区

	len = 2 * (x + 1) * (y + 1);	//计算出图像所占的字节数
	while (i < (len + 8))			//从图像数组的第9位开始递增
	{
		temp = (uint16_t)((Pic[i] << 8) + Pic[i+1]);	//16位总线，需要一次发送2个字节的数据
		LCD_WR_Data(temp);								//将取出的16位像素数据送入显示区
		i += 2;											//取模位置加2，以为获取下一个像素数据
	}	
}

/*** 写寄存器地址函数 ***/
static void LCD_WR_REG(uint32_t cmd)
{
	Bank1_LCD_C = cmd;
}

/*** 延时函数 ***/
void Delay(__IO uint32_t nCnt)
{
	for (; nCnt>0; nCnt--);
}
