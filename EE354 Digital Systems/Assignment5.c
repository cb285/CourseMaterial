/*LEDPA7.c                               October 24, 2016
  This program blinks an LED connected to PA7 about six times a 
   second.
*/
#include "stm32f407vg.h"

int main()
  {int i, tmp;
   //Clock bits
   RCC_AHB1ENR |= 1;         //Bit 0 is GPIOA clock enable bit 
   RCC_AHB1ENR |= 2;         //Bit 1 is GPIOB clock enable bit
   //I/O bits  PA7
   GPIOA_MODER |= 0x4000; //Bits 15-14 = 01 for digital output on PA7
   //OTYPER register resets to 0 so it is push/pull by default
   GPIOA_OSPEEDER |= 0xC000;  //Bits 15-14 = 11 for high speed on PA7
   //PUPDR defaults to no pull up no pull down    
   //Main program loop
   tmp = 0;
   while(1)
     {if(tmp == 0)
         GPIOA_ODR &= ~(1 << 7); //Only PA7 is set up for output so                               
      else                       //  Other bits are not changed
         GPIOA_ODR |= (1 << 7);
      tmp = ~tmp;                
      for(i=0;i<200000;i++);    //Put this in to slow down toggle
     }  
  }
