//////////////////////////////////////////////////////////////////////////////
// * File name: main.c
// *                                                                          
// * Description: This file includes main() and system initialization funcitons.
// *                                                                          
// * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/ 
// *                                                                          
// *                                                                          
// *  Redistribution and use in source and binary forms, with or without      
// *  modification, are permitted provided that the following conditions      
// *  are met:                                                                
// *                                                                          
// *    Redistributions of source code must retain the above copyright        
// *    notice, this list of conditions and the following disclaimer.         
// *                                                                          
// *    Redistributions in binary form must reproduce the above copyright     
// *    notice, this list of conditions and the following disclaimer in the   
// *    documentation and/or other materials provided with the                
// *    distribution.                                                         
// *                                                                          
// *    Neither the name of Texas Instruments Incorporated nor the names of   
// *    its contributors may be used to endorse or promote products derived   
// *    from this software without specific prior written permission.         
// *                                                                          
// *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS     
// *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT       
// *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR   
// *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT    
// *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,   
// *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT        
// *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,   
// *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY   
// *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT     
// *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE   
// *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.    
// *                                                                          
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// * Change History:
// * May 11, 2010:                                                                          
// * BugFix: PLL_100M was undefined, which failed to configure the PLL when
// * running in stand-alone mode. PLL was set to 100MHz by GEL file when running                                                                          
// * from Code Composer.
//////////////////////////////////////////////////////////////////////////////

#define PLL_12M		0
#define PLL_98M		0
#define PLL_100M	1

#include <stdio.h>
#include <math.h>
//#include <complex>
#include "data_types.h"
#include "register_system.h"
#include "register_cpu.h"
#include "rtc_routines.h"
#include "i2s_routines.h"
#include "dma_routines.h"
#include "configuration.h"
#include "hwafft.h"
#include "filter_routines.h"
#include "FIR_HP_coeffs.h"
#include "TMS320.H"
#include "dsplib5535.h"
#include "LPcoeffs.h"
#include "MMA8451.h"

// ezdsp setup libraries
//#include "ezdsp5535.h"
//#include "ezdsp5535_gpio.h"
//#include "ezdsp5535_i2c.h"
//#include "ezdsp5535_i2c.h"
//#include "ezdsp5535_led.h"
#include "pll.h"

// board device libraries
#include "aic3204.h"


//MMA8451
#include "MMA8451.h"


void InitSystem(void);
void ConfigPort(void);
//void SYS_GlobalIntEnable(void);
//void SYS_GlobalIntDisable(void);
void PLL_98MHz(void);

void turnOnLED(void);
void turnOffLED(void);

Uint16 fFilterOn = 0;
Uint16 fBypassOn = 1;
Uint16 clearOverlaps = 1;

extern void AIC3204_init(void);
extern Uint16 CurrentRxL_DMAChannel;
extern Uint16 CurrentRxR_DMAChannel;
extern Uint16 CurrentTxL_DMAChannel;
extern Uint16 CurrentTxR_DMAChannel;
extern Uint16 RunFilterForL;
extern Uint16 RunFilterForR;

Int16 dummy1;

//----------Start af Dennis kode---------------------------------------------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------


void codecRead(Int16 *real, Int16 sampleLength){
	Int16 i = 0;
	ezdsp5535_waitusec(250000);
	for(i = 0; i < sampleLength; i++){
		 aic3204_codec_read_MONO(&real[0], &dummy1,i);	
		 
	}
}



#pragma DATA_SECTION(data_br_buf,"data_br_buf");	//Kommando der placere arrayet med bit reversed pladser et bestemt sted
#pragma DATA_ALIGN(data_br_buf,2048);			//-||-
Int32 data_br_buf[1024];						//Array der har sine indgange bit reversed

#pragma DATA_SECTION(scratch_buf,"scratch_buf");	//Kommando der placere arrayet med bit reversed pladser et bestemt sted
#pragma DATA_ALIGN(scratch_buf,2048);			//-||-
Int32 scratch_buf[1024];						//Array der indeholder bare reele og imaginære værdier, men indgangende har skiftet plads med bit reverse
Int32 absolute_value[1024];

