

#include "lcd.h"

u16 POINT_COLOR = BLACK;					      /*描点默认颜色*/
extern volatile u8 display_direction;		/*0：横屏 1：竖屏*/

unsigned long color1=0;

/****************************************************************************
* 名    称：GUI_CmpColor()
* 功    能：判断颜色值是否一致。
* 入口参数：color1		颜色值1
*		        color2		颜色值2
* 出口参数：返回1表示相同，返回0表示不相同。
* 说    明：
****************************************************************************/
#define  GUI_CmpColor(color1, color2)	( (color1&0x01) == (color2&0x01) )



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


/****************************************************************************
* 名    称：void LCD_clear(void)
* 功    能：清屏
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
****************************************************************************/
void LCD_clear(unsigned short a)
{
    unsigned int l=400,w;
	LCD_WR_CMD(0x0003, 0x01030); //显示方向模式寄存器  横向

	LCD_WR_CMD(0x0210, 0x00000);
	LCD_WR_CMD(0x0211, 0x000EF);
	LCD_WR_CMD(0x0212, 0x00000);
	LCD_WR_CMD(0x0213, 0x0018F);//432=1AF, 400=18F

	LCD_WR_CMD(0x0200, 0x00000);
	LCD_WR_CMD(0x0201, 0x00000);	
	//*(__IO uint16_t *) (Bank1_LCD_C)= 0x202;	 //准备写数据显示区
	LCD_WR_REG(0x0202);				          	//写数据到
	while(l--)
	{
	    for(w=0;w<240;w++)
		{    
          	LCD_WR_Data(a);
		}
	}
}
/*******************************************************************************
* Function Name  : LCD_WriteRAM_Prepare
* Description    : Prepare to write to the LCD RAM.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void LCD_WriteRAM_Prepare(void)
{	  		
	LCD_WR_CMD(0x0003, 0x01030); //显示方向模式寄存器  横向

	LCD_WR_CMD(0x0210, 0x00000);
	LCD_WR_CMD(0x0211, 0x000EF);
	LCD_WR_CMD(0x0212, 0x00000);
	LCD_WR_CMD(0x0213, 0x0018F);//432=1AF, 400=18F

	LCD_WR_CMD(0x0200, 0x00000);
	LCD_WR_CMD(0x0201, 0x00000);	
	//*(__IO uint16_t *) (Bank1_LCD_C)= 0x202;	 //准备写数据显示区
	LCD_WR_REG(0x0202);
} 		
/*******************************************************************************
* Function Name  : LCD_WriteRAM
* Description    : Writes to the LCD RAM.
* Input          : - RGB_Code: the pixel color in RGB mode (5-6-5).
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WriteRAM(u16 RGB_Code)
{	    
*(__IO u16 *) (Bank1_LCD_D) = RGB_Code; //写显示数据	 	
}											   
//==============================================================================	  

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


//奋斗板演示程序
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

// u16 lcd_ReadData(void)
// {
//   u16 val=0;
//   val=LCD_RD_data();
//   return val;
// }
//-----------------修改字符显示函数-----------------//移植到奋斗板子上-------------------
/****************************************************************************
* 名    称：void lcd_SetCursor(u16 x,u16 y)
* 功    能：设置屏幕座标
* 入口参数：x      行座标
*           y      列座标
* 出口参数：无
* 说    明：
* 调用方法：lcd_SetCursor(10,10);
****************************************************************************/
void lcd_SetCursor(u16 x,u16 y)
{	
  *(__IO uint16_t *) (Bank1_LCD_C)= 0x200;
  *(__IO uint16_t *) (Bank1_LCD_D)= y;

  *(__IO uint16_t *) (Bank1_LCD_C)= 0x201;
  *(__IO uint16_t *) (Bank1_LCD_D)= 399-x;
}

