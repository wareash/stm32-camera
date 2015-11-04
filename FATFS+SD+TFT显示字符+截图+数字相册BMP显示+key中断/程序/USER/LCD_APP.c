

#include "lcd.h"

u16 POINT_COLOR = BLACK;					      /*���Ĭ����ɫ*/
extern volatile u8 display_direction;		/*0������ 1������*/

unsigned long color1=0;

/****************************************************************************
* ��    �ƣ�GUI_CmpColor()
* ��    �ܣ��ж���ɫֵ�Ƿ�һ�¡�
* ��ڲ�����color1		��ɫֵ1
*		        color2		��ɫֵ2
* ���ڲ���������1��ʾ��ͬ������0��ʾ����ͬ��
* ˵    ����
****************************************************************************/
#define  GUI_CmpColor(color1, color2)	( (color1&0x01) == (color2&0x01) )



/****************************************************************************
* ��    �ƣ�void LCD_Init(void)
* ��    �ܣ�3 ��TFT(R61509) ��ʼ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void LCD_Init(void)
{
	
	unsigned int i;
	
	FSMC_LCD_Init();							 //FSMC��������		  

	GPIO_ResetBits(GPIOE, GPIO_Pin_1);	  //Ӳ����λ
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
	*(__IO uint16_t *) (Bank1_LCD_C)= 0x202;	 //׼��д������ʾ��
	for(i=0;i<96000;i++)
	{
	  LCD_WR_Data(0xffff);					 //�ú�ɫ����
	}  
	color1=0; 
}


/****************************************************************************
* ��    �ƣ�void LCD_clear(void)
* ��    �ܣ�����
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
****************************************************************************/
void LCD_clear(unsigned short a)
{
    unsigned int l=400,w;
	LCD_WR_CMD(0x0003, 0x01030); //��ʾ����ģʽ�Ĵ���  ����

	LCD_WR_CMD(0x0210, 0x00000);
	LCD_WR_CMD(0x0211, 0x000EF);
	LCD_WR_CMD(0x0212, 0x00000);
	LCD_WR_CMD(0x0213, 0x0018F);//432=1AF, 400=18F

	LCD_WR_CMD(0x0200, 0x00000);
	LCD_WR_CMD(0x0201, 0x00000);	
	//*(__IO uint16_t *) (Bank1_LCD_C)= 0x202;	 //׼��д������ʾ��
	LCD_WR_REG(0x0202);				          	//д���ݵ�
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
	LCD_WR_CMD(0x0003, 0x01030); //��ʾ����ģʽ�Ĵ���  ����

	LCD_WR_CMD(0x0210, 0x00000);
	LCD_WR_CMD(0x0211, 0x000EF);
	LCD_WR_CMD(0x0212, 0x00000);
	LCD_WR_CMD(0x0213, 0x0018F);//432=1AF, 400=18F

	LCD_WR_CMD(0x0200, 0x00000);
	LCD_WR_CMD(0x0201, 0x00000);	
	//*(__IO uint16_t *) (Bank1_LCD_C)= 0x202;	 //׼��д������ʾ��
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
*(__IO u16 *) (Bank1_LCD_D) = RGB_Code; //д��ʾ����	 	
}											   
//==============================================================================	  

