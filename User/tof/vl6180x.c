#include    "./tof/vl6180x.h" 
#include "./systick/bsp_SysTick.h"
#include "OLED_I2C.h"


/*
   VL6180X初始化
*/
  u8 VL6180X_Init()
  {
     u8 reset;
	 reset=I2C_read_hByte(0x016);
	 if(reset==1)
	 {
	     I2C_Write_hByte(0X0207,0X01);
		 I2C_Write_hByte(0X0208,0X01);
		 I2C_Write_hByte(0X0096,0X00);
		 I2C_Write_hByte(0X0097,0XFD);
		 I2C_Write_hByte(0X00E3,0X00);
		 I2C_Write_hByte(0X00E4,0X04);
		 I2C_Write_hByte(0X00E5,0X02);
		 I2C_Write_hByte(0X00E6,0X01);
		 I2C_Write_hByte(0X00E7,0X03);
		 I2C_Write_hByte(0X00F5,0X02);
		 I2C_Write_hByte(0X00D9,0X05);
		 I2C_Write_hByte(0X00DB,0XCE);
		 I2C_Write_hByte(0X02DC,0X03);
		 I2C_Write_hByte(0X00DD,0XF8);
		 I2C_Write_hByte(0X009F,0X00);
		 I2C_Write_hByte(0X00A3,0X3C);
		 I2C_Write_hByte(0X00B7,0X00);
		 I2C_Write_hByte(0X00BB,0X3C);
		 I2C_Write_hByte(0X00B2,0X09);
		 I2C_Write_hByte(0X00CA,0X09);
		 I2C_Write_hByte(0X0198,0X01);
		 I2C_Write_hByte(0X01B0,0X17);
		 I2C_Write_hByte(0X01AD,0X00);
		 I2C_Write_hByte(0X00FF,0X05);
		 I2C_Write_hByte(0X0100,0X05);
		 I2C_Write_hByte(0X0199,0X05);
		 I2C_Write_hByte(0X01A6,0X1B);
		 I2C_Write_hByte(0X01AC,0X3E);
		 I2C_Write_hByte(0X01A7,0X1F);
		 I2C_Write_hByte(0X0030,0X00);
		 
		 I2C_Write_hByte(0X0011,0X10);
		 I2C_Write_hByte(0X010A,0X30);
		 I2C_Write_hByte(0X003F,0X46);
		 I2C_Write_hByte(0X0031,0XFF);
		 I2C_Write_hByte(0X0040,0X63);
		 I2C_Write_hByte(0X002E,0X01);
		 I2C_Write_hByte(0X001B,0X09);
		 I2C_Write_hByte(0X003E,0X31);
		 I2C_Write_hByte(0X0014,0X24);
		
		 I2C_Write_hByte(0x016,0x00);
	 }
     return 0;
  }
/*
    开始测量模式选择
    选择单次测量模式
*/
 u8 VL6180X_Start_Range()
  {
     I2C_Write_hByte(0x018,0x01);
	 return 0;
  }
  
  
  /*poll for new sample ready */
 u8 VL6180X_Poll_Range()
  {
      u8 status;
	  u8 range_status;
	  status=I2C_read_hByte(0x04f);
      range_status=status&0x07;
      while(range_status!=0x04)
	  {
	     status=I2C_read_hByte(0x04f);
		 range_status=status&0x07;
         Delay_ms(1);
	  }		  
      return 0;
  }
   
  
  /*read range result (mm)*/
 u8 VL6180_Read_Range()
  {
    int range;
	range=I2C_read_hByte(0x062);
	return range;
  }
  
  /*clear interrupt*/
  void VL6180X_Clear_Interrupt()
  {
     I2C_Write_hByte(0x015,0x07); 
  }
