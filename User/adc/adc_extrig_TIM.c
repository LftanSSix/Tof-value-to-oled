
#include "./adc/adc_extrig_TIM.h"

static void Rheostat_ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// 使能 GPIO 时钟
	RCC_AHB1PeriphClockCmd(RHEOSTAT_ADC_GPIO_CLK, ENABLE);
		
	// 配置 IO
	GPIO_InitStructure.GPIO_Pin = RHEOSTAT_ADC_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //不上拉不下拉
	GPIO_Init(RHEOSTAT_ADC_GPIO_PORT, &GPIO_InitStructure);		
}

static void Rheostat_ADC_Mode_Config(void)
{
  ADC_InitTypeDef ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
  // 开启ADC时钟
	RCC_APB2PeriphClockCmd(RHEOSTAT_ADC_CLK , ENABLE);

  // -------------------ADC Common 结构体 参数 初始化------------------------
	// 独立ADC模式
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  // 时钟为fpclk x分频	
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
  // 禁止DMA直接访问模式	
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  // 采样时间间隔	
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;  
  ADC_CommonInit(&ADC_CommonInitStructure);
	
  // -------------------ADC Init 结构体 参数 初始化--------------------------
	ADC_StructInit(&ADC_InitStructure);
  // ADC 分辨率
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  // 禁止扫描模式，多通道采集才需要	
  ADC_InitStructure.ADC_ScanConvMode = DISABLE; 
  // 连续转换	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
  //上升沿触发
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
  //外部触发通道
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;
  //数据右对齐	
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  //转换通道 1个
  ADC_InitStructure.ADC_NbrOfConversion = 1; 
  
  ADC_Init(RHEOSTAT_ADC, &ADC_InitStructure);
  //---------------------------------------------------------------------------
	
  // 配置 ADC 通道转换顺序为1，第一个转换，采样时间为3个时钟周期
  ADC_RegularChannelConfig(RHEOSTAT_ADC, RHEOSTAT_ADC_CHANNEL, 1, ADC_SampleTime_56Cycles);
//  // ADC 转换结束产生中断，在中断服务程序中读取转换值
//  ADC_ITConfig(RHEOSTAT_ADC, ADC_IT_EOC, ENABLE);

  // 使能ADC
  ADC_Cmd(RHEOSTAT_ADC, ENABLE);  
  //开始adc转换，软件触发？
  
//ADC_SoftwareStartConv(RHEOSTAT_ADC);//软件触发
}

// 配置中断优先级
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

/*定时器tim2模式配置*/
static void TIM_Mode_Config(void)
{
	// 开启TIM2_CLK
    RCC_APB1PeriphClockCmd(BASIC_TIM_CLK, ENABLE); 

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* 累计 TIM_Period个后产生一个更新或者中断*/		
    //当定时器从0计数到4999，即为5000次，为一个定时周期
    TIM_TimeBaseStructure.TIM_Period = 5000-1; //3
    /*计数器向上计数*/	
	TIM_TimeBaseStructure.TIM_CounterMode= TIM_CounterMode_Up; //2  
	
	//定时器时钟源TIMxCLK = 2 * PCLK1  
    //				PCLK1 = HCLK / 4 
    //				=> TIMxCLK=HCLK/2=SystemCoreClock/2=90MHz
	// 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=10000Hz
    TIM_TimeBaseStructure.TIM_Prescaler = 15000-1;	//1 90MHZ/15000  *5000 0.5us
	
    /*1分频*/
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //4
	
	/*pwm模式 向下计数器值 此值控制pwm周期 max:FF*/
	//TIM_TimeBaseStructure.TIM_RepetitionCounter=
	
	// 初始化定时器TIMx, x[2,3,4,5]
	TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=5000;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	
	/*for TIM1/8*/
//	TIM_OCInitStructure.TIM_OutputNState=
//	TIM_OCInitStructure.TIM_OCNPolarity=
//	TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;
//	TIM_OCInitStructure.TIM_OCNIdleState=
	
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	
	// 使能定时器
	TIM_Cmd(BASIC_TIM, ENABLE);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);//pwm模式下要开启
	TIM_InternalClockConfig(TIM2);
	TIM_GenerateEvent(TIM2,TIM_EventSource_CC2);
	
}


void Rheostat_Init(void)
{
	Rheostat_ADC_GPIO_Config();
	Rheostat_ADC_Mode_Config();
	Rheostat_ADC_NVIC_Config();
	TIM_Mode_Config();
}




//#include "./adc/adc.h"

///*adc引脚初始化*/
//void adc_gpio_init(void)
//{
//    GPIO_InitTypeDef GPIO_InitStructure;
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
//	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AN;     //模拟输入模式
//	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL; //输入不上下拉
//	
//	GPIO_Init(GPIOC,&GPIO_InitStructure);
//	
//}
///*初始化ADC工作模式*/
//void adc_gpio_conf_init(void)
//{
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
//	
//	ADC_InitTypeDef ADC_Initstructure;
//	ADC_Initstructure.ADC_Resolution=ADC_Resolution_12b;
//	ADC_Initstructure.ADC_ScanConvMode=DISABLE;   //单通道，不使用扫描模式
//	ADC_Initstructure.ADC_ContinuousConvMode=ENABLE;//连续转换模式
//	ADC_Initstructure.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None;//禁止外部边缘触发
////	ADC_Initstructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_T1_CC1;
//	ADC_Initstructure.ADC_DataAlign=ADC_DataAlign_Right;//数据右对齐
//	ADC_Initstructure.ADC_NbrOfConversion=1; //1个转换通道
//	ADC_Init(ADC1,&ADC_Initstructure);
//	
//	ADC_CommonInitTypeDef ADC_CommonInitstructure;
//	ADC_CommonInitstructure.ADC_Mode=ADC_Mode_Independent;//独立模式 式即所有ADC转换器（ADC1、ADC2和ADC3）均独立地工作，互不影响
//	ADC_CommonInitstructure.ADC_Prescaler=ADC_Prescaler_Div4;//4分频 180/4=45mhz
//	ADC_CommonInitstructure.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled;//禁用DMA直接访问
//	ADC_CommonInitstructure.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_20Cycles;//采样时间间隔 10个周期
//	
//	ADC_CommonInit(&ADC_CommonInitstructure);
//	
//	
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_56Cycles); //ADC1IN10 采样时间为56个时钟周期
//	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE); //转换结束后进入中断，在中断服务函数中读取转换值

//	ADC_Cmd(ADC1,ENABLE);
//	ADC_SoftwareStartConv(ADC1);//内部软件触发————之前F7使用的是外部定时器中断触发
//	
//}
///*ADC中断初始化*/
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
///*封装初始化函数*/
//void Adc_Haptic_Init(void)
//{
//	adc_gpio_init();
//	adc_gpio_conf_init();
//	ADC_NVIC_INIT();
//}








