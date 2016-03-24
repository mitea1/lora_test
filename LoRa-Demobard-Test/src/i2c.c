
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*----- Header-Files -------------------------------------------------------*/
#include <stm32l1xx.h>             	/* Processor STM32               */
#include <teltronic.h>          	/* Teltronic Module                         */
#include <i2c.h>

/*----- Macros -------------------------------------------------------------*/
#define TIMEOUT_MAX         0x3000
/*----- Data types ---------------------------------------------------------*/

/*----- Function prototypes ------------------------------------------------*/
static void TELTRONIC_I2C_Settings(I2C_TypeDef *I2Cx);
static ERROR_CODES TELTRONIC_I2C_Timeout(I2C_TypeDef* I2Cx);

/*----- Data ---------------------------------------------------------------*/
static TELTRONIC_Port_Pin_t TELTRONIC_I2C_Port_Pin[] = {
/* TELTRONIC I2C Board */
{ GPIOB, GPIO_Pin_10, GPIO_Mode_AF, GPIO_AF_I2C2 },
{ GPIOB, GPIO_Pin_11, GPIO_Mode_AF, GPIO_AF_I2C2 }
};

static I2C_InitTypeDef I2C_InitStruct[2];

/*----- Implementation -----------------------------------------------------*/
void TELTRONIC_I2C_Init(I2C_TypeDef *I2Cx) {

   GPIO_InitTypeDef GPIO_InitStruct;


   //RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);

   /* Configure the GPIO */
   GPIO_StructInit(&GPIO_InitStruct);
   GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
   GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
   GPIO_Init(GPIOB, &GPIO_InitStruct);

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

   /* Configure the I2C */
   if (I2Cx == I2C2 ) {
	   I2C_StructInit(&I2C_InitStruct[0]);
	   I2C_InitStruct[0].I2C_ClockSpeed = 100000;
	   I2C_InitStruct[0].I2C_OwnAddress1 = 0x00;
	   I2C_InitStruct[0].I2C_Ack = I2C_Ack_Enable;
   }
   TELTRONIC_I2C_Settings(I2Cx);
}

static void TELTRONIC_I2C_Settings(I2C_TypeDef *I2Cx) {

   uint8_t i;

   if (I2Cx == I2C2 ) {
	   I2C_Cmd(I2C2, DISABLE);
	   I2C_DeInit(I2C2 );
	   I2C_Init(I2Cx, &I2C_InitStruct[0]);
	   I2C_Cmd(I2Cx, ENABLE);
   }

   i = 0xFF;
   while (i--)
	   /* Wait until hardware is running */
	   ;
}

static ERROR_CODES TELTRONIC_I2C_Timeout(I2C_TypeDef* I2Cx) {

   I2C_GenerateSTOP(I2Cx, ENABLE);
   I2C_SoftwareResetCmd(I2Cx, ENABLE);
   I2C_SoftwareResetCmd(I2Cx, DISABLE);
   TELTRONIC_I2C_Settings(I2Cx);

   return CARME_ERROR_I2C_TIMEOUT;
}

ERROR_CODES TELTRONIC_I2C_Write(I2C_TypeDef *I2Cx, uint8_t addr, uint16_t reg,
	   uint8_t twoByte, uint8_t *pdata, uint16_t count) {

   uint32_t timeout = TIMEOUT_MAX;
   uint16_t i;

   while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY )) {
	   if (--timeout == 0) {
		   return TELTRONIC_I2C_Timeout(I2Cx);
	   }
   }

   I2C_GenerateSTART(I2Cx, ENABLE);
   timeout = TIMEOUT_MAX;
   while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT )) {
	   if (--timeout == 0) {
		   return TELTRONIC_I2C_Timeout(I2Cx);
	   }
   }

   I2C_Send7bitAddress(I2Cx, addr, I2C_Direction_Transmitter );
   timeout = TIMEOUT_MAX;
   while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED )) {
	   if (--timeout == 0) {
		   return TELTRONIC_I2C_Timeout(I2Cx);
	   }
   }

   if (twoByte == 1) {
	   I2C_SendData(I2Cx, (uint8_t) (reg >> 8));
	   timeout = TIMEOUT_MAX;
	   while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING )) {
		   if (--timeout == 0) {
			   return TELTRONIC_I2C_Timeout(I2Cx);
		   }
	   }

	   I2C_SendData(I2Cx, (uint8_t) reg);
	   timeout = TIMEOUT_MAX;
	   while (!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF )) {
		   if (--timeout == 0) {
			   TELTRONIC_I2C_Timeout(I2Cx);
		   }
	   }
   } else {
	   I2C_SendData(I2Cx, (uint8_t) reg);
	   timeout = TIMEOUT_MAX;
	   while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING )) {
		   if (--timeout == 0) {
			   return TELTRONIC_I2C_Timeout(I2Cx);
		   }
	   }
   }

   for (i = 0; i < count; i++) {
	   I2C_SendData(I2Cx, pdata[i]);
	   timeout = TIMEOUT_MAX;
	   while (!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF )) {
		   if (--timeout == 0) {
			   TELTRONIC_I2C_Timeout(I2Cx);
		   }
	   }
   }

   I2C_GenerateSTOP(I2Cx, ENABLE);
   return TELTRONIC_NO_ERROR;
}

