/*
 * 002ledbutton.c
 *
 *  Created on: 6 Sep 2019
 *      Author: aruna
 */

#include <stm32f401xx.h>

#define LD2 GPIO_PIN_NO_5

void delay(void);
void config_leds(GPIO_Handle_t *GpioLed);
void config_pb(GPIO_Handle_t *GpioPb);

int main(void)
{

	GPIO_Handle_t GpioLed;
	config_leds(&GpioLed);

	GPIO_Handle_t GpioPb;
	config_pb(&GpioPb);


	while(1)
	{
		if(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13)== DISABLE)
		{
			delay();
			GPIO_ToggleOutputPin(GPIOA,GPIO_PIN_NO_5);
		}else
		{
			GPIO_WriteToOutputPin(GPIOA,GPIO_PIN_NO_5, DISABLE);
		}

	}
	return 0;
}

void delay(void)
{
	for(int i = 0; i< 500000; i++){}
}

void config_leds(GPIO_Handle_t *GpioLed)
{
	GPIO_PeriClockControl(GPIOA,ENABLE);

	GpioLed->pGPIOx = GPIOA;
	GpioLed->GPIO_PinConfig.GPIO_PinNumber = LD2;
	GpioLed->GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed->GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;
	GpioLed->GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLed->GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(GpioLed);
}

void config_pb(GPIO_Handle_t *GpioPb)
{
	GPIO_PeriClockControl(GPIOC,ENABLE);

	GpioPb->pGPIOx = GPIOC;
	GpioPb->GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GpioPb->GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GpioPb->GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioPb->GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(GpioPb);
}
