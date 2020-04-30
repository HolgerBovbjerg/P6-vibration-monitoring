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

#define SAMPLES_PER_SECOND 48000
#define GAIN 30
#define SAMPLE_SIZE 1024

unsigned long i = 0;

Int16 left_input;
Int16 right_input;

DATA x;
#define SIZE 1024
Uint8 ch[SIZE]; /* Declare a char[1024]
array for experiment */

void main( void ) 
{
	
	FILE *fp;
	Uint32 i;
	
	fopen("codec_data.csv", "wb");
	
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
	ezdsp5535_waitusec(500);
	
    /* Program here */ 
	oled_display_message("Sampling           ", "Data!              ");
	printf("Sampling data! \n");
   	// Sampling of data
   	asm(" bset XF"); // Turn on by setting LED register XF
   	i = 0;
	for ( i = 0  ; i < SAMPLE_SIZE;i++  )
     {
	     aic3204_codec_read(&left_input, &right_input);
	     x = left_input;
	     fwrite(&x, sizeof(Uint8), 2, fp); /* Write 2 bytes (16 bits) of data to output file */
     }
	fclose(fp);
	
	asm(" bclr XF"); // Clear LED register XF // 
	 printf( "Sampling finished\n" );
	 
	/* Disable i2s and reset AIC3204 codec */
	 aic3204_disable();
	
	/* End of program */
	 printf( "\n***Program has Terminated***\n" );
	 oled_display_message("PROGRAM HAS        ", "TERMINATED        ");
     SW_BREAKPOINT;
}