/****************************************************************************
* 名    称：void lcd_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)  横屏
* 功    能：设置窗口区域
* 入口参数：StartX     行起始座标
*           StartY     列起始座标
*           EndX       行结束座标
*           EndY       列结束座标
* 出口参数：无
* 说    明：
* 调用方法：lcd_SetWindows(0,0,100,100)；
****************************************************************************/
void lcd_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)
{

  	
  *(__IO uint16_t *) (Bank1_LCD_C)= 0x0210;	//Window Horizontal RAM Address Start
  *(__IO uint16_t *) (Bank1_LCD_D)= StartX;

  *(__IO uint16_t *) (Bank1_LCD_C)= 0x0211;	//Window Horizontal RAM Address End
  *(__IO uint16_t *) (Bank1_LCD_D)= EndX;

  *(__IO uint16_t *) (Bank1_LCD_C)= 0x0212;	//Window Vertical RAM Address Start
  *(__IO uint16_t *) (Bank1_LCD_D)= StartY;

  *(__IO uint16_t *) (Bank1_LCD_C)= 0x0213;	//Window Vertical RAM Address End
  *(__IO uint16_t *) (Bank1_LCD_D)= EndY;
}
  /**********************************************
函数名：Lcd块选函数
功能：选定Lcd上指定的矩形区域

注意：xStart和 yStart随着屏幕的旋转而改变，位置是矩形框的四个角

入口参数：xStart x方向的起始点
          ySrart y方向的终止点
          W 要选定矩形的x方向长度
          H  要选定矩形的y方向长度
返回值：无
***********************************************/
void Lcd_SetBox(u16 xStart,u16 yStart,u16 W,u16 H)  //??不知对否
{
	
	
// 	LCD_WriteReg(0x01,xStart);                //水平起始位置 
// 	LCD_WriteReg(0x02,yStart);                //垂直起始位置
// 	LCD_WriteReg(0x03,W);                     //x方向长度
// 	LCD_WriteReg(0x04,H);                     //y方向长度
	  	
  *(__IO uint16_t *) (Bank1_LCD_C)= 0x0210;	  //Window Horizontal RAM Address Start
  *(__IO uint16_t *) (Bank1_LCD_D)= xStart;

  *(__IO uint16_t *) (Bank1_LCD_C)= 0x0211;	  //Window Horizontal RAM Address End
  *(__IO uint16_t *) (Bank1_LCD_D)= (W+xStart);

  *(__IO uint16_t *) (Bank1_LCD_C)= 0x0212;	  //Window Vertical RAM Address Start
  *(__IO uint16_t *) (Bank1_LCD_D)= yStart;

  *(__IO uint16_t *) (Bank1_LCD_C)= 0x0213;   //Window Vertical RAM Address End
  *(__IO uint16_t *) (Bank1_LCD_D)= (H+yStart);
}

/****************************************************************************
* 名    称：u16 lcd_GetPoint(u16 x,u16 y)
* 功    能：获取指定座标的颜色值
* 入口参数：x      行座标
*           y      列座标
* 出口参数：当前座标颜色值
* 说    明：
* 调用方法：i=lcd_GetPoint(10,10);
****************************************************************************/
u16 lcd_GetPoint(u16 x,u16 y)
{ 
  
  *(__IO uint16_t *) (Bank1_LCD_C)= 0x0200;
  *(__IO uint16_t *) (Bank1_LCD_D)= y;

  *(__IO uint16_t *) (Bank1_LCD_C)= 0x0201;
  *(__IO uint16_t *) (Bank1_LCD_D)= 399-x;

  *(__IO uint16_t *) (Bank1_LCD_C)= 0x0202;

  //return (lcd_BGR2RGB(lcd_ReadData()));
  return (lcd_ReadData());

}

/****************************************************************************
* 名    称：void lcd_SetPoint(u16 x,u16 y,u16 point)
* 功    能：在指定座标画点
* 入口参数：x      行座标
*           y      列座标
*           point  点的颜色
* 出口参数：无
* 说    明：
* 调用方法：lcd_SetPoint(10,10,0x0fe0);
****************************************************************************/
void lcd_SetPoint(u16 x,u16 y,u16 point)
{
  *(__IO uint16_t *) (Bank1_LCD_C)= 0x0200;	
  *(__IO uint16_t *) (Bank1_LCD_D)= y;

  *(__IO uint16_t *) (Bank1_LCD_C)= 0x0201;	
  *(__IO uint16_t *) (Bank1_LCD_D)= 399-x;

  *(__IO uint16_t *) (Bank1_LCD_C)= 0x0202;	

  *(__IO uint16_t *) (Bank1_LCD_D)= point;
}

/****************************************************************************
* 名    称：void lcd_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
* 功    能：在指定座标显示一个8x16点阵的ascii字符
* 入口参数：x          行座标
*           y          列座标
*           charColor  字符的颜色
*           bkColor    字符背景颜色
* 出口参数：无
* 说    明：显示范围限定为可显示的ascii码
* 调用方法：lcd_PutChar(10,10,'a',0x0000,0xffff);
****************************************************************************/
void lcd_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
{
  u16 i=0;
  u16 j=0;
  
  u8 tmp_char=0;

  for (i=0;i<16;i++)
  {
    tmp_char=ascii_8x16[((c-0x20)*16)+i];
    for (j=0;j<8;j++)
    {
      if ( (tmp_char >> 7-j) & 0x01 == 0x01)
        {
          lcd_SetPoint(x+j,y+i,charColor); // 字符颜色
        }
        else
        {
          lcd_SetPoint(x+j,y+i,bkColor); // 背景颜色
        }
    }
  }
}


/****************************************************************
 * 函数名：LCD_Str_O
 * 描述  ：在指定坐标处显示8*16大小的字符串
 * 输入  : -x   显示位置横向坐标 0 ~ 319
           -y   显示位置纵向坐标 0 ~ 239
 * 		   -str 字符串首址
 * 		   -Color字符颜色
 * 输出  ：无
 * 举例  ：	LCD_Str_O(0, 0, "Runing", 0);
		    LCD_Str_O(320-8*6, 239-16, "Runing",0); 
 * 注意  ：	字符串显示方向为横向 已测试					
******************************************************************/    
void LCD_Str_O(u16 x, u16 y,const u8 *str,u16 Color,u16 bkColor)
{

//    	Set_direction(0);     
    while(*str != '\0')
    {       
        if(x > (320-8)) 
        {	
					//换行
           x = 0;
           y += 16;
        }
        if(y > (240-16)) 
        {	
					//一屏
           x = y = 0;
        }
        
				lcd_PutChar(x,y,*str,Color,bkColor);
        x += 8;
        str ++ ;
    }
		
}




