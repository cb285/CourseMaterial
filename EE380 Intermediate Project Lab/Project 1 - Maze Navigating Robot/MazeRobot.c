/*  EE 380 Project 1
		Clayton, Thomas, Ahmed, Isaiah
		This project will make a robot complete a maze

	Connections:
	
		Outputs:
			PC6 (TIM3_CH1) - Right wheel positive output (high = forward) (PWM)
			PC7 (TIM3_CH2) - Left wheel positive output	(high = forward) (PWM)
			
			PA0 - Right wheel negative output (low = forward) (digital)
			PA3 - Left wheel negative output (low = forward) (digital)
			
		Inputs:
			PA4 - Left wheel encoder (digital)
			PA5 - Right wheel encoder (digital)
			
			PA7 - Left distance sensor (analog)
			PC4 - Front distance sensor (analog)
*/

#include "stm32f407vg.h"		// header file

// Initial speeds for motors
#define LEFTINIT 20000 			// left motor (2500)
#define RIGHTINIT 20000			// right motor (2500)

// Max and min speeds for wheels when wall following or driving straight with encoders:
#define HIGHLIM 42000				// (best: 42000)
#define LOWLIM 20000				// (best: 20000)

// Wall following PD controller constants:
#define TARGETWALL 12.0f		// target distance from left wall ('f' makes it a float)
#define P 2600.0f						// proportional constant (best: 1000)
#define D 1500.0f 					// differential constant (best: 1800)
#define T 0.02f 						// wall following adjusting time interval (equal to TIM6 time)

// Constants for driving straight using encoders:
#define CONST 3000					// Proportional constant for driving stright using encoders (best: 3000)
#define TARGETCOUNT 40			// target encoder count (per 20 ms period)


#define TURNDIST 18.44 + 5 			// distance to get from front wall before turning (best: 18.44 cm)

#define FALLOFFDIST 25

// Turning using encoders constants:
#define RIGHTTURN 31				// encoder ticks for turning right
#define LEFTTURN 31					// encoder ticks for turning left
#define TURNSPEED 42000			// speed for motor while turning


// Function prototypes:
void TurnRight(void);
void TurnLeft(void);
void Delay(void);
float GetLeft(void);
float GetFront(void);
void FollowUntilDistance(float distToFront);
void ForwardUntilDistance(float distToFront);
void ForwardForTicks(int encoderTicks);
void ForwardUntilStartOfWall(void);
void FollowUntilEndOfWall(void);
void FollowForTicks(int encoderTicks);

// Global variables:
// encoder counts
int countLeft = TARGETCOUNT;				// initalize encoder counts to target count
int countRight = TARGETCOUNT;
int encoderDistance = 0;						// for tracking distance traveled (uses left wheel)

// wheel high times for when driving straight using encoders
long encoder_high_time_right = RIGHTINIT;		// set to intial speeds
long encoder_high_time_left = LEFTINIT;

