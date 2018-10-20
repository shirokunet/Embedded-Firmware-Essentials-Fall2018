/*
 * SpekReceiver.cpp
 *
 *  Created on: Oct 17, 2018
 *      Author: wacheda
 */
#include "SpekReceiver.h"

extern DigitalOut marka;
extern DigitalOut markb;

SpekReceiver::SpekReceiver(PinName _tx, PinName _rx , const char *_name)
:   Serial(_tx, _rx, _name), tx(_tx), rx(_rx)
{
    attach(this,&SpekReceiver::rx_handler, Serial::RxIrq);

    // Spek Sat Receiver Runs at 115200 baud
    baud(115200);
    Data_Ready = false;

    last_frame_stop = 0;

    position = 0;
	bufferAB = true;
	sync = false;

	state = NO_SIGNAL;

    tm.reset();
	tm.start();
}

SpekReceiver::~SpekReceiver()
{
}


bool SpekReceiver::Check_Data_Ready(void)
{
	if(Data_Ready)
	{
		Data_Ready=false;
		update();
		return true;
	}
	return false;
}

/*
 *    Receiver Interrupt Function
 */
void SpekReceiver::rx_handler(void)
{
	unsigned char c = getc();
	unsigned int m;
	unsigned int m2;

	m = tm.read_ms();
	m2 = tm.read_ms();
	if(m!=m2)
		m = tm.read_ms();

	if(sync)
	{
		if(bufferAB)
			bytesA[position] = c;
		else
			bytesB[position] = c;
		position++;

		if(position == 16)
		{
			sync = false;
			bufferAB = !bufferAB;  //Swap to other buffer
			Data_Ready = 1;
			last_frame_stop = m;
			marka = 0;
		}
	}
	else
	{
		if((m-last_frame_stop)>7)
		{
			sync = true;
			marka = 1;
			if(bufferAB)
			{
				bytesA[0] = c;
				position = 1;
			}
			else
			{
				bytesB[0] = c;
				position = 1;
			}
		}
		else
		{
			last_frame_stop = m;
		}
	}
}

receive_status_t SpekReceiver::update(void)
{
	unsigned int time_now;
	unsigned int m;
	unsigned int m2;
	uint8_t packet1[17];
	uint16_t temp;
	bool redo = 0;

	time_now = tm.read_ms();
	m2 = tm.read_ms();
	if(time_now!=m2)
		time_now = tm.read_ms();
	m = last_frame_stop;
	m2 = last_frame_stop;
	if(m!=m2)
		m = last_frame_stop;

	if(m == 0) // No data received yet.  Still in START_UP state
	{
		for(int i=0;i<CH_MAX;++i)
			ch[i] = startup_values[i];
		state = START_UP;
	}
	else if(time_now <m + 500)
	{
//		markb = 1;
		if(Data_Ready)
		{
			Data_Ready = false;
			do
			{
				packet1[16] = bufferAB;
				if(bufferAB)
					for(int i=0; i<16; i++)
						packet1[i] = bytesB[i];
				else
					for(int i=0; i<16; i++)
						packet1[i] = bytesA[i];
				if(packet1[16] != bufferAB)
				{
					// current buffer flipped before we finished processing it
					// so we need to go to the next one to catch up
					redo = 1;
				}
				else
				{
					redo = 0;
				}
			} while(redo);

			fades = packet1[0];
			rc_protocol = packet1[1];

			for (int i=2;i<15;i=i+2)
			{
				if(packet1[i]&0B01000000) eleven_bit = 1;
			}

			for(int i=2;i<15;i=i+2)
			{

				int ch_num;
				temp = packet1[i+1] + (packet1[i]<<8);
				if(eleven_bit) {
					ch_num = (temp&0B0111100000000000)>>11;
					ch[ch_num] = temp&0B0000011111111111;
					ch[ch_num] /= 2;
					ch[ch_num] += 1000;
				} else {
					ch_num = (temp&0B0011110000000000)>>10;
					ch[ch_num] = temp&0B0000001111111111;
					ch[ch_num] += 1000;
				}
			}
		}
		state = READY;
	}
	else
	{                // This is the failsafe case
		for(int i=0;i<CH_MAX;i++)
			ch[i] = failsafe_values[i];

		state = FAIL_SAFE;
	}
	markb = 0;
	return state;
}

receive_status_t SpekReceiver::status(void)
{
	return state;
}


