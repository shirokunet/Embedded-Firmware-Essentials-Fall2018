/* Lecture 4 - Ticker Example */

#include "mbed.h"

DigitalOut RedLED(p5);
DigitalOut GreenLED(p6);

Ticker tk1;
Ticker tk2;

void task_RED();
void task_GREEN();

Serial pc(USBTX,USBRX);

int main() 
{

	tk1.attach(&task_RED,1.);
	tk2.attach_us(&task_GREEN,2000000);


	GreenLED = RedLED = 1;

    while(1)
    {
    	// NOTE: there is no code in the infinite loop part of the program related to the LED's
    	pc.printf("I am working on other things and don't care about the flashing LED's\r\n");
    	wait(3.);
    }
}

void task_RED(void)
{
	RedLED = !RedLED;
}

void task_GREEN(void)
{
	GreenLED = !GreenLED;
}
