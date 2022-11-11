/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief  OLED����
//		LED_BLUE;     ��ʱ����
//		Delay_us(1000000);
//		LED_RED;
//		Delay_s(5);
//		LED_YELLOW;
//		Delay_ms(1000);
		VL6180X_Start_Range();
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F429 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://fire-stm32.taobao.com
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
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	unsigned char i,j=0;
	/*��ʼ��LED������*/
	SysTick_Init();   //��ʼ���ӳٺ���
	i2c_CfgGpio();     //
	I2C_Configuration();//oled����CPU��Ӳ��I2C
    LED_GPIO_Config();
	Debug_USART_Config();
	LED_BLUE;
	Delay_s(1);
	TOF_Init();
//	VL6180X_Start_Range();
	Rheostat_Init();	
	OLED_Init();    //��ʼ��OLED
	
	OLED_Fill(0xFF);//ȫ������
	Delay_s(1);		// 2s
		
	OLED_Fill(0x00);//ȫ����
	Delay_s(1);		// 2s
	
	while(1)
	{                           
		
		unsigned char light_temp[5],range[4];
		LED_GREEN;
		
		for(i=0;i<7;i++)
		{
		 	OLED_ShowCN(8+i*16,0,i);								//������ʾ����
		}

		OLED_DrawLine(0,2);                                         //��ˮƽ��
		OLED_ShowStr(0,3,(unsigned char*)"range:",1);				//����6*8�ַ�
		
		j=0;
		for(i=7;i<11;i++)
		{
			OLED_ShowCN(j*16,4,i);                                  //��ʾ����ֵ
			j++;
		}
		j=0;

		for(i=11;i<14;i++)
		{
			OLED_ShowCN(j*16,6,i);                                  //��ʾ����
			j++;
		}
		
		sprintf(light_temp,"%4d",value);
		for(i=0;i<4;i++)
		{
			if(light_temp[i]==' ')
			light_temp[i]='0';
		}
		OLED_ShowStr(39,6,light_temp,2);  //����Ļ��ʾ��λ��ֵ4096
		
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
		OLED_ShowStr(55,4,range,2);  //����Ļ��ʾ����ֵ��mm��

		
//		Delay_s(5);		// 2*100=200s
//		OLED_CLS();//����
//		OLED_OFF();//����OLED����
//		Delay_s(2);		// 2s
//		OLED_ON();//����OLED���ߺ���
//		OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//����BMPλͼ��ʾ
//		Delay_s(5);		// 2*100=200s
//		OLED_CLS();//����






	} 

}



/*********************************************END OF FILE**********************/

