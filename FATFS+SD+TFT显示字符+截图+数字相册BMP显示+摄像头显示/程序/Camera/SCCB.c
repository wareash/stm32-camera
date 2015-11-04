

/* Includes ------------------------------------------------------------------*/
#include "SCCB.h"

int I2C_Start(void);
/*******************************************************************************
* Function Name  : I2C_Configuration
* Description    : SCL  SDA管脚配置
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void I2C_Configuration(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
  /* Configure I2C pins: PE2->SCL and PE3->SDA */
  
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP ;//  
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP ;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}
/*******************************************************************************
* Function Name  : I2C_IN
* Description    : SDA的输入模式
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
 void I2C_IN(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;     
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
  GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_3;
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}
/*******************************************************************************
* Function Name  : I2C_IN
* Description    : SDA的输出模式
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void I2C_OUT(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
  GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP ;//  
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}
/*******************************************************************************
* Function Name  : I2C_delay
* Description    : 延迟时间
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void I2C_delay(void)
{	
   unsigned short i = 200; /* 这里可以优化速度 */
   while(i) 
   { 
     i--; 
   } 
}


/*******************************************************************************
* Function Name  : I2C_Start
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static int I2C_Start(void)
{
	SDA_H;
	I2C_delay();
	SCL_H; 
	I2C_delay();
	SDA_L;
	I2C_delay(); 
    SCL_L;
	I2C_delay(); 
	return ENABLE;
}

/*******************************************************************************
* Function Name  : I2C_Stop
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void I2C_Stop(void)
{
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_H;
	I2C_delay();
}

/*******************************************************************************
* Function Name  : I2C_Ack
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void I2C_Ack(void)
{	
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}

/*******************************************************************************
* Function Name  : I2C_NoAck
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void I2C_NoAck(void)
{	
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
}

/*******************************************************************************
* Function Name  : I2C_WaitAck
* Description    : None
* Input          : None
* Output         : None
* Return         : 返回为:=1有ACK,=0无ACK
* Attention		 : None
*******************************************************************************/
static int I2C_WaitAck(void) 	
{
	I2C_IN();
	I2C_delay();
	SCL_H;
	I2C_delay();
	if(SDA_read)
	{
      SCL_L;
      return DISABLE;
	}
	SCL_L;
	I2C_delay();
	I2C_OUT();
	I2C_delay();
	return ENABLE;
}

 /*******************************************************************************
* Function Name  : I2C_SendByte
* Description    : 数据从高位到低位
* Input          : - SendByte: 发送的数据
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void I2C_SendByte(uint8_t SendByte) 
{
    uint8_t i=8;
    while(i--)
    {
      //SCL_L;
      //I2C_delay();
      if(SendByte&0x80)	SDA_H;  
      else SDA_L;   
      SendByte<<=1;
      I2C_delay();
	  SCL_H;
      I2C_delay();
	  SCL_L;
	  I2C_delay();
    }
}


/*******************************************************************************
* Function Name  : I2C_ReceiveByte
* Description    : 数据从高位到低位
* Input          : None
* Output         : None
* Return         : I2C总线返回的数据
* Attention		 : None
*******************************************************************************/
static int I2C_ReceiveByte(void)  
{ 
    uint8_t i=8;
    uint8_t ReceiveByte=0;

    SDA_H;				
    while(i--)
    {
      ReceiveByte<<=1;      
      SCL_L;
      I2C_delay();
	  SCL_H;
	  I2C_IN();
      I2C_delay();	
	  if(SDA_read)
      {
        ReceiveByte|=0x01;
      } 
    }
    SCL_L;
	I2C_OUT();
    return ReceiveByte;
}

/*******************************************************************************
* Function Name  : I2C_WriteByte
* Description    : 写一字节数据
* Input          : - WriteAddress: 待写入地址
*           	   - SendByte: 待写入数据
*                  - DeviceAddress: 器件类型
* Output         : None
* Return         : 返回为:=1成功写入,=0失败
* Attention		 : None
*******************************************************************************/           
int I2C_WriteByte( uint16_t WriteAddress , uint8_t SendByte , uint8_t DeviceAddress)
{		
    if(!I2C_Start())
	{
	    return DISABLE;
	}
	I2C_delay();
    I2C_SendByte( DeviceAddress );                    /* 器件地址 */
    if( !I2C_WaitAck() )
	{
		I2C_Stop(); 
		return DISABLE;
	}
	I2C_delay();
    I2C_SendByte((uint8_t)(WriteAddress & 0x00FF));   /* 设置低起始地址 */      
    I2C_WaitAck();
	I2C_delay();	
    I2C_SendByte(SendByte);
    I2C_WaitAck();
	I2C_delay();   
    I2C_Stop(); 
	I2C_delay();
    return ENABLE;
}									 

/*******************************************************************************
* Function Name  : I2C_ReadByte
* Description    : 读取一串数据
* Input          : - pBuffer: 存放读出数据
*           	   - length: 待读出长度
*                  - ReadAddress: 待读出地址
*                  - DeviceAddress: 器件类型
* Output         : None
* Return         : 返回为:=1成功读入,=0失败
* Attention		 : None
*******************************************************************************/          
int I2C_ReadByte(uint8_t* pBuffer,   uint16_t length,   uint8_t ReadAddress,  uint8_t DeviceAddress)
{	
    if(!I2C_Start())
	{
	    return DISABLE;
	}
    I2C_SendByte( DeviceAddress );         /* 器件地址 */
    if( !I2C_WaitAck() )
	{
		I2C_Stop(); 
		return DISABLE;
	}
    I2C_SendByte( ReadAddress );           /* 设置低起始地址 */      
    I2C_WaitAck();	
    I2C_Stop(); 
	
    if(!I2C_Start())
	{
		return DISABLE;
	}
    I2C_SendByte( DeviceAddress + 1 );               /* 器件地址 */ 
    if(!I2C_WaitAck())
	{
		I2C_Stop(); 
		return DISABLE;
	}
    while(length)
    {
      *pBuffer = I2C_ReceiveByte();
      if(length == 1)
	  {
		  I2C_NoAck();
	  }
      else
	  {
		I2C_Ack(); 
	  }
      pBuffer++;
      length--;
    }
    I2C_Stop();
    return ENABLE;
}
/******************* (C) COPYRIGHT 2013 奋斗STM32 *****END OF FILE****/
