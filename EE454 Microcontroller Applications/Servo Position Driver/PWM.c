// Clayton Brutus
// EE454
// servo position driver

#include "stm32f407vg.h"

int high_time = 0;

int main()
{
	RCC_AHB1ENR |= (1 << 3);	// GPIOD clock enable
	RCC_AHB1ENR |= 1; 				// GPIOA clock enable
	RCC_APB1ENR |= (1 << 2);	// TIM4 clock enable
	
	NVICISER0 |= (1 << 30); // TIM4 global interrupt enable
	TIM4_DIER |= 1; // update interrupt enable
	TIM4_DIER |= (1 << 6); // trigger interrupt enable
	
	RCC_APB2ENR |= (1 << 8); // ADC 1 clock enable bit
	ADC1_CR2 |= 1;       		 // Bit 0 turn ADC on
	ADC1_CR2 |= (1 << 10); 	 // Bit 10 allows EOC to be set after conversion
	ADC_CCR |= 0x30000; 		// Bits 16 and 17 = 11 so clock divided by 8
	ADC1_SQR3 |= 0x5;        // Bits 4:0 are channel # for first conversion
														// Channel is set to 5 which corresponds to PA5
	
	GPIOA_MODER |= 0xC00; // PA5 Analog
	GPIOA_PUPDR &= 0xFFFFF3FF; // no PUPD on PA5
	
	TIM4_CR1 |= (1 << 7); // auto reload enable
	TIM4_PSC = 4; // prescaler
	TIM4_ARR = 64000; // auto reload (20ms)
	TIM4_CCMR1 |= 0x6868; // channels 1, 2 PWM mode 1
	TIM4_CCMR2 |= 0x6868; // channels 3, 4 PWM mode 1
	TIM4_CCER |= 0x1111; // Channels 1, 2, 3, 4 output enable
	TIM4_CCR4 = 6400; // Channel 4  target (PD15 - Blue) (10% duty cycle)
	TIM4_EGR |= 1; // load timer registers
	
	TIM4_CR1 |= 1; // enable timer 4
	
	GPIOD_MODER &= 0x00FFFFFF;
	GPIOD_MODER |= 0xAA000000; // PD12, 13, 14, 15 to alternate function mode
	GPIOD_OTYPER &= 0x0FFF; // no pupd
	
	GPIOD_OSPEEDER &= 0x00FFFFFF;
	GPIOD_OSPEEDER |= 0xFF000000;
	GPIOD_AFRH &= 0x0000FFFF; // alternate function reg
	GPIOD_AFRH |= 0x22220000; // AF2 (TIM4) for PD12, 13, 14, 15
	
	while(1);
}

void TIM4_IRQHandler() // interrupt handler
{
	ADC1_CR2 |= (1 << 30);   // Bit 30 does software start of A/D convert
		
	while((ADC1_SR & 0x2) == 0); // Bit 1 is End of Conversion

	high_time = 2880 + ((ADC1_DR * 94) / 100); // must scale
	
	TIM4_CCR4 = high_time;
	
	TIM4_SR &= 0xFFFE; // turn interrupt off (turns off timer)
	TIM4_CR1 |= 1; // restart timer
}
