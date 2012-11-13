void  ENCODER1_Config(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//Timer 8 polaczony z portem PC6 i 7
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC , ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//2.--------------------------TIMER 888888888888888------------------------------------------
	//*
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8 , ENABLE);    // odpalamy zegar timera
	TIM_DeInit(TIM8);                              // kasuje poprzednie ustawienia, nie wiem czy potrzebne


	// TimeBase configuration
	TIM_TimeBaseStructure.TIM_Prescaler     = 0x0;
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period        = 0xFFFF;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);              //Inicjalizacja licznika


	TIM_ICStructInit(&TIM_ICInitStructure);
	//Initialize input capture structure: Ch2
	TIM_ICInitStructure.TIM_Channel     = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;
	//TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	//TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	//TIM_ICInitStructure.TIM_ICFilter    = 0;
	TIM_ICInit(TIM8, &TIM_ICInitStructure);

	//Initialize input capture structure: Ch2
	TIM_ICInitStructure.TIM_Channel     = TIM_Channel_2;
	TIM_ICInit(TIM8, &TIM_ICInitStructure);

	// timer 8 jako enkoder
	TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12,
	TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

	//TIM_ClearFlag(TIM8, TIM_FLAG_Update | TIM_FLAG_COM | TIM_FLAG_Break | TIM_FLAG_CC1 | TIM_FLAG_CC2 |TIM_FLAG_CC3|TIM_FLAG_CC4);
	//TIM_ITConfig(TIM8, TIM_IT_Update | TIM_IT_COM | TIM_IT_Break | TIM_IT_CC1 | TIM_IT_CC2 |TIM_IT_CC3|TIM_IT_CC4, ENABLE);

	// Clear all pending interrupts
	TIM_ClearFlag(TIM8, TIM_FLAG_Update);
	TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);

	TIM_SetCounter(TIM8, 0);

	TIM_Cmd(TIM8, ENABLE);


	//3.---------------------NVIC!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1------------------------------------------

	//przerwanie ogolne od timera8
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQn;//TIM_IT_CC1;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //co to k?
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
