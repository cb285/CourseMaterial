// Clayton Brutus
// EE454
// Project 1

// left sensor - PA3
// right sensor - PA5
// Servo signal - PD12

#include "stm32f407vg.h"

#define SENS 100 			// sensitivity (lower => more sensitive)
#define TARGET 450 	// threshold for stopping the search (higher => closer to target) (max: 4095)
#define MOV 40 				// amount to change high_time by (how far servo moves each step)

#define MIN 2880	// min high_time (1.9 ms)
#define MAX 6720  // max high_time (2.1 ms)

int high_time = 3000; // for storing current PWM high time
int change = MOV;			// for storing amount to change high time by

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
	
	GPIOA_MODER |= 0xC00; // PA5 Analog
	GPIOA_MODER |= 0x0C0; // PA3 Analog
	
	GPIOA_PUPDR &= 0xFFFFF3FF; // no PUPD on PA5
	GPIOA_PUPDR &= 0xFFFFFF3F; // no PUPD on PA3
	
	TIM4_CR1 |= (1 << 7); // auto reload enable
	TIM4_PSC = 4; // prescaler
	TIM4_ARR = 64000; // auto reload (20ms)
	TIM4_CCMR1 |= 0x0068; // channels 1 PWM mode 1
	TIM4_CCER |= 1; // Channel 1 output enable
	TIM4_CCR1 = high_time; // Channel 1 target
	TIM4_EGR |= 1; // load timer registers
	
	TIM4_CR1 |= 1; // enable timer 4
	
	GPIOD_MODER &= 0xCFFFFFF;
	GPIOD_MODER |= 0x2000000; // PD12 to alternate function mode
	GPIOD_OTYPER &= 0xFEFF; 	// push pull on PD12
	
	GPIOD_OSPEEDER &= 0xFCFFFFFF;
	GPIOD_OSPEEDER |= 0x03000000; // PD12 high speed
	
	GPIOD_AFRH &= 0xFFF0FFFF;
	GPIOD_AFRH |= 0x20000; // AF2 (TIM4) for PD12
	
	while(1);
}

void TIM4_IRQHandler() // interrupt handler
{
	int leftVal = 0;
	int rightVal = 0;
	
	ADC1_SQR3 &= 0xFFFFFFE0; // clear channel number
	ADC1_SQR3 |= 0x3;        // set channel number to 4 (PA3)
	
	ADC1_CR2 |= (1 << 30);   // Bit 30 does software start of A/D convert
	
	while((ADC1_SR & 0x2) == 0); // Bit 1 is End of Conversion
	
	leftVal = 4095 - ADC1_DR; // get result
	
	ADC1_SQR3 &= 0xFFFFFFE0; // clear channel number
	ADC1_SQR3 |= 0x5;        // set channel number to 5 (PA5)
	
	ADC1_CR2 |= (1 << 30);   // start A/D convert
	
	while((ADC1_SR & 0x2) == 0); // wait for end of conversion
	
	rightVal = 4095 - ADC1_DR; // get result
	
	// if found it
	if (rightVal >= TARGET && leftVal >= TARGET)
	{
		; // don't move
	}
	
	// if found nothing, then search
	if ((rightVal <= SENS) && (leftVal <= SENS))
	{
		// if at min angle, switch direction
		if (high_time <= MIN)
			change = MOV;
		
		// if at max angle switch direction
		else if (high_time >= MAX)
			change = -MOV;
		
		high_time += change; // move by change amount
	}
	
	// if found something left
	else if (leftVal >= rightVal + SENS)
	{
		// if not at min angle, then can move left more
		if (high_time > MIN)
		{
			high_time -= MOV; // move left
			change = -MOV; 		// if loses object then will search left
		}
		
		// otherwise don't move
	}
	
	// if found something right
	else if (rightVal >= leftVal + SENS)
	{
		// if not at max angle, then can move right more
		if (high_time < MAX)
		{
			high_time += MOV; // move right
			change = MOV;			// if loses object then will search right
		}
		
		// otherwise don't move
	}
	
	TIM4_CCR1 = high_time; // change PWM high time
	
	TIM4_SR &= 0xFFFE; // turn interrupt off (turns off timer)
	TIM4_CR1 |= 1; // restart timer
}
