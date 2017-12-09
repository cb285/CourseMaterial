// Clayton Brutus
// EE454
// birthday LEDs using CMSIS
// birthday: 02/24/1996

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "stm32f4xx.h"

#define DELAY 500

void Blinky(void const *argument)
{
	char flag = 1;
	
	while(1)
	{
		if (GPIOA->IDR & 1) // if button pressed, flip from date to year
			flag ^= 1;
		
		if (flag)
		{
			// 0
			osDelay(DELAY);
			// 2
			GPIOD->BSRR |= GPIO_BSRR_BS_13;
			osDelay(DELAY);
			GPIOD->BSRR |= GPIO_BSRR_BR_13;
			// 2
			GPIOD->BSRR |= GPIO_BSRR_BS_13;
			osDelay(DELAY);
			GPIOD->BSRR |= GPIO_BSRR_BR_13;
			// 4
			GPIOD->BSRR |= GPIO_BSRR_BS_14;
			osDelay(DELAY);
			GPIOD->BSRR |= GPIO_BSRR_BR_14;
		}
		
		else
		{
			// 1
			GPIOD->BSRR |= GPIO_BSRR_BS_12;
			osDelay(DELAY);
			GPIOD->BSRR |= GPIO_BSRR_BR_12;
			// 9
			GPIOD->BSRR |= GPIO_BSRR_BS_15;
			GPIOD->BSRR |= GPIO_BSRR_BS_12;
			osDelay(DELAY);
			GPIOD->BSRR |= GPIO_BSRR_BR_15;
			GPIOD->BSRR |= GPIO_BSRR_BR_12;
			// 9
			GPIOD->BSRR |= GPIO_BSRR_BS_15;
			GPIOD->BSRR |= GPIO_BSRR_BS_12;
			osDelay(DELAY);
			GPIOD->BSRR |= GPIO_BSRR_BR_15;
			GPIOD->BSRR |= GPIO_BSRR_BR_12;
			// 6
			GPIOD->BSRR |= GPIO_BSRR_BS_13;
			GPIOD->BSRR |= GPIO_BSRR_BS_14;
			osDelay(DELAY);
			GPIOD->BSRR |= GPIO_BSRR_BR_13;
			GPIOD->BSRR |= GPIO_BSRR_BR_14;
			osDelay(DELAY);
		}
	}
}

osThreadDef (Blinky, osPriorityNormal, 1, 0);

int main (void)
{
	osKernelInitialize();
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= GPIO_MODER_MODER12_0;
	GPIOD->MODER |= GPIO_MODER_MODER13_0;
	GPIOD->MODER |= GPIO_MODER_MODER14_0;
	GPIOD->MODER |= GPIO_MODER_MODER15_0;
	
	osThreadCreate(osThread (Blinky), NULL);
	osKernelStart();
}
