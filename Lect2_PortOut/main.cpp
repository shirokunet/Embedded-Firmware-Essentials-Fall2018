/* Lecture 2 - PortOut() Example */

#include "mbed.h"

// LED1 is connected to Port 1, bit 18;  LED2 is connected to Port 1, bit 20
// LED3 is connected to Port 1, bit 21;  LED4 is connected to Port 1, bit 23
#define LED_MASK 0x00B40000
#define LED1_MASK 0x00040000
#define LED2_MASK 0x00100000
#define LED3_MASK 0x00200000
#define LED4_MASK 0x00800000

PortOut LEDPort(Port1, LED_MASK);

Serial pc(USBTX, USBRX); // tx, rx

void bin_prnt_4byte(uint32_t x)
{
   for(int n=0; n<32; n++)
   {
      if((x & 0x80000000) !=0) pc.printf("1");
      else pc.printf("0");
      if((n+1)%4==0) pc.printf(" "); /* insert a space between nibbles */
      x = x<<1;
   }
}

int main() 
{
    while(1) 
    {
    	for(int i=0; i<=0xF; ++i)
    	{
    		uint32_t value = 0;
    		if(i&0x01) value |= LED1_MASK;
    		if(i&0x02) value |= LED2_MASK;
    		if(i&0x04) value |= LED3_MASK;
    		if(i&0x08) value |= LED4_MASK;

    		pc.printf("%d,\t0x%02x,\t",i,i);
    		pc.printf("value:  0x%08x\t",value);
    		bin_prnt_4byte(value);
    		pc.printf("\r\n");

    		LEDPort = LEDPort & ~(LED_MASK);  // Clear all 4 LED bits.  Leave other bits unaffected.
    		LEDPort = LEDPort | value;		// Set the proper bits for LED1 - LED4

    		wait_ms(250);
    	}
    }
}
