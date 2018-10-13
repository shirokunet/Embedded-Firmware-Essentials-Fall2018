/*********************************************************************************************************/
//
// Lecture 5 - Modular Programming: ADXL345 Using SPI
//
//
//
/*********************************************************************************************************/
#include "mbed.h"
#include "ADXL345.h"

Serial pc(USBTX,USBRX);



int main()
{
	float x, y, z;

	pc.baud(115200);
	pc.printf("Starting ADXL345 Test...\r\n\r\n");
	// Initialize ADXL345
	ADXL345_init();

	while(1)
	{
		wait_ms(200);
		ADXL345_meas();
		x = acc_x();
		y = acc_y();
		z = acc_z();
		pc.printf("x:  %+4.2f g, y:  %+4.2f g, z:  %+4.2f g\r\n",x,y,z);
	}
}
