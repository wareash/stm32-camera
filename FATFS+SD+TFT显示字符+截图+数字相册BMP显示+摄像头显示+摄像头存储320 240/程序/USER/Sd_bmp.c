/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名  ：sd_bmp.c
 * 描述    ：LCD显示bmp图片应用函数库，bmp图片存储在MicroSD卡中。
 *              
 * 实验平台：野火STM32开发板
 * 硬件连接：无
 *
 * 库版本  ：ST3.0.0
 *
 * 作者    ：fire  QQ: 313303034 
 * 博客    ：firestm32.blog.chinaunix.net
**********************************************************************************/
#include "Sd_bmp.h"
#include "stm32f10x.h"
#include "ff.h"
#include "usart1.h"
#include "lcd.h"

#include "OV7670.h"
#define RGB24TORGB16(R,G,B) ((unsigned short int)((((R)>>3)<<11) | (((G)>>2)<<5)	| ((B)>>3)	))

/* 打印BMP文件的头信息，用于调试 */
static void showBmpHead(BITMAPFILEHEADER* pBmpHead)
{
    USART1_printf(USART1,"位图文件头:\r\n");
    USART1_printf(USART1,"文件大小:%d\r\n",(*pBmpHead).bfSize);
    USART1_printf(USART1,"保留字:%d\r\n",(*pBmpHead).bfReserved1);
    USART1_printf(USART1,"保留字:%d\r\n",(*pBmpHead).bfReserved2);
    USART1_printf(USART1,"实际位图数据的偏移字节数:%d\r\n",(*pBmpHead).bfOffBits);    
}

/* 打印BMP文件的头信息，用于调试 */
static void showBmpInforHead(tagBITMAPINFOHEADER* pBmpInforHead)
{
    USART1_printf(USART1,"位图信息头:\r\n");
    USART1_printf(USART1,"结构体的长度:%d\r\n",(*pBmpInforHead).biSize);
    USART1_printf(USART1,"位图宽:%d\r\n",(*pBmpInforHead).biWidth);
    USART1_printf(USART1,"位图高:%d\r\n",(*pBmpInforHead).biHeight);
    USART1_printf(USART1,"biPlanes平面数:%d\r\n",(*pBmpInforHead).biPlanes);
    USART1_printf(USART1,"biBitCount采用颜色位数:%d\r\n",(*pBmpInforHead).biBitCount);
    USART1_printf(USART1,"压缩方式:%d\r\n",(*pBmpInforHead).biCompression);
    USART1_printf(USART1,"biSizeImage实际位图数据占用的字节数:%d\r\n",(*pBmpInforHead).biSizeImage);
    USART1_printf(USART1,"X方向分辨率:%d\r\n",(*pBmpInforHead).biXPelsPerMeter);
    USART1_printf(USART1,"Y方向分辨率:%d\r\n",(*pBmpInforHead).biYPelsPerMeter);
    USART1_printf(USART1,"使用的颜色数:%d\r\n",(*pBmpInforHead).biClrUsed);
    USART1_printf(USART1,"重要颜色数:%d\r\n",(*pBmpInforHead).biClrImportant);
}

void showRgbQuan(tagRGBQUAD* pRGB)
{
    USART1_printf(USART1,"(%d-%d-%d) ",(*pRGB).rgbRed,(*pRGB).rgbGreen,(*pRGB).rgbBlue);    
}

BYTE pColorData[LCD_XSIZE*3];	   //屏幕宽度的3倍
tagRGBQUAD dataOfBmp[17*19];
FATFS bmpfs[2]; 
FIL bmpfsrc, bmpfdst; 
FRESULT bmpres;



 /******************************************************
 * 函数名：Lcd_show_bmp
 * 描述  ：LCD显示RGB888位图图片
 * 输入  : x					--显示纵坐标(0-239) 
 *				 y					--显示横坐标(0-319)   
 *				 pic_name		--图片名称
 * 输出  ：无
 * 举例  ：Lcd_show_bmp(0, 0,"/test.bmp");
 * 注意  ：图片为24为真彩色位图图片
 					 图片宽度不能超过1024
					 图片在LCD上的粘贴范围为:纵向:  [x,x+图像高度]   横向 [Y，Y+图像宽度]
					 当图片为1024*768时--建议X输入0  y输入0
 *********************************************************/    
