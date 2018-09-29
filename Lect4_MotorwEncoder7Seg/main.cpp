/*  Lecture 4  Motor with Rotary Encoder & 7-segment Display */


#include "mbed.h"
#include "Dec7Segment.h"

const unsigned int EncoderCntsperRev = 20;
const unsigned int SampleTime_us = 1000000;
volatile int rpm;
volatile unsigned int A_cnt;
float delay_sec = 4.;
unsigned int pwm_period_us = 1000;

volatile int pulses;
volatile bool halfright;
volatile bool halfleft;

int prev_cnt, cur_cnt;

DigitalOut redLED(p5);
PwmOut motor(p21);

InterruptIn encoder_A(p6);
InterruptIn encoder_B(p7);
Ticker tkRPM;
Ticker tkDutyCycle;

Serial pc(USBTX,USBRX);

void ISR_A()
{
	int chanA = encoder_A.read();
	int chanB = encoder_B.read();

	if(chanA == 0)
	{
		if(chanB == 1 && halfright == false)
			halfright = true;
		if(chanB == 0 && halfleft == true)
		{
			halfleft = false;
			pulses--;
		}
	}
}


void ISR_B()
{	int chanA = encoder_A.read();
	int chanB = encoder_B.read();
	if(chanB == 0)
	{
		if(chanA == 1 && halfleft == false)
			halfleft = true;
		if(chanA == 0 && halfright == true)
		{
			halfright = false;
			pulses++;
		}
	}
}

void calcRPM(void)
{
	cur_cnt = pulses;
	rpm = (cur_cnt-prev_cnt)/EncoderCntsperRev/(SampleTime_us*1.e-6)*60;
	prev_cnt = cur_cnt;
}

void changeDutyCycle(void)
{
	motor = motor.read() - .05;
	if(motor.read()<.24)
		motor = 1.;
	redLED = !redLED;
}


int main() 
{
	encoder_A.rise(&ISR_A);
	encoder_B.rise(&ISR_B);

	motor.period_us(pwm_period_us);
	motor = 1.;

	redLED = 1;

	tkRPM.attach_us(&calcRPM,SampleTime_us);
	tkDutyCycle.attach(&changeDutyCycle,10.);

	pc.printf("\r\n\r\nPWM DC Motor Speed Control\r\n\r\n");
	pc.printf("PWM period = %d us, ",pwm_period_us);

	while(1)
	{
		Dec7Segment(rpm);
	}

}