// Main function:
int main()
{
	// Set up wheel outputs:
	RCC_AHB1ENR |= 0xF; 	    				// enable clocks for ports A, B, C, and D
	GPIOA_MODER |= 0x041;    					// set PA0 and PA3 to output and PA1, PA2, PA4, and PA5 to input
																		// OTYPER register resets to 0 so it is push/pull by default
	GPIOA_OSPEEDER |= 0xAAA; 					// set PA0 to PA5 to high speed
	GPIOC_MODER |= 0xA000;   					// set up PC6 and PC7 as alternate function for PWM
																		// Defaults to low speed for PC_6 and PC_7
	GPIOC_AFRL |= 0x22000000; 				// use alternate function 2 for PC6 and PC7 (for timers 3-5)
	
	// timer 4 setup for driving straight with encoders:
	RCC_APB1ENR |= (1 << 2);  				// enable timer 4 clock
	NVICISER0 |= (1<<30);  	 					// use timer 4
	TIM4_DIER |= 1; 			    				// enable update interrupt
	TIM4_DIER |= (1<<6);  						// enable trigger interrupt
	TIM4_CR1 |= (1 << 3); 	  				// set timer 4 to one-pulse mode
	TIM4_CR1 |= (1 << 7); 						// enable the auto-reload register on timer 4
	TIM4_PSC = 4; 				  					// set the prescaler for timer 4
	TIM4_ARR = 64000;  			  				// set the auto-reload register for timer 4
	TIM4_CR1 |= 0; 				 						// enable the counter
	
	// timer 3 setup for PWM outputs:
	RCC_APB1ENR |= (1 << 1);  				// enable timer 3 clock
	NVICISER0 |= (1<<29);  						// use timer 3
	TIM3_DIER |= 1; 			 						// enable update interrupt
	TIM3_DIER |= (1<<6);							// enable trigger interrupt
	TIM3_CR1 |= (1<<7); 		 					// buffer timer 3 Auto reload register
	TIM3_PSC = 4; 				 						// set timer 3 to 20ms
	TIM3_ARR = 64000; 			 					// set Auto reload register
	TIM3_CCMR1 |=0x6868; 	 	 					// enable preload on on TIM3_CCR1 and TIM3_CCR2 and use PWM mode 1
	TIM3_CCER |=0x11; 			 					// enable capture 1 and 2
	TIM3_CCR1 = 0; 				 						// load 0 into CCMR1
	TIM3_CCR2 = 0; 				 						// load 0 into CCMR2
	TIM3_EGR |= 1; 				 						// reinitialize the counter and generate an update of the registers
	TIM3_CR1 |= 1; 				 						// enable the timer
	
	// timer 6 setup for 20 ms delay:
	RCC_APB1ENR |= (1 << 4);  				// enable peripheral timer for timer 6
	TIM6_CR1 |= (1 << 7);   					// auto reload is buffered 
  TIM6_CR1 |= (1 << 3);   					// one pulse mode is on
  TIM6_PSC = 4;           					// 16 MHz / 5 = 3.2 MHz
  TIM6_ARR = 64000;        					// 64000 / 3.2 MHz = 20 ms
	
	// PA4 external interrupt setup for left encoder:
	RCC_APB2ENR |= 0x4000;       			// system configuration clock enable
	NVICISER0 |= (1 << 10); 	 				// enable EXTI4
	SYSCFG_EXTICR2 &= 0xFFFFFF0F;			// clear all but lowest 4 bits of EXTI4
	SYSCFG_EXTICR2 |= 0x0; 		 				// EXTI4 set to port A4 by default
	EXTI_IMR |= (1<<4); 		 					// don't mask interrupt line 0
	EXTI_RTSR |= (1<<4); 		 					// rising trigger enable line 0
	EXTI_FTSR |= 1; 			 						// falling trigger enable line 0
	
	// PA5 external interrupt setup for right encoder:
	NVICISER0 |= (1 << 23); 	 				// enable EXTI5
																		// EXTI5 set to port A5 by default
	EXTI_IMR |= (1<<5); 		 					// don't mask interrupt line 0	
	EXTI_RTSR |= (1<<5); 		 					// rising trigger enable line 0
	EXTI_FTSR |= 1; 									// falling trigger enable line 0
	
	// ADC setup for reading distance sensor voltages:
	RCC_APB2ENR |= (1 << 8); 					// ADC1 clock enable bit
	ADC1_CR2 |= 1;       		 					// turn ADC1 on
	ADC1_CR2 |= (1 << 10); 	 					// bit 10 allows end of conversion to be set after conversion
	ADC_CCR |= 0x30000; 							// bits 16 and 17 = 11 so clock divided by 8
	
	// PA7, PC4 setup for analog inputs:
	GPIOA_MODER |= 0xC000; 						// PA7 Analog
	GPIOA_PUPDR &= 0xFFFF3FFF; 				// no PUPD on PA7
	
	GPIOC_MODER |= 0x300;							// PC4 Analog
	GPIOC_PUPDR &= 0xFFFFFCFF; 	 			// no PUPD on PC4
	
	/*
	-------------------------------------------------------------------------------------------
	|																		START OF MAZE																					|
	-------------------------------------------------------------------------------------------
	*/
	
	FollowUntilDistance(TURNDIST);					// follow left wall until 16 cm from front wall
	
	TurnRight();														// turn 1
	
	FollowForTicks(31);											// follow wall for ~17 cm (using encoders for distance) (prev: 32)
	
	TurnRight();														// turn 2					
	
	ForwardUntilDistance(TURNDIST + 4);					// drive forward until front sensor sees the front wall
	TurnLeft();															// turn 3
	
	ForwardUntilStartOfWall();							// drive forward until left sensor sees the left wall
	
	ForwardForTicks(5);											// drive forward a small amount to get closer to wall
	FollowForTicks(10);
	
	FollowUntilEndOfWall();									// follow wall until end of wall
	ForwardUntilDistance(TURNDIST + 1.5);		// drive forward until front sensor sees the front wall
	TurnLeft();															// turn 4
															
	ForwardUntilStartOfWall();							// drive forward until left sensor sees the left wall
		
	ForwardForTicks(16);										// drive forward a small amount to get closer to wall
	
	FollowUntilEndOfWall();									// follow wall until end of wall
		
	ForwardUntilDistance(TURNDIST);					// drive forward until TURNDIST cm from front wall
	TurnRight();														// turn 5
	
	FollowUntilDistance(TURNDIST);					// follow left wall until TURNDIST cm from front wall
	TurnRight();														// turn 6
	
	FollowUntilEndOfWall();									// follow wall until end of wall
	ForwardUntilDistance(TURNDIST);					// drive forward until front sensor sees the front wall
	TurnLeft();															// turn 7
	
	//ForwardUntilStartOfWall(); 						// drive forward until left sensor sees the left wall
	
	ForwardForTicks(16);										// drive forward a small amount to get closer to wall
	//FollowForTicks(40);
	
	
	FollowUntilEndOfWall();									// follow wall until end of wall
	
	
	ForwardUntilDistance(TURNDIST);					// drive forward until 16 cm from front wall (using encoders for correcting path)
	TurnRight();														// turn 8
	
	FollowForTicks(33);											// follow wall for ~33 cm (using encoders for distance) (prev: 33)
	TurnRight();														// turn 9
	
	//ForwardUntilStartOfWall();							// drive forward until left sensor sees the left wall
	
	ForwardForTicks(27);											// follow wall for ~20 cm (using encoders for distance)
	TurnLeft();															// turn 10
	
	//ForwardUntilStartOfWall();							// drive forward until left sensor sees the left wall
	
	ForwardForTicks(16);											// drive forward a small amount to get closer to wall
	
	FollowUntilEndOfWall();									// follow wall until end of wall
	ForwardUntilDistance(TURNDIST + 1);					// drive forward until front sensor sees the front wall
	TurnLeft();															// turn 11
	
	ForwardUntilStartOfWall();							// drive forward until left sensor sees the left wall
	
	//ForwardForTicks(5);											// drive forward a small amount to get closer to wall
	
	FollowUntilDistance(TURNDIST);					// follow left wall until front is TURNDIST cm from wall
	TurnRight();														// turn 12
	
	FollowUntilDistance(TURNDIST);					// follow left wall until TURNDIST cm from front wall
	TurnRight();														// turn 13

	FollowUntilDistance(TURNDIST);					// follow left wall until TURNDIST cm from front wall
	TurnRight();														// turn 14
	
	FollowUntilEndOfWall();									// follow wall until end of wall
	ForwardUntilDistance(TURNDIST);					// drive forward until front sensor sees the front wall
	TurnLeft();															// turn 15
	
	ForwardUntilStartOfWall();							// drive forward until left sensor sees left wall
	
	//ForwardForTicks(5);											// drive forward a small amount to get closer to wall
	
	FollowUntilDistance(TURNDIST);					// follow left wall until 16 cm from front wall
	TurnRight();														// turn 16
	
	FollowUntilDistance(10);								// follow left wall until 10 cm from front wall
	
	TIM3_CCR1 = 0; // stop the motors
	TIM3_CCR2 = 0;
	
	while (1); // stop the program
	
	/* 	-------------------------------------------------------------------------------------------
			|																			FINISH!																							|
			-------------------------------------------------------------------------------------------
	 */
}

