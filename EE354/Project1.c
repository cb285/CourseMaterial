#include <at89c51cc03.h>

void setLED(unsigned char x, unsigned char y);
void Delay(int t);

void main()
{
	// create maze
	unsigned char mazeArray[8][8] = {{0x5, 0xC, 0xC, 0xC, 0xE, 0x5, 0xC, 0x6}, {0x9, 0xC, 0xC, 0x6, 0x5, 0xA, 0x7, 0x3}, {0x5, 0x6, 0x7, 0x9, 0xA, 0x5, 0xA, 0x3}, {0x3, 0x9, 0xA, 0x5, 0x6, 0x3, 0x5, 0x2}, {0x3, 0xD, 0xC, 0x2, 0x3, 0x1, 0xA, 0xB}, {0x9, 0xC, 0x6, 0x3, 0x1, 0x8, 0xC, 0x6}, {0x5, 0xC, 0x2, 0xB, 0x9, 0x6, 0x5, 0xA}, {0x9, 0xE, 0x9, 0xC, 0xC, 0xA, 0x9, 0xE}}; 
		
	// create vars for storing current position
	unsigned char yPos;
	unsigned char xPos;
	
	// create vars for storing potentiometer values
	int xPotVal, yPotVal;

	// create temp var for reading analog inputs
	unsigned char tmp;
		
	// setup Timer 0
	CKCON = 1; // x2 mode
	TMOD = 0x1; // put timer 0 into 16 bit timer mode
	// Setting CCKON to 01h will make FTClock = 28.2076MHz/6 = 4.701267 MHz
	// and the processor core will be double clocked.  This gives a 
	// period of 0.2127086 µsecond period.  To get 10 millisecond = 10000 µseconds we need 47012 counts
	// overflows at 65536, so need to start at 65536 - 47012 = 18524
	TH0 = 0x48;	// Timer 0 set to 485Ch = 18524
	TL0 = 0x5C;
	
	// setup ADC
	ADCF  = 0x01;        // P1.0 = ADC[0] Joystick X-axis
	ADCF  = 0x02;        // P1.1 = ADC[1] Joysick Y-axis
	ADCON = 0x20;        // Enable ADC Function 
	ADCLK = 0x00;        // Prescaler to 0
	EA = 0;
							
	// set position to start of maze
	xPos = 3;
	yPos = 7;
	setLED(xPos, yPos);
			
	// loop forever
	while(1)
	{
		// Read A/D X-axis Converter Value
		ADCON &= 0xF8;     // Reset ADC Channel Select 
		ADCON |= 0x00;     // Select ADC = Ch0             
		ADCON |= 0x20;     // Use Standard mode 
		ADCON |= 0x08;     // Start ADC Convert 
		tmp = (ADCON & 0x10);    // Get done bit
		while(tmp != 0x10)       // Loop until complete 
				tmp = (ADCON & 0x10);
		ADCON &= 0xEF;           // Clear ADEOC = 0
		
		yPotVal = ADDH;
		
		// Switch to Y Analog Input channel
		ADCON &= 0xF8;     // Reset ADC Channel Select 
		ADCON |= 0x01;     // Select ADC = Ch1             
		ADCON |= 0x20;     // Use Standard mode
		ADCON |= 0x08;     // Start ADC Convert 
		tmp = (ADCON & 0x10);    // Get done bit
		while(tmp != 0x10)       // Loop until complete 
					tmp = (ADCON & 0x10);
		ADCON &= 0xEF;           // Clear ADEOC = 0
				
		xPotVal = ADDH;
		
		// try to move right
		if ((xPotVal > 90) && (yPotVal > 60) && (yPotVal < 170))
		{	
			if ((mazeArray[xPos][yPos] & 0x2) == 0)
					xPos++;
		}
		// try to move left
		else if ((xPotVal < 40) && (yPotVal > 60) && (yPotVal < 170))
		{
			if ((mazeArray[xPos][yPos] & 0x1) == 0)
				xPos--;
		}
		// try to move up
		else if ((yPotVal < 60) && (xPotVal < 90) && (xPotVal > 40))
		{	
			if ((mazeArray[xPos][yPos] & 0x8) == 0)
				yPos--;
		}
		// try to move down		
		else if ((yPotVal > 170) && (xPotVal < 90) && (xPotVal > 40))
		{
			if ((mazeArray[xPos][yPos] & 0x4) == 0)
				yPos++;
		}
		
		// update LED's
		setLED(xPos, yPos);
		Delay(50);
		
		// check reset button
		if (P1_2 == 0)
		{
			// move back to start
			xPos = 3;
			yPos = 7;
		}
				
		// check if finished
		if (xPos == 4 && yPos == 0)
		{
			// wait for reset button to be pressed
			while (P1_2 != 0);
			
			// move back to start
			xPos = 3;
			yPos = 7;
		}
	}
}

// Function: setLED
// turns on the LED at the position (x, y)
void setLED(unsigned char x, unsigned char y)
{
	// set the correct bit on P3 and P0
	P3 = ~(1<<x);
	P0 = ~(1<<y);
}

// Function: Delay
// delays for t seconds
void Delay(int t) 
{
	int i;
	
	// loop t times
	for (i = 0; i < t; i++)
	{
		// use timer for 1 sec delay
		TR0 = 1; // enable timer 0
		TF0 = 0; // reset timer overflow flag
				
		while (TF0 == 0); // wait for timer 0 to time out
		TR0 = 0;
		TH0 = 0x48;	// reload timer
		TL0 = 0x5C;
	}
}