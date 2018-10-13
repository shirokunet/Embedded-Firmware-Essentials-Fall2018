/*********************************************************************************************************/
//
// Lecture 6 - Serial Interrupt
//
//
//
/*********************************************************************************************************/
#include "mbed.h"

DigitalOut led(LED1);
DigitalOut led2(LED2);

Serial pc(USBTX,USBRX);

volatile bool fKeyInput = false;

void Rx_interrrupt()
{
	led = !led;
	char c = pc.getc();

	if(!fKeyInput && (c == 'c' || c == 'C'))
	{
		led2 = !led2;
		fKeyInput = true;
	}
}

int main()
{
	pc.attach(&Rx_interrrupt, Serial::RxIrq);

	pc.printf("\r\n\r\nStarting Serial Interrupt Example...\r\n");

	while(1)
	{
		// Put this printf in for demo. In real application, you may want to comment this printf out
		pc.printf("Press 'c' or 'C' to enter Input Mode\r\n\r\n");
		if(fKeyInput)
		{
			LPC_UART0->IER = 0;                 //Disable Rx interrupt on mbed1768
			pc.printf("\r\n\r\nConfirm you want to enter Input Mode (y/N)?");
			char c = pc.getc();
			pc.putc(c);
			if(c=='y' || c == 'Y')
			{
				pc.printf("\r\n\r\nNow in INPUT MODE!");
				c = 'n';
				while(1)
				{
					pc.printf("\r\n\r\nConfirm you want to exit Input Mode (y/N)?");
					c = pc.getc();
					pc.putc(c);
					if(c == 'y' || c == 'Y')
						break;
				}
				pc.printf("\r\n\r\nExiting Input Mode");
			}
			pc.printf("\r\n\r\n");
			fKeyInput = false;
			LPC_UART0->IER = 1;                 //RE-enable Rx interrupt on mbed1768
		}
		wait(.5);
	}
}
