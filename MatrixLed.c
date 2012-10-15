/*
    MatrixLed.c - First try at controlling Adafruit 8x8matrix.
    From: Mark A. Yoder, 14-Aug-2012.
    Modified By: Xinyu Cheng
    Page numbers are from the HT16K33 manual
    http://www.adafruit.com/datasheets/ht16K33v110.pdf

    i2cset.c - A user-space program to write an I2C register.
    Copyright (C) 2001-2003  Frodo Looijaard <frodol@dds.nl>, and
                             Mark D. Studebaker <mdsxyz123@yahoo.com>
    Copyright (C) 2004-2010  Jean Delvare <khali@linux-fr.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
    MA 02110-1301 USA.
*/

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "i2c-dev.h"
#include "i2cbusses.h"
#include "MatrixLed.h"

int main(int argc, char *argv[])
{
	int res, i2cbus, address, size, file;
	int value, daddress;
	char filename[20];
	int force = 0, readback = 1;
	int len;

	i2cbus = lookup_i2c_bus("3");
	printf("i2cbus = %d\n", i2cbus);
	if (i2cbus < 0)
		help();

	address = parse_i2c_address("0x70");
	printf("address = 0x%2x\n", address);
	if (address < 0)
		help();

	size = I2C_SMBUS_BYTE;

	daddress = 0x21;
	if (daddress < 0 || daddress > 0xff) {
		fprintf(stderr, "Error: Data address invalid!\n");
		help();
	}

	file = open_i2c_dev(i2cbus, filename, sizeof(filename), 0);
	printf("file = %d\n", file);
	if (file < 0
	 || check_funcs(file, size)
	 || set_slave_addr(file, address, force))
		exit(1);


	switch (size) {
	case I2C_SMBUS_BYTE:
		daddress = 0x21;	// Start oscillator (page 10)
		printf("writing: 0x%02x\n", daddress);
		res = i2c_smbus_write_byte(file, daddress);

		daddress = 0x81;	// Display on, blinking off (page 11)
		printf("writing: 0x%02x\n", daddress);
		res = i2c_smbus_write_byte(file, daddress);

		daddress = 0xe7;	// Full brightness (page 15)
		printf("writing: 0x%02x\n", daddress);
		res = i2c_smbus_write_byte(file, daddress);

		daddress = 0x00;	// Start writing to address 0 (page 13)
		printf("writing: 0x%02x\n", daddress);
		res = i2c_smbus_write_byte(file, daddress);

		int i;

                while(1)
                {  
                   //Display the Square.      
                   display(Square1_bmp,res,daddress,file);
                   display(Square2_bmp,res,daddress,file);
                   display(Square3_bmp,res,daddress,file);
                   display(Square4_bmp,res,daddress,file);
                   display(Square5_bmp,res,daddress,file);
                   display(Square6_bmp,res,daddress,file);
                   display(Square7_bmp,res,daddress,file);

                   //Display Love Heart.
                   display(Love1_bmp,res,daddress,file);
                   display(Love2_bmp,res,daddress,file);
                   display(Love1_bmp,res,daddress,file);

                   //Display I love U.
                   display(I_bmp,res,daddress,file);
                   display(Love1_bmp,res,daddress,file);                  
                   display(U_bmp,res,daddress,file);
                  
                   //Display the Square.
                   display(Square7_bmp,res,daddress,file);
                   display(Square6_bmp,res,daddress,file);
                   display(Square5_bmp,res,daddress,file);
                   display(Square4_bmp,res,daddress,file);
                   display(Square3_bmp,res,daddress,file);
                   display(Square2_bmp,res,daddress,file);
                   display(Square1_bmp,res,daddress,file);
                   
                   //Display Love Heart.
                   display(Love1_bmp,res,daddress,file);
                   display(Love2_bmp,res,daddress,file);
                   display(Love1_bmp,res,daddress,file);          
                }

		break;

	case I2C_SMBUS_WORD_DATA:
		res = i2c_smbus_write_word_data(file, daddress, value);
		break;
	case I2C_SMBUS_BLOCK_DATA:
		res = i2c_smbus_write_block_data(file, daddress, len, (const __u8 *)block);
		break;
	case I2C_SMBUS_I2C_BLOCK_DATA:
		res = i2c_smbus_write_i2c_block_data(file, daddress, len, (const __u8 *)block);
		break;
	default: /* I2C_SMBUS_BYTE_DATA */
		res = i2c_smbus_write_byte_data(file, daddress, value);
		break;
	}
	if (res < 0) {
		fprintf(stderr, "Error: Write failed\n");
		close(file);
		exit(1);
	}

	if (!readback) { /* We're done */
		close(file);
		exit(0);
	}

	switch (size) {
	case I2C_SMBUS_BYTE:
		res = i2c_smbus_read_byte(file);
		value = daddress;
		break;
	case I2C_SMBUS_WORD_DATA:
		res = i2c_smbus_read_word_data(file, daddress);
		break;
	default: /* I2C_SMBUS_BYTE_DATA */
		res = i2c_smbus_read_byte_data(file, daddress);
	}
	close(file);

	if (res < 0) {
		printf("Warning - readback failed\n");
	} else
	if (res != value) {
		printf("Warning - data mismatch - wrote "
		       "0x%0*x, read back 0x%0*x\n",
		       size == I2C_SMBUS_WORD_DATA ? 4 : 2, value,
		       size == I2C_SMBUS_WORD_DATA ? 4 : 2, res);
	} else {
		printf("Value 0x%0*x written, readback matched\n",
		       size == I2C_SMBUS_WORD_DATA ? 4 : 2, value);
	}

	exit(0);
}
