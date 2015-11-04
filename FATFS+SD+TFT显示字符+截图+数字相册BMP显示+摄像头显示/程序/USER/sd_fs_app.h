#ifndef __SD_FS_PP_H
#define	__SD_FS_PP_H

#include "sdio_sdcard.h"
#include "ff.h"
#include "diskio.h"


#define ltk_scanf(format...)    scanf(##format)

void Sd_fs_test(void);
void sd_fs_init(void);
void SDIO_NVIC_Configuration(void);
int Sdfs_new(BYTE *new_file_name, BYTE *write_buffer, BYTE buffer_size);
int Sdfs_write(BYTE *write_file_name, BYTE *write_buffer, BYTE buffer_size);
int Sdfs_read(BYTE *read_file_name, BYTE *save_buffer);
int GetGBKCode_from_sd(unsigned char* pBuffer,const unsigned char * c);
//ɾ��һ���ļ�����Ŀ¼
void ltk_delete_file_or_folder(void);
//�г���Ŀ¼�µ�����Ŀ¼���ļ�
void ltk_list_file(void);


#endif /* __SD_FS_PP_H */