/********************************************************************
 * 函数名：LCD_Char_CH
 * 描述  ：显示单个汉字字符
 * 输入  : 	x: 0~(319-16)
 *         	y: 0~(239-16)
 *			str: 中文字符串首址
 *			Color: 字符颜色   
 *			bkColor: 背景颜色
 * 输出  ：无
 * 举例  ：	LCD_Char_CH(200,100,"好",0,0);
 * 注意	 ：如果输入大于1的汉字字符串，显示将会截断，只显示最前面一个汉字
************************************************************************/    
void LCD_Char_CH(u16 x,u16 y,const u8 *str,u16 Color,u16 bkColor)
{
    
#ifndef NO_CHNISEST_DISPLAY         /*如果汉字显示功能没有关闭*/
    u8 i,j;
    u8 buffer[32];
    u16 tmp_char=0;
    
    
   GetGBKCode_from_sd(buffer,str);  /* 取字模数据 */
    
    for (i=0;i<16;i++)
    {
        tmp_char=buffer[i*2];
        tmp_char=(tmp_char<<8);
        tmp_char|=buffer[2*i+1];
        for (j=0;j<16;j++)
        {
            if ( (tmp_char >> 15-j) & 0x01 == 0x01)
            {
								lcd_SetPoint(x+j,y+i,Color);
            }
            else
            {
                lcd_SetPoint(x+j,y+i,bkColor);
            }
        }
    }
    
#endif
}


/***************************************************************************
 * 函数名：LCD_Str_CH
 * 描述  ：在指定坐标处显示16*16大小的指定颜色汉字字符串
 * 输入  : 	- x: 显示位置横向坐标	 
 *         	- y: 显示位置纵向坐标 
 *			- str: 显示的中文字符串
 *			- Color: 字符颜色   
 *			- bkColor: 背景颜色
 * 输出  ：无
 * 举例  ：	LCD_Str_CH(0,0,"阿莫论坛野火专区",0,0xffff);
		    LCD_Str_CH(50,100,"阿莫论坛野火专区",0,0xffff);
		    LCD_Str_CH(320-16*8,240-16,"阿莫论坛野火专区",0,0xffff); 		
 * 注意  ：	字符串显示方向为横向 已测试
******************************************************************************/    
void LCD_Str_CH(u16 x,u16 y,const u8 *str,u16 Color,u16 bkColor)  
{
   
 //   Set_direction(0);
    while(*str != '\0')
    {
			if(x>(320-16))
			{	 
				 /*换行*/
				x =0;
				y +=16;
					
			}
			if(y >(240-16))
			{
				 /*重新归零*/
				 y =0;
				 x =0;
			}
	     LCD_Char_CH(x,y,str,Color,bkColor);      
       str += 2 ;
       x += 16 ;	
    }
}


/********************************************************************************
 * 函数名：	LCD_Char_CH_O
 * 描述  ： 悬浮显示单个汉字字符
 * 输入  : 	x: 0~(319-16)
 *         	y: 0~(239-16)
 *			str: 中文字符串首址
 *			Color: 字符颜色   
 * 输出  ：无
 * 举例  ：	LCD_Char_CH_O(200,100,"好",0,0);
 * 注意	 ：1 横屏2 如果输入大于1的汉字字符串，显示将会截断，只显示最前面一个汉字
 ********************************************************************************/    
void LCD_Char_CH_O(u16 x,u16 y,const u8 *str,u16 Color) 
{
    u8 i,j;
    u8 buffer[32];
    u16 tmp_char=0;
 //   Set_direction(0);   
    GetGBKCode_from_sd(buffer,str); /* 取字模数据 */
    
    for (i=0;i<16;i++)
    {
        tmp_char=buffer[i*2];
				tmp_char=(tmp_char<<8);
				tmp_char|=buffer[2*i+1];
        for (j=0;j<16;j++)
        {            
            if ( (tmp_char >> 15-j) & 0x01 == 0x01)
            {
                lcd_SetPoint(x+j,y+i,Color);
            }
        }
    }   
}

/****************************************************************************
* 名    称：u16 lcd_BGR2RGB(u16 c)
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
  
  //return( (b<<11) + (g<<5) + (r<<0) );
  return( (b<<11) + (g<<5) + (r<<0) );
}



/****************************************************************************
* 名    称：u16 lcd_ReadData(void)
* 功    能：读取控制器数据
* 入口参数：无
* 出口参数：返回读取到的数据
* 说    明：内部函数
* 调用方法：i=lcd_ReadData();
****************************************************************************/
u16 lcd_ReadData(void)
{
  u16 val=0;
  val=LCD_RD_data();
  return val;
}





