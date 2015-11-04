/****************************************************************************
* Copyright (C), 2011 奋斗嵌入式工作室 www.ourstm.net
*
* 本例程在 奋斗版STM32开发板V2,2.1,V3,MINI上调试通过           
* QQ: 9191274, 旺旺：sun68, Email: sun68@163.com 
* 淘宝店铺：ourstm.taobao.com  
* 奋斗板论坛：www.ourstm.net  
*
* 文件名: main.c
* 内容简述:
*       本例程演示了在3寸TFT屏上显示一副16位色图片，并在图片上透明叠加两个不同显示方向的字符串
*       图片大小范围400X240之内。 字符串取模尺寸：单色400X240 之内 取模X轴长度为8的整数倍数。
*       图片取模软件：img2lcd
*       字符取模软件：ZIMO3
*
* 文件历史:
* 版本号  日期       作者    说明
* v0.1    2011-12-5 sun68  创建该文件
*/


#include "fsmc_sram.h"
#include "LCD_bottom.h"

#define Bank1_LCD_D    ((uint32_t)0x60020000)    //显示区数据地址	  
#define Bank1_LCD_C    ((uint32_t)0x60000000)	 //显示区指令地址

unsigned long color1=0;

void LCD_Init(void);
void LCD_WR_REG(unsigned int index);
void LCD_WR_CMD(unsigned int index,unsigned int val);
void LCD_WR_Data(unsigned int val);

void lcd_rst(void);


void LCD_test(void);
void LCD_clear(unsigned int p);
void lcd_wr_zf(u16 StartX, u16 StartY, u16 X, u16 Y, u16 Color, u8 Dir, u8 *chr); 
unsigned char *num_pub(unsigned int a);


extern unsigned char zm9[];
extern unsigned char zm8[];
extern unsigned char zm7[];
extern unsigned char zm6[];
extern unsigned char zm5[];
extern unsigned char zm4[];
extern unsigned char zm3[];
extern unsigned char zm2[];
extern unsigned char zm1[];
extern unsigned char zm0[];
extern unsigned char a1[];

extern unsigned char zf2[];
extern unsigned char zf3[];	
extern unsigned char pic[];
extern unsigned char a2[];
extern unsigned char a3[];

unsigned int LCD_RD_data(void);
extern void lcd_rst(void);
extern void Delay(__IO uint32_t nCount);


/****************************************************************************
* 名    称：void lcd_DrawPicture(u16 StartX,u16 StartY, u8 Dir，u8 *pic)
* 功    能：在指定座标范围显示一副图片
* 入口参数：StartX     行起始座标
*           StartY     列起始座标
*			Dir		   图像显示方向       
*           pic        图片头指针
* 出口参数：无
* 说    明：图片取模格式为水平扫描，16位颜色模式  取模软件img2LCD
* 调用方法：lcd_DrawPicture(0,0,0,(u16*)demo);
****************************************************************************/
void lcd_DrawPicture(u16 StartX,u16 StartY,u8 Dir,u8 *pic)
{
  u32  i=8, len;
  u16 temp,x,y;
  	  
  
  x=((uint16_t)(pic[2]<<8)+pic[3])-1;	  	//从图像数组里取出图像的长度
  y=((uint16_t)(pic[4]<<8)+pic[5])-1;	  	//从图像数组里取出图像的高度
  if(Dir==0){
	LCD_WR_CMD(0x0003,0x1030);   			//图像显示方向为左下起  行递增  列递减
    LCD_WR_CMD(0x0210, StartX); 			//水平显示区起始地址 0-239
  	LCD_WR_CMD(0x0211, StartX+x);           	//水平显示区结束地址 0-239
  	LCD_WR_CMD(0x0212, StartY);     		//垂直显示区起始地址 0-399
  	LCD_WR_CMD(0x0213, StartY+y);         	//垂直显示区结束地址 0-399
  
  	LCD_WR_CMD(0x0200, StartX);		          	//水平显示区地址
  	LCD_WR_CMD(0x0201, StartY);		      		//垂直显示区地址
  }	 
  else if(Dir==1){
	LCD_WR_CMD(0x0003,0x1018);   			//图像显示方向为左下起  行递增  列递减
    LCD_WR_CMD(0x0210, StartY); 			//水平显示区起始地址 0-239
  	LCD_WR_CMD(0x0211, StartY+y);           //水平显示区结束地址 0-239
  	LCD_WR_CMD(0x0212, 399-(x+StartX));     //垂直显示区起始地址 0-399
  	LCD_WR_CMD(0x0213, 399-StartX);         //垂直显示区结束地址 0-399
  
  	LCD_WR_CMD(0x200, StartY);		          	//水平显示区地址
  	LCD_WR_CMD(0x201, 399-StartX);		      	//垂直显示区地址
  }	 
  LCD_WR_REG(0x0202);				          	//写数据到显示区

  len=2*((uint16_t)(pic[2]<<8)+pic[3])*((uint16_t)(pic[4]<<8)+pic[5]);   //计算出图像所占的字节数  
  while(i<(len+8)) {							 //从图像数组的第9位开始递增
  	temp=(uint16_t)( pic[i]<<8)+pic[i+1];		 //16位总线， 需要一次发送2个字节的数据
  	LCD_WR_Data(temp);							 //将取出的16位像素数据送入显示区
	i=i+2;										 //取模位置加2，以为获取下一个像素数据
  }
}

