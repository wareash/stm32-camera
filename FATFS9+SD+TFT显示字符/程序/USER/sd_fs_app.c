/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * �ļ���  ��sd_fs_app.c
 * ����    ��MicroSD���ļ�ϵͳӦ�ú����⡣
 *              
 * ʵ��ƽ̨��Ұ��STM32������
 * Ӳ�����ӣ���
 *
 * ��汾  ��ST3.5.0
 *
 * ����    ��wildfire team 
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
**********************************************************************************/
#include "sd_fs_app.h"
#include "lcd_botton.h"
#include "lcd.h"

#include <stdio.h>

FATFS myfs[2];                 // Work area (file system object) for logical drive
FIL myfsrc, myfdst;            // file objects
FRESULT myres;                 // FatFs function common result code
BYTE mybuffer[512];            // file copy buffer
BYTE my_latest_buffer[512];
uint8_t mystring[512]="������ͤǿ���ʺ�---------SD��TXT�ļ�����ʵ��---->OK \n";
UINT mybr, mybw;               // File R/W count
int mya = 0;
char mypath[512]="0:";         // һ��Ҫ��ʼ��Ϊ0:

/*
 * ��������NVIC_Configuration
 * ����  ��SDIO �ж�ͨ������
 * ����  ����
 * ���  ����
 */
void SDIO_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    
    NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


void sd_fs_init(void)
{
    /* SD���жϳ�ʼ�� */
		SDIO_NVIC_Configuration();
		
		/* SD ��Ӳ����ʼ������ʼ���̷�Ϊ0 */	 
   f_mount(0,&myfs[0]);	       
}

void Sd_fs_test(void)
{
    int count=0;
	
		printf("\r\n ����һ��MicroSD���ļ�ϵͳ����ʵ��(FATFS R0.09)\n");
	
    SDIO_NVIC_Configuration();
    disk_initialize( 0 );	     /* SD ��Ӳ����ʼ�� */

    f_mount(0, &myfs[0]);
    myres = f_open( &myfsrc , "0:/my__Demo.TXT" , FA_CREATE_NEW | FA_WRITE);
	
		printf("\r\n my__Demo.TXT  �����ɹ�\n");
	
    if ( myres == FR_OK )
    { 
        /* Write buffer to file */
        myres = f_write(&myfsrc, mystring, sizeof(mystring), &mybr);    
        f_close(&myfsrc);      
    }    
    else if ( myres == FR_EXIST )  //����ļ��Ѿ�����
    {	
    }    
    
    myres = f_open(&myfsrc, "0:/my__Demo.TXT", FA_OPEN_EXISTING | FA_READ); /* ���ļ� */	  //ok    
				
		printf("\r\n my__Demo.TXT �Ѵ��ڣ��򿪳ɹ�\n");

    mybr = 1;
    mya = 0; 
		
		printf("��ȡ������TXT�ļ� ----\n");		
		
    for (;;) 
    {        
        for ( mya=0; mya<512; mya++ ) 	/* �建���� */
            mybuffer[mya]=0;
        
     	  myres = f_read( &myfsrc, mybuffer, sizeof(mybuffer), &mybr ); /* ���ļ���������ݶ��������� */
//         sprintf( (char*)&my_latest_buffer[count*512], "%s" , mybuffer );	  //��ӡ��������  
						printf("\r\n %s \n", mybuffer);											
						count++;							
        if (myres || mybr == 0) break;   // error or eof        	    	
    }   
		
		printf("\r\n %d ", count);
		count = 0;		
		
    f_close(&myfsrc);	 /* �رմ򿪵��ļ� */
    
//   for (;;) 
// 	{
// 		for ( a=0; a<512; a++ ) 	                          /* �建���� */
// 			buffer[a]=0;
// 			 
//   	res = f_read( &fdst, buffer, sizeof(buffer), &br ); /* ���ļ���������ݶ��������� */
// 		printf("\r\n %s ", buffer);						
//   	if (res || br == 0) break;                          /* ������ߵ����ļ�β */        	    	
//   }
// 	f_close(&fdst);	 		
		
//    Set_direction(0);		    
}


