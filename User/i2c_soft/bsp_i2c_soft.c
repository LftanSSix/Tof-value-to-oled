/*
	Ӧ��˵����
	�ڷ���I2C�豸ǰ�����ȵ��� i2c_CheckDevice() ���I2C�豸�Ƿ�����

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
*	�� �� ��: i2c_Delay
*	����˵��: I2C����λ�ӳ٣����400KHz
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
//static void i2c_Delay(void)
//{

//	
//	
//}

/*
*********************************************************************************************************
*	�� �� ��: i2c_Start
*	����˵��: CPU����I2C���������ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_Start(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
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
*	�� �� ��: i2c_Start
*	����˵��: CPU����I2C����ֹͣ�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_Stop(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
	TOF_I2C_SDA_0();
	TOF_I2C_SCL_1();
	Delay_us(4);
	TOF_I2C_SDA_1();
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_SendByte
*	����˵��: CPU��I2C�����豸����8bit����
*	��    �Σ�_ucByte �� �ȴ����͵��ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* �ȷ����ֽڵĸ�λbit7 */
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
			 TOF_I2C_SDA_1(); // �ͷ�����
		}
		_ucByte <<= 1;	/* ����һ��bit */
	    Delay_us(2);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_ReadByte
*	����˵��: CPU��I2C�����豸��ȡ8bit����
*	��    �Σ���
*	�� �� ֵ: ����������
*********************************************************************************************************
*/
uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* ������1��bitΪ���ݵ�bit7 */
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
*	�� �� ��: i2c_WaitAck
*	����˵��: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
*	��    �Σ���
*	�� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
*********************************************************************************************************
*/
 uint8_t i2c_WaitAck(void)
{
	uint8_t re;

	TOF_I2C_SDA_1();	/* CPU�ͷ�SDA���� */
	Delay_us(1);
	TOF_I2C_SCL_1();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	Delay_us(1);
	if (TOF_I2C_SDA_READ())	/* CPU��ȡSDA����״̬ */
	{
		re = 1;
	}
	else
	{
		re = 0;  //0Ӧ���ź�
	}
	TOF_I2C_SCL_0();
	Delay_us(1);
	return re;
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_Ack
*	����˵��: CPU����һ��ACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_Ack(void)
{
	TOF_I2C_SDA_0();	/* CPU����SDA = 0 */
	Delay_us(2);
	TOF_I2C_SCL_1();	/* CPU����1��ʱ�� */
	Delay_us(2);
	TOF_I2C_SCL_0();
	Delay_us(2);
	TOF_I2C_SDA_1();	/* CPU�ͷ�SDA���� */
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_NAck
*	����˵��: CPU����1��NACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_NAck(void)
{
	TOF_I2C_SDA_1();	/* CPU����SDA = 1 */
	Delay_us(2);
	TOF_I2C_SCL_1();	/* CPU����1��ʱ�� */
	Delay_us(2);
	TOF_I2C_SCL_0();
	Delay_us(2);
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_CfgGpio
*	����˵��: ����I2C���ߵ�GPIO������ģ��IO�ķ�ʽʵ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_CfgGpio(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(TOF_RCC_I2C_PORT, ENABLE);	/* ��GPIOʱ�� */

	GPIO_InitStructure.GPIO_Pin = TOF_I2C_SCL_PIN | TOF_I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(TOF_GPIO_PORT_I2C, &GPIO_InitStructure);

	/* ��һ��ֹͣ�ź�, ��λI2C�����ϵ������豸������ģʽ */
	i2c_Stop();
}




/*
   ������reg �Ĵ�����ַ
         data д��Ĵ�����ַ����
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
   ������regҪ��ȡ�Ĵ�����ַ

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
   ������addrdataд��ַ 52
         *dataҪ���͵�����
		 count���͵��ֽڸ���
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
   ������addrdata����ַ53
         ���ص����ֽ�����
*/
uint8_t Data_read(uint8_t addrdata)
{
	uint8_t data=0;
	i2c_Start();
	i2c_SendByte(addrdata);//�Լ����Ӧ����0x53
	i2c_WaitAck(); 
	data=i2c_ReadByte(); //???
	i2c_NAck();	
	i2c_Stop();	
    return data;
}
 /**
  * @brief  TOF_Init����ʼ��TOF
  * @param  ��
  * @retval ��
  */


uint8_t TOF_Init(void)
{
	uint8_t reset;
//	Delay_ms(200);	
	reset = ReadByte(0x016);
	
	if (reset==1){ // ����Ƿ��ʼ�����

		// ��������-ר�üĴ��� : private registers
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

	//��������-ͨ�üĴ���: Public registers 
	WriteByte(0x0011, 0x10); // Enables polling for ��New Sample ready�� 
	 // when measurement completes
	WriteByte(0x010a, 0x30); // Set the averaging sample period
	 // (compromise between lower noise and 
	 // increased execution time)
	WriteByte(0x003f, 0x46); // Sets the light and dark gain (uppernibble). Dark gain should not be changed.

	WriteByte(0x0031, 0xFF); // sets the # of range measurements after 
	 // which auto calibration of system is performed 
	WriteByte(0x0041, 0x63); // Set ALS integration time to 100ms

	WriteByte(0x002e, 0x01); // perform a single temperature calibration of the ranging sensor 
	
	//ѡ������-ͨ�üĴ���: Public registers
	WriteByte(0x001b, 0x09); // Set default ranging inter-measurement 
	 // period to 100ms
	WriteByte(0x003e, 0x31); // Set default ALS inter-measurement period 
	 // to 500ms
	WriteByte(0x0014, 0x24); // Configures interrupt on ��New Sample 
	 // Ready threshold event�� 
		 
	WriteByte(0x016, 0x00); //��־λ����1to0 change fresh out of set status to 0
	
	printf("tof��ʼ���ɹ�\r\n");

	LED_RED;
	
	return 1;

}else
    return 0;
}

/*
    ��ʼ����ģʽѡ��
    ѡ�񵥴β���ģʽ
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
  
  
  
  
  
  
  
  
  
  

 
