/*********************************************************************************************************/
//
// Lecture 7 - ADC Conversion Time
//
//
//
/*********************************************************************************************************/
//#define MBED_CODE

#include <stdio.h>

#ifdef MBED_CODE
#include "mbed.h"
AnalogIn Ain(p16);
#endif

//const int SystemCoreClock = 96000000;	// mbed LPC1768 System Clock Frequency is 96 MHz
const int buffer_size = 100;

// Control registers for Timer 0
#define TIM0_IR		(*(volatile uint32_t *)(0x40004000)) // These registers are 32 bits wide
#define TIM0_TCR 	(*(volatile uint32_t *)(0x40004004)) // See Section 21.6, Table 426 TIMER/COUNTER-3 register map
#define TIM0_TC 	(*(volatile uint32_t *)(0x40004008))

// Control registers for PWM
#define PINSEL4 	(*(volatile uint32_t *)(0x4002C010)) // Pin Function Select Register 4
#define PINMODE1	(*(volatile uint32_t *)(0x4002C044)) // Pin Mode Select Register 1
#define PWM1_MCR	(*(volatile uint32_t *)(0x40018014)) // PWM1 Match Control Register
#define PWM1_MR0	(*(volatile uint32_t *)(0x40018018)) // PWM1 Match Match Register 0
#define PWM1_MR4	(*(volatile uint32_t *)(0x40018040)) // PWM1 Match Match Register 4
#define PWM1_TCR	(*(volatile uint32_t *)(0x40018004)) // PWM1 Timer Control Register
#define PWM1_PCR	(*(volatile uint32_t *)(0x4001804C)) // PWM1 PWM Control Register
#define PWM1_LER	(*(volatile uint32_t *)(0x40018050)) // PWM1 Load Enable Register

#define PINSEL1 	(*(volatile uint32_t *)(0x4002C004)) // Pin Function Select Register 1
#define PCONP   	(*(volatile uint32_t *)(0x400FC0C4)) // Peripheral Power Contorl Register

#define AD0CR    	(*(volatile uint32_t *)(0x40034000)) // A/D Control Register
#define AD0GDR    	(*(volatile uint32_t *)(0x40034004)) // A/D Global Data Register

#define PWMPRESCALE (24-1) //25 PCLK cycles to increment TC by 1 i.e. 1 Micro-second

void SetupPWM50Percent(float);

int main()
{
	unsigned int y[buffer_size];
	float frequency = 5000.;
	SetupPWM50Percent(frequency);

#ifdef MBED_CODE
	printf("\r\n\r\nUsing MBED ADC - PWM Freq = %.2f Hz\r\n",frequency);

	for(int i = 0;i < buffer_size;++i)
		y[i] = Ain.read_u16();
	printf("\r\n");
	for(int i = 0;i<buffer_size;++i)
			printf("%d, %d\r\n",i,y[i]);
#else
	printf("\r\n\r\nUsing control register ADC\r\n\r\n");

	char ADC_channel = 1;
	int ADCdata;

	// Setup A/D converter
	PCONP |= (1<<12);		// Power up ADC and turn on clock to ADC

	AD0CR = (1 << 21);	// Set PDN = 1  A/D is operational; Must Be set after turning on PCONP (bit 12) for ADC)
	PINSEL1 &= ~((unsigned int)0x3 << 16);	// clear bits 17:16
	PINSEL1 |= (unsigned int)0x1 << 16;		// set bits 17:16 to 01 to enable AD0,1 (mbed pin 16)
	PINMODE1 &= ~((unsigned int)0x3 << 16);	// clear bits 17:16
	PINMODE1 |= (unsigned int)0x2 << 16;	// Disable pull-up and pull-downs on AD0.1 pin (mbed pin 16)

	printf("Starting ADC - PWM Freq = %.2f Hz\r\n",frequency);

	for(int i = 0;i <buffer_size;++i)
	{
		AD0CR &= (AD0CR & 0xFFFFFF00);		// Clear bits 7:0, sets bit 16 to burst mode for ADC conversion (conversion rate governed by ADC hardware and runs at 200 kHz)
//		AD0CR &= 0xFFFFFF00;		// NOTE:  If you do this instead of AD0CR &= (AD0CR & 0xFFFFFF00), then conversion rate is actually 4.76 us instead of 5 us!
		AD0CR |= (1<<ADC_channel) | (1 << 24);	// Select ADC channel (bits 7:0) and start ADC conversion (bits 26:24)
		while((AD0GDR & 0x80000000) == 0)	// Wait for ADC conversion to complete (bit 31 = 1 when done)
		{
		}
		ADCdata = AD0GDR;			// Get ADC results in bits 15:4
		AD0CR &= 0xF8FFFFFF;		// Stop ADC conversion bits 26:24 = 0
		y[i] = (ADCdata>>4)&0x3FF;	// Parse out ADC results into useable form
	}

	printf("\r\n");
	for(int i = 0;i<buffer_size;++i)
		printf("%d, %d\r\n",i,y[i]);
#endif
}

void SetupPWM50Percent(float frequency)
{
	int PWMwidth;
	int PWMclock;
	int PWMperiod;

//	PWMclock = SystemCoreClock / 4;
	PWMclock = 96000000/4;
	printf("PWMclock = %d\r\n",PWMclock);
	PWMperiod = PWMclock / frequency;
	printf("PWMperiod = %d\r\n",PWMperiod);

	PINSEL4 |= 0x00000040;		// Set P2.3 for PWM1.4
	PWM1_MCR |= 0x00000002; 		// Reset PWM0 on match
	// Set up match registers
	PWM1_MR0 = PWMperiod; 			// Set MR0 (PWM rate)
	PWM1_PCR |= 0x00000000; 		// Select single edge PWM for PWM4
	PWM1_PCR |= 0x00001000; 		// Enable PWM4
	PWM1_TCR |= 0x08; 				// Enable PWM mode
	PWM1_TCR |= 0x01;				 // Enable counter

	PWMwidth = PWMperiod/2;	// Set to 50% duty cycle
	printf("PWMperiod = %d\r\n",PWMperiod);
	printf("PWMwidth = %d\r\n",PWMwidth);

	PWM1_MR4 = PWMwidth; // Set falling edge
	PWM1_LER |= 0x11; // Enable PWM Match 0 & 4 latch
}
