

/* Lecture 4 - Timer Example */

#include "mbed.h"

DigitalOut RedLED(p5);
DigitalOut GreenLED(p6);

Timer tm1;
Timer tm2;

void task_RED();
void task_GREEN();

int main() 
{

	tm1.start();
	tm2.start();

	GreenLED = RedLED = 1;

    while(1)
    {
    	if(tm1.read_ms()>=1000)
    	{
    		task_RED();
    		tm1.reset();
    	}

    	if(tm2.read_ms()>=2000)
    	{
    		task_GREEN();
    		tm2.reset();
    	}

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

