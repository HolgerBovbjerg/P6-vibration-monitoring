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

#define SAMPLES_PER_SECOND 48000
#define GAIN 10

//MMA8451
#include "MMA8451.h"

void inits(){
	/* Initialise system clocks */
	ezdsp5535_init();
	
	/* Initialize PLL */
	pll_frequency_setup(100);
	
	/* Initialise hardware interface and I2C for code */
    aic3204_hardware_init();
    
    /* Initialise the AIC3204 codec */
	aic3204_init(); 
	//SAR_init();
	/* Setup sampling frequency and gain */
    set_sampling_frequency_and_gain(SAMPLES_PER_SECOND, GAIN);	
    
    /* Initialise I2C */
    ezdsp5535_I2C_init();
    
	/* Default to XF LED off */
	//asm(" bclr XF"); // Clear register XF
	
}


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

void MMAread(Int16 *save_buffer){
	
	
	Uint16 MMA_I2C_ADDR 			=	0x1D;
	unsigned char MMA8451_REG_OUT_X_MSB[1];
	
	Uint8 MMAdata[6];
	Uint16 x=0;
	Uint16 y=0;
	Uint16 z=0;
	int i = 0;
	MMA8451_REG_OUT_X_MSB[0] = 0x01;
	printf("          ----- STARTING MEASUREMENT -----\n");
	for(i = 0; i < 1024; i++){
		ezdsp5535_I2C_reset();
	
	// Master transmitter. Set registeraddress
	ezdsp5535_I2C_write_SR(MMA_I2C_ADDR, MMA8451_REG_OUT_X_MSB, 1);	
	
	// Master receiver. Read data
	ezdsp5535_I2C_read( MMA_I2C_ADDR, MMAdata, 6 );
	
	x = MMAdata[0]; x <<= 8; x |= MMAdata[1]; x >>= 2;
	y = MMAdata[2]; y <<= 8; y |= MMAdata[3]; y >>= 2;
	z = MMAdata[4]; z <<= 8; z |= MMAdata[5]; z >>= 2;
//	printf("%s %d ","x:   " , x);
//	printf("%s %d ", "		y:   ", y);
//	printf("%s %d \n", "		z:   ", z);	
	*(save_buffer+i) = y;
	ezdsp5535_waitusec(2500);
	
	}

	//for(i = 0; i < 6; i++){
	printf("          ----- MEASUREMENTS DONE -----\n");	
	//}
	
//	printf("%s %d \n", "MMA: ", &MMA8451_REG_OUT_X_MSB);
}
