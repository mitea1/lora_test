/*
 * teltronic_io2.c
 *
 *  Created on: Mar 24, 2016
 *      Author: Simon
 */
/*----- Header-Files -------------------------------------------------------*/
#include <stm32l1xx.h>				/* Processor STM32F407IG				*/
//#include <TELTRONIC.h>					/* TELTRONIC Module							*/
#include <teltronic_io2.h>			/* TELTRONIC IO2 Module					*/
/**
 * @brief	TELTRONIC IO2 Port and Pin association
 */
static TELTRONIC_Port_Pin_t TELTRONIC_IO2_Port_Pin[] = {
		{ GPIOB, GPIO_Pin_0, GPIO_Mode_AN },				/**< ADC Port 0	*/
		{ GPIOC, GPIO_Pin_0, GPIO_Mode_AN },				/**< ADC Port 1	*/
		{ GPIOC, GPIO_Pin_2, GPIO_Mode_AN },				/**< ADC Port 2	*/
		{ GPIOG, GPIO_Pin_8, GPIO_Mode_IN },				/**< GPIO In 0	*/
		{ GPIOG, GPIO_Pin_6, GPIO_Mode_IN },				/**< GPIO In 1	*/
		{ GPIOG, GPIO_Pin_7, GPIO_Mode_IN },				/**< GPIO In 2	*/
		//		{ TELTRONIC_AGPIO_108, GPIO_Mode_IN },				/**< GPIO In 3	*/
		//		{ TELTRONIC_AGPIO_21, GPIO_Mode_OUT },				/**< PWM Phase	*/
		{ GPIOA, GPIO_Pin_0, GPIO_Mode_OUT, GPIO_AF_TIM5 },	/**< PWM 0		*/
		{ GPIOH, GPIO_Pin_11, GPIO_Mode_OUT, GPIO_AF_TIM5 },/**< PWM 1		*/
		{ GPIOH, GPIO_Pin_12, GPIO_Mode_OUT, GPIO_AF_TIM5 },/**< PWM 2		*/
		{ GPIOA, GPIO_Pin_3, GPIO_Mode_AF, GPIO_AF_TIM5 },	/**< PWM 3		*/
		{ GPIOB, GPIO_Pin_5, GPIO_Mode_AF, GPIO_AF_SPI1 },	/**< SPI1 MOSI	*/
		{ GPIOA, GPIO_Pin_6, GPIO_Mode_AF, GPIO_AF_SPI1 },	/**< SPI1 MISO	*/
		{ GPIOA, GPIO_Pin_5, GPIO_Mode_AF, GPIO_AF_SPI1 },	/**< SPI1 CLK	*/
		{ GPIOA, GPIO_Pin_4, GPIO_Mode_OUT },				/**< SPI1 NSS	*/
		//		{ TELTRONIC_AGPIO_22, GPIO_Mode_OUT },				/**< nPSC SEL	*/
};
void teltronic_IO2_Init(void);
void RCC_Configuration(void);
void GPIO_SPI_Configure(void);

/**
 *****************************************************************************
 * @brief		TELTRONIC IO2 initialization.
 *
 * @return		None
 *****************************************************************************
 */
void teltronic_IO2_Init(void){
	/* Initialize the SPI */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	TELTRONIC_IO2_SPI_CS_Out(Bit_SET);	/* set CS high */
	SPI_DeInit(SPI1);
	SPI_InitTypeDef SPI_InitStruct;
	// Diller Technologies
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	/**
	 * TELTRONIC IO2 SPI bus
	 *	- Mode = Master
	 *	- DataSize = 16 Bit
	 *	- CPOL = high
	 *	- CPHA = 2 Edge
	 *	- NSS = Soft internal
	 *	- Baud rate prescaler = 64 (APB2 / 64)
	 */
	//	SPI_StructInit(&SPI_InitStruct);
	//	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	//	SPI_InitStruct.SPI_DataSize = SPI_DataSize_16b;
	//	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
	//	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	//	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set;
	//	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	//	SPI_Init(SPI1, &SPI_InitStruct);
	//	SPI_Cmd(SPI1, ENABLE);
//	TELTRONIC_GPIO_Init(TELTRONIC_IO2_Port_Pin, &GPIO_InitStruct,
//		                sizeof(TELTRONIC_IO2_Port_Pin) / sizeof(TELTRONIC_Port_Pin_t));

	/*spetca Github*/
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex ;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStruct);
	SPI_Cmd(SPI1, ENABLE);

	/*diller technologies*/
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_SPI1, ENABLE);

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_WriteBit(GPIOA, GPIO_Pin_8, SET);

	NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CRCPolynomial = 0;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI1, &SPI_InitStructure);

	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);

	SPI_Cmd(SPI1, ENABLE);
	/*diller technologies*/
}
/*
 * Author: specta Github
 *
 * */
void GPIO_SPI_Configure(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11,GPIO_AF_SPI1) ;
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12,GPIO_AF_SPI1) ;
}
/*
 * Author: specta Github
 *
 * */
