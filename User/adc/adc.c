
#include "./adc/adc.h"

static void Rheostat_ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// ʹ�� GPIO ʱ��
	RCC_AHB1PeriphClockCmd(RHEOSTAT_ADC_GPIO_CLK, ENABLE);
		
	// ���� IO
	GPIO_InitStructure.GPIO_Pin = RHEOSTAT_ADC_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //������������
	GPIO_Init(RHEOSTAT_ADC_GPIO_PORT, &GPIO_InitStructure);		
}

static void Rheostat_ADC_Mode_Config(void)
{
  ADC_InitTypeDef ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
  // ����ADCʱ��
	RCC_APB2PeriphClockCmd(RHEOSTAT_ADC_CLK , ENABLE);

  // -------------------ADC Common �ṹ�� ���� ��ʼ��------------------------
	// ����ADCģʽ
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  // ʱ��Ϊfpclk x��Ƶ	
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
  // ��ֹDMAֱ�ӷ���ģʽ	
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  // ����ʱ����	
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;  
  ADC_CommonInit(&ADC_CommonInitStructure);
	
  // -------------------ADC Init �ṹ�� ���� ��ʼ��--------------------------
	ADC_StructInit(&ADC_InitStructure);
  // ADC �ֱ���
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  // ��ֹɨ��ģʽ����ͨ���ɼ�����Ҫ	
  ADC_InitStructure.ADC_ScanConvMode = DISABLE; 
  // ����ת��	
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  //��ֹ�ⲿ���ش���
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  //�ⲿ����ͨ����������ʹ�������������ֵ��㸳ֵ����
//  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  //�����Ҷ���	
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  //ת��ͨ�� 1��
  ADC_InitStructure.ADC_NbrOfConversion = 1;                                    
  ADC_Init(RHEOSTAT_ADC, &ADC_InitStructure);
  //---------------------------------------------------------------------------
	
  // ���� ADC ͨ��ת��˳��Ϊ1����һ��ת��������ʱ��Ϊ3��ʱ������
  ADC_RegularChannelConfig(RHEOSTAT_ADC, RHEOSTAT_ADC_CHANNEL, 1, ADC_SampleTime_3Cycles);
	// ADC ת�����������жϣ����жϷ�������ж�ȡת��ֵ
	

  ADC_ITConfig(RHEOSTAT_ADC, ADC_IT_EOC, ENABLE);
  // ʹ��ADC
  ADC_Cmd(RHEOSTAT_ADC, ENABLE);  
  //��ʼadcת�����������
  ADC_SoftwareStartConv(RHEOSTAT_ADC);
}

// �����ж����ȼ�
static void Rheostat_ADC_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
  NVIC_InitStructure.NVIC_IRQChannel = Rheostat_ADC_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
  NVIC_Init(&NVIC_InitStructure);
}


void Rheostat_Init(void)
{
	Rheostat_ADC_GPIO_Config();
	Rheostat_ADC_Mode_Config();
	Rheostat_ADC_NVIC_Config();
}




//#include "./adc/adc.h"

///*adc���ų�ʼ��*/
//void adc_gpio_init(void)
//{
//    GPIO_InitTypeDef GPIO_InitStructure;
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
//	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AN;     //ģ������ģʽ
//	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL; //���벻������
//	
//	GPIO_Init(GPIOC,&GPIO_InitStructure);
//	
//}
///*��ʼ��ADC����ģʽ*/
//void adc_gpio_conf_init(void)
//{
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
//	
//	ADC_InitTypeDef ADC_Initstructure;
//	ADC_Initstructure.ADC_Resolution=ADC_Resolution_12b;
//	ADC_Initstructure.ADC_ScanConvMode=DISABLE;   //��ͨ������ʹ��ɨ��ģʽ
//	ADC_Initstructure.ADC_ContinuousConvMode=ENABLE;//����ת��ģʽ
//	ADC_Initstructure.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None;//��ֹ�ⲿ��Ե����
////	ADC_Initstructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_T1_CC1;
//	ADC_Initstructure.ADC_DataAlign=ADC_DataAlign_Right;//�����Ҷ���
//	ADC_Initstructure.ADC_NbrOfConversion=1; //1��ת��ͨ��
//	ADC_Init(ADC1,&ADC_Initstructure);
//	
//	ADC_CommonInitTypeDef ADC_CommonInitstructure;
//	ADC_CommonInitstructure.ADC_Mode=ADC_Mode_Independent;//����ģʽ ʽ������ADCת������ADC1��ADC2��ADC3���������ع���������Ӱ��
//	ADC_CommonInitstructure.ADC_Prescaler=ADC_Prescaler_Div4;//4��Ƶ 180/4=45mhz
//	ADC_CommonInitstructure.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled;//����DMAֱ�ӷ���
//	ADC_CommonInitstructure.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_20Cycles;//����ʱ���� 10������
//	
//	ADC_CommonInit(&ADC_CommonInitstructure);
//	
//	
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_56Cycles); //ADC1IN10 ����ʱ��Ϊ56��ʱ������
//	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE); //ת������������жϣ����жϷ������ж�ȡת��ֵ

//	ADC_Cmd(ADC1,ENABLE);
//	ADC_SoftwareStartConv(ADC1);//�ڲ����������������֮ǰF7ʹ�õ����ⲿ��ʱ���жϴ���
//	
//}
///*ADC�жϳ�ʼ��*/
//void ADC_NVIC_INIT(void)
//{
//	NVIC_InitTypeDef ADC_NVIC_Initstructure;
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//	
//	ADC_NVIC_Initstructure.NVIC_IRQChannel=ADC_IRQn;
//	ADC_NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority=1;
//	ADC_NVIC_Initstructure.NVIC_IRQChannelSubPriority=1;
//	ADC_NVIC_Initstructure.NVIC_IRQChannelCmd=ENABLE;

//	NVIC_Init(&ADC_NVIC_Initstructure);
//	
//}
///*��װ��ʼ������*/
//void Adc_Haptic_Init(void)
//{
//	adc_gpio_init();
//	adc_gpio_conf_init();
//	ADC_NVIC_INIT();
//}