/****************************************************************************
* ��    �ƣ�lcd_wr_zf(u16 StartX, u16 StartY, u16 X, u16 Y, u16 Color, u8 Dir, u8 *chr) 
* ��    �ܣ���ָ��������ʾһ���ַ�͸�������ڱ���ͼƬ��
* ��ڲ�����StartX     ����ʼ����	 0-239
*           StartY     ����ʼ����	 0-399
*           X          ��(Ϊ8�ı�����0-400
*           Y          ��			 0-240
*			Color      ��ɫ0-65535
*           Dir		   ͼ����ʾ����
*           chr        �ַ���ָ��
* ���ڲ�������
* ˵    �����ַ�ȡģ��ʽΪ��ɫ��ģ������ȡģ���ֽ�����  ȡģ�����ZIMO3
* ���÷�����lcd_wr_zf(0,0,100,100,(u16*)demo);
****************************************************************************/
//+++++++++++++++++++++++LCDд�ַ��ӳ���
void lcd_wr_zf(u16 StartX, u16 StartY, u16 X, u16 Y, u16 Color, u8 Dir, u8 *chr)
{	unsigned int temp=0,num,R_dis_mem=0,Size=0,x=0,y=0,i=0;


	if(Dir==2) LCD_WR_CMD(0x0003,0x1010);   //ͼ����ʾ����Ϊ������  �еݼ�  �е���  AM=0  I/D[1:0]=00	<--
	else if(Dir==3) LCD_WR_CMD(0x0003,0x1028);   //ͼ����ʾ����Ϊ������  �еݼ�  �е���  AM=1  I/D[1:0]=10	V
  	if(Dir==0){
		LCD_WR_CMD(0x0003,0x1030);		  //ͼ����ʾ����Ϊ������  �е���  �е���  AM=0  I/D[1:0]=11  -->
		LCD_WR_CMD(0x0210, StartX); 	  //ˮƽ��ʾ����ʼ��ַ 0-239
  		LCD_WR_CMD(0x0211, StartX+X-1);   //ˮƽ��ʾ��������ַ 0-239
  		LCD_WR_CMD(0x0212, StartY);       //��ֱ��ʾ����ʼ��ַ 0-399
  		LCD_WR_CMD(0x0213, StartY+Y-1);   //��ֱ��ʾ��������ַ 0-399
		LCD_WR_CMD(0x0200, StartX);		  //ˮƽ��ʾ����ַ
  		LCD_WR_CMD(0x0201, StartY);		  //��ֱ��ʾ����ַ	
		LCD_WR_REG(0x0202);               //׼��д������ʾ��
		Size=X*Y;						  //�ַ������ַ�ռ�õ����سߴ�
		while(i<Size){					  
			temp=*chr++;				  //һ���ֽڴ���8�����أ���˼�1������������8������
			for(num=0; num<8; num++){	  //�����ÿ���ֽڴ�����8������	    
				if((temp&0x80)>0){		  //���ֽڵĸ�λ�����жϣ�Ϊ1���ô��������16λ��ɫֵ��ʾ��д�뵽����λ�á�						 
				
					LCD_WR_Data(Color); 		
				}
				else{
					LCD_WR_CMD(0x0200, StartX+x);		//ˮƽ��ʾ����ַ
  					LCD_WR_CMD(0x0201, StartY+y);		//��ֱ��ʾ����ַ								
					LCD_WR_REG(0x0202);					//׼����������ʾ��
					R_dis_mem=LCD_RD_data();		  	//��ȡ����ɫ��Ϊ���Ӳ���͸��Ч����׼��	
					LCD_WR_Data(R_dis_mem); 		//���ֽڵĸ�λ�����жϣ�Ϊ0���õ�ǰ��������16λ��ɫֵ��ʾ�� 						
				}
				temp=temp<<1; 		  				//�ֽڸ�λ���Ƴ�
				x++;
				if(x>=X){x=0; y++;}				    //�������ص���Ϊ��ǰ��x��y��Ϊ��ǰ���ض�������ɫ��׼��
				i++;	
			}				  
							
		}
	}
	else if(Dir==1){
		LCD_WR_CMD(0x0003,0x1018);   	  		//ͼ����ʾ����Ϊ������  �е���  �еݼ�  AM=1  I/D[1:0]=01	A
		LCD_WR_CMD(0x0210, StartY); 	  		//ˮƽ��ʾ����ʼ��ַ 0-239
  		LCD_WR_CMD(0x0211, StartY+Y-1);    		//ˮƽ��ʾ��������ַ 0-239
  		LCD_WR_CMD(0x0212, 399-(StartX+X-1));   //��ֱ��ʾ����ʼ��ַ 0-399
  		LCD_WR_CMD(0x0213, 399-StartX);    		//��ֱ��ʾ��������ַ 0-399
		LCD_WR_CMD(0x0200, StartY);		  	  	//ˮƽ��ʾ����ַ
  		LCD_WR_CMD(0x0201, 399-StartX);	 	  	//��ֱ��ʾ����ַ	
		LCD_WR_REG(0x0202);                   	//׼��д������ʾ��

		Size=X*Y;						  		//�ַ������ַ�ռ�õ����سߴ�
		while(i<Size){					  
			temp=*chr++;				  		//һ���ֽڴ���8�����أ���˼�1������������8������
			for(num=0; num<8; num++){	  		//�����ÿ���ֽڴ�����8������	    
				if((temp&0x80)>0){		  		//���ֽڵĸ�λ�����жϣ�Ϊ1���ô��������16λ��ɫֵ��ʾ��д�뵽����λ�á�						 
				
					LCD_WR_Data(Color); 		
				}
				else{
					LCD_WR_CMD(0x0200, StartY+y);		//ˮƽ��ʾ����ַ
  					LCD_WR_CMD(0x0201, 399-(StartX+x));	//��ֱ��ʾ����ַ								
					LCD_WR_REG(0x0202);					//׼����������ʾ��
					R_dis_mem=LCD_RD_data();		  	//��ȡ����ɫ��Ϊ���Ӳ���͸��Ч����׼��	
					LCD_WR_Data(R_dis_mem); 		//���ֽڵĸ�λ�����жϣ�Ϊ0���õ�ǰ��������16λ��ɫֵ��ʾ�� 						
				}
				temp=temp<<1; 		  				//�ֽڸ�λ���Ƴ�        
				x++;
				if(x>=X){x=0; y++;}				    //�������ص���Ϊ��ǰ��x��y��Ϊ��ǰ���ض�������ɫ��׼��
				i++;
			}							
		}
	}
}


