#include "at89c51cc03.h"

void main ()
{
	CKCON = 1; // x2 mode
	TMOD = 0x1; // put timer 0 into 16 bit timer mode
// Setting CCKON to 01h will make FTClock = 28.2076MHz/6 = 4.701267 MHz
// and the processor core will be double clocked.  This gives a 
// period of 0.2127086 µsecond period.  To get 5 millisecond = 5000 µseconds we need 23506 counts
// overflows at 65536, so need to start at 65536 - 23506 = 42030
	TH0 = 0xA4;	// Timer 0 set to A42Eh = 42030
	TL0 = 0x2E;
	
	// turn all LED's off at start of program
	P1_0 = 1;
	P1_1 = 1;
	P1_2 = 1;
	P1_3 = 1;
		
	// loop forever
	while(1)
	{
		// reset timeout counter
		unsigned char time = 0;
		
		// wait for button to be pressed
		while (P3_4 == 1);
		
		// loop while button is pressed
		while(P3_4 == 0)
		{
			TR0 = 1; // enable timer 0
			TF0 = 0; // reset timer overflow flag
			
			while (TF0 == 0); // wait for timer 0 to time out
			TR0 = 0;
			TH0 = 0xA4;	// reload timer
			TL0 = 0x2E;
			
			// blink p1.3 8 times per second (125 ms / 5 = 25 timeouts)
			if (time != 0 && time % 25 == 0)
				P1 = P1 ^ 8;
			// blink p1.2 4 times per second (250 ms / 5 = 50 timeouts)
			if (time != 0 && time % 50 == 0)
				P1 = P1 ^ 4;
			// blink p1.1 2 times per second (500 ms / 5 = 100 timeouts)
			if (time != 0 && time % 100 == 0)
				P1 = P1 ^ 2;
			// blink p1.0 1 time per second (1000 ms / 5 = 200 timeouts)
			if (time == 200)
			{
				P1 = P1 ^ 1;
				time = 0xFF; // reset timeout counter at 200 timeouts
			}
			
			time++; // increment timeout counter
		}
	}
}
