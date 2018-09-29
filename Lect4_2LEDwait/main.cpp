/* Lecture 4 - wait example */

#include "mbed.h"

DigitalOut RedLED(p5);
DigitalOut GreenLED(p6);

Serial pc(USBTX, USBRX); // tx, rx


int main() 
{
	RedLED = GreenLED = 1;
    while(1)
    {
    	wait(1.);
    	RedLED = !RedLED;

    	wait(1.);
    	RedLED = !RedLED;
    	GreenLED = !GreenLED;  		// operator= (DigitalOut &rhs)  - C++ shorthand for write
    }
}

