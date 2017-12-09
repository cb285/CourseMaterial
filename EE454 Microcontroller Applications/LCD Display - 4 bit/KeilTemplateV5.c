/* 
	EE454 LCD Display

	DB4-7 -> PB0-3
	RS 		-> PA0
	R/W 		-> PA1
	E 			-> PA2

 */
	
#include "stm32f407vg.h"

#define MAX 100						// max message length

void LCDSetUp(void);
void LCDCommand(char comm);
void LCDWrite(char word[MAX]);
void SetE(char e);
void LCDWait(void);
void LCDClear(void);
void Delay(int t);

int main()
{
	RCC_AHB1ENR |= 1; 					// PA clock enable
	RCC_AHB1ENR |= 2;						// PB clock enable
	GPIOA_MODER &= 0xFFFFFFC0;	// clear mode for PA0-2
	GPIOA_MODER |= 0x15;				// set PA0-2 to output
	GPIOB_MODER &= 0xFFFF00; 		// clear mode for PB0-3
	GPIOB_MODER |= 0x55; 				// set PB0-3 to output
	
	Delay(5);										// wait for screen to power on
	
	LCDCommand(0x33);
	LCDCommand(0x32);						// setup LCD for 4 bit mode
	
	LCDCommand(0x2C); 					// 2 lines, 5x11 font
	LCDCommand(0x06); 					// increment cursor to right when writing, don't shift screen
	LCDCommand(0x0F); 					// turn on display, cursor, and cursor blinking
	
	LCDClear();									// clear and return cursor to home
	LCDWrite("Hello World!"); 	// display message
	
	while (1); // do nothing
}

void LCDClear()
{
	LCDCommand(0x01);
}

void LCDSetUp()
{
	GPIOA_ODR &= 0xFFF8; // RS low (command), R/W low (write), E low
	
	GPIOB_ODR &= 0xFFF0;
	GPIOB_ODR |= 0x3;
	
	SetE(1); // E high
	Delay(5); // delay 5 ms
	
	SetE(0);	// E low
	Delay(5);	// delay 5 ms
	
	GPIOB_ODR &= 0xFFF0;
	GPIOB_ODR |= 0x3;
	
	SetE(1);	// E high
	Delay(5); // delay 5 ms

	SetE(0);	// E low
	Delay(5);	// delay 5 ms
	
	GPIOB_ODR &= 0xFFF0;
	GPIOB_ODR |= 0x3;
	
	SetE(1);
	Delay(5); // delay 5 ms
	
	SetE(0);	// E low
	Delay(5);	// delay 5 ms
	
	GPIOB_ODR &= 0xFFF0;
	GPIOB_ODR |= 0x2; 		// 4 bit mode
	
	SetE(1); //	E high
	Delay(5); // delay 5 ms
	
	SetE(0);	// E low
	Delay(5); // delay 5 ms
	
	// now in 4 bit mode
}

void LCDCommand(char comm)
{
	char nibble = 0;
	
	SetE(0); // E low
	LCDWait();
	
	GPIOA_ODR = 0; 	// command mode
	
	nibble = comm >> 4; // get upper nibble
	
	GPIOB_ODR &= 0xFFF0;
	GPIOB_ODR |= nibble; // send upper nibble
	
	SetE(1); // E high
	LCDWait();
	
	SetE(0); // E low
	LCDWait();
	
	nibble = comm & 0xF; // get lower nibble
	GPIOB_ODR &= 0xFFF0;
	GPIOB_ODR |= nibble; // send lower nibble
	
	SetE(1); // E high
	LCDWait();
	
	SetE(0); // E low
	LCDWait();
}

void LCDWrite(char word[MAX])
{	
	SetE(0); // E low
	//LCDWait();
	Delay(2);
	
	GPIOA_ODR |= 1; // RS High -> data mode
	
	char nibble = 0;
	
	int i = 0;
	
	while (word[i] != '\0')
	{	
		// check if need to move to next line
		if (i == 16)
		{
			LCDCommand(0xC0);	// move cursor to next line
		}
		
		nibble = (word[i] >> 4); 	// get upper nibble
		GPIOB_ODR &= 0xFFF0;
		GPIOB_ODR |= nibble; 			// send upper nibble
		
		SetE(1); 									// E high
		LCDWait();
		SetE(0);									// E low
		LCDWait();
		
		nibble = (word[i] & 0xF); // get lower nibble
		GPIOB_ODR &= 0xFFF0;
		GPIOB_ODR |= nibble; 			// send upper nibble
		
		SetE(1); 									// E high
		LCDWait();		
		SetE(0);									// E low
		LCDWait();
		
		i++;
	}
}

void LCDWait()
{
	
	Delay(5);
	
	/*
	SetE(0); 											// E low
	
	GPIOA_ODR &= 0xFFFE; 					// RS low (command)
	GPIOA_ODR |= 2; 							// set R/W high (read)
	
	GPIOB_MODER &= 0xFFFFFF3F; 		// set PB3 to input
		
	SetE(1); 											// E high
		
	while ((GPIOB_IDR & 8)); // wait for Busy Flag
	
	Delay(1);
	
	GPIOB_MODER |= 0x40; 					// set PB0-3 to output
	*/
}

void SetE(char e)
{
	GPIOA_ODR &= (0xFFFFB | e); // clear if e is 0
	GPIOA_ODR |= (e << 2);			// set if e is 1
}

void Delay(int t)
{
	for (int i = 0; i < t; i++)
		for (int j = 0; j < 2; j++)
			for (int x = 0; x < 1000; x++);
}
