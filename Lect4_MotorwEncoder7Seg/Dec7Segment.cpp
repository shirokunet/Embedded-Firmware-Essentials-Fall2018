/*
 * Dec7Segment.cpp
 *
 *  Created on: Sep 28, 2018
 *      Author: wacheda
 */
#include "mbed.h"
#include "Dec7Segment.h"

void Dec7Segment(int i)
{
	int value[4];
	value[3] = i/1000;
	value[2] = (i-value[3]*1000)/100;
	value[1] = (i - value[3]*1000 - value[2]*100)/10;
	value[0] = (i - value[3]*1000 - value[2]*100 - value[1]*10);

	unsigned int display_time_ms = 0;

	while(display_time_ms <= total_display_time_ms)
	{
		for(int j = 0;j<4; ++j)	// Use all 4 digits
		{
			digit = 1<<j;
			switch((i>>(4*j))&0x00F)
			{
			case 0x0:	number = 0x3F; 	break;
			case 0x1:	number = 0x06; 	break;
			case 0x2:	number = 0x5B; 	break;
			case 0x3:	number = 0x4F;	break;
			case 0x4: 	number = 0x66;	break;
			case 0x5:	number = 0x6D;	break;
			case 0x6:	number = 0x7D;	break;
			case 0x7:	number = 0x07;	break;
			case 0x8:	number = 0x7F;	break;
			case 0x9:	number = 0x6F;	break;
			case 0xA:	number = 0x77;	break;
			case 0xB:	number = 0x7C;	break;
			case 0xC:	number = 0x39;	break;
			case 0xD:	number = 0x5E;	break;
			case 0XE:	number = 0x79;	break;
			case 0xF:	number = 0x71;	break;
			}
			wait_ms(persist_time_ms);
			display_time_ms += persist_time_ms;
		}
	}
}
