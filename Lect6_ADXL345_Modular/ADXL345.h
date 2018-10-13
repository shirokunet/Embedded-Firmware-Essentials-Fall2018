/*
 * ADXL345.h
 *
 *  Created on: Oct 12, 2018
 *      Author: wacheda
 */

#ifndef ADXL345_H_
#define ADXL345_H_


#include "mbed.h"

const float Sensitivity = 0.0039;	// G's/LSB (per ADXL345 Datasheet)

void ADXL345_init(void);
void ADXL345_meas(void);
float acc_x(void);
float acc_y(void);
float acc_z(void);



#endif /* ADXL345_H_ */
