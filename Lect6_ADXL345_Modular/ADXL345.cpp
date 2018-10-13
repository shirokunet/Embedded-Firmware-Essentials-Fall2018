/*
 * ADXL345.cpp
 *
 *  Created on: Oct 12, 2018
 *      Author: wacheda
 */

/*
 * ADXL345.cpp
 *
 *  Created on: May 31, 2018
 *      Author: wacheda
 */


#include "ADXL345.h"
SPI acc(p11,p12,p13);  // MOSI (SDA), MISO (SDO), SCL
DigitalOut cs(p14);

char buffer[6];
int16_t data[3];

void ADXL345_init(void)
{
	cs = 1;					// ADXL345 not selected
	acc.format(8,3);		// 8 bit data, SPI Mode 3
	acc.frequency(2000000);	// 2 MHz SPI clock rate

	cs = 0;					// Select the ADXL345
	acc.write(0x31);		// ADXL345 Data Format Register
	acc.write(0x0B);		// format +/-16g, 0.003g/LSB
	cs = 1;					// End of Transmission

	cs = 0;					// Start a new Tranmission
	acc.write(0x2D);		// ADXL345 Power Control Register
	acc.write(0x08);		// Select measure mode
	cs = 1;					// End of Transmission
}

void ADXL345_meas()
{
	cs = 0;				// Start a transmission
	acc.write(0x80|0x40|0x32);	// RW bit high, MB bit high, plus address
	for(int i = 0;i <= 5; ++i)
	{
		buffer[i] = acc.write(0x00);	// Read back 6 data bytes
	}
	cs = 1; 			// End of Transmission

	// Interpret the raw data bytes into meaningful results
	data[0] = buffer[1]<<8 | buffer[0]; 	// Combine MSB with LSB
	data[1] = buffer[3]<<8 | buffer[2];
	data[2] = buffer[5]<<8 | buffer[4];
}

float acc_x(void)
{
	return data[0]*Sensitivity;	// x-axis acceleration in G's
}

float acc_y(void)
{
	return data[1]*Sensitivity;	// x-axis acceleration in G's
}

float acc_z(void)
{
	return data[2]*Sensitivity;	// x-axis acceleration in G's
}


