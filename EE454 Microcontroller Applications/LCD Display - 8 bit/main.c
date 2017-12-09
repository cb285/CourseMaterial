// EE454 LCD Display

// DB0-7 -> PB0-7
// RS -> PA0
// R/W -> Low
// E -> PC0
#include "stm32f407vg.h"

#define MAX 100

void Delay(int t);
void Command(char comm);
void Write(char word[MAX]);

int main()
{
	RCC_AHB1ENR |= 1; 					// PA clock enable
	RCC_AHB1ENR |= 2;						// PB clock enable
	RCC_AHB1ENR |= 4;						// PC clock enable
	GPIOA_MODER &= 0xFFFFFFFC;	// clear mode for PA0
	GPIOA_MODER |= 0x1;					// set PA0 to output
	GPIOB_MODER &= 0xFFFF0000; 	// clear mode for PB0-7
	GPIOB_MODER |= 0x5555; 			// set PB0-7 to output
	GPIOC_MODER &= 0xFFFFFFFC; 	// clear PC0 mode
	GPIOC_MODER |= 1;						// PC0 to output
	
	
	Command(0x0F); // turn on display, cursor, and cursor blinking
	Command(0x1); // clear display
	Command(0x38); // enable 2 line mode
	Command(0x01); // clear screen, cursor home
	Command(0x06); // increment cursor to right when writing, don't shift screen
	
	Write("Hello World!"); // write word
	
	while (1); // do nothing
}

void Command(char comm)
{
	GPIOC_ODR = 0; // E low
	Delay(5);
	
	GPIOA_ODR = 0; 	// command mode
	
	GPIOB_ODR = comm; // send command
	
	GPIOC_ODR = 1; // E high
	Delay(5);
	
	GPIOC_ODR = 0; // E low
}

void Write(char word[MAX])
{
	GPIOC_ODR = 0; // E low
	Delay(1);
	
	GPIOA_ODR = 1; // RS High -> data mode
	
	int i = 0;
	
	while (word[i] != '\0')
	{		
		Delay(1);
		
		// for 4 bit mode:
		/*
		nibble = (word[i] >> 4); 	// upper nibble
															// send upper nibble
		nibble = (word[i] & 0xF) 	// lower nibble
															// send lower nibble
		*/
		GPIOB_ODR = word[i]; // write character
		GPIOC_ODR = 1; // E high
		
		Delay(1);
		
		GPIOC_ODR = 0; // E low
		
		i++;
	}
}

void Delay(int t)
{
	for (int i = 0; i < t; i++)
		for (int j = 0; j < 100; j++)
			for (int x = 0; x < 1000; x++);
}