/*******************************************************************************
* Function Name  : Sdfs_new
* Description    : �˽�һ���ļ���д������  
* Input          : new_file_name--�˽��ļ�������  
*				   write_buffer--д���ļ������ݻ�������ַ  
*				   buffer_size--��������С
* Output         : None
* Return         : 0(success)  1(file existed )  -1(fail)
* Attention		 : None
*******************************************************************************/           
int Sdfs_new(BYTE *new_file_name, BYTE *write_buffer, BYTE buffer_size)
{
    BYTE name_buffer[50];
    sprintf((char*)name_buffer,"0:%s",new_file_name);
    
    f_mount(0, &myfs[0]);
    myres = f_open( &myfsrc , (char*)name_buffer , FA_CREATE_NEW | FA_WRITE);
    
    if ( myres == FR_OK ) 
    { 
        
        myres = f_write(&myfsrc, write_buffer,buffer_size, &mybr); //д������   
        f_close(&myfsrc);
        
        return 0;      
    }
    
    else if ( myres == FR_EXIST )  //����ļ��Ѿ�����
    {
        return FR_EXIST;	 
    }
    
    else
    {
        return -1;
    }
    
}


/*******************************************************************************
* Function Name  : Sdfs_write
* Description    : ���ļ�д������  
* Input          : new_file_name--���������ļ�������  
*				   write_buffer--д���ļ������ݻ�������ַ  
*				   buffer_size--��������С
* Output         : None
* Return         : 0(success)   -1(fail)
* Attention		 : None
*******************************************************************************/            
int Sdfs_write(BYTE *write_file_name, BYTE *write_buffer, BYTE buffer_size)
{
    BYTE name_buffer[50];
    sprintf((char*)name_buffer,"0:%s",write_file_name);
    
    f_mount(0, &myfs[0]);
    myres = f_open( &myfsrc , (char*)name_buffer ,FA_WRITE);
    
    if ( myres == FR_OK )  
    { 
        /* Write buffer to file */	
        myres = f_write(&myfsrc, write_buffer,buffer_size, &mybr); //д������   
        f_close(&myfsrc);
        
        return 0;      
    }
    else 
	if(myres == FR_NO_FILE)	 //���û�и��ļ�
	{
        return FR_NO_FILE;
    } 
    else	
	return -1;
}



/*******************************************************************************
* Function Name  : Sdfs_read
* Description    : ��һ���ļ��������ݵ������� 
* Input          : read_file_name--�ļ�������  
*				   				save_buffer--������Ҫ����ĵ�ַ 
* Output         : None
* Return         : 0(success)  -1(fail)
* Attention		 : None
*******************************************************************************/ 
int Sdfs_read(BYTE *read_file_name, BYTE *save_buffer)
{
    
    int count=0;
    BYTE name_buffer[50];
    sprintf((char*)name_buffer,"0:%s",read_file_name);
    sd_fs_init();
    f_mount(0, &myfs[0]);
    myres = f_open(&myfsrc , (char*)name_buffer , FA_OPEN_EXISTING | FA_READ);
    
    if ( myres == FR_OK ) 
    { 
        for (;;) 
        {
            
            for ( mya=0; mya<512; mya++ ) 	/* �建���� */
                mybuffer[mya]=0;
            
            myres = f_read( &myfsrc, mybuffer, sizeof(mybuffer), &mybr ); /* ���ļ������������512�ֽ�Ϊ��λ�������ػ����� */
            sprintf((char*)&save_buffer[count*512],"%s",mybuffer);					//��ӡ���û�ָ���Ļ�����������
            
            count++;							
            if (myres || mybr == 0) break;   // error or eof        	    	
        }  	
        
        return 0;
    }
    
    else
        return -1;
    
    
}




/*******************************************************************************
* Function Name  : GetGBKCode_from_sd
* Description    : ��SD���ֿ��ж�ȡ�������ݵ�ָ���Ļ����� 
* Input          : pBuffer---���ݱ����ַ  
*				   					c--�����ַ����ֽ��� 
* Output         : None
* Return         : 0(success)  -1(fail)
* Attention		 	 : None
*******************************************************************************/ 
int GetGBKCode_from_sd(unsigned char* pBuffer,const unsigned char * c)
{ 
    unsigned char High8bit,Low8bit;
    unsigned int pos;
    High8bit=*c;     /* ȡ��8λ���� */
    Low8bit=*(c+1);  /* ȡ��8λ���� */
    
    pos = ((High8bit-0xb0)*94+Low8bit-0xa1)*2*16 ;	
    f_mount(0, &myfs[0]);
    myres = f_open(&myfsrc , "0:/HZLIB.bin", FA_OPEN_EXISTING | FA_READ);
    
    if ( myres == FR_OK ) 
    {
        f_lseek (&myfsrc, pos);														 //ָ��ƫ��
        myres = f_read( &myfsrc, pBuffer, 32, &mybr );		 //16*16��С�ĺ��� ����ģ ռ��16*2���ֽ�
        
        f_close(&myfsrc);
        
        return 0;  
    }
    
    else
        return -1;    
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/


