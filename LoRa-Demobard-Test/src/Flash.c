/*
 * Flash.c
 *
 *  Created on: Apr 3, 2016
 *      Author: Simon
 */

#include <stdio.h>
#include <time.h>
#include "Flash.h"
#include "SPI.h"
#include <stdint.h>


#define TEST_ADDRESS_1					0x01
#define TEST_PAGE_1						0x00
#define SECTOR_1						0x01
/**
 *
 * @brief 		Send data to flash
 * @param 		data		Address of data to be sent.
 * @param 		size		Number of bytes.
 */
void writeToFlash(uint16_t *data, uint8_t size){
	TELTRONIC_SPI_Send(1, FLASH_OPCODE_MAINP_TO_BUF);
	TELTRONIC_SPI_Send(1, SECTOR_1);
	TELTRONIC_SPI_Send(1, TEST_ADDRESS_1);
	TELTRONIC_SPI_Send(1, TEST_PAGE_1);
	uint16_t i;

	for (i=0;i<size;i++){
		TELTRONIC_SPI_Send(1, data[i]);
	}

}
/**
 *
 * @brief 		Reads from flash
 * @param 		pValue		Address of data to be received.
 * @param 		size		Number of bytes.
 */
void readFromFlash(uint16_t *pValue, uint8_t size){

	TELTRONIC_SPI_Send(1, FLASH_OPCODE_LOWF_READ);
	TELTRONIC_SPI_Send(1, SECTOR_1);
	TELTRONIC_SPI_Send(1, TEST_ADDRESS_1);
	TELTRONIC_SPI_Send(1, TEST_PAGE_1);

	TELTRONIC_SPI_Receive(1, &pValue[2]);

}
