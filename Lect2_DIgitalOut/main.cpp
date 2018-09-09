/* Lecture 2 - DigitalOuput() Examples */

#include "mbed.h"

DigitalOut ledA(LED1);
DigitalOut ledB(LED2,1);

// To enable printing output to a terminal program such as minicom in Ubuntu
Serial pc(USBTX, USBRX); // tx, rx


int main() 
{

	int i = 0;
	int value;
    while(1) 
    {
    	ledA = 1;			// operator= (int value)  		- C++ shorthand for write
    	wait(1.);			// wait takes float input of seconds

    	ledB = !ledB;  		// operator= (DigitalOut &rhs)  - C++ shorthand for write

    	value = int(ledB);	// operator int()    			- C++ shorthand for read

    	// prints to pc object defined above using printtf
    	// NOTE:  to print floats, need to modify Makefile variable NO_FLOAT_PRINTF := 0
    	pc.printf("%d: Value of ledB = %d\r\n",i++, value);
    	wait_ms(1000);		// wait_ms takes int input of milli-seconds

    	ledA.write(0);
    	wait_us(1000000);	// wait_us takes int input of micro-seconds


    }
}
