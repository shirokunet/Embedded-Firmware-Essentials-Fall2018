/*
 * SpekReceiver.h
 *
 *  Created on: Oct 17, 2018
 *      Author: wacheda
 */

#ifndef SPEKRECEIVER_H_
#define SPEKRECEIVER_H_

#include "mbed.h"

const uint8_t CH_MAX = 7;

enum receive_status_t {NO_SIGNAL, START_UP, FAIL_SAFE, READY};

class SpekReceiver : public Serial {
public:
    SpekReceiver(PinName _tx, PinName _rx, const char *_name=NULL);
    virtual ~SpekReceiver();

    uint16_t ch[CH_MAX];

    bool Check_Data_Ready(void);
    receive_status_t update(void);
    receive_status_t status(void);

private:
	PinName tx;
	PinName rx;

	receive_status_t state;

	Timer tm;

	volatile bool bufferAB;
	volatile uint8_t position;
	volatile uint8_t bytesA[16];
	volatile uint8_t bytesB[16];

	volatile bool Data_Ready;
	volatile bool sync;
	volatile unsigned int last_frame_stop;

	const uint16_t failsafe_values[8] = {1500,1500,1000,1500,1500,1500,1500,1295};
	const uint16_t startup_values[8]  = {1500,1500,1000,1500,1500,1500,1500,1900};

	uint8_t fades;
	uint8_t rc_protocol;
	bool eleven_bit = false;

	void rx_handler(void);
};





#endif /* SPEKRECEIVER_H_ */
