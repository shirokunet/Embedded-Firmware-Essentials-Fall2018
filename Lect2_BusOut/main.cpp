/* Lecture 2 - BusOut() Example */

#include "mbed.h"

BusOut led(LED1,LED2,LED3,LED4);

// To enable printing output to a terminal program such as minicom in Ubuntu
Serial pc(USBTX, USBRX); // tx, rx

void bin_prnt_byte(int x)
{
   for(int n=0; n<8; n++)
   {
      if((x & 0x80) !=0)
         pc.printf("1");
      else
         pc.printf("0");
      if (n==3)
         pc.printf(" "); /* insert a space between nibbles */
      x = x<<1;
   }
}

int main() 
{
    while(1) 
    {
    	for(int i=0; i<=0xF; ++i)
    	{
    		pc.printf("%d,\t0x%02x,\t",i,i);
    		bin_prnt_byte(i);
    		pc.printf("\r\n");
    		led = i;
    		wait_ms(500);
    	}
    }
}
