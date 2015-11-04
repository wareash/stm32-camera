
#include "BSP_TEST.h"

int res;  
int a;

FIL fsrc,fdst;
FATFS fs;
UINT br, bw;            // File R/W count
BYTE buffer[512];       // file copy buffer
BYTE textFileBuffer[] = "��л��ѡ�� Ұ��STM32������ ��^_^ \r\n";

void test_config(void)
{
		USART1_Config();
	
		NVIC_Configuration();

}


void SD_test(void)
{

	/* Interrupt Config */

 	printf("\r\n ����һ��MicroSD���ļ�ϵͳʵ��(FATFS R0.09)\n");

	printf ( "\r\n disk_initialize starting......\n " );

	f_mount(0,&fs);		   

	res = f_open(&fdst,"0:/Demo.TXT",FA_CREATE_NEW | FA_WRITE);
	 
	if ( res == FR_OK )
 	 { 
    /* ��������������д���ļ��� */
		res = f_write(&fdst, textFileBuffer, sizeof(textFileBuffer), &bw); 
	  printf( "\r\n �ļ������ɹ� \n" );    
      /*�ر��ļ� */
   		 f_close(&fdst);      
  	}
   else if ( res == FR_EXIST )
 	 {
//��д��������	 ��������
//		res = f_lseek(&fdst, file.fsize);
//		res = f_write(&fdst, textFileBuffer, sizeof(textFileBuffer), &bw); 
		printf( "\r\n �ļ��Ѿ����� \n" );
 	 }

   /*---------------- ���ո��½����ļ���������ݴ�ӡ�������ն� -----------------------*/
  /* ��ֻ���ķ�ʽ�򿪸ոմ������ļ� */
	res = f_open(&fdst, "0:/Demo.TXT", FA_OPEN_EXISTING | FA_READ); /* ���ļ� */		
	br = 1;
	a = 0;	
  for (;;) 
	{
		for ( a=0; a<512; a++ ) 	                          /* �建���� */
			buffer[a]=0;
			 
  	res = f_read( &fdst, buffer, sizeof(buffer), &br ); /* ���ļ���������ݶ��������� */
		printf("\r\n %s ", buffer);						
  	if (res || br == 0) break;                          /* ������ߵ����ļ�β */        	    	
  }
	f_close(&fdst);	                                      /* �رմ򿪵��ļ� */	

}

