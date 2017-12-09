// Clayton Brutus
// EE454
// Blinks onboard LED's at 1 Hz
#include "stm32f407vg.h"

int milliseconds = 0;
unsigned long seconds = 0;

int main()
{
	RCC_AHB1ENR |= 8;					// GPIOD clock enable
	RCC_APB1ENR |= 2; 				// TIM3 clock enable
	
	NVICISER0 |= (1 << 29); // TIM3 global interrupt enable
	TIM3_DIER |= 1; // update interrupt enable
	TIM3_DIER |= (1 << 6); // trigger interrupt enable
	
	TIM3_CR1 |= (1 << 7); // auto reload enable
	TIM3_PSC = 0; // prescaler
	TIM3_ARR = 16000; // auto reload (1ms)
	TIM3_CR1 |= 1; // enable timer 3
	
	GPIOD_MODER &= 0x55FFFFFF;
	GPIOD_MODER |= 0x55000000;
	GPIOD_OTYPER &= 0x0FFF;
	
	GPIOD_OSPEEDER &= 0x00FFFFFF;
	GPIOD_PUPDR &= 0xAAFFFFFF;
	GPIOD_PUPDR |= 0xAA000000;
	
	GPIOD_ODR = 0xF000;
	
	while(1);
}

void TIM3_IRQHandler() // interrupt handler
{
	milliseconds++;
	
	if (milliseconds > 999)
	{
		milliseconds = 0;
		seconds++;
		
		GPIOD_ODR ^= 0xF000;
	}
	
	TIM3_SR &= 0xFFFE; // turn interrupt off (turns off timer)
	TIM3_CR1 |= 1; // restart timer
}
