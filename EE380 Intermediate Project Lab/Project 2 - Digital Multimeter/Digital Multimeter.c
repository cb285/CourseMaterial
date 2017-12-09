/* 
	EE454
	Project 2
	Digital Multimeter

LCD:
	DB4-7 - PB0-3
	RS 		- PA0
	R/W 	- PA1
	E 		- PA2
	
Measurement:
	Voltage Analog Input 				- PA4
	Current Analog Input 				- PA5
	Capacitance Digital Input 	- PC1
	Capacitance Digital Output 	- PC0
	
 */
	
#include "stm32f407vg.h"
#include <stdio.h>

#define MAX 16						// max message length

void LCDSetUp(void);
void LCDCommand(char comm);
void LCDWrite(char word[MAX]);
void LCDWriteChar(char c);
void SetE(char e);
void LCDWait(void);
void LCDClear(void);
void Delayms(int t);

float GetADC(char channel);
float GetVoltage(void);
float GetCurrent(void);
float GetCapacitance(void);

unsigned long time = 0;

int main()
{
	// LCD pins setup:
	RCC_AHB1ENR |= 1; 					// PA clock enable
	RCC_AHB1ENR |= 2;						// PB clock enable
	GPIOA_MODER &= 0xFFFFFFC0;	// clear mode for PA0-2
	GPIOA_MODER |= 0x15;				// set PA0-2 to output
	GPIOB_MODER &= 0xFFFF00; 		// clear mode for PB0-3
	GPIOB_MODER |= 0x55; 				// set PB0-3 to output
	
	// ADC setup:
	RCC_APB2ENR |= (1 << 8); 		// ADC 1 clock enable bit
	ADC1_CR2 |= 1;       			 	// Bit 0 turn ADC on
	ADC1_CR2 |= (1 << 10); 	 		// Bit 10 allows EOC to be set after conversion
	ADC_CCR |= 0x30000; 				// Bits 16 and 17 = 11 so clock divided by 8
	
	GPIOA_MODER |= 0x0300;   		// PA4 analog
	GPIOA_MODER |= 0x0C00; 			// PA5 analog
	GPIOA_MODER |= 0x3000;   		// PA6 analog
	
	GPIOA_PUPDR &= 0xFFFFFCFF; 	// no PUPD on PA4
	GPIOA_PUPDR &= 0xFFFFF3FF; 	// no PUPD on PA5
	GPIOA_PUPDR &= 0xFFFFCFFF; 	// no PUPD on PA6
	
	// capactitance output pin setup:
	RCC_AHB1ENR |= 4; 					// PC clock enable
	GPIOC_MODER &= 0xFFFFFFFC;	// clear mode for PC0
	GPIOC_MODER |= 0x1; 				// set PC0 to output
															// push pull by default
	
	GPIOC_ODR &= 0xFFFE; 				// turn off PC0
	
	// TIM4 setup:
	RCC_APB1ENR |= (1 << 1);		// TIM3 clock enable
	
	NVICISER0 |= (1 << 29); 		// TIM3 global interrupt enable
	TIM3_DIER |= 1; 						// update interrupt enable
	TIM3_DIER |= (1 << 6); 			// trigger interrupt enable
	
	TIM3_CR1 |= (1 << 7); 			// auto reload enable
	TIM3_PSC = 99; 							// prescaler
	TIM3_ARR = 8; 							// auto reload (50 us)
	
	TIM3_CR1 |= 1;							// enable TIM3
	
	// LCD initialization:
	LCDWait();									// wait for screen to power on
	
	LCDCommand(0x33);
	LCDCommand(0x32);						// setup LCD for 4 bit mode
	
	LCDCommand(0x2C); 					// 2 lines, 5x11 font
	LCDCommand(0x06); 					// increment cursor to right when writing, don't shift screen
	LCDCommand(0x0F); 					// turn on display, cursor, and cursor blinking
	
	LCDClear();									// clear and return cursor to home
	
	float V = 0;
	float C = 0;
	float I = 0;
	
	char str[MAX];
	
	while (1)
	{
		// Voltage:
		V = GetVoltage(); 				// get voltage
		
		LCDClear();								// clear LCD
		
		LCDWrite("V = ");			 		// print V label
		
		sprintf(str, "%0.3f", V); // convert number to string
		
		LCDWrite(str); 						// print voltage
		
		LCDCommand(0xC0);	// move cursor to next line
		
		if (GPIOC_IDR & (1 << 2))
		{
		// Capacitance:
		C = GetCapacitance(); 		// get capacitance
		
		LCDWrite("C = ");			 		// print C label
		
		sprintf(str, "%0.3f", C); // convert number to string
		
		LCDWrite(str); 						// print capacitance
		
		LCDWrite(" nF");					// print nF
		
		GPIOC_ODR &= 0xFFFE;
		}
		
		else
		{
			// Current:
			I = GetCurrent(); 				// get capacitance
			
			LCDWrite("I = ");			 		// print C label
			
			sprintf(str, "%0.3f", I); // convert number to string
			
			LCDWrite(str); 						// print capacitance
			
			LCDWrite(" mA");					// print nF
		}			
		
		Delayms(1000);							// 1 s delay between measurements
	}
}

void LCDClear()
{
	LCDCommand(0x01);
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
	Delayms(2);
	
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
	Delayms(5);
}

void SetE(char e)
{
	GPIOA_ODR &= (0xFFFFB | e); // clear if e is 0
	GPIOA_ODR |= (e << 2);			// set if e is 1
}

void TIM3_IRQHandler()
{
	time++;
	TIM3_SR &= 0xFFFE;  		// clear interrupt flag
	TIM3_CR1 |= 1;      		// restart the timer
}

// Delays t ms
void Delayms(int t)
{
	t = t*10;
	time = 0;
	
	while (time < t);
}

float GetADC(char channel)
{
	ADC1_SQR3 &= 0xFFFFFFE0; 											// clear channel
	ADC1_SQR3 |= channel;       									// set channel
	
	ADC1_CR2 |= (1 << 30);   											// start ADC
	
	while((ADC1_SR & 0x2) == 0); 									// wait for end of conversion
	
	return (ADC1_DR & 0xFFF)*(3.00f / 4095.0f);					// return the voltage
}

float GetVoltage()
{
	return (GetADC(4)*6.848f);							// return the voltage
}

float GetCurrent()
{	
	return (1000*GetADC(5))/26;										// return current in mA
}

float GetCapacitance()
{
	GPIOC_ODR &= 0xFFFE; 	// turn off PC0
	
	while ((GPIOC_IDR & (1<< 3)) == 1); // wait for cap to discharge
	
	Delayms(4500);
	
	GPIOC_ODR |= 0x1; 		// turn on PC0
	
	time = 0;
	
	while ((GPIOC_IDR & 2) == 0);
	
	return (time / (20*.69315)); // return Capacitance in nF
}
