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

#define SAMPLES_PER_SECOND 48000
#define GAIN 30

unsigned long i = 0;

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
	oled_display_message("                 ", "                   "); // Clear display
	
	/* Setup sampling frequency and gain */
    set_sampling_frequency_and_gain(SAMPLES_PER_SECOND, GAIN);	
    
    oled_display_message("Hello              ", "World!             ");
    
	/* Default to XF LED off */
	asm(" bclr XF"); // Clear register XF

    /* Program here 
     * */
	printf("Hello World! \n");
	ezdsp5535_waitusec(500000);
	i = 0;
	// blink LED 20 times
	for ( i = 0  ; i < 20;i++  )
     {
		ezdsp5535_LED_on(0);
    	ezdsp5535_waitusec(500000);
		ezdsp5535_LED_off(0);
    	ezdsp5535_waitusec(500000);
   } 
     
    /* 
     * 
     * */
	
	
	
	/* Disable i2s and reset AIC3204 codec */
	 aic3204_disable();
	
	/* End of program */
	 printf( "\n***Program has Terminated***\n" );
	 oled_display_message("PROGRAM HAS        ", "TERMINATED        ");
     SW_BREAKPOINT;
}
