/* 
	EE454
	Project 2
	3/28/17

	Send button:
		PC0 -	high => sends first
					low => waits to receive
	
	LCD Connections:
		DB4-7 (pins 11-14) 	-> PB0-3
		RS 		(pin 4)				-> PA0
		R/W 	(pin 5)				-> PA1
		E 		(pin 6)				-> PA2
	
	UART Connections:
		TX -> PC6
		RX -> PC7
 */

#include "stm32f407vg.h"	// board header file
#include <string.h>				// library for strcomp

#define MAX 16						// max message length

// Function Prototypes:
void LCDCommand(char comm);
void LCDWrite(char word[MAX]);
void SetE(char e);
void LCDWait(void);
void LCDClear(void);

void UARTSend (const char msg[]);
void UARTPutChar(char ch);

char UARTMsgPending(void);

void UARTRecieve(char* message);
char UARTGetChar(void);

int Random(void);

void Delay(int t);
void StringCopy (char * dest, char * src);

int main()
{
	// send button setup:
	RCC_AHB1ENR |= 3;						// PC clock enable
															// PC0 input by default
	
	// LCD Setup:
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
	
	// UART setup:
	RCC_AHB1ENR |= 4;         	// PC clock enable
  RCC_APB1ENR |= (1 << 4);  	// timer 6 clock enable
  RCC_APB2ENR |= (1 << 5);  	// USART6 clock enable
	
  GPIOC_AFRL = 0x88000000;  	// alternate Func PC 6-7 to USART6
  GPIOC_MODER |= 0x0A000;    	// bits 15-12 = 1010 for Alt Func on PC6, PC7
															// OTYPER register resets to 0 so it is push/pull by default
  GPIOC_OSPEEDER |= 0x3000;  	// bits 7-6 = 11 for high speed on PC6
															//PUPDR defaults to no pull up no pull down
	
	USART6_CR1 = 0;        			// disable during set up. Wd len = 8, Parity = off
  USART6_BRR = 26;  					// set up baud rate divisor for 38400 baud
  USART6_CR2 = 0;            	// 1 stop bit
  USART6_CR1 = 0x200C;
  USART6_CR3 = 0;            	// disable interrupts and DMA
	
	char message[MAX] = "";			// create string vars
	char response[MAX] = "";
	
	char done = 0; 							// done flag

	
		
	LCDClear();									// clear LCD
	
	// wait for button to be pressed or message to be recieved
	while (((GPIOC_IDR & 1) == 0) && !UARTMsgPending());
	
	// if message pending
	if (!UARTMsgPending())
	{
		Delay(5000);
		UARTSend("Hello!"); // send response
		LCDCommand(0xC0);   // go to second line
		LCDWrite("Hello!"); // write response to LCD
	}
	
	// loop:
	while(1)
	{		
		// wait to recieive a message
		while (!UARTMsgPending());
		
		UARTRecieve(message); // get message
		
		// check for each message possibility and choose response
		if (strcmp(message, "Hello!") == 0)
		{
			StringCopy(response, "Hey there!");
		}
		
		else if (strcmp(message, "Hey there!") == 0)
		{
			StringCopy(response, "How are you?");
		}
	
		else if (strcmp(message, "How are you?") == 0)
		{
			StringCopy(response, "I'm fine, you?");
		}
	
		else if (strcmp(message, "I'm fine, you?") == 0)
		{
			StringCopy(response, "I'm good");
		}
	
		else if (strcmp(message, "I'm good") == 0)
		{
			StringCopy(response, "Goodbye");
		}
		
		else if (strcmp(message, "Goodbye") == 0)
		{
			StringCopy(response, "Okay, see ya!");
			done = 1;
		}
		
		else if (strcmp(message, "Okay, see ya!") == 0)
		{
			response[0] = '\0';
			done = 1;
		}
		
		LCDClear();							// clear LCD
		LCDWrite(message); 			// display the receieved message
		
		Delay(3000); 						// delay 5 seconds
		
		UARTSend(response); 		// send the response
		
		LCDCommand(0xC0);				// move cursor to 2nd line
		LCDWrite(response); 		// write response to 2nd line
		
		// check done flag
		if (done)
		{
			Delay(3000);					// delay 5 seconds
			LCDClear();						// clear LCD
			while (1);						// stop program
		}
	}
}

