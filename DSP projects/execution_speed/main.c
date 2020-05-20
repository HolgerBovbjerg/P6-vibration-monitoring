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
#include "LP_coeffs.h"
#include "HPcoeffs.h"
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
//#include "MMA8451.h"


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
Int16 errorFlag = 0;

//----------Start af Dennis kode---------------------------------------------------------------------------------------------------------------------------------------------------------------
void fft_create_datapoint_array(Int16 *real_array, Uint16 fft_length, Int16 *fft_pointer)
{
	Int16 i;

	for(i=0 ; i < fft_length;  i++)
	{	
		*(fft_pointer+(i*2)) = *(real_array+i);
		*(fft_pointer+(i*2)+1) = 0;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Int16 RMS(Int16 *data, Int16 length)
{
	Int16 i;
	Int32 intermediateResult = 0;
	
	for(i = 0; i < length; i++)
	{		
		// add (DATA *x, DATA *y, DATA *r, ushort nx, ushort scale) might be used 
		// and mul32(LDATA *x, LDATA *y, LDATA *r, ushort nx) might be used instead
		// or short power (DATA *x, LDATA *r, ushort nx)
		intermediateResult = intermediateResult + ((Int32)*(data+i))*((Int32)*(data+i));
	}

	// void ldiv16 (LDATA *x, DATA *y, DATA *r, DATA *rexp, ushort nx) might be used
	intermediateResult = intermediateResult/length;
	
	// ushort sqrt_16 (DATA *x, DATA *r, short nx) might be used
	intermediateResult = sqrt(intermediateResult);
	
	return (Int16)intermediateResult;
}


void codecRead(Int16 *real, Int16 sampleLength){
	Int16 i = 0;
	for(i = 0; i < sampleLength; i++){
		 aic3204_codec_read_MONO(&real[0], &dummy1,i);	 
	}
}

Int16 peakDetect(Int16 *data,Int16 *peakArray, Int16 length, Int16 limit)
{
	Int16 i;
	Int16 max = 0;
	Int16 peakPosition;
	Int16 nrPeaks = 0;
	Int16 fallingEdge = 0;
	Int16 oldEdge = 0;

	// short maxval (DATA *x, ushort nx) might be used instead
	// and short maxidx (DATA *x, ushort ng, ushort ng_size)
	// or just use void maxvec (DATA *x, ushort nx, DATA *r_val,DATA *r_idx)
	for(i = 0; i < length; i++)
	{
		if (*(data+i) > limit)
		{
			fallingEdge = 1;
			if(max < *(data+i))
			{
				max = *(data+i);
				peakPosition = i;
			}
		} 
		else {
			fallingEdge = 0;
		}
		
		if (fallingEdge == 0 && oldEdge == 1)
		{
			*(peakArray+nrPeaks) = peakPosition;
			max = 0;
			nrPeaks++;
		}
		oldEdge = fallingEdge;
	}
	
	return nrPeaks;
}

void printErrorMessages(Int32 expBits, Int32 fractionBits, Int16 rms, Int16 HZ, Int16 peakF, Int16 crest){
	printf("%s \n \n", "---------------- TIME DOMAIN ANALYSIS ------------------");
		
	printf("%s %d %s \n", "The shaft frequency is: ", HZ, "Hz");
	
	printf("%s %d %s \n", "The peak frequency is: ", peakF, "Hz");
	
	printf("%s %ld%s%ld \n \n","The shaft frequency to peak frequency ratio is:  ", expBits,".",fractionBits);
	
	printf("%s %d \n", "The crest factor is: ", peakF);
	
	printf("%s %d \n", "The measured RMS value is: ", rms);
	if(rms > 600) { printf("Bearing condition critical. Please do further inspection to locate the fault. Replacement of bad part is needed! \n \n");}
	else if (rms > 450 && rms < 600){ printf("Bearing condition mediocre. Consider changing the bearing \n \n");}
	else if (rms < 450){ printf("Bearing condition good. No further action needed \n \n");}
		
	printf("%s \n \n", "-------------- END OF TIME DOMAIN ANALYSIS -------------");
	
	printf("%s \n \n", "---------------- FREQUENCY DOMAIN ANALYSIS ------------------");
	
	printf("%s \n \n", "-------------- END OF FREQUENCY DOMAIN ANALYSIS -------------");
	if(errorFlag & 0x01){
		printf("%s \n", "PEAK");	
	}	
}

void calculate_abs(Int16 real, Int16 imag, Int32 *absolute_ptr, Int16 current_entry){
	Int32 real_squared = 0;
	Int32 imag_squared = 0;
	Int32 scaling = 1;
	
	//real_squared = *(real);
	//THIS IS UTTERLY FUCKING RETARDED	
//	---------------------------------------------------------------------
	real_squared = (real/scaling)*(real/scaling);
	imag_squared = (imag/scaling)*(imag/scaling);
	
	
	//real_squared = ((real/1000)*(real/1000));
	//imag_squared = ((imag/1000)*(imag/1000));
	
	*(absolute_ptr+current_entry) = sqrt(real_squared+imag_squared);

//----------------------------------------------------------------------- 	


}

Int32 homemadePOW(Int16 exponent){
	Int16 i = 0;
	Int32 VAL = 2;
	for(i = 0; i < exponent+1;i++){
		VAL = VAL*2;
	}	
	return VAL;
}

Int32 peaksPerRev(Int16 peaks, Int16 rpm){
	// Int32 rpmToHz = 166;		// 0,01666*10000	
	Int32 secondsPerMeas = 170; // 0,17*1000
	Int32 Hz = rpm/60;
	Int32 revPerSampling = ((Hz*secondsPerMeas)); // Measured revolutions
	Int32 peakFreq = revPerSampling/(Int32)peaks; // Revolution/peak
	if (peaks == 0) return 0;
	else return peakFreq;
}

Uint16 getAbs(Int16 data) 
{ 
    Int16 bitmask = data >> 15; // Generate bitmask (0 for positive number, -1 for negative)
    return ((data ^ bitmask) - bitmask);  // number XORed with bitmask. Bitmask subtracted afterwards
} 

#pragma DATA_SECTION(data_br_buf,"data_br_buf");	//Kommando der placere arrayet med bit reversed pladser et bestemt sted
#pragma DATA_ALIGN(data_br_buf,2048);			//-||-
Int32 data_br_buf[1024];						//Array der har sine indgange bit reversed

#pragma DATA_SECTION(scratch_buf,"scratch_buf");	//Kommando der placere arrayet med bit reversed pladser et bestemt sted
#pragma DATA_ALIGN(scratch_buf,2048);			//-||-
Int32 scratch_buf[1024];						//Array der indeholder bare reele og imagin�re v�rdier, men indgangende har skiftet plads med bit reverse

Int32 absolute_value[1024];

Int16 real_part[8192];								//Array der indeholder de reelle datapunkter i tidsdom�net
Int16 envelope_data[1024];

Int16 i = 0;
Int16 rpm = 0;
Int16 nrOfPeaks;
Int16 peakIndices[30];
//Int16 xfilter[8192];
Int16 dBuffer[121+2];
Int16 *dBufferer_ptr = &dBuffer[0];
Uint16 firflag = 0;
Int16 fft_datapoints[1024*2];						//Array der indeholder b�de reele og imagin�re v�rdier
Int16 loopCounter = 0;
Int16 real_freq[1024];								//Array der indeholder de FFT/IFFT transformerede reele v�rdier
//Int16 imaginary_freq[1024];							//Array der indeholder de FFT/IFFT transformerede imagin�re v�rdier

Int32 *fft_output_location;							//Indeholder memory location af den array der har outputtet af FFT/IFFT'en
const Uint16 fft_length = 1024;								//L�ngde af FFT eller IFFT
Uint16 fft_save_location;	
Int16 rmsValue = 0;
Int16 maxValue = 0;
Int16 rmsValueRaw = 0;
Int16 CrestFactor = 0;

Int32 peakFreq = 0;
Int32 fraction = 0;
Int16 digit = 0;
Int32 fractionBits = 0;
Int32 expBits = 0;

void main(void) //main
{		
	Int16 status;	
	inits(); // Setting up dsp, including stuff for I2C
	
	for(i = 0; i < 8192; i++) real_part[i] = 0;
	for(i = 0; i < 1024; i++) envelope_data[i] = 0;
	
	MMAbegin();
	
	status = 1;
	while(status){
		printf("Sampling data \n");
		
		
		
		requestFromArduino(0); // START
		// Sampling accelerometer data - 17 064 992 cycles!!!
		codecRead(&real_part[0], 2048);
		codecRead(&real_part[2048], 2048);
		codecRead(&real_part[4096], 2048);
		codecRead(&real_part[6144], 2048);
		// Finished sampling
		requestFromArduino(1); // STOP
		
		// Read RPM data via i2c 
		// rpmReadI2C(&rpm); //32 033 221 cycles
		
		// Hvad g�r dette?
		/*
		for(i = 0; i < 2048; i++)
		{
			real_part[1024+i] = real_part[1024+i] + 360;
			real_part[3072+i] = real_part[3072+i] + 180;
			real_part[5120+i] = real_part[5120+i] + 60;
		}
		for(i = 0; i < 1024; i++){
			real_part[0+i] = real_part[1024+i*8];
		}
		*/
		
		// RMS
		// printf("Calculating RMS \n");
		
		// rmsValueRaw = RMS(&real_part[0], fft_length); // 29 156 cycles
		
		// Highpass
		printf("Highpass data \n");
		
		firflag = fir2(real_part, HP_coeffs, real_part, dBuffer, 8192, 121); //1 012 280 cycles
		
		// Absolute svalue
		
		printf("Demodulate \n");
		
		for(i = 0; i < 8192; i++){ // 45 cycles each loop using getAbs - 21 cycles using _abss
			/* if(real_part[i] < 0) // 27 each loop
			{
				real_part[i] = real_part[i] * -1;
			}	*/
			//real_part[i+1024] = getAbs(real_part[i+1024]); // 31 cycles each loop
			real_part[i] = _abss(real_part[i]); // 9 cycles each loop
		}
		
		printf("Lowpass \n");
		// Lowpass
		// firflag = fir2(&real_part[0], lowpass, &real_part[0], dBufferer_ptr, 8192, 121); // 1 012 280 cycles
		
		// Lowpass and decimate
		firflag = firdec(real_part, LP_coeffs, envelope_data, dBuffer, 8192, 121,8); // 124 174 cycles
		
		// printf("Decimate data \n");
		
		// Decimation (Downsampling)
		//for(i = 0; i < 1024; i++){ // 22 cycles each loop => total = 22*1024 == 22528
		// real_part[0] = real_part[i*8]; 
		//}

	printf("RMS envelope data \n");
	// Calculate RMS
	rmsValue = RMS(&envelope_data[0], fft_length); // 6156 cycles
	
	printf("Finding peaks in envelope \n");
	// Calculate peaks
	nrOfPeaks = peakDetect(&envelope_data[0], &peakIndices[0], fft_length, rmsValueRaw*3); // 54 375 cycles
	
	printf("Frequency of peaks \n");
	// Calculate peak frequency
	peakFreq = peaksPerRev(nrOfPeaks, rpm); // 130 cycles
	
	// 196 cycles
	fraction = 123;

		fractionBits = peakFreq % 1000;
		peakFreq /=1000;
		expBits = peakFreq % 10;
		
	printf("Computing FFT \n");
/*
 * 	FFT FUNCTIONS BELOW
 *-------------------------------------------------------------------------------------------------------------------------------------------------------------------
 */
 	//Kommando der sammens�tter real_part og imaginary_part sammen til en array, fft_datapoints
	fft_create_datapoint_array(&real_part[2048], fft_length, &fft_datapoints[0]); // 2073 cycles
	//Kommando der bit reverser pladserne s� de havner i data_br_buf
	hwafft_br((Int32 *)&fft_datapoints[0], &data_br_buf[0],fft_length); // 5245 cycles
	
	//Sidste v�rdi er skalering (1 uden skalering, 0 med)
	fft_save_location = hwafft_1024pts(&data_br_buf[0], &scratch_buf[0],0,1); // 5245 cycles

// Check FFT save ----------------------------------------------------------------------------------------------
	if(fft_save_location == 17857){ 								//Tjekker om FFT'en rent faktisk er blevet udf�rt
		printf("FFT NOT executed correctly");}						//Hvis ikke, surt show
	else {		
		if(fft_save_location == 0){ 								//Tjekker om dataen er i bitrev array
			fft_output_location = &data_br_buf[0];}
		else if(fft_save_location == 1){ 							//Tjekker om dataen er i scratch array
			fft_output_location = &scratch_buf[0];} 
		else {printf("Output array MIA \n");}						//Hvis ikke, surt show
	
// FFT check end - 46 cycles

	// This is utterly fucking retarded
	//For loop der splitter den FFT/IFFT transformered array op i reel og imagin�r del	
	for(loopCounter = 0; loopCounter < fft_length; loopCounter++){ 	// around 6250 cycles each loop => total = 1024 * 6250 = 6400000 cycles!!!					
		real_part[loopCounter+4096] = (*(fft_output_location+loopCounter+1)) & 0x0000FFFF; 	//Imagin�r del
		real_freq[loopCounter] = (*(fft_output_location+loopCounter)) >> 16; 			//Reel del
				
		calculate_abs(real_freq[loopCounter], real_part[loopCounter+4096], &absolute_value[0], loopCounter);
		}
	}
	//printf("%s \n \n","----------FFT---------");
//	for(i = 0; i < 1024; i++){ printf("%ld \n", absolute_value[i]);}

	printf("Computing crest factor \n");
	
	CrestFactor = maxValue/rmsValueRaw; // 41 cycles
	
	if(CrestFactor > 3) // 9 cycles
	{
		// THIS IS ALSO UTTERLY FUCKING RETARDED
		errorFlag |= 0x01;
	}
	
	printErrorMessages(expBits, fractionBits, rmsValueRaw, rpm/60, nrOfPeaks*6, CrestFactor); // 124565 cycles
  	
  	printf("Breakpoint reached \n \n");
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
