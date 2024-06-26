/*
 * 006spi_tx_rx.c
 *
 *  Created on: 28 Jan 2020
 *      Author: aruna
 */

/*
 * 005spitxtesting.c
 *
 *  Created on: 13 Oct 2019
 *      Author: aruna
 */

#include <stdio.h>
#include <string.h>

#include "stm32f401xx.h"
#include "stm32f401xx_gpio_driver.h"
#include "stm32f401xx_spi_driver.h"

void SPI2_GPIOInit(void);
void SPI2Inits(void);

uint8_t SPI_verify_rsp(uint8_t ack_byte);

void config_pb(void);
void config_leds(void);
void delay(uint32_t time);
void DEBUG_LED(uint8_t ONorOFF);

#define LD2 GPIO_PIN_NO_5

#define LD9 9




#define LED_ON					1
#define LED_OFF					0

#define ACK						0x55

#define ANALOG_PIN0	0
#define ANALOG_PIN1 1
#define ANALOG_PIN2 2
#define ANALOG_PIN3 3
#define ANALOG_PIN4 4
#define ANALOG_PIN5 5

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

typedef enum{
	COMMAND_LED_CTRL = 0x50,
	COMMAND_SENSOR_READ,
	COMMAND_LED_READ,
	COMMAND_PRINT,
	COMMAND_READ
}CommandId;


int main(void)
{

	//Initialise SPI GPIO pins
	SPI2_GPIOInit();

	//Initialise push button
	config_pb();

	//Initialise led
	config_leds();

	//Initialise the SPI2 peripheral
	SPI2Inits();

	//Hardware SS enabled
	SPI_SSOEConfig(SPI2, ENABLE);

	uint8_t dummy_byte = 0xff;
	uint8_t dummy_read;
	uint8_t analog_read;
	uint8_t led_state;

	char *msg = "This is a message";
	uint8_t rx_msg[11];


	while(1)
	{
		while(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13) == ENABLE);
//		DEBUG_LED(LED_OFF);
		delay(500000);
		//Enable LED



		//Enable the SPI enable bit
		SPIPeripheralControl(SPI2, ENABLE);

		CommandId cmd_code = COMMAND_LED_CTRL;
		uint8_t args[2];
		SPI_SendData(SPI2, &cmd_code , 1);

//		//dummy read to clear RXNE
		SPI_ReceiveData(SPI2, &dummy_read,1);

		SPI_SendData(SPI2, &dummy_byte, 1);
		uint8_t ack_byte;
//
		SPI_ReceiveData(SPI2, &ack_byte,1);

		if(SPI_verify_rsp(ack_byte))
		{
			args[0] = LD9;
			args[1] = LED_ON;
			SPI_SendData(SPI2, args, 2);
		}

		//end of LED CTRL
		//CMD Sensor read
		while(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13) == ENABLE);
		delay(500000);

		cmd_code = COMMAND_SENSOR_READ;
		SPI_SendData(SPI2, &cmd_code , 1);

//		//dummy read to clear RXNE
		SPI_ReceiveData(SPI2, &dummy_read,1);

		// send dummy to make slave send ack
		SPI_SendData(SPI2, &dummy_byte, 1);
		ack_byte = 0;
