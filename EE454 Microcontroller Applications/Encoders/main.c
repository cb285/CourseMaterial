// Clayton Brutus

#include "stm32f407vg.h"

int count = 0;

int main()
{
	RCC_AHB1ENR |= 1; 				// GPIOA clock enable
	RCC_AHB1ENR |= (1 << 3); 	// GPIOD clock enable
	
	RCC_APB2ENR |= (1 << 14); // SYSCFG clock enable
	
	NVICICER0 |= (1 << 6); // EXTI Line0 interrupt enable
	
	SYSCFG_EXTICR1 &= 0xFFFFFFF0; // EXTI0 set to PA0
	
	// PA0 input by default
	GPIOD_MODER |= 0x1000000; // PD12 to output
	
	EXTI_IMR |= 1;  // don't mask interrupt request
	
	EXTI_RTSR |= 1; // rising trigger enable
	
	while(1);
}

void EXTI0_IRQHandler() // interrupt handler
{
	count++; // increment counter variable
	
	if ((count % 10) == 0)
		GPIOD_ODR ^= (1 << 12); // flip output every 10 counts
	
	if (count == 101)
		count = 1; // reset count
	
	EXTI_PR |= 1; // clear flag
}
