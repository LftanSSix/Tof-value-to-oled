#ifndef __adc_extrig_TIM_H
#define	__adc_extrig_TIM_H

#include "stm32f4xx.h"

// ADC GPIO �궨��
#define RHEOSTAT_ADC_GPIO_PORT    GPIOC
#define RHEOSTAT_ADC_GPIO_PIN     GPIO_Pin_0
#define RHEOSTAT_ADC_GPIO_CLK     RCC_AHB1Periph_GPIOC

// ADC ��ź궨��
#define RHEOSTAT_ADC              ADC1
#define RHEOSTAT_ADC_CLK          RCC_APB2Periph_ADC1
#define RHEOSTAT_ADC_CHANNEL      ADC_Channel_10

// ADC �жϺ궨��
#define Rheostat_ADC_IRQ            ADC_IRQn
#define Rheostat_ADC_INT_FUNCTION   ADC_IRQHandler

// ��ʱ��TIM2
#define BASIC_TIM           		TIM2
#define BASIC_TIM_CLK       		RCC_APB1Periph_TIM2

void Rheostat_Init(void);

#endif /* _adc_extrig_TIM_H */