void Lcd_show_bmp(unsigned short int x, unsigned short int y,unsigned char *pic_name,uint8_t Dir)
{
    int i, j, k;
    int width, height, l_width;
		  
    BYTE red,green,blue;
    BITMAPFILEHEADER bitHead;
    BITMAPINFOHEADER bitInfoHead;
    WORD fileType;

    unsigned int read_num;
    unsigned char tmp_name[20];
    sprintf((char*)tmp_name,"0:%s",pic_name);
    f_mount(0, &bmpfs[0]);
    
    bmpres = f_open( &bmpfsrc , (char *)tmp_name, FA_OPEN_EXISTING | FA_READ);
    Set_direction(0,0,Dir);

    if(bmpres == FR_OK)
    {
        USART1_printf(USART1,"\r Open file success\r\n");
        
        //读取位图文件头信息        
        f_read(&bmpfsrc,&fileType,sizeof(WORD),&read_num);     
        
        if(fileType != 0x4d42)
        {
            USART1_printf(USART1,"file is not .bmp file!\r\n");
            return;
        }
        else
        {
            USART1_printf(USART1,"Ok this is .bmp file\r\n");	
        }        
        
        f_read(&bmpfsrc,&bitHead,sizeof(tagBITMAPFILEHEADER),&read_num);    
        
        showBmpHead(&bitHead);
        USART1_printf(USART1,"\r\n");
        
        //读取位图信息头信息
        f_read(&bmpfsrc,&bitInfoHead,sizeof(BITMAPINFOHEADER),&read_num);        
        showBmpInforHead(&bitInfoHead);
        USART1_printf(USART1,"\r\n");
    }    
    else
    {
        USART1_printf(USART1,"file open fail!\r\n");
        return;
    }    
    
    width = bitInfoHead.biWidth;
    height = bitInfoHead.biHeight;
    
    l_width = WIDTHBYTES(width* bitInfoHead.biBitCount);		//计算位图的实际宽度并确保它为32的倍数	    
    
    if(l_width>LCD_XSIZE*3)
    {
        USART1_printf(USART1,"\nSORRY, PIC IS TOO BIG (<=LCD_XSIZE)\n");
        return;
    }
    
    if(bitInfoHead.biBitCount>=24)
    {
        
        for(i=0;i<height; i++)   //+1
        {	
            
            for(j=0; j<l_width; j++)	 													//将一行数据全部读入
            {
                
                f_read(&bmpfsrc,pColorData+j,1,&read_num);
            }            							
            for(j=0;j<width;j++) 																//一行有效信息
            {
                k = j*3;																				//一行中第K个像素的起点
                red = pColorData[k+2];
                green = pColorData[k+1];
                blue = 	pColorData[k];
                LCD_WriteRAM(RGB24TORGB16(red,green,blue));		//写入LCD-GRAM
            }            
        }	 		
  	LCD_WriteRAM_Prepare();									//lcd扫描方向复原	 
				
		USART1_printf(USART1,"Display has been finished!\n\n");
				
    }    
    else 
    {        
        USART1_printf(USART1,"SORRY, THIS PIC IS NOT A 24BITS REAL COLOR\n");
        return ;
    }
    
    f_close(&bmpfsrc);    
}

