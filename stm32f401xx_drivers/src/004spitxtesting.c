/*
 * 004spitxtesting.c
 *
 *  Created on: 13 Oct 2019
 *      Author: aruna
 */

#include <string.h>

#include "stm32f401xx.h"
#include "stm32f401xx_gpio_driver.h"
#include "stm32f401xx_spi_driver.h"

void SPI2_GPIOInit(void);
void SPI2Inits(void);
/*
 * SPI2 PINS SELECTED
 *
 * PB12: SPI2_NSS
 * PB13: SPI2_SCK
 * PB14: SPI2_MISO
 * PB15: SPI2_MOSI
 * ALTFN_MODE: 5
 *
 * */



int main(void)
{
	char user_data[] = "Hello World!";

	//Initialise SPI GPIO pins
	SPI2_GPIOInit();

	//Initialise the SPI2 peripheral
	SPI2Inits();

	SPI_SSIConfig(SPI2, ENABLE);

	//Enable the SPI enable bit
	SPIPeripheralControl(SPI2, ENABLE);

	SPI_SendData(SPI2, (uint8_t*)user_data,strlen(user_data));

	while(SPI_GetFlagStatus(SPI2, SPI_BSY_FLAG) == SPI_BSY); //Wait
	SPIPeripheralControl(SPI2, DISABLE);
	while(1);

	return 0;
}

void SPI2_GPIOInit(void)
{
	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOB;

	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//CLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	//MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);


	/*
	 * NOT USED IN THIS TEST
	//MISO
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);

	//NSS
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);
	*/
}

void SPI2Inits(void)
{
	SPI_Handle_t SPI2Handle;

	SPI2Handle.pSPIx = SPI2;

	SPI2Handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2Handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2Handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV2;
	SPI2Handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2Handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2Handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2Handle.SPIConfig.SPI_SSM = SPI_SSM_EN;
	SPI2Handle.SPIConfig.SPI_SSI = SPI_SSI_EN;

	SPI_Init(&SPI2Handle);
}