//++++++++������ֵ��ȡ��λ�������ַ������׵�ַ+++++++++++++++++++++++++++++++++++++++++++
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


//�ܶ�����ʾ����
void LCD_test(void)
{

	unsigned char *p;


// 	lcd_DrawPicture(0,0,0,a1);	    	//��ĳһָ��λ����ʾͼƬ�� ͼƬ�ĳߴ�Ҫ��240X400��Χ�ڡ�  
// 	Delay(0x1afffff);
// 	lcd_DrawPicture(0,0,1,a2);	    	//��ĳһָ��λ����ʾͼƬ�� ͼƬ�ĳߴ�Ҫ��400X240��Χ�ڡ�      

// 	Delay(0x1afffff);

  lcd_wr_zf(0,0,280,32,color1,1,&zf3[0]);  		//��ʾ�ַ���
	
	//��ʾ5λ�����֣� ��ֵ��COLOR1ֵ���ڱ仯
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


	//��ɫ����
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
//-----------------�޸��ַ���ʾ����-----------------//��ֲ���ܶ�������-------------------
/****************************************************************************
* ��    �ƣ�void lcd_SetCursor(u16 x,u16 y)
* ��    �ܣ�������Ļ����
* ��ڲ�����x      ������
*           y      ������
* ���ڲ�������
* ˵    ����
* ���÷�����lcd_SetCursor(10,10);
****************************************************************************/
void lcd_SetCursor(u16 x,u16 y)
{	
  *(__IO uint16_t *) (Bank1_LCD_C)= 0x200;
  *(__IO uint16_t *) (Bank1_LCD_D)= y;

  *(__IO uint16_t *) (Bank1_LCD_C)= 0x201;
  *(__IO uint16_t *) (Bank1_LCD_D)= 399-x;
}

/****************************************************************************
* ��    �ƣ�void lcd_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)  ����
* ��    �ܣ����ô�������
* ��ڲ�����StartX     ����ʼ����
*           StartY     ����ʼ����
*           EndX       �н�������
*           EndY       �н�������
* ���ڲ�������
* ˵    ����
* ���÷�����lcd_SetWindows(0,0,100,100)��
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
��������Lcd��ѡ����
���ܣ�ѡ��Lcd��ָ���ľ�������

ע�⣺xStart�� yStart������Ļ����ת���ı䣬λ���Ǿ��ο���ĸ���

��ڲ�����xStart x�������ʼ��
          ySrart y�������ֹ��
          W Ҫѡ�����ε�x���򳤶�
          H  Ҫѡ�����ε�y���򳤶�
����ֵ����
***********************************************/
void Lcd_SetBox(u16 xStart,u16 yStart,u16 W,u16 H)  //??��֪�Է�
{
	
	
// 	LCD_WriteReg(0x01,xStart);                //ˮƽ��ʼλ�� 
// 	LCD_WriteReg(0x02,yStart);                //��ֱ��ʼλ��
// 	LCD_WriteReg(0x03,W);                     //x���򳤶�
// 	LCD_WriteReg(0x04,H);                     //y���򳤶�
	  	
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
* ��    �ƣ�u16 lcd_GetPoint(u16 x,u16 y)
* ��    �ܣ���ȡָ���������ɫֵ
* ��ڲ�����x      ������
*           y      ������
* ���ڲ�������ǰ������ɫֵ
* ˵    ����
* ���÷�����i=lcd_GetPoint(10,10);
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
* ��    �ƣ�void lcd_SetPoint(u16 x,u16 y,u16 point)
* ��    �ܣ���ָ�����껭��
* ��ڲ�����x      ������
*           y      ������
*           point  �����ɫ
* ���ڲ�������
* ˵    ����
* ���÷�����lcd_SetPoint(10,10,0x0fe0);
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
* ��    �ƣ�void lcd_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
* ��    �ܣ���ָ��������ʾһ��8x16�����ascii�ַ�
* ��ڲ�����x          ������
*           y          ������
*           charColor  �ַ�����ɫ
*           bkColor    �ַ�������ɫ
* ���ڲ�������
* ˵    ������ʾ��Χ�޶�Ϊ����ʾ��ascii��
* ���÷�����lcd_PutChar(10,10,'a',0x0000,0xffff);
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
          lcd_SetPoint(x+j,y+i,charColor); // �ַ���ɫ
        }
        else
        {
          lcd_SetPoint(x+j,y+i,bkColor); // ������ɫ
        }
    }
  }
}


