/*********************************************************************************************************/
//
// Lecture 7 - Digital Output & Timing using Control Registers
//
//
//
/*********************************************************************************************************/
#include <stdint.h> // needed in order to define uintx_t variable type

// Control registers for fast GPIO
#define FIO2DIR0 	(*(volatile uint8_t *)(0x2009C040))  // These registers are only 8 bits wide
#define FIO2PIN0 	(*(volatile uint8_t *)(0x2009C054))  // See LPC1768 User Manual Table 105 Fast GPIO Direction Control Byte Registers

// Control registers for Timer 0
#define TIM0_IR		(*(volatile uint32_t *)(0x40004000)) // These registers are 32 bits wide
#define TIM0_TCR 	(*(volatile uint32_t *)(0x40004004)) // See Section 21.6, Table 426 TIMER/COUNTER-3 register map
#define TIM0_TC 	(*(volatile uint32_t *)(0x40004008))
#define TIM0_PR 	(*(volatile uint32_t *)(0x4000400C))
#define TIM0_PC 	(*(volatile uint32_t *)(0x40004010))
#define TIM0_MCR 	(*(volatile uint32_t *)(0x40004014))
#define TIM0_MR0 	(*(volatile uint32_t *)(0x40004018))

void delay_ms(uint32_t delayInMs);

int main()
{

	FIO2DIR0 = 0xFF;		// set port 2, lowest byte to output (MBED p26)
	while(1)
	{
		FIO2PIN0 |= 0x01;	// OR bit 0 with 1 to set pin HIGH
		delay_ms(100);
		FIO2PIN0 &= ~0x01;	// AND bit 0 with 0 to set pin LOW
		delay_ms(100);
	}
}

// ms delay function using LPC1768 Timer 0
void delay_ms(uint32_t delayInMs)
{
	TIM0_TCR =	0x02;	// Reset Timer 0
	TIM0_PR	 =	0x00;	// Set prescaler to zero; Default clock determined by bits 3:2 of PCLKSEL0 (Reset value is 00 or CCLK/4 = 24 MHz)
	TIM0_MR0 = 	(uint32_t)(delayInMs * (24000000 / 1000-1)); // Set value for timer to match for mS resolution
	TIM0_IR  = 	0xFF;	// Reset all Timer 0 interrupts
	TIM0_MCR =	0x04;	// Stop Timer 0 on match
	TIM0_TCR =	0x01;	// Start Timer 0

	/* wait until delay time has elapsed */
	while (TIM0_TCR & 0x01);
}
