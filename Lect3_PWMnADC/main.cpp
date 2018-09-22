/* Lecture 3 	Controlling Motor Speed with PWM,
 * 				Measuring Battery Voltage with AnalogIn
*/
#include "mbed.h"

const unsigned int EncoderCntsperRev = 20;
volatile unsigned int A_cnt;
float delay_sec = 4.;
unsigned int pwm_period_us = 10000;

//DigitalOut da(p10);


PwmOut motor(p21);
AnalogIn Vmeas(p20);
InterruptIn encoder_A(p6);
Serial pc(USBTX, USBRX);

void ISR_A()
{
	A_cnt++;
}

int main() 
{
	float bat_volt;
	encoder_A.rise(&ISR_A);

	motor.period_us(pwm_period_us);

	pc.printf("\r\n\r\nPWM DC Motor Speed Control\r\n\r\n");
	pc.printf("PWM period = %d us, ",pwm_period_us);
	pc.printf("PWM frequency = %.3f kHZ\r\n\r\n",1./pwm_period_us/1.e-6/1.e3);

	while(1)
    {
    	for(int i=0;i<=10;++i)
    	{
    		bat_volt = Vmeas*3.3*(9.8+3.3)/3.3;
    		pc.printf("Duty Cycle = %.1f %%,\tVbat = %.2f V,\t",(10-i)/10.*100., bat_volt);

    		motor = (10-i)/10.;
    		wait(1.);  // Let's give some time for the motor speed to change

    		A_cnt = 0;  // Reset encoder A pulse count to 0 to restart RPM measurement
    		wait(delay_sec);
    		pc.printf("%4d cnts,\t%4d RPM\r\n",A_cnt,int(A_cnt/EncoderCntsperRev/delay_sec*60.));

    	}
    	pc.printf("\r\n");

    }
}