ERROR_CODES TELTRONIC_I2C_Read(I2C_TypeDef *I2Cx, uint8_t addr, uint16_t reg,
	   uint8_t twoByte, uint8_t *pdata, uint16_t count) {

   uint32_t timeout = TIMEOUT_MAX;
   uint16_t i;


   while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY )) {
	   if (--timeout == 0) {
		   return TELTRONIC_I2C_Timeout(I2Cx);
	   }
   }

   I2C_GenerateSTART(I2Cx, ENABLE);
   timeout = TIMEOUT_MAX;
   // TODO Check if start condition is generated -> oscilloscope
   // (SR2 << 16 | SR1) = 00030001 := SB=1 MSL=1 BUSY=1
   // Problem SR1: SB=0
   while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT )) {
	   if (--timeout == 0) {
		   return TELTRONIC_I2C_Timeout(I2Cx);
	   }
   }

   I2C_Send7bitAddress(I2Cx, addr, I2C_Direction_Transmitter );
   timeout = TIMEOUT_MAX;
   while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED )) {
	   if (--timeout == 0) {
		   return TELTRONIC_I2C_Timeout(I2Cx);
	   }
   }

   if (twoByte == 1) {
	   I2C_SendData(I2Cx, (uint8_t) (reg >> 8));
	   timeout = TIMEOUT_MAX;
	   while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING )) {
		   if (--timeout == 0) {
			   return TELTRONIC_I2C_Timeout(I2Cx);
		   }
	   }

	   I2C_SendData(I2Cx, (uint8_t) reg);
	   timeout = TIMEOUT_MAX;
	   while (!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF )) {
		   if (--timeout == 0) {
			   TELTRONIC_I2C_Timeout(I2Cx);
		   }
	   }
   } else {
	   I2C_SendData(I2Cx, (uint8_t) reg);
	   timeout = TIMEOUT_MAX;
	   while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING )) {
		   if (--timeout == 0) {
			   return TELTRONIC_I2C_Timeout(I2Cx);
		   }
	   }
   }

   I2C_GenerateSTART(I2Cx, ENABLE);
   timeout = TIMEOUT_MAX;
   while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT )) {
	   if (--timeout == 0) {
		   return TELTRONIC_I2C_Timeout(I2Cx);
	   }
   }

   I2C_Send7bitAddress(I2Cx, addr, I2C_Direction_Receiver );
   timeout = TIMEOUT_MAX;
   while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDR ) == RESET) {
	   if (--timeout == 0) {
		   return TELTRONIC_I2C_Timeout(I2Cx);
	   }
   }

   for (i = 0; i < count - 1; i++) {
	   timeout = TIMEOUT_MAX;
	   while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED )) {
		   if (--timeout == 0) {
			   return TELTRONIC_I2C_Timeout(I2Cx);
		   }
	   }
	   pdata[i] = I2C_ReceiveData(I2Cx);
   }

   I2C_AcknowledgeConfig(I2Cx, DISABLE);
   (void) I2Cx->SR2;

   I2C_GenerateSTOP(I2Cx, ENABLE);
   timeout = TIMEOUT_MAX;
   while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE ) == RESET) {
	   if (--timeout == 0) {
		   return TELTRONIC_I2C_Timeout(I2Cx);
	   }
   }

   pdata[count - 1] = I2C_ReceiveData(I2Cx);
   timeout = TIMEOUT_MAX;
   while (I2Cx->CR1 & I2C_CR1_STOP ) {
	   if (--timeout == 0) {
		   return TELTRONIC_I2C_Timeout(I2Cx);
	   }
   }

   I2C_AcknowledgeConfig(I2Cx, ENABLE);
   I2C_ClearFlag(I2Cx, I2C_FLAG_AF );

   return TELTRONIC_NO_ERROR;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
