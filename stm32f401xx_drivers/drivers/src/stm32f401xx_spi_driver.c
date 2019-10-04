/*
 * stm32f4xx_spi_driver.c
 *
 *  Created on: 15 Sep 2019
 *      Author: aruna
 */

#include <stm32f401xx_spi_driver.h>

/*
 * Peripheral Clock setup
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_EN();
		}else if (pSPIx == SPI2)
		{
			SPI2_PCLK_EN();
		}else if (pSPIx == SPI3)
		{
			SPI3_PCLK_EN();
		}else if (pSPIx == SPI4)
		{
			SPI4_PCLK_EN();
		}
	}
	else
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_DI();
		}else if (pSPIx == SPI2)
		{
			SPI2_PCLK_DI();
		}else if (pSPIx == SPI3)
		{
			SPI3_PCLK_DI();
		}else if (pSPIx == SPI4)
		{
			SPI4_PCLK_DI();
		}
	}
}

/*
 * Init and De-init
 */
void SPI_Init(SPI_Handle_t *pSPIHandle)
{
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	GPIO_Handle_t SPIPins;
=======
	uint32_t temp = 0;
>>>>>>> parent of 13e1cfe... Updated SPI4 not finished though

	//Config device mode

	temp |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR;

	//Config Bus
	if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		temp &= ~(1 << SPI_CR1_BIDIMODE);
	} else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		temp |= (1 << SPI_CR1_BIDIMODE);
	} else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
	{
		temp &= ~(1 << SPI_CR1_BIDIMODE);
		temp |= (1 << SPI_CR1_RXONLY);
	}

	//Config Clock Speed
	temp |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR;

	//Config DFF
	temp |= (pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF);

    //Config CPOL
	temp |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;

<<<<<<< HEAD
=======
>>>>>>> parent of 7f87328... Added SPI_Init(), started SendData
=======
>>>>>>> parent of 7f87328... Added SPI_Init(), started SendData
=======
	//Config CPHA
	temp |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;
>>>>>>> parent of 13e1cfe... Updated SPI4 not finished though

	pSPIHandle->pSPIx->CR1 = temp;




<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	SPI_Init(&SPI2handle);
=======
>>>>>>> parent of 13e1cfe... Updated SPI4 not finished though
}


void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
<<<<<<< HEAD
	char user_data[] = "Hello world";
=======
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
>>>>>>> parent of 7f87328... Added SPI_Init(), started SendData
=======
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
>>>>>>> parent of 7f87328... Added SPI_Init(), started SendData
=======
>>>>>>> parent of 13e1cfe... Updated SPI4 not finished though

}

/*
 * Data Send and Receive
 */

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName)
{
	if(pSPIx->SR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		while(SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);
	}

}
void SPI_ReceiveData (SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len)
{

}

/*
 * IRQ Configuration and ISR handling
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{

}

void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{

}

void SPI_IRQHandling(SPI_Handle_t *pSPIHandle)
{

}
