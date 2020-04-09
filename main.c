#include "stm32f446xx.h"

void main(void)
{

//clock at 180mhz
RCC   -> PLLCFGR  &= ~(1<<22);                                //set HSI as PLL input
RCC   -> PLLCFGR  |= (1<<4);                                  //set PLLM prescaler to 16
RCC   -> PLLCFGR  &= ~(0x7FC0);                               //Clears PLL multimpiler
RCC   -> PLLCFGR  |= ((1<<9) | (1<<11) | (1<<12) | (1<<14));  //PLL multimpiler *360
RCC   -> CR       |= (1<<24);                                 //enables PLL
PWR   -> CR       |= (1<<17);                                 //enables overdrive mode
FLASH -> ACR      |= ((1<<0) | (1<<2));                       //5 wait states
RCC   -> CFGR     |= ((1<<10) | (1<<12));                     //AHB1 presclaer = /4
RCC   -> CFGR     |= (1<<15);                                 //AHB2 prescaler = /2
RCC   -> CFGR     |= (1<<1);                                  //sets PLL as main clock input

//Set up PA8(Timer1/Channel1 Output)
GPIOA -> MODER    |= 0x00020000; //PA Pin 8 as Alternate Function
GPIOA -> OTYPER   |= 0x00000000; //PA Pin 8 as Push/Pull
GPIOA -> OSPEEDR  |= 0x00030000; //PA Pin 8 as High speed
GPIOA -> PUPDR    |= 0x00000000; //PA Pin 8 as No pull-up/down
GPIOA -> AFR[1]   |= 0x00000001; //PA Pin 8 as Timer1/Channel1 Output

//Set up Timer
//enable clock input fot the timer
RCC   -> APB2ENR  |= 0x0001;     //enables clock for Timer1
//set prescaler
TIM1  -> PSC      |= 0x0000;     //Precaler for Timer 1 = 0(+1 from Hardware)
//set overflow value
TIM1  -> ARR      = 225;
//enable PWM by writing 110 to TIM1 ->CCMR1 in bits OC1M
TIM1  -> CCMR1    |= 0x0060;
//enable fast mode
TIM1  -> CCMR1    |= 0x000C;
//enable Auto reload preload
TIM1  -> CR1      |= 0x0080;
//enables the output of Channel 1
TIM1  -> CCER     |= 0x0005;
//enable Output
TIM1  -> BDTR     |= 0x8000;     //Main Output enable
//Set compare register
TIM1  -> CCR1     |= 150;
//generate an Update
TIM1  -> EGR      |= 0x0001;
//enable counter
TIM1  -> CR1      |= 0x0001;



while(1)
  {
    //main loop
    for(uint64_t time = 0; time<1000000; time=time+1);
    TIM1  -> CCR1     = 0;
    for(uint64_t time = 0; time<1000000; time=time+1);
    TIM1  -> CCR1     = 150;
  }
}
