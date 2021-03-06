// standard c libraries
#include <stdio.h>

// ezdsp setup libraries
#include "ezdsp5535.h"
#include "ezdsp5535_gpio.h"
#include "ezdsp5535_i2c.h"
//#include "ezdsp_i2c.h"
#include "ezdsp5535_led.h"
#include "pll.h"

// board device libraries
#include "aic3204.h"


//MMA8451
#include "MMA8451.h"


void MMAbegin(){
	Uint16 MMA_I2C_ADDR 			=	0x1D;
	unsigned char MMA8451_REG_CTRL_REG2[2] = {0x2B,0x40};
	unsigned char MMA8451_REG_CTRL_REG2_2[2] = {0x2B,0x02};
	unsigned char MMA8451_REG_XYZ_DATA_CFG[2] = {0x0E,0};
	unsigned char MMA8451_REG_CTRL_REG4[2] = {0x2D, 0x01};
	unsigned char MMA8451_REG_CTRL_REG5[2] = {0x2E, 0x01};
	unsigned char MMA8451_REG_PL_CFG[2] = {0x11,0x40};
	unsigned char MMA8451_REG_CTRL_REG1[2] = {0x2A, 0x01 | 0x04};
	
	ezdsp5535_I2C_write(MMA_I2C_ADDR, MMA8451_REG_CTRL_REG2, 2); // reset
	ezdsp5535_waitusec(50);
	
	ezdsp5535_I2C_write(MMA_I2C_ADDR, MMA8451_REG_XYZ_DATA_CFG, 2); // 2G
	ezdsp5535_I2C_write(MMA_I2C_ADDR, MMA8451_REG_CTRL_REG2_2, 2); // reset high
	ezdsp5535_I2C_write(MMA_I2C_ADDR, MMA8451_REG_CTRL_REG4, 2);
	ezdsp5535_I2C_write(MMA_I2C_ADDR, MMA8451_REG_CTRL_REG5, 2);
	
	ezdsp5535_I2C_write(MMA_I2C_ADDR, MMA8451_REG_PL_CFG, 2);
	ezdsp5535_I2C_write(MMA_I2C_ADDR, MMA8451_REG_CTRL_REG1, 2);
}

void MMAread(){
	
	
	Uint16 MMA_I2C_ADDR 			=	0x1D;
	unsigned char MMA8451_REG_OUT_X_MSB[1];
	
	Uint8 MMAdata[6];
	Uint16 x;
	Uint16 y;
	Uint16 z;
	int i = 0;
	MMA8451_REG_OUT_X_MSB[0] = 0x01;
	
	
	// Master transmitter. Set registeraddress
	ezdsp5535_I2C_write_SR(MMA_I2C_ADDR, MMA8451_REG_OUT_X_MSB, 1);	
	
	// Master receiver. Read data
	ezdsp5535_I2C_read( MMA_I2C_ADDR, MMAdata, 6 );
	
	x = MMAdata[0]; x <<= 8; x |= MMAdata[1]; x >>= 2;
	y = MMAdata[2]; y <<= 8; y |= MMAdata[3]; y >>= 2;
	z = MMAdata[4]; z <<= 8; z |= MMAdata[5]; z >>= 2;
	
	printf("%s %d ", "x:   ", x);
	printf("%s %d ", "		y:   ", y);
	printf("%s %d \n ", "		z:   ", z);	
	for(i = 0; i < 6; i++){
	//printf("%d \n",MMAdata[i]);	
	}
//	printf("%s %d \n", "MMA: ", &MMA8451_REG_OUT_X_MSB);
}
