#ifndef _BSP_I2C_SOFT_H
#define _BSP_I2C_SOFT_H
#include "./tof/sys.h"

#define TOF_READ_ADDRESS	0x53 //  设备地址0x29
#define TOF_WRITE_ADDRESS	0x52 //

/* 定义I2C总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */
#define TOF_GPIO_PORT_I2C	GPIOC			/* GPIO端口 */
#define TOF_RCC_I2C_PORT 	RCC_AHB1Periph_GPIOC		/* GPIO端口时钟 */
#define TOF_I2C_SCL_PIN		GPIO_Pin_7			/* 连接到SCL时钟线的GPIO */
#define TOF_I2C_SDA_PIN		GPIO_Pin_6			/* 连接到SDA数据线的GPIO */


/* 定义读写SCL和SDA的宏，已增加代码的可移植性和可阅读性 */
#if 1	/* 条件编译： 1 选择GPIO的库函数实现IO读写 */
	#define TOF_I2C_SCL_1()  GPIO_SetBits(TOF_GPIO_PORT_I2C, TOF_I2C_SCL_PIN)		/* SCL = 1 */
	#define TOF_I2C_SCL_0()  GPIO_ResetBits(TOF_GPIO_PORT_I2C, TOF_I2C_SCL_PIN)		/* SCL = 0 */
			
	#define TOF_I2C_SDA_1()  GPIO_SetBits(TOF_GPIO_PORT_I2C, TOF_I2C_SDA_PIN)		/* SDA = 1 */
	#define TOF_I2C_SDA_0()  GPIO_ResetBits(TOF_GPIO_PORT_I2C, TOF_I2C_SDA_PIN)		/* SDA = 0 */
			
	#define TOF_I2C_SDA_READ()  GPIO_ReadInputDataBit(TOF_GPIO_PORT_I2C, TOF_I2C_SDA_PIN)	/* 读SDA口线状态 */
#else	/* 这个分支选择直接寄存器操作实现IO读写 */
    /*　注意：如下写法，在IAR最高级别优化时，会被编译器错误优化 */
	#define OLED_I2C_SCL_1()  OLED_GPIO_PORT_I2C->BSRR = OLED_I2C_SCL_PIN				/* SCL = 1 */
	#define OLED_I2C_SCL_0()  OLED_GPIO_PORT_I2C->BRR = OLED_I2C_SCL_PIN				/* SCL = 0 */
	
	#define OLED_I2C_SDA_1()  OLED_GPIO_PORT_I2C->BSRR = OLED_I2C_SDA_PIN				/* SDA = 1 */
	#define OLED_I2C_SDA_0()  OLED_GPIO_PORT_I2C->BRR = OLED_I2C_SDA_PIN				/* SDA = 0 */
	
	#define OLED_I2C_SDA_READ()  ((OLED_GPIO_PORT_I2C->IDR & OLED_I2C_SDA_PIN) != 0)	/* 读SDA口线状态 */
#endif

void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte(void);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);

void i2c_CfgGpio(void);

uint8_t OLED_CheckDevice(uint8_t _Address);//检测I2C总线设备OLED
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

