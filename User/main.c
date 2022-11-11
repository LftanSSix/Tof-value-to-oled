/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief  OLED测试
//		LED_BLUE;     延时测试
//		Delay_us(1000000);
//		LED_RED;
//		Delay_s(5);
//		LED_YELLOW;
//		Delay_ms(1000);
		VL6180X_Start_Range();
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_debug_usart.h"
#include "OLED_I2C.h"
#include "./systick/bsp_SysTick.h"
#include "./adc/adc.h"
#include "./tof/sys.h"
#include "./i2c_soft/bsp_i2c_soft.h"

volatile uint16_t value;
uint16_t range_value;
extern const unsigned char BMP1[];

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	unsigned char i,j=0;
	/*初始化LED、串口*/
	SysTick_Init();   //初始化延迟函数
	i2c_CfgGpio();     //
	I2C_Configuration();//oled配置CPU的硬件I2C
    LED_GPIO_Config();
	Debug_USART_Config();
	LED_BLUE;
	Delay_s(1);
	TOF_Init();
//	VL6180X_Start_Range();
	Rheostat_Init();	
	OLED_Init();    //初始化OLED
	
	OLED_Fill(0xFF);//全屏点亮
	Delay_s(1);		// 2s
		
	OLED_Fill(0x00);//全屏灭
	Delay_s(1);		// 2s
	
	while(1)
	{                           
		
		unsigned char light_temp[5],range[4];
		LED_GREEN;
		
		for(i=0;i<7;i++)
		{
		 	OLED_ShowCN(8+i*16,0,i);								//测试显示中文
		}

		OLED_DrawLine(0,2);                                         //划水平线
		OLED_ShowStr(0,3,(unsigned char*)"range:",1);				//测试6*8字符
		
		j=0;
		for(i=7;i<11;i++)
		{
			OLED_ShowCN(j*16,4,i);                                  //显示测量值
			j++;
		}
		j=0;

		for(i=11;i<14;i++)
		{
			OLED_ShowCN(j*16,6,i);                                  //显示亮度
			j++;
		}
		
		sprintf(light_temp,"%4d",value);
		for(i=0;i<4;i++)
		{
			if(light_temp[i]==' ')
			light_temp[i]='0';
		}
		OLED_ShowStr(39,6,light_temp,2);  //在屏幕显示电位器值4096
		
		LED_YELLOW;
		VL6180X_Start_Range();
		VL6180X_Poll_Range();
		range_value = VL6180_Read_Range();
		VL6180X_Clear_Interrupt();
		printf("%d\r\n",range_value);
//		Delay_ms(20);
		
		sprintf(range,"%3d",range_value); 
		for(i=0;i<3;i++)
		{
			if(range[i]==' ')
			range[i]='0';
		}
		OLED_ShowStr(55,4,range,2);  //在屏幕显示距离值（mm）

		
//		Delay_s(5);		// 2*100=200s
//		OLED_CLS();//清屏
//		OLED_OFF();//测试OLED休眠
//		Delay_s(2);		// 2s
//		OLED_ON();//测试OLED休眠后唤醒
//		OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//测试BMP位图显示
//		Delay_s(5);		// 2*100=200s
//		OLED_CLS();//清屏






	} 

}



/*********************************************END OF FILE**********************/

