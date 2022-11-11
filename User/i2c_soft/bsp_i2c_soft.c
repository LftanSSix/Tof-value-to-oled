/*
	应用说明：
	在访问I2C设备前，请先调用 i2c_CheckDevice() 检测I2C设备是否正常

*/
#include "./i2c_soft/bsp_i2c_soft.h"
#include "stm32f4xx.h"
#include "OLED_I2C.h"
#include "./systick/bsp_SysTick.h"
#include "./tof/sys.h"
#include "./usart/bsp_debug_usart.h"
#include "./led/bsp_led.h"

/*
*********************************************************************************************************
*	函 数 名: i2c_Delay
*	功能说明: I2C总线位延迟，最快400KHz
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
//static void i2c_Delay(void)
//{

//	
//	
//}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线启动信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Start(void)
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
	TOF_I2C_SDA_1();
	TOF_I2C_SCL_1();
	Delay_us(4);
	TOF_I2C_SDA_0();
	Delay_us(4);
	TOF_I2C_SCL_0();
	Delay_us(4);
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线停止信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Stop(void)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	TOF_I2C_SDA_0();
	TOF_I2C_SCL_1();
	Delay_us(4);
	TOF_I2C_SDA_1();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_SendByte
*	功能说明: CPU向I2C总线设备发送8bit数据
*	形    参：_ucByte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			TOF_I2C_SDA_1();
		}
		else
		{
			TOF_I2C_SDA_0();
		}
	    Delay_us(2);
		TOF_I2C_SCL_1();
	    Delay_us(2);
		TOF_I2C_SCL_0();
		if (i == 7)
		{
			 TOF_I2C_SDA_1(); // 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
	    Delay_us(2);
	}
}

/*
*********************************************************************************************************
*	函 数 名: i2c_ReadByte
*	功能说明: CPU从I2C总线设备读取8bit数据
*	形    参：无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* 读到第1个bit为数据的bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		TOF_I2C_SCL_1();
	    Delay_us(2);
		if (TOF_I2C_SDA_READ())
		{
			value++;
		}
		TOF_I2C_SCL_0();
	    Delay_us(2);  //???
	}
	return value;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_WaitAck
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参：无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
 uint8_t i2c_WaitAck(void)
{
	uint8_t re;

	TOF_I2C_SDA_1();	/* CPU释放SDA总线 */
	Delay_us(1);
	TOF_I2C_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	Delay_us(1);
	if (TOF_I2C_SDA_READ())	/* CPU读取SDA口线状态 */
	{
		re = 1;
	}
	else
	{
		re = 0;  //0应答信号
	}
	TOF_I2C_SCL_0();
	Delay_us(1);
	return re;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Ack(void)
{
	TOF_I2C_SDA_0();	/* CPU驱动SDA = 0 */
	Delay_us(2);
	TOF_I2C_SCL_1();	/* CPU产生1个时钟 */
	Delay_us(2);
	TOF_I2C_SCL_0();
	Delay_us(2);
	TOF_I2C_SDA_1();	/* CPU释放SDA总线 */
}

/*
*********************************************************************************************************
*	函 数 名: i2c_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_NAck(void)
{
	TOF_I2C_SDA_1();	/* CPU驱动SDA = 1 */
	Delay_us(2);
	TOF_I2C_SCL_1();	/* CPU产生1个时钟 */
	Delay_us(2);
	TOF_I2C_SCL_0();
	Delay_us(2);
}

/*
*********************************************************************************************************
*	函 数 名: i2c_CfgGpio
*	功能说明: 配置I2C总线的GPIO，采用模拟IO的方式实现
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_CfgGpio(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(TOF_RCC_I2C_PORT, ENABLE);	/* 打开GPIO时钟 */

	GPIO_InitStructure.GPIO_Pin = TOF_I2C_SCL_PIN | TOF_I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(TOF_GPIO_PORT_I2C, &GPIO_InitStructure);

	/* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
	i2c_Stop();
}




/*
   参数：reg 寄存器地址
         data 写入寄存器地址数据
*/
void WriteByte(uint16_t reg,uint8_t data)
{
   uint8_t data_write[3];
   data_write[0]=(reg>>8)&0xff;
   data_write[1]=reg&0xff;
   data_write[2]=data&0xff;
   Data_write(TOF_WRITE_ADDRESS,data_write,3);
}

/*
   参数：reg要读取寄存器地址

*/
  uint8_t ReadByte(uint16_t reg)
{
    uint8_t data_write[2];
	uint8_t receive_data=0;
	data_write[0]=(reg>>8)&0xff;
    data_write[1]=reg&0xff;
	Data_write(TOF_WRITE_ADDRESS,data_write,2);
	receive_data=Data_read(TOF_READ_ADDRESS);
	return receive_data;
}

