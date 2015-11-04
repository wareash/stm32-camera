/******************** (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� ********************
 * �ļ���  ��sd_bmp.c
 * ����    ��LCD��ʾbmpͼƬӦ�ú����⣬bmpͼƬ�洢��MicroSD���С�
 *              
 * ʵ��ƽ̨��Ұ��STM32������
 * Ӳ�����ӣ���
 *
 * ��汾  ��ST3.0.0
 *
 * ����    ��fire  QQ: 313303034 
 * ����    ��firestm32.blog.chinaunix.net
**********************************************************************************/
#include "Sd_bmp.h"
#include "stm32f10x.h"
#include "ff.h"
#include "usart1.h"
#include "lcd.h"

#include "OV7670.h"
#define RGB24TORGB16(R,G,B) ((unsigned short int)((((R)>>3)<<11) | (((G)>>2)<<5)	| ((B)>>3)	))

/* ��ӡBMP�ļ���ͷ��Ϣ�����ڵ��� */
static void showBmpHead(BITMAPFILEHEADER* pBmpHead)
{
    USART1_printf(USART1,"λͼ�ļ�ͷ:\r\n");
    USART1_printf(USART1,"�ļ���С:%d\r\n",(*pBmpHead).bfSize);
    USART1_printf(USART1,"������:%d\r\n",(*pBmpHead).bfReserved1);
    USART1_printf(USART1,"������:%d\r\n",(*pBmpHead).bfReserved2);
    USART1_printf(USART1,"ʵ��λͼ���ݵ�ƫ���ֽ���:%d\r\n",(*pBmpHead).bfOffBits);    
}

/* ��ӡBMP�ļ���ͷ��Ϣ�����ڵ��� */
static void showBmpInforHead(tagBITMAPINFOHEADER* pBmpInforHead)
{
    USART1_printf(USART1,"λͼ��Ϣͷ:\r\n");
    USART1_printf(USART1,"�ṹ��ĳ���:%d\r\n",(*pBmpInforHead).biSize);
    USART1_printf(USART1,"λͼ��:%d\r\n",(*pBmpInforHead).biWidth);
    USART1_printf(USART1,"λͼ��:%d\r\n",(*pBmpInforHead).biHeight);
    USART1_printf(USART1,"biPlanesƽ����:%d\r\n",(*pBmpInforHead).biPlanes);
    USART1_printf(USART1,"biBitCount������ɫλ��:%d\r\n",(*pBmpInforHead).biBitCount);
    USART1_printf(USART1,"ѹ����ʽ:%d\r\n",(*pBmpInforHead).biCompression);
    USART1_printf(USART1,"biSizeImageʵ��λͼ����ռ�õ��ֽ���:%d\r\n",(*pBmpInforHead).biSizeImage);
    USART1_printf(USART1,"X����ֱ���:%d\r\n",(*pBmpInforHead).biXPelsPerMeter);
    USART1_printf(USART1,"Y����ֱ���:%d\r\n",(*pBmpInforHead).biYPelsPerMeter);
    USART1_printf(USART1,"ʹ�õ���ɫ��:%d\r\n",(*pBmpInforHead).biClrUsed);
    USART1_printf(USART1,"��Ҫ��ɫ��:%d\r\n",(*pBmpInforHead).biClrImportant);
}

void showRgbQuan(tagRGBQUAD* pRGB)
{
    USART1_printf(USART1,"(%d-%d-%d) ",(*pRGB).rgbRed,(*pRGB).rgbGreen,(*pRGB).rgbBlue);    
}