/* 	Function: TurnRight
			Turns robot right by stopping right wheel and driving left wheel RIGHTTURN encoder ticks
 */
void TurnRight()
{
	TIM3_CCR1 = 0; // stop the motors
	TIM3_CCR2 = 0;
	
	Delay();
	Delay();
	
	TIM4_CR1 &= 0xFFFE; 						// disable timer 4
																	
	countLeft = 0;									// reset encoder counters
	countRight = 0;
	
	TIM3_CCR2 = TURNSPEED; 					// turn on left motor
	TIM3_CCR1 = 0;									// turn off right motor
	
	while (countLeft < RIGHTTURN);	// wait for encoder count to reach target
	
	TIM3_CCR2 = 0; 									// turn off left motor
	
	countLeft = 0;									// reset encoder counters
	countRight = 0;
	
	TIM3_CCR1 = 0; // stop the motors
	TIM3_CCR2 = 0;
	
	Delay();
	Delay();
}

/* 	Function: TurnLeft()
			Turns robot left by stopping left wheel and driving right wheel LEFTTURN encoder ticks
 */
void TurnLeft()
{
	TIM3_CCR1 = 0; // stop the motors
	TIM3_CCR2 = 0;
	
	Delay();
	Delay();
	
	TIM4_CR1 &= 0xFFFE; 						// disable timer 4
																
	countLeft = 0;									// reset encoder counters
	countRight = 0;
	
	TIM3_CCR1 = TURNSPEED; 					// turn on right motor
	TIM3_CCR2 = 0;									// turn off left motor
	
	while (countRight < LEFTTURN);	// wait for encoder count to reach target
	
	TIM3_CCR1 = 0; 									// turn off right motor
	
	countLeft = 0;									// reset encoder counters
	countRight = 0;
	
	TIM3_CCR1 = 0; // stop the motors
	TIM3_CCR2 = 0;
	
	Delay();
	Delay();
}