void RCC_Configuration(void)
{
	/* Enable HSI Clock */
	RCC_HSICmd(ENABLE);

	/*!< Wait till HSI is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
	{}

	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);

	RCC_MSIRangeConfig(RCC_MSIRange_6);
	//clock the spi


	/* Enable the GPIOs Clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC| RCC_AHBPeriph_GPIOD| RCC_AHBPeriph_GPIOE| RCC_AHBPeriph_GPIOH, ENABLE);

	/*Enable SPI Clock*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1, ENABLE);


	/* Enable  comparator clock LCD and PWR mngt */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_COMP | RCC_APB1Periph_LCD | RCC_APB1Periph_PWR,ENABLE);

	/* Enable ADC clock & SYSCFG */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_SYSCFG , ENABLE);

	/* Allow access to the RTC */
	PWR_RTCAccessCmd(ENABLE);

	/* Reset Backup Domain */
	RCC_RTCResetCmd(ENABLE);
	RCC_RTCResetCmd(DISABLE);

	/* LSE Enable */
	RCC_LSEConfig(RCC_LSE_ON);

	/* Wait till LSE is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{}

	RCC_RTCCLKCmd(ENABLE);

	/* LCD Clock Source Selection */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	RCC_HSEConfig(RCC_HSE_OFF);

	if(RCC_GetFlagStatus(RCC_FLAG_HSERDY) != RESET )
	{
		while(1);
	}

}
/**
 *****************************************************************************
 * @brief		Initialize GPIO ports with a CARME_Port_Pin_t table.
 *
 * @param[in]	pPortPinAssociation	CARME_Port_Pin_t Port and Pin association
 * @param[in]	pGPIO_InitStruct	GPIO_InitTypeDef GPIO init type definition
 * @param[in]	size				Size of the struct
 * @return		None
 *****************************************************************************
 */
void TELTRONIC_GPIO_Init(TELTRONIC_Port_Pin_t *pPortPinAssociation,
                     GPIO_InitTypeDef *pGPIO_InitStruct, uint8_t size) {

	uint8_t i;

	for (i = 0; i < size; i++) {
		pGPIO_InitStruct->GPIO_Pin = pPortPinAssociation[i].GPIO_Pin;
		pGPIO_InitStruct->GPIO_Mode = pPortPinAssociation[i].GPIO_Mode;
		GPIO_Init(pPortPinAssociation[i].GPIOx, pGPIO_InitStruct);
		if (pPortPinAssociation[i].GPIO_Mode == GPIO_Mode_AF) {
			GPIO_PinAFConfig(
			        pPortPinAssociation[i].GPIOx,
			        CARME_GPIO_PIN_TO_SOURCE(pPortPinAssociation[i].GPIO_Pin),
			        pPortPinAssociation[i].GPIO_AF);
		}
	}
}

/**
 *****************************************************************************
 * @brief		Select the peripheral SPI chip.
 *
 * @param[in]	select	Select nPSC0 or nPSC1\n
 *						@arg nPSC0:	DAC (SPI0)\n
 *						@arg nPSC1:	External (SPI1)
 * @return		None
 *****************************************************************************
 */
void TELTRONIC_IO2_SPI_Select(TELTRONIC_IO2_SPI_CHANNEL select) {

	TELTRONIC_AGPIO_Set(select == TELTRONIC_IO2_nPSC0 ? 0 : TELTRONIC_AGPIO_PIN_22,
	                TELTRONIC_AGPIO_PIN_22 );
}

/**
 *****************************************************************************
 * @brief		Generate the chip select pin.
 *
 * @param[in]	cs		Set the chip select pin.\n
 *						@arg Bit_RESET:	CS = low\n
 *						@arg Bit_SET:	CS = high
 * @return		None
 *****************************************************************************
 */
void TELTRONIC_IO2_SPI_CS_Out(uint8_t cs) {

	GPIO_WriteBit(GPIOA, GPIO_Pin_4, cs);
}

/**
 *****************************************************************************
 * @brief		Send a half word over the SPI port.
 *
 * @param[in]	data	Half word you want to send.
 * @return		None
 *****************************************************************************
 */
void TELTRONIC_IO2_SPI_Send(uint16_t data) {

	/* write data to be transmitted to the SPI data register */
	SPI_I2S_SendData(SPI1, data);

	/* wait until transmit complete */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
		;
	/* wait until receive complete */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
		;
	/* wait until SPI is not busy anymore */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET)
		;
}

/**
 *****************************************************************************
 * @brief		Receive a half word from the SPI input buffer.
 *
 * @param[out]	pValue	Pointer to the half word buffer to write data in.
 * @return		None
 *****************************************************************************
 */
void TELTRONIC_IO2_SPI_Receive(uint16_t *pValue) {

	/* wait until transmit complete */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
		;
	/* wait until receive complete */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
		;
	/* wait until SPI is not busy anymore */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET)
		;

	*pValue = SPI_I2S_ReceiveData(SPI1);
}
