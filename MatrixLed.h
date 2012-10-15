/********************************************************/
/*File Name:  MatrixLed.h
**Created by: Xinyu Cheng
**Function:   Create some 8*8 Pattern for the Matrix Led.
/*******************************************************/

#ifndef MATRIX_LED
#define MATRIX_LED

__u16 block[I2C_SMBUS_BLOCK_MAX];

static __u16 Love1_bmp[]={0x0C, 0x1E, 0x3E, 0x7C, 0x3E, 0x1E, 0x0C, 0x00};
static __u16 Love2_bmp[]={0x0C, 0x12, 0x22, 0x44, 0x22, 0x12, 0x0C, 0x00};
static __u16 Love3_bmp[]={0x0C, 0x1E, 0x3E, 0x7C, 0x3E, 0x1E, 0x0C, 0x00};

static __u16 I_bmp[]={0x00, 0x00, 0x42, 0x7E, 0x42, 0x00, 0x00, 0x00};
static __u16 U_bmp[]={0x00, 0x00, 0x3E, 0x40, 0x40, 0x3E, 0x00, 0x00};
static __u16 Liu_bmp[]={0xFF, 0x40, 0x18, 0x84, 0x5C, 0x26, 0x5D, 0x84};
static __u16 Ben_bmp[]={0x92, 0x57, 0x72, 0xDC, 0x72, 0x57, 0x92, 0x04};
static __u16 Blank_bmp[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

static __u16 Square1_bmp[] = {0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00};
static __u16 Square2_bmp[] = {0x00,0x00,0x3C,0x24,0x24,0x3C,0x00,0x00};
static __u16 Square3_bmp[] = {0x00,0x7E,0x42,0x42,0x42,0x42,0x7E,0x00};
static __u16 Square4_bmp[] = {0xFF,0x81,0x81,0x81,0x81,0x81,0x81,0xFF};
static __u16 Square5_bmp[] = {0xFF,0xFF,0xC3,0xC3,0xC3,0xC3,0xFF,0xFF};
static __u16 Square6_bmp[] = {0xFF,0xFF,0xFF,0xE7,0xE7,0xFF,0xFF,0xFF};
static __u16 Square7_bmp[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};


//The Function to Display a BMP.
int display(__u16 bmp[], int res, int daddress, int file)
{
     int i;
     for(i=0; i<8; i++) 
     {
	block[i] = (bmp[i]&0xfe) >>1 | 
        	(bmp[i]&0x01) << 7;
     }
     res = i2c_smbus_write_i2c_block_data(file, daddress, 16, 
	   (__u8 *)block);
     usleep(400000);

}

static void help(void) __attribute__ ((noreturn));

static void help(void)
{
	fprintf(stderr, "Usage: my2cset (hardwired to bus 3, address 0x70)\n");
	exit(1);
}


static int check_funcs(int file, int size)
{
	unsigned long funcs;

	/* check adapter functionality */
	if (ioctl(file, I2C_FUNCS, &funcs) < 0) {
		fprintf(stderr, "Error: Could not get the adapter "
			"functionality matrix: %s\n", strerror(errno));
		return -1;
	}

	switch (size) {
	case I2C_SMBUS_BYTE:
		if (!(funcs & I2C_FUNC_SMBUS_WRITE_BYTE)) {
			fprintf(stderr, MISSING_FUNC_FMT, "SMBus send byte");
			return -1;
		}
		break;

	case I2C_SMBUS_BYTE_DATA:
		if (!(funcs & I2C_FUNC_SMBUS_WRITE_BYTE_DATA)) {
			fprintf(stderr, MISSING_FUNC_FMT, "SMBus write byte");
			return -1;
		}
		break;

	case I2C_SMBUS_WORD_DATA:
		if (!(funcs & I2C_FUNC_SMBUS_WRITE_WORD_DATA)) {
			fprintf(stderr, MISSING_FUNC_FMT, "SMBus write word");
			return -1;
		}
		break;

	case I2C_SMBUS_BLOCK_DATA:
		if (!(funcs & I2C_FUNC_SMBUS_WRITE_BLOCK_DATA)) {
			fprintf(stderr, MISSING_FUNC_FMT, "SMBus block write");
			return -1;
		}
		break;
	case I2C_SMBUS_I2C_BLOCK_DATA:
		if (!(funcs & I2C_FUNC_SMBUS_WRITE_I2C_BLOCK)) {
			fprintf(stderr, MISSING_FUNC_FMT, "I2C block write");
			return -1;
		}
		break;
	}

	return 0;
}

#endif
