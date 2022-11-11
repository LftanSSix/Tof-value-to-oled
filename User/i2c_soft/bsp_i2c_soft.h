#ifndef _BSP_I2C_SOFT_H
#define _BSP_I2C_SOFT_H
#include "./tof/sys.h"

#define TOF_READ_ADDRESS	0x53 //  �豸��ַ0x29
#define TOF_WRITE_ADDRESS	0x52 //

/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
#define TOF_GPIO_PORT_I2C	GPIOC			/* GPIO�˿� */
#define TOF_RCC_I2C_PORT 	RCC_AHB1Periph_GPIOC		/* GPIO�˿�ʱ�� */
#define TOF_I2C_SCL_PIN		GPIO_Pin_7			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define TOF_I2C_SDA_PIN		GPIO_Pin_6			/* ���ӵ�SDA�����ߵ�GPIO */


/* �����дSCL��SDA�ĺ꣬�����Ӵ���Ŀ���ֲ�ԺͿ��Ķ��� */
#if 1	/* �������룺 1 ѡ��GPIO�Ŀ⺯��ʵ��IO��д */
	#define TOF_I2C_SCL_1()  GPIO_SetBits(TOF_GPIO_PORT_I2C, TOF_I2C_SCL_PIN)		/* SCL = 1 */
	#define TOF_I2C_SCL_0()  GPIO_ResetBits(TOF_GPIO_PORT_I2C, TOF_I2C_SCL_PIN)		/* SCL = 0 */
			
	#define TOF_I2C_SDA_1()  GPIO_SetBits(TOF_GPIO_PORT_I2C, TOF_I2C_SDA_PIN)		/* SDA = 1 */
	#define TOF_I2C_SDA_0()  GPIO_ResetBits(TOF_GPIO_PORT_I2C, TOF_I2C_SDA_PIN)		/* SDA = 0 */
			
	#define TOF_I2C_SDA_READ()  GPIO_ReadInputDataBit(TOF_GPIO_PORT_I2C, TOF_I2C_SDA_PIN)	/* ��SDA����״̬ */
#else	/* �����֧ѡ��ֱ�ӼĴ�������ʵ��IO��д */
    /*��ע�⣺����д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
	#define OLED_I2C_SCL_1()  OLED_GPIO_PORT_I2C->BSRR = OLED_I2C_SCL_PIN				/* SCL = 1 */
	#define OLED_I2C_SCL_0()  OLED_GPIO_PORT_I2C->BRR = OLED_I2C_SCL_PIN				/* SCL = 0 */
	
	#define OLED_I2C_SDA_1()  OLED_GPIO_PORT_I2C->BSRR = OLED_I2C_SDA_PIN				/* SDA = 1 */
	#define OLED_I2C_SDA_0()  OLED_GPIO_PORT_I2C->BRR = OLED_I2C_SDA_PIN				/* SDA = 0 */
	
	#define OLED_I2C_SDA_READ()  ((OLED_GPIO_PORT_I2C->IDR & OLED_I2C_SDA_PIN) != 0)	/* ��SDA����״̬ */
#endif

void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte(void);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);

void i2c_CfgGpio(void);

uint8_t OLED_CheckDevice(uint8_t _Address);//���I2C�����豸OLED
//void I2C_WriteByte(uint8_t addr,uint8_t data);
//void I2C_WriteHByte(uint16_t addr,uint8_t data);
//uint8_t I2C_ReadHByte(uint16_t addr);

void Data_write(uint8_t addrdata,uint8_t *data,uint8_t count);
uint8_t Data_read(uint8_t addrdata);
void WriteByte(uint16_t reg,uint8_t data);
uint8_t ReadByte(uint16_t reg);
uint8_t TOF_Init(void);
uint8_t VL6180X_Start_Range(void);
uint8_t VL6180X_Poll_Range(void);
uint8_t VL6180_Read_Range(void);
void VL6180X_Clear_Interrupt(void);

#endif