/* 	Function: Delay()
		uses timer 6 to create a delay of 20 ms
 */
void Delay()
{	
	TIM6_CR1 |= 1; 									// enable timer
	while((TIM6_CR1 & 1) != 0); 		// pause while timer is enabled (stops due to one-pulse mode)
}

/*  Function: TIM4_IRQHandler()
			handles interrupt for timer 4 (every 20 ms)
			corrects motor speeds using encoders to measure so that robot drives straight
 */
void TIM4_IRQHandler()
{
	int high_time_change_right = (TARGETCOUNT - countRight)*CONST;		// change speed proportional to difference between actual motor speed
	int high_time_change_left = (TARGETCOUNT - countLeft)*CONST;
	
	encoder_high_time_right += high_time_change_right;		// adjust right motor
	encoder_high_time_left += high_time_change_left;			// adjust left motor
	
	// keep motor speeds between lower and upper limits:
	// left
	if (encoder_high_time_left < LOWLIM)
		encoder_high_time_left = LOWLIM;
	else if (encoder_high_time_left > HIGHLIM)
		encoder_high_time_left = HIGHLIM;
	// right
	if (encoder_high_time_right < LOWLIM)
		encoder_high_time_right = LOWLIM;
	else if (encoder_high_time_right > HIGHLIM)
		encoder_high_time_right = HIGHLIM;
	
	// set wheel speeds
	TIM3_CCR1 = encoder_high_time_right;
	TIM3_CCR2 = encoder_high_time_left;
	
	countLeft = 0;					// reset encoder counters
	countRight = 0;
	
	TIM4_SR &= 0xFFFE;  		// clear interrupt flag
	TIM4_CR1 |= 1;      		// restart the timer
}

/* 	Function: EXTI4_IRQHandler()
			PA4 external interrupt handler (rising and falling)
			increment left wheel counter variable and distance variable
 */
void EXTI4_IRQHandler()
{
	countLeft++; 	    		// increment number of ticks
	encoderDistance++;		// increment encoder distance
	EXTI_PR |= 1; 				// clear interrupt flag
}

/* 	Function: EXTI9_5_IRQHandler()
			PA5 external interrupt handler (rising and falling)
			increment right wheel counter variable
 */
void EXTI9_5_IRQHandler()
{
	countRight++; 				// increment number of ticks
	EXTI_PR |= 1; 				// clear interrupt flag
}

/* 	Function: TIM3_IRQHandler()
			timer 3 interrupt handler (timer 3 is used for PWM outputs)
			only clears the interrupt flag
 */
void TIM3_IRQHandler()
{
	TIM3_SR &= 0xFFFE;  	// clear interrupt flag
}

/* 	Function: GetLeft()
			uses ADC to get distance from left distance sensor connected to PC5 and returns distance in cm
 */
float GetLeft()
{	
	ADC1_SQR3 &= 0xFFFFFFE0; 			// clear channel to convert
	ADC1_SQR3 |= 0x7;       			// set channel to PA7
	
	ADC1_CR2 |= (1 << 30);   			// start ADC
	
	while((ADC1_SR & 0x2) == 0); // wait for end of conversion
	
	float dist = 10.0 / (((float)ADC1_DR*(3.0/4095.0)) - .42); // calculate distance in cm
	
	if (dist < 0)			// if returned a negative distance, correct to 35 cm (equation doesn't correctly handle long distances)
		dist = 35;
	
	return dist;			// return the distance
}

/* 	Function: GetFront
			uses ADC to get distance from front distance sensor connected to PC4 and returns distance in cm
 */
