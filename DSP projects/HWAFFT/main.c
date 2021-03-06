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
#include "data_types.h"
#include "register_system.h"
#include "register_cpu.h"
#include "rtc_routines.h"
#include "i2s_routines.h"
#include "dma_routines.h"
#include "configuration.h"
#include "hwafft.h"
#include "filter_routines.h"


void InitSystem(void);
void ConfigPort(void);
void SYS_GlobalIntEnable(void);
void SYS_GlobalIntDisable(void);
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

//----------Start af Dennis kode---------------------------------------------------------------------------------------------------------------------------------------------------------------
void fft_create_datapoint_array(Int16 *real_array, Int16 *imaginary_array, Uint16 fft_length, Int16 *fft_pointer)
{
	Int16 i;
	
	for(i=0 ; i < fft_length;  i++)
	{
		*(fft_pointer+(i*2)+1) = *(imaginary_array+i);
		*(fft_pointer+(i*2)) = *(real_array+i);
	}
	
	return;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Uint16 fft_fft(Int32 *data, Int32 *scratch, Uint16 fft_falg, Uint16 scale_flag, Uint16 fft_length_1)
{
	Uint16 data_save_location;
	
	if(fft_length_1 == 1024)
	{
		data_save_location = hwafft_1024pts(data, scratch, fft_falg, scale_flag);
		return data_save_location;
	}
	
	if(fft_length_1 == 512)
	{
		data_save_location = hwafft_512pts(data, scratch, fft_falg, scale_flag);
		return data_save_location;
	}
	
	if(fft_length_1 == 256)
	{
		data_save_location = hwafft_256pts(data, scratch, fft_falg, scale_flag);
		return data_save_location;
	}
	
	if(fft_length_1 == 128)
	{
		data_save_location = hwafft_128pts(data, scratch, fft_falg, scale_flag);
		return data_save_location;
	}
	
	if(fft_length_1 == 64)
	{
		data_save_location = hwafft_64pts(data, scratch, fft_falg, scale_flag);
		return data_save_location;
	}
	
	if(fft_length_1 == 32)
	{
		data_save_location = hwafft_32pts(data, scratch, fft_falg, scale_flag);
		return data_save_location;
	}
	
	if(fft_length_1 == 16)
	{
		data_save_location = hwafft_16pts(data, scratch, fft_falg, scale_flag);
		return data_save_location;
	}
	
	if(fft_length_1 == 8)
	{
		data_save_location = hwafft_8pts(data, scratch, fft_falg, scale_flag);
		printf("%d \n", data_save_location);
		return data_save_location;
	}
	printf("NULL! \n");
	return 17857;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Uint16 fft_length = 8;								//L�ngde af FFT eller IFFT
Uint16 fft_save_location;							//Variabel til at holde styr p� hvor outputtet af FFT/IFFT er
Int16 real_part[1024] = {100,300,500,700,900,1100,1300,1500};			//Input array med de reele v�rdier af signalet
Int16 imaginary_part[1024] = {0,0,0,0,0,0,0,0};		//Input array med de imagin�re v�rdier af signalet
Int16 fft_datapoints[2*1024];						//Array der indeholder b�de reele og imagin�re v�rdier
Int32 fft_scratch_array[1024];						//Array der indeholder bare reele og imagin�re v�rdier, men indgangende har skiftet plads med bit reverse
Uint16 ii;											//Variable til at t�lle med i et for loop
Int32 *fft_output_location;							//Indeholder memory location af den array der har outputtet af FFT/IFFT'en
Uint16 real_freq[1024];								//Array der indeholder de FFT/IFFT transformerede reele v�rdier
Uint16 imaginary_freq[1024];						//Array der indeholder de FFT/IFFT transformerede imagin�re v�rdier

#pragma DATA_SECTION(fft_data_bitrev,"data_buf");	//Kommando der placere arrayet med bit reversed pladser et bestemt sted
#pragma DATA_ALIGN(fft_data_bitrev,2048);			//-||-
Int32 fft_data_bitrev[1024];						//Array der har sine indgange bit reversed


void main(void) //main
{
	Uint16 lengthData = 16;
	int tmp = 0;
	Uint16 *ptr;
	Uint16 bitCopy[1024];
	fft_create_datapoint_array(&real_part[0], &imaginary_part[0], fft_length, &fft_datapoints[0]); //Kommando der sammens�tter real_part og imaginary_part sammen til en array, fft_datapoints
	//for(tmp = 0; tmp < lengthData; tmp++){ printf("%d \n",fft_datapoints[tmp]);} // FOR DEBUG
	
	
	hwafft_br((Int32 *)&fft_datapoints[0], &fft_data_bitrev[0],fft_length); //Kommando der bit reverser pladserne s� de havner i fft_data_bitrev
	ptr = (Int16 *)&fft_data_bitrev[0];
	for(tmp = 0; tmp < 1024; tmp++){bitCopy[tmp] = *(ptr+tmp);}
	for(tmp = 0; tmp < lengthData; tmp++){ printf("%d \n",*(ptr+tmp));} // FOR DEBUG
	fft_save_location = fft_fft(&fft_data_bitrev[0], &fft_scratch_array[0], 0, 1, fft_length); //Kommando der rent faktisk udf�rer FFT'en
	
	if(fft_save_location == 17857) //Tjekker om FFT'en rent faktisk er blevet udf�rt
	{
		printf("FFT NOT executed correctly"); //Hvis ikke, surt show
	} else {
		
		if(fft_save_location == 0) //Tjekker om dataen er i bitrev array
		{
			fft_output_location = &fft_data_bitrev[0];
		}
		else if(fft_save_location == 1) //Tjekker om dataen er i scratch array
		{
			fft_output_location = &fft_scratch_array[0];
		} else {
			printf("Output array MIA \n"); //Hvis ikke, surt show
		}
		
		for(ii = 0; ii < fft_length; ii++) //For loop der splitter den FFT/IFFT transformered array op i reel og imagin�r del
		{
			real_freq[ii] = (*(fft_output_location+ii)) >> 16; //Reel del
			imaginary_freq[ii] = (*(fft_output_location+ii)) & 0x0000FFFF; //Imagin�r del
			printf("%s %d \n","RE: ",real_freq[ii]); //Printer output
			printf("%s %d \n","IM: ",imaginary_freq[ii]); //Printer output
		}
	}
	
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


void SYS_GlobalIntEnable(void)
{
    asm(" BIT (ST1, #ST1_INTM) = #0");
}

void SYS_GlobalIntDisable(void)
{
    asm(" BIT (ST1, #ST1_INTM) = #1");
}

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