//写寄存器地址函数
void LCD_WR_REG(unsigned int index)
{
	*(__IO uint16_t *) (Bank1_LCD_C)= index;  

}



//写寄存器数据函数
//输入：dbw 数据位数，1为16位，0为8位。
void LCD_WR_CMD(unsigned int index,unsigned int val)
{	
	*(__IO uint16_t *) (Bank1_LCD_C)= index;	
	*(__IO uint16_t *) (Bank1_LCD_D)= val;
}

//读显示地址数据
unsigned int LCD_RD_data(void){
	unsigned int a=0;
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //空操作
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //读出的实际16位像素数据	  
	return(a);	
}

/****************************************************************************
* 名    称：u16 ili9320_BGR2RGB(u16 c)
* 功    能：RRRRRGGGGGGBBBBB 改为 BBBBBGGGGGGRRRRR 格式
* 入口参数：c      BRG 颜色值
* 出口参数：RGB 颜色值
* 说    明：内部函数调用
* 调用方法：
****************************************************************************/
u16 lcd_BGR2RGB(u16 c)
{
  u16  r, g, b;

  b = (c>>0)  & 0x1f;
  g = (c>>5)  & 0x3f;
  r = (c>>11) & 0x1f;
  
  return( (b<<11) + (g<<5) + (r<<0) );
}

//写16位像素数据函数
void    LCD_WR_Data(unsigned int val)
{   
	*(__IO uint16_t *) (Bank1_LCD_D)= val; 	
}



