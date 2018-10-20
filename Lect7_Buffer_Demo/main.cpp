/*
 * 		Lecture 7 - Circular Buffer Demo using
 * 		Spektrum Satellite RC Receiver
 */
#include "mbed.h"
#include "SpekReceiver.h"

SpekReceiver rc(p9,p10);
Serial pc(USBTX, USBRX);

Timer tm_status;
Timeout Process_Buffer;

DigitalOut marka(p5);
DigitalOut markb(p6);
DigitalOut markc(p7);

int main(void){
	uint8_t state = 0;

	pc.baud(115200);

    pc.printf("Starting Buffer Demo ...\r\n");

    tm_status.reset();
    tm_status.start();

    while(1)
    {
//      markb = 1;
		state = rc.update();
//		markb = 0;

		switch(state)
		{
		    // Use state to determine what control signals to send to car
		}

		if(tm_status.read()>=.5)
		{
			markb = 1;
			switch(rc.status())
			{
			case NO_SIGNAL:
				pc.printf("NO_SIG\r\n");
				break;

			case START_UP:
				pc.printf("START_UP\r\n");
				break;

			case FAIL_SAFE:
				pc.printf("FAIL_SAFE:  ");
				for(uint8_t i=0;i<CH_MAX;++i)
					pc.printf("%d ",rc.ch[i]);
				pc.printf("\r\n");
				break;

			case READY:
				pc.printf("READY:  ");

				for(uint8_t i=0;i<CH_MAX;++i)
					pc.printf("%d ",rc.ch[i]);
				pc.printf("\r\n");
				break;
			}
			tm_status.reset();
			markb = 0;
		}
    }
}