// Function: UARTSend
// given a string, sends it using UART on PC6, 7
void UARTSend (const char msg[])
{
	int i = 0;
  while(msg[i] != '\0') 	// until end of string
	{
		UARTPutChar(msg[i]); 	// send each character
		i++;		
  }
	
	UARTPutChar('\0'); 			// send null character (end of string)
}

// Function: UARTPutChar
// given a character, sends it using UART on PC6, 7
void UARTPutChar(char ch)
{
	// wait for empty flag
  while((USART6_SR & 0x80) == 0);
	
  USART6_DR = ch; 	// send character
	
	Delay(20);		 		// delay 20 ms
}

// Function: UARTMsgPending()
// returns true if UART has receivied data
// false otherwise
char UARTMsgPending()
{
	return ((USART6_SR & 0x20) != 0); // check UART status register
}

// Function: UARTRecieve
// receives a string using UART and passes it back
void UARTRecieve(char* message)
{	
	char i = 0; 					// counter var
	
	char c = 0; 					// character var
	
	c = UARTGetChar();		// get character
	
  while(c != '\0')			// until end of string
	{
		message[i] = c;			// put character into string
		c = UARTGetChar(); 	// get next character
		i++;								
	}
	
	message[i] = '\0'; 		// add null character
}

// Function: UARTGetChar
// recieves a charater using UART and returns it
char UARTGetChar()
{	
	// wait for empty flag
	while((USART6_SR & 0x20) == 0);
  
	char c = USART6_DR; 			// get character
	
	USART6_SR &= 0xFFFFFFEF; 	// clear bit 5 (data ready bit)
	
	return c;									// return character
}

// Function: LCDClear
// clears the LCD and returns to home position
void LCDClear()
{
	LCDCommand(0x01); // send appropriate command
}

// Function: LCDCommand
// sends a command to the LCD
void LCDCommand(char comm)
{
	char nibble = 0;			// var for storing nibbles
	
	SetE(0); 							// E low
	LCDWait();						// delay
	
	GPIOA_ODR = 0; 				// command mode
	
	nibble = comm >> 4; 	// get upper nibble
	
	GPIOB_ODR &= 0xFFF0;
	GPIOB_ODR |= nibble; 	// send upper nibble
	
	SetE(1); 							// E high
	LCDWait(); 						// delay
	
	SetE(0); 							// E low
	LCDWait();
	
	nibble = comm & 0xF; 	// get lower nibble
	GPIOB_ODR &= 0xFFF0;
	GPIOB_ODR |= nibble; 	// send lower nibble
	
	SetE(1); 							// E high
	LCDWait();						// delay
	
	SetE(0); 							// E low
	LCDWait();						// delay
}

// Function: LCDWrite
// writes a string to the LCD 
void LCDWrite(char word[MAX])
{		
	SetE(0); 										// E low
	LCDWait(); 									// delay
	
	GPIOA_ODR |= 1;	 						// RS High -> data mode
	
	char nibble = 0; 						// var for storing nibbles
	
	char i = 0;									// counter var
	
	while (word[i] != '\0')			// until end of string
	{		
		nibble = (word[i] >> 4); 	// get upper nibble
		GPIOB_ODR &= 0xFFF0;
		GPIOB_ODR |= nibble; 			// send upper nibble
		
		SetE(1); 									// E high
		LCDWait();								// delay
		SetE(0);									// E low
		LCDWait();								// delay
		
		nibble = (word[i] & 0xF); // get lower nibble
		GPIOB_ODR &= 0xFFF0;
		GPIOB_ODR |= nibble; 			// send upper nibble
		
		SetE(1); 									// E high
		LCDWait();								// delay
		SetE(0);									// E low
		LCDWait();								// delay
		
		i++;
	}
}

// Function: LCDWait
// delays ~2 ms
void LCDWait()
{
	Delay(2);
}

// Function: StringCopy
// copies a string to another string
void StringCopy (char* dest, char* src)
{
	int i = 0;								// counter var
	while (src[i] != '\0')		// until end of string
	{
		dest[i] = src[i];				// copy character
		i++;
	}
	
	dest[i] = '\0';						// add null character (end of string)
}

// Function: SetE
// given 1 or 0, sets E high (1) or low (0)
void SetE(char e)
{
	GPIOA_ODR &= (0xFFFFB | e); // clear if e is 0
	GPIOA_ODR |= (e << 2);			// set if e is 1
}

// Function: Delay
// delays for given number of ms
void Delay(int t)
{
	for (int i = 0; i < t; i++)
		for (int j = 0; j < 2; j++)
			for (int x = 0; x < 1000; x++);
}