/****************************************************************************
* 名    称：void LCD_Init(void)
* 功    能：3 寸TFT(R61509) 初始化函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void LCD_Init(void)
{
	
	unsigned int i;
	
	 FSMC_LCD_Init();							 //FSMC总线配置		  

	GPIO_ResetBits(GPIOE, GPIO_Pin_1);	  //硬件复位
    Delay(0x1AFFf);			   
    GPIO_SetBits(GPIOE, GPIO_Pin_1 );		 
	Delay(0x1AFFf);	

	LCD_WR_CMD(0x0000, 0x00000);
    LCD_WR_CMD(0x0000, 0x00000);
	LCD_WR_CMD(0x0000, 0x00000);
	LCD_WR_CMD(0x0000, 0x00000);

	LCD_WR_CMD(0x0400, 0x06200);
	LCD_WR_CMD(0x0008, 0x00808);
	LCD_WR_CMD(0x0300, 0x00C00);//gamma
	LCD_WR_CMD(0x0301, 0x05A0B);
	LCD_WR_CMD(0x0302, 0x00906);
	LCD_WR_CMD(0x0303, 0x01017);
	LCD_WR_CMD(0x0304, 0x02300);
	LCD_WR_CMD(0x0305, 0x01700);
	LCD_WR_CMD(0x0306, 0x06309);
	LCD_WR_CMD(0x0307, 0x00C09);
	LCD_WR_CMD(0x0308, 0x0100C);
	LCD_WR_CMD(0x0309, 0x02232);
	LCD_WR_CMD(0x0010, 0x00016);//69.5Hz
	LCD_WR_CMD(0x0011, 0x00101);//
	LCD_WR_CMD(0x0012, 0x00000);//
	LCD_WR_CMD(0x0013, 0x00001);//
	LCD_WR_CMD(0x0100, 0x00330);//BT,AP
	LCD_WR_CMD(0x0101, 0x00237);//DC0,DC1,VC
	LCD_WR_CMD(0x0103, 0x00F00);//VDV
	LCD_WR_CMD(0x0280, 0x06100);//VCM
	LCD_WR_CMD(0x0102, 0x0C1B0);//VRH,VCMR,PSON,PON

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
	LCD_WR_CMD(0x0213, 0x0018F);//432=1AF, 400=18F
	LCD_WR_CMD(0x0500, 0x00000);
	LCD_WR_CMD(0x0501, 0x00000);
	LCD_WR_CMD(0x0502, 0x0005F);
	LCD_WR_CMD(0x0401, 0x00001);
	LCD_WR_CMD(0x0404, 0x00000);

	LCD_WR_CMD(0x0007, 0x00100);//BASEE

	LCD_WR_CMD(0x0200, 0x00000);
	LCD_WR_CMD(0x0201, 0x00000);
    LCD_WR_CMD(0x200, 0);
    LCD_WR_CMD(0x201, 0);
	*(__IO uint16_t *) (Bank1_LCD_C)= 0x202;	 //准备写数据显示区
	for(i=0;i<96000;i++)
	{
	  LCD_WR_Data(0xffff);					 //用黑色清屏
	}  
	color1=0; 
}

// ++++++++++++++++TFT 复位操作
void lcd_rst(void){
	GPIO_ResetBits(GPIOE, GPIO_Pin_1);	      //PE1 为LCD 复位信号
    Delay(0xAFFFFf);					   
    GPIO_SetBits(GPIOE, GPIO_Pin_1 );		 	 
	Delay(0xAFFFFf);	
}




/****************************************************************************
* 名    称：lcd_wr_zf(u16 StartX, u16 StartY, u16 X, u16 Y, u16 Color, u8 Dir, u8 *chr) 
* 功    能：在指定座标显示一串字符透明叠加在背景图片上
* 入口参数：StartX     行起始座标	 0-239
*           StartY     列起始座标	 0-399
*           X          长(为8的倍数）0-400
*           Y          宽			 0-240
*			Color      颜色0-65535
*           Dir		   图像显示方向
*           chr        字符串指针
* 出口参数：无
* 说    明：字符取模格式为单色字模，横向取模，字节正序  取模软件：ZIMO3
* 调用方法：lcd_wr_zf(0,0,100,100,(u16*)demo);
****************************************************************************/
//+++++++++++++++++++++++LCD写字符子程序
void lcd_wr_zf(u16 StartX, u16 StartY, u16 X, u16 Y, u16 Color, u8 Dir, u8 *chr)
{	unsigned int temp=0,num,R_dis_mem=0,Size=0,x=0,y=0,i=0;


	if(Dir==2) LCD_WR_CMD(0x0003,0x1010);   //图像显示方向为右下起  行递减  列递增  AM=0  I/D[1:0]=00	<--
	else if(Dir==3) LCD_WR_CMD(0x0003,0x1028);   //图像显示方向为右上起  行递减  列递增  AM=1  I/D[1:0]=10	V
  	if(Dir==0){
		LCD_WR_CMD(0x0003,0x1030);		  //图像显示方向为左上起  行递增  列递增  AM=0  I/D[1:0]=11  -->
		LCD_WR_CMD(0x0210, StartX); 	  //水平显示区起始地址 0-239
  		LCD_WR_CMD(0x0211, StartX+X-1);   //水平显示区结束地址 0-239
  		LCD_WR_CMD(0x0212, StartY);       //垂直显示区起始地址 0-399
  		LCD_WR_CMD(0x0213, StartY+Y-1);   //垂直显示区结束地址 0-399
		LCD_WR_CMD(0x0200, StartX);		  //水平显示区地址
  		LCD_WR_CMD(0x0201, StartY);		  //垂直显示区地址	
		LCD_WR_REG(0x0202);               //准备写数据显示区
		Size=X*Y;						  //字符串或字符占用的像素尺寸
		while(i<Size){					  
			temp=*chr++;				  //一个字节代表8个像素，因此加1代表索引到下8个像素
			for(num=0; num<8; num++){	  //数组的每个字节代表了8个像素	    
				if((temp&0x80)>0){		  //对字节的各位进行判断，为1的用带入参数的16位颜色值标示，写入到像素位置。						 
				
					LCD_WR_Data(Color); 		
				}
				else{
					LCD_WR_CMD(0x0200, StartX+x);		//水平显示区地址
  					LCD_WR_CMD(0x0201, StartY+y);		//垂直显示区地址								
					LCD_WR_REG(0x0202);					//准备读数据显示区
					R_dis_mem=LCD_RD_data();		  	//读取背景色，为叠加产生透明效果作准备	
					LCD_WR_Data(R_dis_mem); 		//对字节的各位进行判断，为0的用当前背景像素16位颜色值标示。 						
				}
				temp=temp<<1; 		  				//字节各位的移出
				x++;
				if(x>=X){x=0; y++;}				    //计算像素递增为当前的x和y，为当前像素读背景颜色做准备
				i++;	
			}				  
							
		}
	}
	else if(Dir==1){
		LCD_WR_CMD(0x0003,0x1018);   	  		//图像显示方向为左下起  行递增  列递减  AM=1  I/D[1:0]=01	A
		LCD_WR_CMD(0x0210, StartY); 	  		//水平显示区起始地址 0-239
  		LCD_WR_CMD(0x0211, StartY+Y-1);    		//水平显示区结束地址 0-239
  		LCD_WR_CMD(0x0212, 399-(StartX+X-1));   //垂直显示区起始地址 0-399
  		LCD_WR_CMD(0x0213, 399-StartX);    		//垂直显示区结束地址 0-399
		LCD_WR_CMD(0x0200, StartY);		  	  	//水平显示区地址
  		LCD_WR_CMD(0x0201, 399-StartX);	 	  	//垂直显示区地址	
		LCD_WR_REG(0x0202);                   	//准备写数据显示区

		Size=X*Y;						  		//字符串或字符占用的像素尺寸
		while(i<Size){					  
			temp=*chr++;				  		//一个字节代表8个像素，因此加1代表索引到下8个像素
			for(num=0; num<8; num++){	  		//数组的每个字节代表了8个像素	    
				if((temp&0x80)>0){		  		//对字节的各位进行判断，为1的用带入参数的16位颜色值标示，写入到像素位置。						 
				
					LCD_WR_Data(Color); 		
				}
				else{
					LCD_WR_CMD(0x0200, StartY+y);		//水平显示区地址
  					LCD_WR_CMD(0x0201, 399-(StartX+x));	//垂直显示区地址								
					LCD_WR_REG(0x0202);					//准备读数据显示区
					R_dis_mem=LCD_RD_data();		  	//读取背景色，为叠加产生透明效果作准备	
					LCD_WR_Data(R_dis_mem); 		//对字节的各位进行判断，为0的用当前背景像素16位颜色值标示。 						
				}
				temp=temp<<1; 		  				//字节各位的移出        
				x++;
				if(x>=X){x=0; y++;}				    //计算像素递增为当前的x和y，为当前像素读背景颜色做准备
				i++;
			}							
		}
	}
}