float GetFront()
{
	ADC1_SQR3 &= 0xFFFFFFE0; 			// clear channel to convert
	ADC1_SQR3 |= 14;       				// set channel to PC4
	
	ADC1_CR2 |= (1 << 30);   			// start ADC
	
	while((ADC1_SR & 0x2) == 0); 	// wait for end of conversion
	
	float dist = 10.0 / (((float)ADC1_DR*(3.0/4095.0)) - .42);	// calculate distance in cm
	
	if (dist < 0)		// if returned a negative distance, correct to 35 cm (equation doesn't correctly handle long distances)
		dist = 35;
	
	return dist;		// return the distance
}

/* 	Function: FollowUntilDistance(float distToFront)	
			input: distToFront (cm)
			follows the left wall (using left distance sensor) until the front sensor sees a wall that is the specified distance away
 */
void FollowUntilDistance(float distToFront)
{
		long high_time_right = RIGHTINIT;				// initialize right motor speed
		long high_time_left = LEFTINIT;					// initialize left motor speed
		int high_time_change = 0;								// intialize motor speed change to 0
	
		float distToWall, oldDistToWall = TARGETWALL;		// initialize old distance to wall to the target distance
	
	while(1)																	// loop until gets to specified distance
	{
		distToWall = GetLeft();									// get current distance from left wall
		
		// calculate the value of the change variable using a proportional and differential controller
		high_time_change = -1*P*(TARGETWALL - distToWall) + D*((distToWall - oldDistToWall) / T);
		
		oldDistToWall = distToWall;  						// store old distance
		
		high_time_right += high_time_change;		// adjust wheel speeds
		high_time_left -= high_time_change;
		
		// keep motor speeds between lower and upper limits:
		// left
		if (high_time_left > HIGHLIM)
			high_time_left = HIGHLIM;
		if (high_time_left < LOWLIM)
			high_time_left = LOWLIM;
		// right
		if (high_time_right > HIGHLIM)
			high_time_right = HIGHLIM;
		if (high_time_right < LOWLIM)
			high_time_right = LOWLIM;
		
		// set wheel speeds
		TIM3_CCR1 = high_time_right;
		TIM3_CCR2 = high_time_left;
		
		Delay();													// delay for 20 ms (using timer 6)											
		
		if (GetFront() <= distToFront)		// exit function when the front is specified distance from the front wall
			return;
	}
}

/* 	ForwardUntilDistance(float distToFront)
			input: distToFront (cm)
			drives straight forward (using encoders to correct wheel speed) until the front sensor sees a wall that is the specified distance away
 */
void ForwardUntilDistance(float distToFront)
{
	countLeft = TARGETCOUNT;   				// don't adjust first interrupt
	countRight = TARGETCOUNT;
	
	TIM3_CCR1 = RIGHTINIT;	 					// set right wheel to intial speed
	TIM3_CCR2 = LEFTINIT; 						// set left wheel to intial speed
	
	encoder_high_time_left = LEFTINIT;
	encoder_high_time_right = RIGHTINIT;
	
	TIM4_CR1 |= 1; 										// enable timer 4 (adjusts wheel speeds to stay straight)
	
	while(GetFront() > distToFront)		// move forward until specified distance from front wall
		Delay();												// delay 20 ms
	
	// when finished:
	TIM4_CR1 &= 0xFFFE; 							// disable timer 4
	
	TIM3_CCR1 = 0;	 									// stop right wheel
	TIM3_CCR2 = 0; 										// stop left wheel
}

/* 	Function: FollowForTicks(int encoderTicks)
			input: encoderTicks (distance to drive in wheel encoder ticks)
			follows the left wall (using left distance sensor) until it has moved the specified distance
 */
void FollowForTicks(int encoderTicks)
{
	encoderDistance = 0;																	// reset left encoder count (used for tracking distance traveled)
	long high_time_right = RIGHTINIT;								// initialize right motor speed
	long high_time_left = LEFTINIT;									// initialize left motor speed
	int high_time_change =0;												// intialize motor speed change to 0
	
	float distToWall, oldDistToWall = TARGETWALL;		// initialize distance to wall
	
	while(1)																				// loop until reached target distance
	{
		distToWall = GetLeft();												// get current distance from left wall
		
		// calculate the value of the change variable using a proportional and differential controller
		high_time_change = -1*P*(TARGETWALL - distToWall) + D*((distToWall - oldDistToWall) / T);
		
		oldDistToWall = distToWall;  									// store old distance
		
		high_time_right += high_time_change;					// adjust wheel speeds
		high_time_left -= high_time_change;
		
		// keep motor speeds between lower and upper limits:
		// left
		if (high_time_left > HIGHLIM)
			high_time_left = HIGHLIM;
		if (high_time_left < LOWLIM)
			high_time_left = LOWLIM;
		// right
		if (high_time_right > HIGHLIM)
			high_time_right = HIGHLIM;
		if (high_time_right < LOWLIM)
			high_time_right = LOWLIM;
		
		// set wheel speeds
		TIM3_CCR1 = high_time_right;
		TIM3_CCR2 = high_time_left;
		
		Delay();																			// delay for 20 ms (using timer 6)	
		
		if (encoderDistance >= encoderTicks)								// exit function after robot has moved specified distance
			return;
	}
}

