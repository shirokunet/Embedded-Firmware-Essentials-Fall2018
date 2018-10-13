/**    IAP demo : demo code for internal Flash memory access library
 *
 *        The internal Flash memory access is described in the LPC1768 and LPC11U24 usermanual.
 *            http://www.nxp.com/documents/user_manual/UM10360.pdf
 *            http://www.nxp.com/documents/user_manual/UM10462.pdf
 *
 *               LPC1768 --
 *                    Chapter  2: "LPC17xx Memory map"
 *                    Chapter 32: "LPC17xx Flash memory interface and programming"
 *                    refering Rev. 01 - 4 January 2010
 *
 *               LPC11U24 --
 *                    Chapter  2: "LPC11Uxx Memory mapping"
 *                    Chapter 20: "LPC11Uxx Flash programming firmware"
 *                    refering Rev. 03 - 16 July 2012
 *
 *  This main.cpp demonstrates how the flash can be erased and wrote.
 *
 *  This program tries to...
 *    0. read device ID and serial#
 *    1. check if the targat sector blank
 *    2. erase the sector if it was not blank
 *    3. write into the flash (prepare before write)
 *    4. verify the data by IAP command
 *    5. show the content of the flash
 *
 *  The Flash must be erased as sectors. No overwrite can be done like SRAM.
 *  So erase should be done in size of 4K or 32K.
 *
 *  Writing sector can be done with size of 256, 512, 1024 or 4096.
 *  If other size is used, the IAP returns an error.
 *  The SRAM memory should be allocated in
 *
 *
 *        Released under the MIT License: http://mbed.org/license/mit
 *
 *        revision 1.0  09-Mar-2010   1st release
 *        revision 1.1  12-Mar-2010   chaged: to make possible to reserve flash area for user
 *                                            it can be set by USER_FLASH_AREA_START and USER_FLASH_AREA_SIZE in IAP.h
 *        revision 2.0  26-Nov.2012   LPC11U24 code added
 *        revision 2.1  26-Nov-2012   EEPROM access code imported from Suga koubou san's (http://mbed.org/users/okini3939/) library
 *                                            http://mbed.org/users/okini3939/code/M0_EEPROM_test/
 *        revision 3.0  09-Jan-2015   LPC812 and LPC824 support added
 *        revision 3.1  13-Jan-2015   LPC1114 support added
 *        revision 3.1.1 16-Jan-2015  Target MCU name changed for better compatibility across the platforms
 *        revision 3.1.2 10-Mar-2015  merged with pull requests. reinvoke_isp() added and modified read_serial() to return a pointer.
 */

#include    "mbed.h"
#include    "IAP.h"

#define     MEM_SIZE        256		// memory buffer size must be either 256, 512, 1024 or 4096 when copying to flash
#define     TARGET_SECTOR    29     //  use sector 29 as target sector if it is on LPC1768

IAP     iap;

struct DATA_A
{
	float x;
	float y;
	int z;
};

struct DATA_B
{
	int a;
	char password[40];
};

Serial pc(USBTX,USBRX);