//演示程序
void LCD_test(void)
{

	unsigned char *p;


// 	lcd_DrawPicture(0,0,0,a1);	    	//在某一指定位置显示图片， 图片的尺寸要在240X400范围内。  
// 	Delay(0x1afffff);
// 	lcd_DrawPicture(0,0,1,a2);	    	//在某一指定位置显示图片， 图片的尺寸要在400X240范围内。      

// 	Delay(0x1afffff);

  lcd_wr_zf(0,0,280,32,color1,1,&zf3[0]);  		//显示字符串
	
	//显示5位的数字， 数值按COLOR1值周期变化
	p=num_pub((color1/10000));
	lcd_wr_zf(50,30,24,32,color1,0,p);  
	
	p=num_pub((color1%10000)/1000);
	lcd_wr_zf(74,30,24,32,color1,0,p);  
	
	p=num_pub(((color1%10000)%1000)/100);
	lcd_wr_zf(98,30,24,32,color1,0,p);  

	p=num_pub((((color1%10000)%1000)%100)/10);
	lcd_wr_zf(122,30,24,32,color1,0,p);  

	p=num_pub((color1%10));
	lcd_wr_zf(146,30,24,32,color1,0,p);  


	//颜色渐变
	color1++; 
	if(color1>=65536) color1=0;  
	Delay(0xafffff);			
}


//++++++++根据数值获取各位的数字字符数据首地址+++++++++++++++++++++++++++++++++++++++++++
unsigned char *num_pub(unsigned  int a){
	unsigned char *b;
	switch(a){
		case 0x01: b=&zm1[0]; break;
 		case 0x02: b=&zm2[0]; break;
		case 0x03: b=&zm3[0]; break;
		case 0x04: b=&zm4[0]; break;
		case 0x05: b=&zm5[0]; break;
		case 0x06: b=&zm6[0]; break;
		case 0x07: b=&zm7[0]; break;
		case 0x08: b=&zm8[0]; break;
		case 0x09: b=&zm9[0]; break;
		case 0x00: b=&zm0[0]; break;
		default: b=&zm0[0];break;
	}
	return(b);
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
