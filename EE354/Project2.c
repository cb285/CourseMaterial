/* 	Project 2 - On the Level
		Clayton Brutus
		12/5/16
*/
#include "stm32f407vg.h"

#define SENS 200
#define xStart 0
#define yStart 7

__asm void TurnOn(int x, int y);
int GetX(void);
int GetY(void);
void Delay(int t);
char Up(int x, int y);
char Down(int x, int y);
char Left(int x, int y);
char Right(int x, int y);

// make maze
unsigned char maze[8][8] = {{5, 3, 3, 3, 9, 5, 3, 9}, {12, 5, 3, 9, 6, 10, 5, 8}, {14, 12, 13, 6, 3, 9, 14, 12}, {13, 12, 12, 5, 9, 6, 9, 12}, {6, 10, 6, 8, 6, 3, 10, 12}, {5, 11, 5, 10, 5, 1, 3, 10}, {6, 9, 6, 3, 10, 6, 3, 9}, {7, 2, 3, 3, 3, 3, 3, 10}};

int main()
{
	// vars for storing current location in maze
	int currX, currY;
	int xVal = 0, yVal = 0;
		
	// Setup GPIO:
		
	// first turn all led's off
	GPIOA_ODR = 0;
	GPIOB_ODR = 0xFF;
		
	//Clock bits
	RCC_AHB1ENR |= 7; // Bit 0 is GPIOA clock enable bit 
										// Bit 1 is GPIOB clock enable bit
										// Bit 2 is GPIOC clock enable bit
		
	// IO Mode
	GPIOA_MODER |= 0x5555;  // digital output on PA0-7
	GPIOB_MODER |= 0x5555;	// digital output on PB0-7
		
	// PC0 defaults to input
	
	GPIOC_PUPDR |= 0x4000; // pull up on PC0
	
	// OTYPER register resets to 0 so it is push/pull by default
		
	GPIOA_OSPEEDER |= 0xFFFF;  // high speed on PA0-7
	GPIOB_OSPEEDER |= 0xFFFF;	 // high speed on PB0-7
		
	// PUPDR defaults to no pull up no pull down    
	
	// Setup ADC_CCR:
	RCC_APB2ENR |= 0x100;     // Bit 8 is ADC 1 clock enable bit
		
	GPIOC_MODER |= 0xF00;   // PC4, PC5 in Analog mode
													// PC7 Input by default
	
	RCC_APB1ENR |= 2; // Enable peripheral timer for timer 3
	
	TIM3_CR1 |= (1 << 7); // auto reload is buffered
	TIM3_CR1 |= (1 << 3); // one pulse mode is on.
	
	TIM3_PSC = 0; // no prescale
	TIM3_ARR = 16000; // 1 ms
		
	//Main program loop
		
	// set starting position
  currX = xStart;
	currY = yStart;
	
	TurnOn(currY, currX);
		
	while(1)
	{	
		// check reset button
		if ((GPIOC_IDR & (1<<7)) == 0)
		{
			// turn LEDs off
			GPIOA_ODR = 0;
			GPIOB_ODR = 0xFF;
			
			// delay
			Delay(1000);
			
			// reset position
			currX = xStart;
			currY = yStart;
			
			// light up correct LED
			TurnOn(currY, currX);
			
			// delay .2 sec
			Delay(200);
		}
		
		// check x and y directions
		xVal = GetX();
		yVal = GetY();
		
		
		// left
		if (xVal >= 2000 + SENS)
		{	
			if (Left(currX, currY))
					currX++;
		}
		// right
		else if (xVal <= 2000 - SENS)
		{	
			if (Right(currX, currY))
					currX--;
		}
		// up
		else if (yVal >= (2000 + SENS))
		{
			if (Up(currX, currY))
				currY++;
		}
		// down
		else if (yVal <= (2000 - SENS))
		{	
			if (Down(currX, currY))
					currY--;
		}
		
		// turn on right LED
		TurnOn(currY, currX);
			
		// delay .4 sec
		Delay(400);
			
		// check if finished
		if ((currX == 3) && (currY == 0))
		{
			// reset position
			currX = xStart;
			currY = yStart;
			
			// turn all off
			GPIOB_ODR = 0xFF;
			GPIOA_ODR = 0xFF;
			
			// turn on 1 row at a time, and delay .2 seconds between each
			GPIOB_ODR = ~(1<<7);
			Delay(200);
			GPIOB_ODR = ~(1<<6);
			Delay(200);
			GPIOB_ODR = ~(1<<5);
			Delay(200);
			GPIOB_ODR = ~(1<<4);
			Delay(200);
			GPIOB_ODR = ~(1<<3);
			Delay(200);
			GPIOB_ODR = ~(1<<2);
			Delay(200);
			GPIOB_ODR = ~(1<<1);
			Delay(200);
			GPIOB_ODR = ~(1<<0);
			Delay(200);
			
			// turn on start position LED
			TurnOn(currY, currX);
		}
	}
}

