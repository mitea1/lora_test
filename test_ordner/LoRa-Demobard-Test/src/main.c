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
#include <stddef.h>
#include <stdio.h>
#include "stm32l1xx.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_gpio.h"
#include "UART_Print.h"
#include "Light_Sensor.h"



/* Private typedef */
/* Private define  */
  #define MESSAGE5   " program built with "
  #define MESSAGE6   " Atollic TrueSTUDIO "


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
  volatile uint16_t i = 0;
  char  string[]="Hello World\n";
  GPIO_InitTypeDef GPIOInit;
  /* TODO - Add your application code here */

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_StructInit(&GPIOInit);
  GPIOInit.GPIO_Pin = GPIO_Pin_15;
  GPIOInit.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init((GPIO_TypeDef*)GPIOA_BASE, &GPIOInit);

  Usart1Init();

  test();


  /* Infinite loop */
  while (1)
  {


	i++;
	if (i == 0) {


//		GPIO_ToggleBits((GPIO_TypeDef*)GPIOA_BASE, GPIO_Pin_15);
//		while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
//		UART1_Write(string);
	}

  }
  return 0;
}


/*
 * Minimal __assert_func used by the assert() macro
 * */
void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
  while(1)
  {}
}

/*
 * Minimal __assert() uses __assert__func()
 * */
void __assert(const char *file, int line, const char *failedexpr)
{
   __assert_func (file, line, NULL, failedexpr);
}
