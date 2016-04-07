/**
 *****************************************************************************
 **
 **  File        : main.c
 **
 **  Abstract    : main function.
 **
 **  Functions   : main
 **
 **  Environment : Atollic TrueSTUDIO/STM32
 **                STMicroelectronics STM32Lxx Standard Peripherals Library
 **
 **  Distribution: The file is distributed "as is", without any warranty
 **                of any kind.
 **
 **  (c)Copyright Atollic AB.
 **  You may use this file as-is or modify it according to the needs of your
 **  project. This file may only be built (assembled or compiled and linked)
 **  using the Atollic TrueSTUDIO(R) product. The use of this file together
 **  with other tools than Atollic TrueSTUDIO(R) is not permitted.
 **
 *****************************************************************************
 */

/* Includes */
#include <stdio.h>
#include "stm32l1xx.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_gpio.h"
#include "SPI.h"
#include "Flash.h"




/* Private typedef */
/* Private define  */
#define MESSAGE5   " program built with "
#define MESSAGE6   " Atollic TrueSTUDIO "


#define DS1722_CONFIG_ADDR  0x80    /* Configuration Register       */

#define DS1722_DEFAULT      0xE0    /* Bit 5, 6, 7 always 1         */
#define DS1722_1SHOT        0x10    /* Bit 4 one shot               */
#define DS1722_RES_8BIT     0x00    /* Bit 1,2,3 Resolution 8Bit    */
#define DS1722_RES_9BIT     0x02    /* Bit 1,2,3 Resolution 9Bit    */
#define DS1722_RES_10BIT    0x04    /* Bit 1,2,3 Resolution 10Bit   */
#define DS1722_RES_11BIT    0x06    /* Bit 1,2,3 Resolution 11Bit   */
#define DS1722_RES_12BIT    0x08    /* Bit 1,2,3 Resolution 12Bit   */
#define DS1722_SHUTDOWN     (1<<0)  /* Bit 0 shut down              */
#define DS1722_TEMP_LSB     0x01    /* Data register Temp LSB       */
#define DS1722_TEMP_MSB     0x02    /* Data register Temp MSB       */

/* Private macro */
/* Private variables */
USART_InitTypeDef USART_InitStructure;

/* Private function prototypes */
/* Private functions */


/**
 **===========================================================================
 **
 **  Abstract: main program
 **
 **===========================================================================
 */
int main(void)
{
	uint16_t spiData[5];
	uint8_t celsius[5] = {0x66,0xFF,0xC4,0x01,0x80};
	uint8_t eraseFlash[] = {0xc7, 0x94, 0x80, 0x9a};
	uint8_t size = 5;

	/* Initialize SPI */
	teltronic_SPI_Init();

	TELTRONIC_SPI_CS_Out(3, Bit_RESET);

	for (;;) {
		/* Enable Chip Select at SPI1 */
		TELTRONIC_SPI_CS_Out(1, Bit_RESET);

		writeToFlash(celsius,size);

		/* Disable Chip Select at SPI1 */
		TELTRONIC_SPI_CS_Out(1, Bit_SET);


		/* Enable Chip Select at SPI1 */
		TELTRONIC_SPI_CS_Out(1, Bit_RESET);

		readFromFlash(&spiData[0],size);

	}
	return 0;
}