/****************************************************************
 * ��������LCD_Str_O
 * ����  ����ָ�����괦��ʾ8*16��С���ַ���
 * ����  : -x   ��ʾλ�ú������� 0 ~ 319
           -y   ��ʾλ���������� 0 ~ 239
 * 		   -str �ַ�����ַ
 * 		   -Color�ַ���ɫ
 * ���  ����
 * ����  ��	LCD_Str_O(0, 0, "Runing", 0);
		    LCD_Str_O(320-8*6, 239-16, "Runing",0); 
 * ע��  ��	�ַ�����ʾ����Ϊ���� �Ѳ���					
******************************************************************/    
void LCD_Str_O(u16 x, u16 y,const u8 *str,u16 Color,u16 bkColor)
{

//    	Set_direction(0);     
    while(*str != '\0')
    {       
        if(x > (320-8)) 
        {	
					//����
           x = 0;
           y += 16;
        }
        if(y > (240-16)) 
        {	
					//һ��
           x = y = 0;
        }
        
				lcd_PutChar(x,y,*str,Color,bkColor);
        x += 8;
        str ++ ;
    }
		
}




/********************************************************************
 * ��������LCD_Char_CH
 * ����  ����ʾ���������ַ�
 * ����  : 	x: 0~(319-16)
 *         	y: 0~(239-16)
 *			str: �����ַ�����ַ
 *			Color: �ַ���ɫ   
 *			bkColor: ������ɫ
 * ���  ����
 * ����  ��	LCD_Char_CH(200,100,"��",0,0);
 * ע��	 ������������1�ĺ����ַ�������ʾ����ضϣ�ֻ��ʾ��ǰ��һ������
************************************************************************/    
void LCD_Char_CH(u16 x,u16 y,const u8 *str,u16 Color,u16 bkColor)
{
    
#ifndef NO_CHNISEST_DISPLAY         /*���������ʾ����û�йر�*/
    u8 i,j;
    u8 buffer[32];
    u16 tmp_char=0;
    
    
   GetGBKCode_from_sd(buffer,str);  /* ȡ��ģ���� */
    
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
 * ��������LCD_Str_CH
 * ����  ����ָ�����괦��ʾ16*16��С��ָ����ɫ�����ַ���
 * ����  : 	- x: ��ʾλ�ú�������	 
 *         	- y: ��ʾλ���������� 
 *			- str: ��ʾ�������ַ���
 *			- Color: �ַ���ɫ   
 *			- bkColor: ������ɫ
 * ���  ����
 * ����  ��	LCD_Str_CH(0,0,"��Ī��̳Ұ��ר��",0,0xffff);
		    LCD_Str_CH(50,100,"��Ī��̳Ұ��ר��",0,0xffff);
		    LCD_Str_CH(320-16*8,240-16,"��Ī��̳Ұ��ר��",0,0xffff); 		
 * ע��  ��	�ַ�����ʾ����Ϊ���� �Ѳ���
******************************************************************************/    
void LCD_Str_CH(u16 x,u16 y,const u8 *str,u16 Color,u16 bkColor)  
{
   
 //   Set_direction(0);
    while(*str != '\0')
    {
			if(x>(320-16))
			{	 
				 /*����*/
				x =0;
				y +=16;
					
			}
			if(y >(240-16))
			{
				 /*���¹���*/
				 y =0;
				 x =0;
			}
	     LCD_Char_CH(x,y,str,Color,bkColor);      
       str += 2 ;
       x += 16 ;	
    }
}


/********************************************************************************
 * ��������	LCD_Char_CH_O
 * ����  �� ������ʾ���������ַ�
 * ����  : 	x: 0~(319-16)
 *         	y: 0~(239-16)
 *			str: �����ַ�����ַ
 *			Color: �ַ���ɫ   
 * ���  ����
 * ����  ��	LCD_Char_CH_O(200,100,"��",0,0);
 * ע��	 ��1 ����2 ����������1�ĺ����ַ�������ʾ����ضϣ�ֻ��ʾ��ǰ��һ������
 ********************************************************************************/    
void LCD_Char_CH_O(u16 x,u16 y,const u8 *str,u16 Color) 
{
    u8 i,j;
    u8 buffer[32];
    u16 tmp_char=0;
 //   Set_direction(0);   
    GetGBKCode_from_sd(buffer,str); /* ȡ��ģ���� */
    
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
* ��    �ƣ�u16 lcd_BGR2RGB(u16 c)
* ��    �ܣ�RRRRRGGGGGGBBBBB ��Ϊ BBBBBGGGGGGRRRRR ��ʽ
* ��ڲ�����c      BRG ��ɫֵ
* ���ڲ�����RGB ��ɫֵ
* ˵    �����ڲ���������
* ���÷�����
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
* ��    �ƣ�u16 lcd_ReadData(void)
* ��    �ܣ���ȡ����������
* ��ڲ�������
* ���ڲ��������ض�ȡ��������
* ˵    �����ڲ�����
* ���÷�����i=lcd_ReadData();
****************************************************************************/
u16 lcd_ReadData(void)
{
  u16 val=0;
  val=LCD_RD_data();
  return val;
}





