#include <at89c51cc03.h>

void main()
{
	// setup ADC
	unsigned char tmp;      
	ADCF  = 0x01;        // P1.0 = ADC[0] Joystick X-axis
	ADCF  = 0x02;        // P1.1 = ADC[1] Joysick Y-axis
	ADCON = 0x20;        // Enable ADC Function 
	ADCLK = 0x00;        // Prescaler to 0
	EA = 0;
	
	// setup PWM
	CKCON = 0x01;   // x2 mode 
	CMOD = 0x02;    //Clock bit CPS1, CPS0 = 01 so PCA clock 
									//  is cpu clock/2 
	CCON = 0x40;    //Bit 6 in CCON turns on PCA timer 
	CCAPM0 = 0x42;  //Bit 6 enables the compare mode for PWM and bit 2 enables PWM 
	
	// loop forever
	while(1)
	{
		ADCON &= 0xF8;     // Reset ADC Channel Select 
		ADCON |= 0x00;     // Select ADC = Ch0             
		ADCON |= 0x20;     // Use Standard mode 
		ADCON |= 0x08;     // Start ADC Convert 
		tmp = (ADCON & 0x10);    // Get done bit
		while(tmp != 0x10)       // Loop until complete 
				tmp = (ADCON & 0x10);
		ADCON &= 0xEF;           // Clear ADEOC = 0
		
		int xVal = (ADDH << 2) + ADDL;
		
		if (xVal > 700)
			// move right
		else if (xVal < 350
			// move left
		
		// Switch to Y Analog Input channel
		ADCON &= 0xF8;     // Reset ADC Channel Select 
		ADCON |= 0x01;     // Select ADC = Ch1             
		ADCON |= 0x20;     // Use Standard mode
		ADCON |= 0x08;     // Start ADC Convert 
		tmp = (ADCON & 0x10);    // Get done bit
		while(tmp != 0x10)       // Loop until complete 
				tmp = (ADCON & 0x10);
		ADCON &= 0xEF;           // Clear ADEOC = 0
		
		int xVal = (ADDH << 2) + ADDL;
		
		if (xVal > 700)
			// move right
		else if (xVal < 350
			// move left
	}
}
