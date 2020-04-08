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

#define SAMPLES_PER_SECOND 48000
#define GAIN 30

unsigned long i = 0;



int readMMA8451(){
	unsigned char receivedData[6];
	while(1){
	ezdsp5535_I2C_reset();
	ezdsp5535_waitusec(2000);
	/*ezdsp5535_I2C_read( 0x1D, receivedData, 6 );	
	//SW_BREAKPOINT;
	for(i = 0; i < 6;i++){ printf("%d \n", receivedData[i]);}*/ 
	MMAread();
	}
	return 0;
}

void main( void ) 
{
	/* Initialise system clocks */
	ezdsp5535_init();
	
	/* Initialize PLL */
	pll_frequency_setup(100);
	
	/* Initialise hardware interface and I2C for code */
    aic3204_hardware_init();
    
    /* Initialise the AIC3204 codec */
	aic3204_init(); 
	
	/* Setup sampling frequency and gain */
    set_sampling_frequency_and_gain(SAMPLES_PER_SECOND, GAIN);	
    
    /* Initialise I2C */
    ezdsp5535_I2C_init();
    
	/* Default to XF LED off */
	asm(" bclr XF"); // Clear register XF

    /* Program here 
     * */
	printf("Hello World! \n");
	ezdsp5535_waitusec(500000);
	i = 0;
	MMAbegin();
	readMMA8451();
	for ( i = 0  ; i < 2;i++  )
     {
		ezdsp5535_LED_on(0);
    	ezdsp5535_waitusec(500000);
		ezdsp5535_LED_off(0);
    	ezdsp5535_waitusec(500000);
   } 
     
    /* 
     * 
     * */
	
	
	
	/* Diable i2s and reset AIC3204 codec */
	 aic3204_disable();
	
	/* End of program */
	 printf( "\n***Program has Terminated***\n" );
     SW_BREAKPOINT;
}
