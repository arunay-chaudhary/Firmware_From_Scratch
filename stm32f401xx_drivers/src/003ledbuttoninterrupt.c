/*
 * 003ledbuttoninterrupt.c
 *
 *  Created on: 13 Oct 2019
 *      Author: aruna
 */

#include "stm32f401xx.h"
#include <string.h>




#define LD2 GPIO_PIN_NO_5

/* Interrupt Service Routine */
void EXTI15_10_IRQHandler(void);

/* Function Prototypes */
void delay(void);
void config_leds(GPIO_Handle_t *GpioLed);
void config_pb(GPIO_Handle_t *GpioPb);

int main(void)
{

	GPIO_Handle_t GpioLed,GpioPb;

	memset(&GpioLed, 0, sizeof(GpioLed));
	memset(&GpioPb, 0, sizeof(GpioPb));

	config_leds(&GpioLed);
	config_pb(&GpioPb);



	while(1)
	{


	}
	return 0;
}

void delay(void)
{
	for(int i = 0; i< 250000; i++){}
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
	GpioPb->GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
	GpioPb->GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioPb->GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(GpioPb);

	GPIO_IRQInterruptConfig(IRQ_NO_EXTI15_10, ENABLE);
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI15_10, NVIC_IRQ_PRI15);

}


void EXTI15_10_IRQHandler(void)
{
	delay();
	GPIO_IRQHandling(GPIO_PIN_NO_13); //Clears the PR
	GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_5);
}