BYTE pColorData[LCD_XSIZE*3];	   //��Ļ��ȵ�3��
tagRGBQUAD dataOfBmp[17*19];
FATFS bmpfs[2]; 
FIL bmpfsrc, bmpfdst; 
FRESULT bmpres;



 /******************************************************
 * ��������Lcd_show_bmp
 * ����  ��LCD��ʾRGB888λͼͼƬ
 * ����  : x					--��ʾ������(0-239) 
 *				 y					--��ʾ������(0-319)   
 *				 pic_name		--ͼƬ����
 * ���  ����
 * ����  ��Lcd_show_bmp(0, 0,"/test.bmp");
 * ע��  ��ͼƬΪ24Ϊ���ɫλͼͼƬ
 					 ͼƬ��Ȳ��ܳ���1024
					 ͼƬ��LCD�ϵ�ճ����ΧΪ:����:  [x,x+ͼ��߶�]   ���� [Y��Y+ͼ����]
					 ��ͼƬΪ1024*768ʱ--����X����0  y����0
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
        
        //��ȡλͼ�ļ�ͷ��Ϣ        
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
        
        //��ȡλͼ��Ϣͷ��Ϣ
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
    
    l_width = WIDTHBYTES(width* bitInfoHead.biBitCount);		//����λͼ��ʵ�ʿ�Ȳ�ȷ����Ϊ32�ı���	    
    
    if(l_width>LCD_XSIZE*3)
    {
        USART1_printf(USART1,"\nSORRY, PIC IS TOO BIG (<=LCD_XSIZE)\n");
        return;
    }
    
    if(bitInfoHead.biBitCount>=24)
    {
        
        for(i=0;i<height; i++)   //+1
        {	
            
            for(j=0; j<l_width; j++)	 													//��һ������ȫ������
            {
                
                f_read(&bmpfsrc,pColorData+j,1,&read_num);
            }            							
            for(j=0;j<width;j++) 																//һ����Ч��Ϣ
            {
                k = j*3;																				//һ���е�K�����ص����
                red = pColorData[k+2];
                green = pColorData[k+1];
                blue = 	pColorData[k];
                LCD_WriteRAM(RGB24TORGB16(red,green,blue));		//д��LCD-GRAM
            }            
        }	 		
  	LCD_WriteRAM_Prepare();									//lcdɨ�跽��ԭ	 
				
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
*	  ����:write_bmp
*	  ����:��RGB_PIXEL����������Ϊ24λ���ɫbmp��ʽͼƬ
*   ����: pixarr--������������ַ  xize--����ͼƬ�Ŀ��  
*         ysize--����ͼƬ�ĸ߶�  filename--����ͼƬ������ 
*   ���: 0 ---�ɹ�  -1 ---ʧ��			8--�ļ��Ѵ���
*   ע��: ע���ļ�����Ҫ�Ѵ���,����ֱ�ӷ���
*   ����:  RGB_PIXEL Pix_buff[272];		//17*16=272
*					 Pix_buff�����ʼ��
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
        file_size = (long)xsize * (long)ysize * 3 + ysize*(xsize%4) + 54;		//��*�� +������ֽ� + ͷ����Ϣ
    
    
    
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
                
                bmpres = f_write(&bmpfsrc, kk,sizeof(unsigned char)*(xsize%4), &mybw);		 //�ֽڲ���
                
            }
        }
        
        f_close(&bmpfsrc);        
        return 0;
    }
    
    else if ( bmpres == FR_EXIST ) 	 	//����ļ��Ѿ�����
    {
        return FR_EXIST;	 						//����8
    }
    
    else
    {
        return -1;
    }    
}



/*	����:write_bmp2
*	  ����:��rgb888������(uchar��---ǰ��˳��Ϊbgrbgrbgr....)����Ϊ24λ���ɫbmp��ʽͼƬ
*   ����: pixarr--������������ַ  xize--����ͼƬ�Ŀ��  ysize--����ͼƬ�ĸ߶�  filename--����ͼƬ������ 
*   ���: 0 ---�ɹ�  -1 ---ʧ��	 8--�ļ��Ѵ���
*   ע��: ��
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
        file_size = (long)xsize * (long)ysize * 3 + ysize*(xsize%4) + 54;		//��*�� +������ֽ� + ͷ����Ϣ
    
    
    
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
        for(i=0;i<ysize;i++)						//��
        {
            if(!(xsize%4))
            {
                for(j=0;j<xsize;j++)  	//��
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
                
                
                bmpres = f_write(&bmpfsrc, kk,sizeof(unsigned char)*(xsize%4), &mybw); //�����ֽ��Բ���
            }
        }
        
        f_close(&bmpfsrc);
        return 0;
    }
    
    else if ( bmpres == FR_EXIST ) 	//����ļ��Ѿ�����
    {
        return FR_EXIST;						//����8 
    }
    
    else
    {
        return -1;
    }
    
    
}




/*	����:write_bmp3
*	  ����:��rgb565������(uchar��---ǰ���ֽ�˳��Ϊ b-g g-r b-g g-r b-g g-r....)����Ϊ24λ���ɫbmp��ʽͼƬ
*   ����: pixarr--������������ַ  xize--����ͼƬ�Ŀ��  ysize--����ͼƬ�ĸ߶�  filename--����ͼƬ������ 
*   ���: 0 ---�ɹ�  -1 ---ʧ��
*   ע��:
*   ������unsigned short int rgb_buffer2[18*17*2];
*					rgb_buffer2�����ʼ��
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
        file_size = (long)xsize * (long)ysize * 3 + ysize*(xsize%4) + 54;		//��*�� +������ֽ� + ͷ����Ϣ
    
    
    
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
        
        for(i=0;i<ysize;i++)						//��
        {
            if(!(xsize%4))
            {
                for(j=0;j<xsize;j++)  	//��
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
    else if ( bmpres == FR_EXIST )  //����ļ��Ѿ�����
    {
        return FR_EXIST;	 					//8
    }
    
    else
    {
        return -1;
    }
    
    
}




 /**********************************************************
 * ��������Screen_shot
 * ����  ����ȡLCDָ��λ��  ָ����ߵ����� ����Ϊ24λ���ɫbmp��ʽͼƬ
 * ����  : 	x								---ˮƽλ�� 
 *					y								---��ֱλ��  
 *					Width						---ˮƽ���   
 *					Height					---��ֱ�߶�  	
 *					filename				---�ļ���
 * ���  ��	0 		---�ɹ�
 *  				-1 		---ʧ��
 *	    		8			---�ļ��Ѵ���
 * ����  ��Screen_shot(0, 0, 320, 240, "/myScreen");-----ȫ����ͼ
 * ע��  ��x��Χ[0,319]  y��Χ[0,239]  Width��Χ[0,320-x]  Height��Χ[0,240-y]
 *					����ļ��Ѵ���,��ֱ�ӷ���	
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
    file_size = (long)Width * (long)Height * 3 + Height*(Width%4) + 54;		//��*�� +������ֽ� + ͷ����Ϣ
    
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
        for(i=0;i<Height;i++)						//��
        {
            if(!(Width%4))
            {
                for(j=0;j<Width;j++)  	//��
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
    else if ( bmpres == FR_EXIST )  //����ļ��Ѿ�����
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
    file_size = (long)Width * (long)Height * 3 + Height*(Width%4) + 54;		//��*�� +������ֽ� + ͷ����Ϣ
    
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

						
					if( Vsync == 2 )   //дͼ�����ݵ�TFT��ʾ��
					{	
											
								 FIFO_RRST_L(); 
								 FIFO_RCLK_L();
						 
								 FIFO_RCLK_H();
								 FIFO_RRST_H();
								 FIFO_RCLK_L();
							
								 FIFO_RCLK_H();
/*********************************************************************/

										
			for(i=0;i<Height;i++)						//��
        {
            if(!(Width%4))
            {
                for(j=0;j<Width;j++)  	//��
                { 
                    
/************************READ Carmera DATA*****************************/		
									FIFO_RCLK_L(); 
									CMOS_Data = ((GPIOC->IDR<<8) & 0xff00);	  
									FIFO_RCLK_H();
									FIFO_RCLK_L();
									CMOS_Data |= (((GPIOC->IDR) & 0x00ff));
									FIFO_RCLK_H();  
// 								*(__IO uint16_t *) (Bank1_LCD_D)= CMOS_Data; 		//��FIFO�е�16λ����д����ʾ�� 	
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
// 								*(__IO uint16_t *) (Bank1_LCD_D)= CMOS_Data; 		//��FIFO�е�16λ����д����ʾ�� 	
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
    else if ( bmpres == FR_EXIST )  //����ļ��Ѿ�����
    {
        return FR_EXIST;	 					//8
    }
    
    else
    {
        return -1;
    }    
}


/******************** (C) MODIFIED  2011 Ұ��Ƕ��ʽ���������� *****END OF FILE***/

