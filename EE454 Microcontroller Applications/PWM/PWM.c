// Clayton Brutus
// EE454
// PWM blue LED (on PD15)
// uses TIM4 channel 4

#include "stm32f407vg.h"
#define CH 700

int change = CH; // change in breathing LED target count

int main()
{
	RCC_AHB1ENR |= (1 << 3);	// GPIOD clock enable
	RCC_APB1ENR |= (1 << 2);	// TIM4 clock enable
	
	NVICISER0 |= (1 << 30); // TIM4 global interrupt enable
	TIM4_DIER |= 1; // update interrupt enable
	TIM4_DIER |= (1 << 6); // trigger interrupt enable
	
	TIM4_CR1 |= (1 << 7); // auto reload enable
	TIM4_PSC = 4; // prescaler
	TIM4_ARR = 64000; // auto reload (20ms)
	TIM4_CCMR1 |= 0x6868; // channels 1, 2 PWM mode 1
	TIM4_CCMR2 |= 0x6868; // channels 3, 4 PWM mode 1
	TIM4_CCER |= 0x1111; // Channels 1, 2, 3, 4 output enable
	TIM4_CCR1 = 64000; // Channel 1 target (PD12 - Green) (breath)
	TIM4_CCR2 = 48000; // Channel 2 target (PD13 - Orange) (75% duty cycle)
	TIM4_CCR3 = 16000; // Channel 3 target (PD14 - Red) (25% duty cycle)
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
	// if duty cycle is 100%
	if (TIM4_CCR1 >= 64000)
		// start decreasing it
		change = -CH;
	// if duty cycle is 0%
	else if (TIM4_CCR1 <= 1000)
		// start increasing it
		change = CH;
	
	// change PD12 duty cycle
	TIM4_CCR1 += change;
			
	TIM4_SR &= 0xFFFE; // turn interrupt off (turns off timer)
	TIM4_CR1 |= 1; // restart timer
}