/*
   参数：addrdata写地址 52
         *data要发送的数据
		 count发送的字节个数
*/
 void Data_write(uint8_t addrdata,uint8_t *data,uint8_t count)
{
	uint8_t i=0;
	i2c_Start();
	i2c_SendByte(addrdata);
	i2c_WaitAck(); 
    for(i=0;i<count;i++)
    {			
	  i2c_SendByte(*(data+i));	
      i2c_WaitAck(); 	
    }
    i2c_Stop();	
}

/*
   参数：addrdata读地址53
         返回的是字节数据
*/
uint8_t Data_read(uint8_t addrdata)
{
	uint8_t data=0;
	i2c_Start();
	i2c_SendByte(addrdata);//自己理解应该是0x53
	i2c_WaitAck(); 
	data=i2c_ReadByte(); //???
	i2c_NAck();	
	i2c_Stop();	
    return data;
}
 /**
  * @brief  TOF_Init，初始化TOF
  * @param  无
  * @retval 无
  */


uint8_t TOF_Init(void)
{
	uint8_t reset;
//	Delay_ms(200);	
	reset = ReadByte(0x016);
	
	if (reset==1){ // 检查是否初始化完成

		// 必须设置-专用寄存器 : private registers
	WriteByte(0x0207, 0x01);
	WriteByte(0x0208, 0x01);
	WriteByte(0x0096, 0x00);
	WriteByte(0x0097, 0xfd);
	WriteByte(0x00e3, 0x01);
	WriteByte(0x00e4, 0x03);
	WriteByte(0x00e5, 0x02);
	WriteByte(0x00e6, 0x01);
	WriteByte(0x00e7, 0x03);
	WriteByte(0x00f5, 0x02);
	WriteByte(0x00d9, 0x05);
	WriteByte(0x00db, 0xce);
	WriteByte(0x00dc, 0x03);
	WriteByte(0x00dd, 0xf8);
	WriteByte(0x009f, 0x00);
	WriteByte(0x00a3, 0x3c);
	WriteByte(0x00b7, 0x00);
	WriteByte(0x00bb, 0x3c);
	WriteByte(0x00b2, 0x09);
	WriteByte(0x00ca, 0x09);
	WriteByte(0x0198, 0x01);
	WriteByte(0x01b0, 0x17);
	WriteByte(0x01ad, 0x00);
	WriteByte(0x00ff, 0x05);
	WriteByte(0x0100, 0x05);
	WriteByte(0x0199, 0x05);
	WriteByte(0x01a6, 0x1b);
	WriteByte(0x01ac, 0x3e);
	WriteByte(0x01a7, 0x1f);
	WriteByte(0x0030, 0x00);

	//建议设置-通用寄存器: Public registers 
	WriteByte(0x0011, 0x10); // Enables polling for ‘New Sample ready’ 
	 // when measurement completes
	WriteByte(0x010a, 0x30); // Set the averaging sample period
	 // (compromise between lower noise and 
	 // increased execution time)
	WriteByte(0x003f, 0x46); // Sets the light and dark gain (uppernibble). Dark gain should not be changed.

	WriteByte(0x0031, 0xFF); // sets the # of range measurements after 
	 // which auto calibration of system is performed 
	WriteByte(0x0041, 0x63); // Set ALS integration time to 100ms

	WriteByte(0x002e, 0x01); // perform a single temperature calibration of the ranging sensor 
	
	//选择设置-通用寄存器: Public registers
	WriteByte(0x001b, 0x09); // Set default ranging inter-measurement 
	 // period to 100ms
	WriteByte(0x003e, 0x31); // Set default ALS inter-measurement period 
	 // to 500ms
	WriteByte(0x0014, 0x24); // Configures interrupt on ‘New Sample 
	 // Ready threshold event’ 
		 
	WriteByte(0x016, 0x00); //标志位清零1to0 change fresh out of set status to 0
	
	printf("tof初始化成功\r\n");

	LED_RED;
	
	return 1;

}else
    return 0;
}

/*
    开始测量模式选择
    选择单次测量模式
*/
 uint8_t VL6180X_Start_Range()
  {
     WriteByte(0x018,0x01);      //01  03
	 return 0;
  }
  
uint16_t timeoutcnt=0; 
/*poll for new sample ready */
uint8_t VL6180X_Poll_Range()
{
    uint8_t status;
	  uint8_t range_status;
	  status=ReadByte(0x04f);
    range_status=status&0x07;
		while(range_status!=0x04)
		{
			  timeoutcnt++;
				if(timeoutcnt > 2)
				{
					break;
				}
				status=ReadByte(0x04f);
				range_status=status&0x07;
				Delay_ms(1);
			
		}		  
		return 0;
}
   
  
  /*read range result (mm)*/
 uint8_t VL6180_Read_Range()
  {
    int range;
	range=ReadByte(0x062);
	return range;
  }
  
  /*clear interrupt*/
  void VL6180X_Clear_Interrupt()
  {
     WriteByte(0x015,0x07); 
  }
  
  
  
  
  
  
  
  
  
  

 