/* 	Function: ForwardForTicks(int encoderTicks)
			input: encoderTicks (distance to drive in wheel encoder ticks)
			drives straight forward (using encoders to correct wheel speed) for the distance specified (measured using left encoder)
 */
void ForwardForTicks(int encoderTicks)
{
	encoderDistance = 0;
	
	countLeft = TARGETCOUNT;   	// don't adjust first interrupt
	countRight = TARGETCOUNT;
	
	encoder_high_time_left = LEFTINIT;
	encoder_high_time_right = RIGHTINIT;
	
	TIM3_CCR1 = RIGHTINIT;	 		// set right wheel to intial speed
	TIM3_CCR2 = LEFTINIT; 			// set left wheel to intial speed
	
	TIM4_CR1 |= 1; 							// enable timer 4	
	
	while(encoderDistance < encoderTicks)	// move forward until left sensor sees a wall
		Delay();														// delay 20ms
	
	TIM4_CR1 &= 0xFFFE; // disable timer 4
	
	// when finished:
	TIM3_CCR1 = 0;	 			// stop right wheel
	TIM3_CCR2 = 0; 				// stop left wheel
}

/*  Function: ForwardUntilStartOfWall()
			drives straight forward (using encoders to correct wheel speed) until there is a wall to the left
 */
void ForwardUntilStartOfWall()
{
	countLeft = TARGETCOUNT;   	// don't adjust first interrupt
	countRight = TARGETCOUNT;
	
	encoder_high_time_right = RIGHTINIT;
	encoder_high_time_left = LEFTINIT;
	
	TIM3_CCR1 = RIGHTINIT;	 		// set right wheel to intial speed
	TIM3_CCR2 = LEFTINIT; 			// set left wheel to intial speed
	
	TIM4_CR1 |= 1; 							// enable timer 4	
	
	while(GetLeft() > 20)				// move forward until left sensor sees a wall
		Delay();									// delay between checks
	
	TIM4_CR1 &= 0xFFFE; 				// disable timer 4
	
	// when finished:
	TIM3_CCR1 = 0;	 						// stop right wheel
	TIM3_CCR2 = 0; 							// stop left wheel
}

/* 	Function: FollowUntilEndOfWall()
			follows the left wall (using left distance sensor) until the left wall stops
 */
void FollowUntilEndOfWall()
{
	long high_time_right = RIGHTINIT;								// initialize right motor speed
	long high_time_left = LEFTINIT;									// initialize left motor speed
	int high_time_change =0;												// intialize motor speed change to 0
	
	float distToWall, oldDistToWall = TARGETWALL;		// initialize distance to wall
	
	while(1)																				// loop until reached target distance
	{
		distToWall = GetLeft();												// get current distance from left wall
		
		// calculate the value of the change variable using a proportional and differential controller
		high_time_change = -1*P*(TARGETWALL - distToWall) + D*((distToWall - oldDistToWall) / T);
		
		oldDistToWall = distToWall;  									// store old distance
		
		high_time_right += high_time_change;					// adjust wheel speeds
		high_time_left -= high_time_change;
		
		// keep motor speeds between lower and upper limits:
		// left
		if (high_time_left > HIGHLIM)
			high_time_left = HIGHLIM;
		if (high_time_left < LOWLIM)
			high_time_left = LOWLIM;
		// right
		if (high_time_right > HIGHLIM)
			high_time_right = HIGHLIM;
		if (high_time_right < LOWLIM)
			high_time_right = LOWLIM;
		
		// set wheel speeds
		TIM3_CCR1 = high_time_right;
		TIM3_CCR2 = high_time_left;
		
		Delay();																			// delay for 20 ms (using timer 6)	
		
		if (distToWall >= FALLOFFDIST)													// stop when wall is over 30 cm away from left sensor
			return;
	}
}
