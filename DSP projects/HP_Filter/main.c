// standard c libraries
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

// ezdsp5535 setup libraries
#include "ezdsp5535.h"

// On-chip peripheral libraries
#include "ezdsp5535_gpio.h"
#include "ezdsp5535_i2c.h"
#include "ezdsp5535_led.h"
#include "ezdsp5535_pll.h"

// Board device libraries
#include "oled.h"
#include "aic3204.h"
#include "pushbuttons.h"

// Misc. libraries
#include "dsplib5535.h"
#include "FIR_HP_coeffs.h"

#define SAMPLES_PER_SECOND 48000
#define GAIN 30

#define SIZE 1024

unsigned long i = 0;

Int16 left_input;
Int16 right_input;

DATA x[SIZE];

DATA dBuffer[121+2];

#pragma DATA_SECTION(xfilter, ".bss:SARAM0")
DATA xfilter[SIZE];
ushort nx = SIZE;

DATA  *dBufferer_ptr = &dBuffer[0];

ushort firflag = 0;

DATA h[121];
ushort nh = 121;

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
	
	/* Initialise oled display */
	oled_init();
	SAR_init();
	oled_display_message("                 ", "                   "); // Clear display
	
	/* Setup sampling frequency and gain */
    set_sampling_frequency_and_gain(SAMPLES_PER_SECOND, GAIN);	
    
    
    
	/* Default to XF LED off */
	asm(" bclr XF"); // Clear LED register XF 

    /* Program here */ 
    
	oled_display_message("Hello              ", "World!             ");
	printf("Hello World! \n");
	ezdsp5535_waitusec(500000);
   	
   	// Filter example
  
   	i = 0;
   	
   	// Initialise filter array and delay buffer to zeroes
   	for (i = 0; i < (SIZE); i++) {
   		xfilter[i] = 0;
   	}
   	for (i = 0; i < (121 + 2); i++) {
   		dBuffer[i] = 0;
   	}
   	
	for (i = 0; i < 1024; i++)
    {
    	aic3204_codec_read(&left_input, &right_input);
    	x[i] = left_input;
    }
    
    firflag = fir2(x, FIR_HP_1000Hz, xfilter, dBufferer_ptr, SIZE, 121);
	
	/* Disable i2s and reset AIC3204 codec */
	 aic3204_disable();
	
	/* End of program */
	 printf( "\n***Program has Terminated***\n" );
	 oled_display_message("PROGRAM HAS        ", "TERMINATED        ");
     SW_BREAKPOINT;
}
