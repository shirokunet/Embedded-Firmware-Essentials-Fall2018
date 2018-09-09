/* Lecture 1 DigitalIn Example
*/
#include "mbed.h"

DigitalOut leds[] = {(p17),(p18),(p19),(p20)};
DigitalIn direction(p10);

int main() 
{
//	direction.mode(PullUp);
//	direction.mode(PullDown);
//	direction.mode(PullNone);
    while(1) 
    {
    	if(direction)
    	{
			leds[0] = 1;
			wait(0.5);
			leds[0] = 0;
			leds[1] = 1;
			wait(0.5);
			leds[1] = 0;
			leds[2] = 1;
			wait(0.5);
			leds[2] = 0;
			leds[3] = 1;
			wait(0.5);
			leds[3] = 0;
    	}
    	else
    	{
    		leds[3] = 1;
			wait(0.5);
			leds[3] = 0;
			leds[2] = 1;
			wait(0.5);
			leds[2] = 0;
			leds[1] = 1;
			wait(0.5);
			leds[1] = 0;
			leds[0] = 1;
			wait(0.5);
			leds[0] = 0;
    	}
    }
}
