/*
	Clayton Brutus
	Assignment 6
	Digital Systems
	
  Connect six LEDs to bit PB0 to PB5.  Connect a potentiometer that varies from 0 to 3.3 volts to PA5.
	The program turns on a single LED on GPIOB and moves it left if
	PA5 is less than about 0.8 volts and moves it right if the ADC indicates a
	voltage on PA5 greater than 2.4 volts
*/
#include "stm32f407vg.h"

int GetAnalogIn(void);
void Delay(int d);
void TurnOnLEDAt(int bitNum);

int main()
  {
		int tmp, curr;
		
		// Set up ADC on PA5
     RCC_AHB1ENR |= 1;         	//Bit 0 is GPIOA clock enable bit 
     RCC_APB2ENR |= 0x100;     	//Bit 8 is ADC 1 clock enable bit
     GPIOA_MODER |= 0xF00;      //PA4-PA5 are analog
     GPIOA_PUPDR &= 0xFFFFF0FF; //Pins PA4 PA5 are no pull up and no pull down
     DAC_CR |= 0x3E;          	//Bits 3, 4, 5 = 111 for software trigger ch1
																//Bit 2 = 1 for Ch 1 trigger enabled
																//Bit 1 = 1 for Ch 1 output buffer enabled
     DAC_CR |= 1;              	//Bit 0 = 1 for Ch 1 enabled
     ADC_CR2 |= 1;             	//Bit 0 turn ADC on
     ADC_CR2 |= 0x400;         	//Bit 10 allows EOC to be set after conversion
     ADC_CCR |= 0x30000;      	//Bits 16 and 17 = 11 so clock divided by 8
     ADC_SQR3 |= 0x5;          	//Bits 4:0 are channel # for first conversion
																// Channel is set to 5 which corresponds to PA5
		
		// Setup PB0 - PB5 for output
		RCC_AHB1ENR |= 0x2; 				// bit 1 is GPIOB clock enable bit
		GPIOB_MODER &= 0xFFFFF000;
		GPIOB_MODER |= 0x555;				// set mode to general output
		
		// Setup timer 3
		RCC_APB1ENR |= 2; 			// enable peripheral timer for time 3 (bit 1)
		TIM3_CR1 |= (1 << 7); 	// auto reload is buffered
		TIM3_CR1 |= (1 << 3);		// one pulse mode is on
		TIM3_PSC = 0;						// no clock prescale
		TIM3_ARR = 16000;				// 16MHz / 16000 = 1 kHz (1ms)			
		
		// turn on LED at PB0 first
		TurnOnLEDAt(0);
		curr = 0;
		
		while(1)
		{
			tmp = GetAnalogIn();
			
			if (tmp < 993)
				// left
				curr++;
				
			else if (tmp > 2978)
				// right
				curr--;
			
			if (curr < 0)
				curr = 5;
			else if (curr > 5)
				curr = 0;
			
			TurnOnLEDAt(curr);
			
			Delay(100);
		}
  }
	
	int GetAnalogIn(void)
	{
		ADC_CR2 |= 0x40000000;   // Bit 30 does software start of A/D convert
		
		while((ADC_SR & 0x2) == 0); //Bit 1 is End of Conversion
		
		return ADC_DR;
	}
	
	void Delay(int d)
	{
		int i;
		
		for (i = 0; i < d; i++)
		{
			TIM3_CR1 |= 1;									// enable timer 3
			while ((TIM3_CR1 & 1) != 0);		// wait for timer to time out
		}
	}
	
	void TurnOnLEDAt(int bitNum)
	{
		GPIOB_ODR = ~(1 << bitNum);
	}