void main(void) //main
{			
	Int16 real_part[10240];								//Array der indeholder de reelle datapunkter i tidsdomænet
	Int16 i = 0;
	Int16 rpm = 0;
	//Int16 xfilter[8192];
	Int16 dBuffer[121+2];
	Int16 *dBufferer_ptr = &dBuffer[0];
	Uint16 firflag = 0;
	
	inits(); // Setting up stuff for I2C
	
	

	MMAbegin();
		
		//MMAread(&real_part[0]);
	for(i = 0; i < 100; i++){
		requestFromArduino(0); // START
		codecRead(&real_part[2048], 2048);
		codecRead(&real_part[4096], 2048);
		codecRead(&real_part[6144], 2048);
		codecRead(&real_part[8192], 2048);
		requestFromArduino(1); // STOP
		rpmReadI2C(&rpm);
		printf("%hd \n", rpm);}
		firflag = fir2(&real_part[2048], FIR_HP_1000Hz, &real_part[1024], dBufferer_ptr, 8192, 121);
		for(i = 1024; i < 9216; i++){
			if(real_part[i] < 0)
			{
				real_part[i] = real_part[i] * -1;
			}	
		}
		firflag = fir2(&real_part[1024], LP, &real_part[0], dBufferer_ptr, 8192, 121);
			//ezdsp5535_waitusec(250000);
			//printf("%s \n", "STARTING");
	/*	for(i = 0; i < 1024; i++){ printf("%hd \n", real_part[i]);}
		
		//while( i < 8192) { printf("%d \n", real_part[i]); i++; }
		for(i = 1024; i < 2048; i++){ printf("%hd \n", real_part[i]);}
		for(i = 2048; i < 3072; i++){ printf("%hd \n", real_part[i]);}
		for(i = 3072; i < 4096; i++){ printf("%hd \n", real_part[i]);}
		for(i = 4096; i < 5120; i++){ printf("%hd \n", real_part[i]);}
		for(i = 5120; i < 6144; i++){ printf("%hd \n", real_part[i]);}
		for(i = 6144; i < 7168; i++){ printf("%hd \n", real_part[i]);}
		for(i = 7168; i < 8192; i++){ printf("%hd \n", real_part[i]);}*/
		//for(loopCounter = 0; loopCounter < 1024; loopCounter++){real_part[loopCounter] = loopCounter;}
		//for(loopCounter = 0; loopCounter < 1024; loopCounter++){imaginary_part[loopCounter] = 0;} 
		
/*
 * 	FFT FUNCTIONS BELOW
 *----------------------------------------------------------------------------------------------
 */

  printf("Breakpoint reached \n");
}

//----------Slut af Dennis kode----------------------------------------------------------------------------------------------------------------------------------------------------------------

void PLL_98MHz(void)
{
	// PLL set up from RTC
    // bypass PLL
    CONFIG_MSW = 0x0;

#if (PLL_100M ==1)
    PLL_CNTL2 = 0x8000;
    PLL_CNTL4 = 0x0000;
    PLL_CNTL3 = 0x0806;
    PLL_CNTL1 = 0x82FA;
    
#elif (PLL_12M ==1)
    PLL_CNTL2 = 0x8000;
    PLL_CNTL4 = 0x0200;
    PLL_CNTL3 = 0x0806;
    PLL_CNTL1 = 0x82ED;
#elif (PLL_98M ==1)    
    PLL_CNTL2 = 0x8000;
    PLL_CNTL4 = 0x0000;
    PLL_CNTL3 = 0x0806;
    PLL_CNTL1 = 0x82ED;
    
#endif
    while ( (PLL_CNTL3 & 0x0008) == 0);
    // Switch to PLL clk
    CONFIG_MSW = 0x1;
}

void InitSystem(void)
{
	Uint16 i;
	// PLL set up from RTC
    // bypass PLL
    CONFIG_MSW = 0x0;

#if (PLL_100M ==1)
    PLL_CNTL2 = 0x8000;
    PLL_CNTL4 = 0x0000;
    PLL_CNTL3 = 0x0806;
    PLL_CNTL1 = 0x82FA;
    
#elif (PLL_12M ==1)
    PLL_CNTL2 = 0x8000;
    PLL_CNTL4 = 0x0200;
    PLL_CNTL3 = 0x0806;
    PLL_CNTL1 = 0x82ED;
#elif (PLL_98M ==1)    
    PLL_CNTL2 = 0x8000;
    PLL_CNTL4 = 0x0000;
    PLL_CNTL3 = 0x0806;
    PLL_CNTL1 = 0x82ED;

#elif (PLL_40M ==1)        
    PLL_CNTL2 = 0x8000;
    PLL_CNTL4 = 0x0300;
    PLL_CNTL3 = 0x0806;
    PLL_CNTL1 = 0x8262;    
#endif

    while ((PLL_CNTL3 & 0x0008) == 0);
    // Switch to PLL clk
    CONFIG_MSW = 0x1;

	// clock gating
	// enable all clocks
    IDLE_PCGCR = 0x0;
    IDLE_PCGCR_MSW = 0xFF84;
    

	// reset peripherals
    PER_RSTCOUNT = 0x02;
    PER_RESET = 0x00fb;    
    for (i=0; i< 0xFFF; i++);
}

void ConfigPort(void)
{
    Int16 i;
    //  configure ports
    PERIPHSEL0 = 0x6900;        // parallel port: mode 6, serial port1: mode 2 

    for (i=0; i< 0xFFF; i++);
}

/*
void SYS_GlobalIntEnable(void)
{
    asm(" BIT (ST1, #ST1_INTM) = #0");
}

void SYS_GlobalIntDisable(void)
{
    asm(" BIT (ST1, #ST1_INTM) = #1");
}
*/
void turnOnLED(void)
{
    Uint16 temp;
    
    temp = ST1_55;
    if((temp&0x2000) == 0)
    {
        // turn on LED
        temp |= 0x2000;
        ST1_55 =temp;
    }
    
}

void turnOffLED(void)
{
    Uint16 temp;
    
    temp = ST1_55;
    if((temp&0x2000) != 0)
    {
        // turn off LED
        temp &=0xDFFF;
        ST1_55 =temp;
    }
}