/*
*	  函数:write_bmp
*	  功能:将RGB_PIXEL数据流保存为24位真彩色bmp格式图片
*   输入: pixarr--像素数据流地址  xize--保存图片的宽度  
*         ysize--保存图片的高度  filename--保存图片的名称 
*   输出: 0 ---成功  -1 ---失败			8--文件已存在
*   注意: 注意文件名不要已存在,否则直接返回
*   举例:  RGB_PIXEL Pix_buff[272];		//17*16=272
*					 Pix_buff数组初始化
*					 write_bmp(Pix_buff, 17, 16, "/such");
*/
int write_bmp(RGB_PIXEL *pixarr, int xsize, int ysize, char *filename) 
{
    unsigned char header[54] =
    {
        0x42, 0x4d, 0, 0, 0, 0, 
        0, 0, 0, 0, 54, 0, 
        0, 0, 40,0, 0, 0, 
        0, 0, 0, 0, 0, 0, 
        0, 0, 1, 0, 24, 0, 
        0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 
        0, 0, 0
    };
    int i;
    int j;
    long file_size;     
    long width;
    long height;	
    unsigned char tmp_name[30];
    unsigned int mybw;
    char kk[4]={0,0,0,0};
    
    
    if(!(xsize%4))
        file_size = (long)xsize * (long)ysize * 3 + 54;
    else
        file_size = (long)xsize * (long)ysize * 3 + ysize*(xsize%4) + 54;		//宽*高 +补充的字节 + 头部信息
    
    
    
    header[2] = (unsigned char)(file_size &0x000000ff);
    header[3] = (file_size >> 8) & 0x000000ff;
    header[4] = (file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;
    
    width=xsize;
    header[18] = width & 0x000000ff;
    header[19] = (width >> 8) &0x000000ff;
    header[20] = (width >> 16) &0x000000ff;
    header[21] = (width >> 24) &0x000000ff;
    
    height = ysize;
    header[22] = height &0x000000ff;
    header[23] = (height >> 8) &0x000000ff;
    header[24] = (height >> 16) &0x000000ff;
    header[25] = (height >> 24) &0x000000ff;
    
    
    sprintf((char*)tmp_name,"0:%s.bmp",filename);
    f_mount(0, &bmpfs[0]);
    bmpres = f_open( &bmpfsrc , (char*)tmp_name, FA_CREATE_NEW | FA_WRITE);
    
    if ( bmpres == FR_OK )
    {
        bmpres = f_write(&bmpfsrc, header,sizeof(unsigned char)*54, &mybw);                
        for(i=0;i<ysize;i++)
        {
            if(!(xsize%4))
            {
                for(j=0;j<xsize;j++)
                { 
                    bmpres = f_write(&bmpfsrc, pixarr+j+i*xsize,sizeof(RGB_PIXEL), &mybw);
                }		
            }
            else
            {
                for(j=0;j<xsize;j++)
                { 
                    bmpres = f_write(&bmpfsrc, pixarr+j+i*xsize,sizeof(RGB_PIXEL), &mybw);
                    
                }
                
                bmpres = f_write(&bmpfsrc, kk,sizeof(unsigned char)*(xsize%4), &mybw);		 //字节补齐
                
            }
        }
        
        f_close(&bmpfsrc);        
        return 0;
    }
    
    else if ( bmpres == FR_EXIST ) 	 	//如果文件已经存在
    {
        return FR_EXIST;	 						//返回8
    }
    
    else
    {
        return -1;
    }    
}



/*	函数:write_bmp2
*	  功能:将rgb888数据流(uchar型---前后顺序为bgrbgrbgr....)保存为24位真彩色bmp格式图片
*   输入: pixarr--像素数据流地址  xize--保存图片的宽度  ysize--保存图片的高度  filename--保存图片的名称 
*   输出: 0 ---成功  -1 ---失败	 8--文件已存在
*   注意: 无
*/
int write_bmp2(unsigned char  *pixarr, int xsize, int ysize, char *filename) 
{
    unsigned char header[54] =
    {
        0x42, 0x4d, 0, 0, 0, 0, 
        0, 0, 0, 0, 54, 0, 
        0, 0, 40,0, 0, 0, 
        0, 0, 0, 0, 0, 0, 
        0, 0, 1, 0, 24, 0, 
        0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 
        0, 0, 0
    };
    int i;
    int j;
    long file_size;     
    long width;
    long height;	
    unsigned char tmp_name[30];
    unsigned int mybw;
    char kk[4]={0,0,0,0};
    
    
    if(!(xsize%4))
        file_size = (long)xsize * (long)ysize * 3 + 54;
    else
        file_size = (long)xsize * (long)ysize * 3 + ysize*(xsize%4) + 54;		//宽*高 +补充的字节 + 头部信息
    
    
    
    header[2] = (unsigned char)(file_size &0x000000ff);
    header[3] = (file_size >> 8) & 0x000000ff;
    header[4] = (file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;
    
    
    width=xsize;
    header[18] = width & 0x000000ff;
    header[19] = (width >> 8) &0x000000ff;
    header[20] = (width >> 16) &0x000000ff;
    header[21] = (width >> 24) &0x000000ff;
    
    height = ysize;
    header[22] = height &0x000000ff;
    header[23] = (height >> 8) &0x000000ff;
    header[24] = (height >> 16) &0x000000ff;
    header[25] = (height >> 24) &0x000000ff;
    
    sprintf((char*)tmp_name,"0:%s.bmp",filename);
    f_mount(0, &bmpfs[0]);
    bmpres = f_open( &bmpfsrc , (char*)tmp_name, FA_CREATE_NEW | FA_WRITE);
    
    if ( bmpres == FR_OK )
    {
        
        bmpres = f_write(&bmpfsrc, header,sizeof(unsigned char)*54, &mybw); 
        
        printf("%0d-%0d\n",ysize,xsize);
        for(i=0;i<ysize;i++)						//高
        {
            if(!(xsize%4))
            {
                for(j=0;j<xsize;j++)  	//宽
                { 
                    
                    bmpres = f_write(&bmpfsrc, pixarr+i*3*xsize+j*3,sizeof(unsigned char)*3, &mybw);
                    
                }
                
            }
            else
            {
                for(j=0;j<xsize;j++)
                { 
                    bmpres = f_write(&bmpfsrc, pixarr+i*3*xsize+j*3,sizeof(unsigned char)*3, &mybw);
                }
                
                
                bmpres = f_write(&bmpfsrc, kk,sizeof(unsigned char)*(xsize%4), &mybw); //补充字节以补齐
            }
        }
        
        f_close(&bmpfsrc);
        return 0;
    }
    
    else if ( bmpres == FR_EXIST ) 	//如果文件已经存在
    {
        return FR_EXIST;						//返回8 
    }
    
    else
    {
        return -1;
    }
    
    
}




/*	函数:write_bmp3
*	  功能:将rgb565数据流(uchar型---前后字节顺序为 b-g g-r b-g g-r b-g g-r....)保存为24位真彩色bmp格式图片
*   输入: pixarr--像素数据流地址  xize--保存图片的宽度  ysize--保存图片的高度  filename--保存图片的名称 
*   输出: 0 ---成功  -1 ---失败
*   注意:
*   举例：unsigned short int rgb_buffer2[18*17*2];
*					rgb_buffer2数组初始化
*			  	write_bmp3((unsigned char *)rgb_buffer2, 18, 17, "/myimg3");
* 
*/
int write_bmp3(unsigned char  *pixarr, int xsize, int ysize, char *filename) 
{
    unsigned char header[54] =
    {
        0x42, 0x4d, 0, 0, 0, 0, 
        0, 0, 0, 0, 54, 0, 
        0, 0, 40,0, 0, 0, 
        0, 0, 0, 0, 0, 0, 
        0, 0, 1, 0, 24, 0, 
        0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 
        0, 0, 0
    };
    int i;
    int j;
    long file_size;     
    long width;
    long height;
    unsigned short int tmp_rgb;
    unsigned char r,g,b;	
    unsigned char tmp_name[30];
    unsigned int mybw;
    char kk[4]={0,0,0,0};
    
    
    if(!(xsize%4))
        file_size = (long)xsize * (long)ysize * 3 + 54;
    else
        file_size = (long)xsize * (long)ysize * 3 + ysize*(xsize%4) + 54;		//宽*高 +补充的字节 + 头部信息
    
    
    
    header[2] = (unsigned char)(file_size &0x000000ff);
    header[3] = (file_size >> 8) & 0x000000ff;
    header[4] = (file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;
    
    
    width=xsize;
    header[18] = width & 0x000000ff;
    header[19] = (width >> 8) &0x000000ff;
    header[20] = (width >> 16) &0x000000ff;
    header[21] = (width >> 24) &0x000000ff;
    
    height = ysize;
    header[22] = height &0x000000ff;
    header[23] = (height >> 8) &0x000000ff;
    header[24] = (height >> 16) &0x000000ff;
    header[25] = (height >> 24) &0x000000ff;
    
    sprintf((char*)tmp_name,"0:%s.bmp",filename);
    f_mount(0, &bmpfs[0]);
    bmpres = f_open( &bmpfsrc , (char*)tmp_name, FA_CREATE_NEW | FA_WRITE);
    
    
    if ( bmpres == FR_OK )
    {    
        bmpres = f_write(&bmpfsrc, header,sizeof(unsigned char)*54, &mybw);
        
        for(i=0;i<ysize;i++)						//高
        {
            if(!(xsize%4))
            {
                for(j=0;j<xsize;j++)  	//宽
                { 
                    
                    tmp_rgb = (((*(pixarr+i*2*xsize+j*2+1))<<8) | (*(pixarr+i*2*xsize+j*2)));
                    r =  GETR_FROM_RGB16(tmp_rgb);
                    g =  GETG_FROM_RGB16(tmp_rgb);
                    b =  GETB_FROM_RGB16(tmp_rgb);
                    
                    bmpres = f_write(&bmpfsrc, &b,sizeof(unsigned char), &mybw);
                    bmpres = f_write(&bmpfsrc, &g,sizeof(unsigned char), &mybw);
                    bmpres = f_write(&bmpfsrc, &r,sizeof(unsigned char), &mybw);
                    
                }
                
            }
            else
            {
                for(j=0;j<xsize;j++)
                { 
                    
                    tmp_rgb = (((*(pixarr+i*2*xsize+j*2+1))<<8) | (*(pixarr+i*2*xsize+j*2)));
                    r =  GETR_FROM_RGB16(tmp_rgb);
                    g =  GETG_FROM_RGB16(tmp_rgb);
                    b =  GETB_FROM_RGB16(tmp_rgb);
                    bmpres = f_write(&bmpfsrc, &b,sizeof(unsigned char), &mybw);
                    bmpres = f_write(&bmpfsrc, &g,sizeof(unsigned char), &mybw);
                    bmpres = f_write(&bmpfsrc, &r,sizeof(unsigned char), &mybw);
                    
                    
                }
                
                bmpres = f_write(&bmpfsrc, kk,sizeof(unsigned char)*(xsize%4), &mybw);
                
            }
        }
        
        f_close(&bmpfsrc); 
        return 0;
    }
    else if ( bmpres == FR_EXIST )  //如果文件已经存在
    {
        return FR_EXIST;	 					//8
    }
    
    else
    {
        return -1;
    }
    
    
}




 /**********************************************************
 * 函数名：Screen_shot
 * 描述  ：截取LCD指定位置  指定宽高的像素 保存为24位真彩色bmp格式图片
 * 输入  : 	x								---水平位置 
 *					y								---竖直位置  
 *					Width						---水平宽度   
 *					Height					---竖直高度  	
 *					filename				---文件名
 * 输出  ：	0 		---成功
 *  				-1 		---失败
 *	    		8			---文件已存在
 * 举例  ：Screen_shot(0, 0, 320, 240, "/myScreen");-----全屏截图
 * 注意  ：x范围[0,319]  y范围[0,239]  Width范围[0,320-x]  Height范围[0,240-y]
 *					如果文件已存在,将直接返回	
 **************************************************************/    
int Screen_shot(unsigned short int x, unsigned short int y, unsigned short int Width, unsigned short int Height, unsigned char *filename)
{
    unsigned char header[54] =
    {
        0x42, 0x4d, 0, 0, 0, 0, 
        0, 0, 0, 0, 54, 0, 
        0, 0, 40,0, 0, 0, 
        0, 0, 0, 0, 0, 0, 
        0, 0, 1, 0, 24, 0, 
        0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 
        0, 0, 0
    };
    int i;
    int j;
    long file_size;     
    long width;
    long height;
    unsigned short int tmp_rgb;
    unsigned char r,g,b;	
    unsigned char tmp_name[30];
    unsigned int mybw;
    char kk[4]={0,0,0,0};
    
    
   // if(!(Width%4))
    //    file_size = (long)Width * (long)Height * 3 + 54;
    //else
    file_size = (long)Width * (long)Height * 3 + Height*(Width%4) + 54;		//宽*高 +补充的字节 + 头部信息
    
    header[2] = (unsigned char)(file_size &0x000000ff);
    header[3] = (file_size >> 8) & 0x000000ff;
    header[4] = (file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;
    
    
    width=Width;
    header[18] = width & 0x000000ff;
    header[19] = (width >> 8) &0x000000ff;
    header[20] = (width >> 16) &0x000000ff;
    header[21] = (width >> 24) &0x000000ff;
    
    height = Height;
    header[22] = height &0x000000ff;
    header[23] = (height >> 8) &0x000000ff;
    header[24] = (height >> 16) &0x000000ff;
    header[25] = (height >> 24) &0x000000ff;
    
    sprintf((char*)tmp_name,"0:%s.bmp",filename);
    f_mount(0, &bmpfs[0]);
    bmpres = f_open( &bmpfsrc , (char*)tmp_name, FA_CREATE_NEW | FA_WRITE);
    
    if ( bmpres == FR_OK )
    {    
        bmpres = f_write(&bmpfsrc, header,sizeof(unsigned char)*54, &mybw);
        for(i=0;i<Height;i++)						//高
        {
            if(!(Width%4))
            {
                for(j=0;j<Width;j++)  	//宽
                { 
                    
                    tmp_rgb = lcd_GetPoint(j+y,Height-i+x-1);
                    
                    r =  GETR_FROM_RGB16(tmp_rgb);
                    g =  GETG_FROM_RGB16(tmp_rgb);
                    b =  GETB_FROM_RGB16(tmp_rgb);
                    
                    bmpres = f_write(&bmpfsrc, &b,sizeof(unsigned char), &mybw);
                    bmpres = f_write(&bmpfsrc, &g,sizeof(unsigned char), &mybw);
                    bmpres = f_write(&bmpfsrc, &r,sizeof(unsigned char), &mybw);
                    
                }
                
            }
            else
            {
                for(j=0;j<Width;j++)
                { 
                    
                    tmp_rgb =lcd_GetPoint(j+y,Height-i+x-1);
                    r =  GETR_FROM_RGB16(tmp_rgb);
                    g =  GETG_FROM_RGB16(tmp_rgb);
                    b =  GETB_FROM_RGB16(tmp_rgb);
                    
                    bmpres = f_write(&bmpfsrc, &b,sizeof(unsigned char), &mybw);
                    bmpres = f_write(&bmpfsrc, &g,sizeof(unsigned char), &mybw);
                    bmpres = f_write(&bmpfsrc, &r,sizeof(unsigned char), &mybw);
                    
                    
                }
                
                bmpres = f_write(&bmpfsrc, kk,sizeof(unsigned char)*(Width%4), &mybw);
                
		
            }	
        }
        
        f_close(&bmpfsrc); 
        return 0;
    }
    else if ( bmpres == FR_EXIST )  //如果文件已经存在
    {
        return FR_EXIST;	 					//8
    }
    
    else
    {
        return -1;
    }    
}




int Camera_shot(unsigned short int x, unsigned short int y, unsigned short int Width, unsigned short int Height, unsigned char *filename)
{
    unsigned char header[54] =
    {
        0x42, 0x4d, 0, 0, 0, 0, 
        0, 0, 0, 0, 54, 0, 
        0, 0, 40,0, 0, 0, 
        0, 0, 0, 0, 0, 0, 
        0, 0, 1, 0, 24, 0, 
        0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 
        0, 0, 0
    };
    int i;
    int j;
    long file_size;     
    long width;
    long height;
    unsigned short int tmp_rgb;
    unsigned char r,g,b;	
    unsigned char tmp_name[30];
    unsigned int mybw;
    char kk[4]={0,0,0,0};
		
/****************Camrea Define******************************/		
//			uint32_t count;
			uint16_t CMOS_Data;    
/*****************************************************/
   // if(!(Width%4))
    //    file_size = (long)Width * (long)Height * 3 + 54;
    //else
    file_size = (long)Width * (long)Height * 3 + Height*(Width%4) + 54;		//宽*高 +补充的字节 + 头部信息
    
    header[2] = (unsigned char)(file_size &0x000000ff);
    header[3] = (file_size >> 8) & 0x000000ff;
    header[4] = (file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;
    
    
    width=Width;
    header[18] = width & 0x000000ff;
    header[19] = (width >> 8) &0x000000ff;
    header[20] = (width >> 16) &0x000000ff;
    header[21] = (width >> 24) &0x000000ff;
    
    height = Height;
    header[22] = height &0x000000ff;
    header[23] = (height >> 8) &0x000000ff;
    header[24] = (height >> 16) &0x000000ff;
    header[25] = (height >> 24) &0x000000ff;
    
    sprintf((char*)tmp_name,"0:%s.bmp",filename);
    f_mount(0, &bmpfs[0]);
    bmpres = f_open( &bmpfsrc , (char*)tmp_name, FA_CREATE_NEW | FA_WRITE);
		
		
		
		
    
    if ( bmpres == FR_OK )
    {    
        bmpres = f_write(&bmpfsrc, header,sizeof(unsigned char)*54, &mybw);
			

/***********************READ Carmera Preparation******************************************/

						
					if( Vsync == 2 )   //写图像数据到TFT显示区
					{	
											
								 FIFO_RRST_L(); 
								 FIFO_RCLK_L();
						 
								 FIFO_RCLK_H();
								 FIFO_RRST_H();
								 FIFO_RCLK_L();
							
								 FIFO_RCLK_H();
/*********************************************************************/

										
			for(i=0;i<Height;i++)						//高
        {
            if(!(Width%4))
            {
                for(j=0;j<Width;j++)  	//宽
                { 
                    
/************************READ Carmera DATA*****************************/		
									FIFO_RCLK_L(); 
									CMOS_Data = ((GPIOC->IDR<<8) & 0xff00);	  
									FIFO_RCLK_H();
									FIFO_RCLK_L();
									CMOS_Data |= (((GPIOC->IDR) & 0x00ff));
									FIFO_RCLK_H();  
// 								*(__IO uint16_t *) (Bank1_LCD_D)= CMOS_Data; 		//将FIFO中的16位数据写入显示区 	
									tmp_rgb = CMOS_Data;
/**********************************************************/									
									
									
									
//                     tmp_rgb = lcd_GetPoint(j+y,Height-i+x-1);
                    
                    r =  GETR_FROM_RGB16(tmp_rgb);
                    g =  GETG_FROM_RGB16(tmp_rgb);
                    b =  GETB_FROM_RGB16(tmp_rgb);
                    
									
									
									
                    bmpres = f_write(&bmpfsrc, &b,sizeof(unsigned char), &mybw);
                    bmpres = f_write(&bmpfsrc, &g,sizeof(unsigned char), &mybw);
                    bmpres = f_write(&bmpfsrc, &r,sizeof(unsigned char), &mybw);
                    
                }
                
            }
            else
            {
														
							
                for(j=0;j<Width;j++)
                { 
                    
                    
/***********************READ Carmera DATA*****************************/		
									FIFO_RCLK_L(); 
									CMOS_Data = ((GPIOC->IDR<<8) & 0xff00);	  
									FIFO_RCLK_H();
									FIFO_RCLK_L();
									CMOS_Data |= (((GPIOC->IDR) & 0x00ff));
									FIFO_RCLK_H();  
// 								*(__IO uint16_t *) (Bank1_LCD_D)= CMOS_Data; 		//将FIFO中的16位数据写入显示区 	
									tmp_rgb = CMOS_Data;
/*********************************************************************/									
									
//                     tmp_rgb =lcd_GetPoint(j+y,Height-i+x-1);
                    r =  GETR_FROM_RGB16(tmp_rgb);
                    g =  GETG_FROM_RGB16(tmp_rgb);
                    b =  GETB_FROM_RGB16(tmp_rgb);
                    
                    bmpres = f_write(&bmpfsrc, &b,sizeof(unsigned char), &mybw);
                    bmpres = f_write(&bmpfsrc, &g,sizeof(unsigned char), &mybw);
                    bmpres = f_write(&bmpfsrc, &r,sizeof(unsigned char), &mybw);
                    
                }
                
                bmpres = f_write(&bmpfsrc, kk,sizeof(unsigned char)*(Width%4), &mybw);                
		
            }	
        }

/***********************Carmera Reset*****************************/		

						 Vsync = 0;
			}
/*********************************************************************/									

        f_close(&bmpfsrc); 
        return 0;
    }
    else if ( bmpres == FR_EXIST )  //如果文件已经存在
    {
        return FR_EXIST;	 					//8
    }
    
    else
    {
        return -1;
    }    
}


/******************** (C) MODIFIED  2011 野火嵌入式开发工作室 *****END OF FILE***/