int main()
{
    static char    mem[ MEM_SIZE ];    //  RAM memory buffer to use when copying data to flash
    int     r;

    char *p;



    while(1)
    {
		pc.printf( "\r\n\r\n=== IAP: Flash memory commands ===\r\n" );
		pc.printf(         "*** NXP Device-ID = 0x%08X ***\r\n\r\n", iap.read_ID() );
		DEVICE_ID sn = iap.read_serial();

		pc.printf("Device SN: 0x%08X 0x%08X 0x%08X 0x%08X\r\n\r\n",sn.DIN_0, sn.DIN_1, sn.DIN_2, sn.DIN_3);



		pc.printf("What would you like to do?\r\n");
		pc.printf("   (1) Blank Check All Sectors\r\n");
		pc.printf("   (2) Blank Check Sector 29\r\n");
		pc.printf("   (3) Erase Sector 29\r\n");
		pc.printf("   (4) Read 1st 256 bytes of Sector 29\r\n");
		pc.printf("   (5) Write DATA Type A to flash\r\n");
		pc.printf("   (6) Write DATA Type B to flash\r\n");
		pc.printf("   (7) Read DATA Type A from flash\r\n");
		pc.printf("   (8) Read DATA Type B from flash\r\n");

		pc.printf("   ?  ");

		while(pc.readable()==false)
			;
		char ch = pc.getc();
		pc.putc(ch);

		pc.printf("\r\n");

		switch(ch)
		{
		case '1':
			for(int i=0;i<=29;++i)
			{
				if(i%5==0)
					pc.printf("\r\n");
				r   = iap.blank_check( i, i );
				pc.printf( "Sector %02d: %02d\t", i, r );
			}
			break;
		case '2':
			int sector;
			sector = 29;
			pc.printf("\r\n");
			r = iap.blank_check( sector, sector );
			pc.printf( "Sector %02d: %02d\r\n", sector, r );
			break;
		case '3':
			// Check to see if TARGET_SECTOR is BLANK
			r = iap.blank_check( TARGET_SECTOR, TARGET_SECTOR );

			// Erase Target Sector if NOT BLANK
			if ( r == SECTOR_NOT_BLANK ) {
				pc.printf("TARGET SECTOR is NOT BLANK!  Erasing...\r\n");
				iap.prepare( TARGET_SECTOR, TARGET_SECTOR );	// Always must prepare sector before erasing or writing
				r   = iap.erase( TARGET_SECTOR, TARGET_SECTOR );
				pc.printf( "erase result       = 0x%08X\r\n", r );
			}
			break;
		case '4':
			p = (char *) FLASH_SECTOR_29;
			for(int i = 0;i<256;++i)
			{
				if(i%16==0)
					pc.printf("\r\n");
				pc.printf("%02X\t",*(p+i));
			}
			break;
		case '5':
			struct DATA_A dataA;

			dataA.x = 3.14;
			dataA.y = -10.;
			dataA.z = 1024;

			// copy struct data into RAM mem buffer
			p = (char *)&dataA;

			memset(&mem[0], 0, sizeof(mem));	// Set all elements of mem array to 0

			// Copy data struct into mem array
			for(uint i = 0; i< sizeof(dataA);++i)
				mem[i] = *(p+i);

			/*
			// Check to see if TARGET_SECTOR is BLANK
			r = iap.blank_check( TARGET_SECTOR, TARGET_SECTOR );

			// Erase Target Sector if NOT BLANK
			if ( r == SECTOR_NOT_BLANK ) {
				pc.printf("TARGET SECTOR is NOT BLANK!  Erasing...\r\n");
				iap.prepare( TARGET_SECTOR, TARGET_SECTOR );	// Always must prepare sector before erasing or writing
				r   = iap.erase( TARGET_SECTOR, TARGET_SECTOR );
				pc.printf( "erase result       = 0x%08X\r\n", r );
			}
			*/

			// copy RAM to Flash
			iap.prepare( TARGET_SECTOR, TARGET_SECTOR );	// Always must prepare sector before erasing or writing
			r   = iap.write( mem, sector_start_adress[ TARGET_SECTOR ], MEM_SIZE );
			pc.printf( "copied: SRAM(0x%08X)->Flash(0x%08X) for %d bytes. (result=0x%08X)\r\n", mem, sector_start_adress[ TARGET_SECTOR ], MEM_SIZE, r );

			// compare
			r   = iap.compare( mem, sector_start_adress[ TARGET_SECTOR ], MEM_SIZE );
			pc.printf( "compare result     = \"%s\"\r\n", r ? "FAILED - Sector was probably not Blank before writing" : "OK" );

			break;
		case '6':
			struct DATA_B dataB;
			dataB.a = 1023;
			strcpy(dataB.password,"Test");
			// copy struct data into RAM mem buffer
			p = (char *)&dataB;

			memset(&mem[0], 0, sizeof(mem));	// Set all elements of mem array to 0

			// Copy data struct into mem array
			for(uint i = 0; i< sizeof(dataB);++i)
				mem[i] = *(p+i);
			/*
			// Check to see if TARGET_SECTOR is BLANK
			r = iap.blank_check( TARGET_SECTOR, TARGET_SECTOR );

			// Erase Target Sector if NOT BLANK
			if ( r == SECTOR_NOT_BLANK ) {
				pc.printf("TARGET SECTOR is NOT BLANK!  Erasing...\r\n");
				iap.prepare( TARGET_SECTOR, TARGET_SECTOR );
				r   = iap.erase( TARGET_SECTOR, TARGET_SECTOR );
				pc.printf( "erase result       = 0x%08X\r\n", r );
			}
			*/

			// copy RAM to Flash
			iap.prepare( TARGET_SECTOR, TARGET_SECTOR );
			r   = iap.write( mem, sector_start_adress[ TARGET_SECTOR ], MEM_SIZE );
			pc.printf( "copied: SRAM(0x%08X)->Flash(0x%08X) for %d bytes. (result=0x%08X)\r\n", mem, sector_start_adress[ TARGET_SECTOR ], MEM_SIZE, r );

			// compare
			r   = iap.compare( mem, sector_start_adress[ TARGET_SECTOR ], MEM_SIZE );
			pc.printf( "compare result     = \"%s\"\r\n", r ? "FAILED - Sector was probably not Blank before writing" : "OK" );

			break;
		case '7':
			struct DATA_A *Aptr;  // Pointer to structure with format of data
			// Check to see if TARGET_SECTOR is BLANK
			r = iap.blank_check( TARGET_SECTOR, TARGET_SECTOR );
			if ( r == SECTOR_NOT_BLANK ) {
				Aptr = (struct DATA_A *) FLASH_SECTOR_29;
				pc.printf("\r\nDATA_A:\tx = %f\ty = %f\tz = %d\r\n",Aptr->x,Aptr->y,Aptr->z);
			} else{
				pc.printf("\r\nTARGET SECTOR IS BLANK!  No data available\r\n");
			}
			break;
		case '8':
			 struct DATA_B *Bptr;	// Pointer to structure with format of data
			// Check to see if TARGET_SECTOR is BLANK
			r = iap.blank_check( TARGET_SECTOR, TARGET_SECTOR );
			if ( r == SECTOR_NOT_BLANK ) {
				Bptr = (struct DATA_B *) FLASH_SECTOR_29;
				pc.printf("\r\nDATA_B:\ta = %d\tpassword = %s\r\n",Bptr->a,Bptr->password);
			} else {
				pc.printf("\r\nTARGET SECTOR IS BLANK!  No data available\r\n");
			}
			break;
		}

	}
}
