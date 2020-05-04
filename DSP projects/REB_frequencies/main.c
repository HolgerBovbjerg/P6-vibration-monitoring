// standard c libraries
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
#include "filter_coeffs.h"
#include "REB_freqs.h"

#define SAMPLES_PER_SECOND 48000
#define GAIN 30

#define RPM_MIN 2000
#define RPM_MAX 3700

unsigned long i = 0;

Int16 left_input;
Int16 right_input;

Int16 bpfiCalc = 0;
Int16 bpfoCalc = 0;


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
   	
   	// Prints constant rpm, bpfi bpfo lookup values
   	printf( "Prints constant rpm, bpfi bpfo lookup values\n"); 
   	i = 0;
	for ( i = 0  ; i < (RPM_MAX-RPM_MIN);i++  )
    {
		printf( "Index %d\n", i);
	    bpfoCalc = ( ( (rpm[i]*7645) >> 15 ) * 24066 ) >> 15;
	    printf( "BPFO at %d: %d Hz\n", rpm[i], bpfo[i]);
	    printf( "Calculated %d: %d Hz\n", rpm[i], bpfoCalc);
	    bpfiCalc = ( ( (rpm[i]*7645) >> 15 ) * 20753 ) >> 14;
	    printf( "BPFI at %d: %d Hz\n\n", rpm[i], bpfi[i] );
	    printf( "Calculated at %d: %d Hz\n", rpm[i], bpfiCalc);
	    
    }
    
	
	/* Program end here */
	
	/* Disable i2s and reset AIC3204 codec */
	 aic3204_disable();
	
	/* End of program */
	 printf( "\n***Program has Terminated***\n" );
	 oled_display_message("PROGRAM HAS        ", "TERMINATED        ");
     SW_BREAKPOINT;
}