char Up(int x, int y)
{
	return ((maze[x][y] & 0x8) == 0);
}

char Down(int x, int y)
{
	return ((maze[x][y] & 0x4) == 0);
}

char Left(int x, int y)
{
	return ((maze[x][y] & 0x2) == 0);
}

char Right(int x, int y)
{
	return ((maze[x][y] & 0x1) == 0);
}

int GetX(void)
{
	ADC_CR2 |= 1;             //Bit 0 turn ADC on
	ADC_CR2 |= 0x400;         //Bit 10 allows EOC to be set after conversion
	ADC_CCR |= 0x30000;    
	//Bits 16 and 17 = 11 so clock divided by 8
	ADC_SQR3 &= 0;
	ADC_SQR3 |= 0xE;          //Bits 4:0 are channel # for first conversion
														// Channel is set to 5 which corresponds to PC4

	ADC_CR2 |= 0x40000000;   // Bit 30 does software start of A/D convert
	
	while((ADC_SR & 0x2) == 0); //Bit 1 is End of Conversion
	
	return ADC_DR & 0xFFF; 
}

int GetY(void)
{
	ADC_CR2 |= 1;             //Bit 0 turn ADC on
	ADC_CR2 |= 0x400;         //Bit 10 allows EOC to be set after conversion
	ADC_CCR |= 0x30000;    
	//Bits 16 and 17 = 11 so clock divided by 8
	ADC_SQR3 &= 0;
	ADC_SQR3 |= 0xF;          //Bits 4:0 are channel # for first conversion
														// Channel is set to 9 which corresponds to PC5

	ADC_CR2 |= 0x40000000;   // Bit 30 does software start of A/D convert
	
	while((ADC_SR & 0x2) == 0); //Bit 1 is End of Conversion
	
	return ADC_DR & 0xFFF; 
}

void Delay(int t)
{
	// loop t times
	for (int i = 0; i < t; i++)
	{
		TIM3_CR1 |= 1;	// enable timer 3
		while ((TIM3_CR1 & 1) != 0); // wait to time out
	}
}
	
	
	// Function: TurnOn
	// turns on the LED of the matrix at the specified location
	// turns all other LED's off
__asm void TurnOn(int x, int y) // x -> R0, y -> R1
{
	push {R1-R4};
	ldr R2, =0x40020014; 		R2 = address of PA
	ldr R3, =0x40020414; 		R3 = address of PB
	mov R4, #1 ; 						R4 = 1			
	lsls R4, R4, R0; 				R4 = R4 (1) << R0 (x)
	str R4, [R2];						PA = R4
	mov R4, #1;							R4 = 1
	lsls R4, R4, R1;				R4 = R4 (1) << R1 (y)
	mvn R4, R4;							R4 = ~R4
	str R4, [R3];						PB = R4
	pop {R1-R4};
	bx LR; 									return
}