//
		SPI_ReceiveData(SPI2, &ack_byte,1);

		if(SPI_verify_rsp(ack_byte))
		{
			args[0] = ANALOG_PIN0;
			SPI_SendData(SPI2, args, 1);
			delay(500000);

			SPI_ReceiveData(SPI2, &dummy_read,1);
			SPI_SendData(SPI2, &dummy_byte, 1);

			SPI_ReceiveData(SPI2, &analog_read,1);
			if(analog_read)
			{
				DEBUG_LED(LED_ON);
			}
		}
		//wait before disabling!
		while(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13) == ENABLE);
		delay(500000);

		cmd_code = COMMAND_LED_READ;

		SPI_SendData(SPI2, &cmd_code, 1);

		//clear RXNE w/ dummy read
		SPI_ReceiveData(SPI2, &dummy_read, 1);

		//Send dummy data to receiv ack
		SPI_SendData(SPI2, &dummy_byte, 1);

		ack_byte = 0;
		SPI_ReceiveData(SPI2, &ack_byte, 1);

		if(SPI_verify_rsp(ack_byte))
		{
			args[0] = LD9;
			SPI_SendData(SPI2, args, 1);
			SPI_ReceiveData(SPI2, &dummy_read,1);
			SPI_SendData(SPI2, &dummy_byte, 1);


			SPI_ReceiveData(SPI2, &led_state,1);
			if(led_state)
			{
				DEBUG_LED(LED_OFF);
			}

		}

		while(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13) == ENABLE);
		delay(500000);

		cmd_code = COMMAND_PRINT;

		SPI_SendData(SPI2, &cmd_code, 1);

		//clear RXNE w/ dummy read
		SPI_ReceiveData(SPI2, &dummy_read, 1);

		//Send some dummy byte to fetch the response from the slave
		SPI_SendData(SPI2, &dummy_byte, 1);

		ack_byte = 0;
		SPI_ReceiveData(SPI2, &ack_byte, 1);

		if(SPI_verify_rsp(ack_byte))
		{
			args[0] = strlen(msg);
			SPI_SendData(SPI2, args, 1);
			SPI_SendData(SPI2, (uint8_t*)msg, args[0]);


		}

		while(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13) == ENABLE);
		delay(500000);

		cmd_code = COMMAND_READ;

		SPI_SendData(SPI2, &cmd_code, 1);

		//clear RXNE w/ dummy read
		SPI_ReceiveData(SPI2, &dummy_read, 1);

		//Send dummy data to fetch
		SPI_SendData(SPI2, &dummy_byte, 1);

		ack_byte = 0;
		SPI_ReceiveData(SPI2, &ack_byte, 1);

		if(SPI_verify_rsp(ack_byte))
		{

			uint32_t i = 0;
			for(i=0; i < 10;i++)
			{
				//Send dummy data to receive ack
				SPI_SendData(SPI2, &dummy_byte, 1);
				SPI_ReceiveData(SPI2, &rx_msg[i],1);

			}
			rx_msg[11] = '\0';
//			printf("RX_MESSAGE : %s\n", rx_msg);
		}


		while(SPI_GetFlagStatus(SPI2, SPI_BSY_FLAG)); //Wait
		SPIPeripheralControl(SPI2, DISABLE);

	}



	return 0;
}

void SPI2_GPIOInit(void)
{
	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOB;

	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//CLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	//MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);


	//MISO
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);

	//NSS
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);

}

void SPI2Inits(void)
{
	SPI_Handle_t SPI2Handle;

	SPI2Handle.pSPIx = SPI2;

	SPI2Handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2Handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2Handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV4;
	SPI2Handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2Handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2Handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2Handle.SPIConfig.SPI_SSM = SPI_SSM_DI;


	SPI_Init(&SPI2Handle);
}

void config_pb(void)
{
	GPIO_Handle_t GpioPb;

    GpioPb.pGPIOx = GPIOC;
	GpioPb.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GpioPb.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GpioPb.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioPb.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(&GpioPb);


}

void config_leds(void)
{
	GPIO_Handle_t GpioLed;

	GpioLed.pGPIOx = GPIOA;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = LD2;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(&GpioLed);
}

uint8_t SPI_verify_rsp(uint8_t ack_byte)
{
	if (ack_byte == 0xF5)
	{
		return 1;
	}

	return 0;

}

void delay(uint32_t time)
{
	for(int i = 0; i< time; i++){}
}

void DEBUG_LED(uint8_t ONorOFF)
{
	GPIO_WriteToOutputPin(GPIOA, 5, ONorOFF);
}
