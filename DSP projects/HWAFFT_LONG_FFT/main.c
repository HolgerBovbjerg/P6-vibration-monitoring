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
	Int16 j = 0;
	Int32 intermediateResult = 0;
	
	for(i = 0; i < length; i++)
	{		
			intermediateResult = intermediateResult + ((Int32)*(data+i))*((Int32)*(data+i));
	}

	
	intermediateResult = intermediateResult/length;
	
	intermediateResult = sqrt(intermediateResult);
	
	return (Int16)intermediateResult;
}


void codecRead(Int16 *real, Int16 sampleLength){
	Int16 i = 0;
	ezdsp5535_waitusec(250000);
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
		} else {
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

void printErrorMessages(Int32 expBits, Int32 fractionBits, Int16 rms, Int16 HZ, Int16 peakF, Int16 crestEx, Int16 crestFra, Int16 maxV){
	printf("%s \n \n", "---------------- TIME DOMAIN ANALYSIS ------------------");
		
	printf("%s %d %s \n", "The shaft frequency is: ", HZ, "Hz");
	
	printf("%s %d %s \n", "The peak frequency is: ", peakF, "Hz");
	
	printf("%s %ld%s%ld \n \n","The shaft frequency to peak frequency ratio is:  ", expBits,".",fractionBits);
	
	printf("%s %d%s%d \n", "The crest factor is: ", crestEx,".",crestFra);
	
	printf("%s %d \n", "The measured RMS value is: ", rms);
	
	printf("%s %d \n", "The highest peak: ", maxV);
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
	Int32 rpmToHz = 166;		// 0,01666*10000	
	Int32 secondsPerMeas = 170; // 0,17*1000
	Int32 Hz = rpm/60;
	Int32 revPerSampling = ((Hz*secondsPerMeas)); // Measured revolutions
	Int32 peakFreq = revPerSampling/(Int32)peaks; // Revolution/peak
	if (peaks == 0) return 0;
	else return peakFreq;
}

#pragma DATA_SECTION(data_br_buf,"data_br_buf");	//Kommando der placere arrayet med bit reversed pladser et bestemt sted
#pragma DATA_ALIGN(data_br_buf,2048);			//-||-
Int32 data_br_buf[1024];						//Array der har sine indgange bit reversed

#pragma DATA_SECTION(scratch_buf,"scratch_buf");	//Kommando der placere arrayet med bit reversed pladser et bestemt sted
#pragma DATA_ALIGN(scratch_buf,2048);			//-||-
Int32 scratch_buf[1024];						//Array der indeholder bare reele og imagin�re v�rdier, men indgangende har skiftet plads med bit reverse
Int32 absolute_value[1024];

void main(void) //main
{			
	Int16 real_part[9216];								//Array der indeholder de reelle datapunkter i tidsdom�net
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
	
	Int16 crestFrac = 0;
	Int16 crestExp = 0; 
	
	inits(); // Setting up stuff for I2C
	
	

	MMAbegin();
		
		//MMAread(&real_part[0]);
	for(i = 0; i < 1; i++){
		requestFromArduino(0); // START
		codecRead(&real_part[1024], 2048);
		codecRead(&real_part[3072], 2048);
		codecRead(&real_part[5120], 2048);
		codecRead(&real_part[7168], 2048);
		requestFromArduino(1); // STOP
		rpmReadI2C(&rpm);
		//printf("%d \n", rpm);}
		for(i = 0; i < 2048; i++)
		{
			real_part[1024+i] = real_part[1024+i] + 360;
			real_part[3072+i] = real_part[3072+i] + 180;
			real_part[5120+i] = real_part[5120+i] + 60;
		}
		
		for(i = 0; i < 1024; i++){
			real_part[0+i] = real_part[1024+i*8];
		}
		
	//	printf("%s \n \n","----------RAW---------");
	//	for(i = 0; i < 1024; i++){ printf("%hd \n", real_part[i]);}
		/*for(i = 2048; i < 3072; i++){ printf("%hd \n", real_part[i]);}
		for(i = 3072; i < 4096; i++){ printf("%hd \n", real_part[i]);}
		for(i = 4096; i < 5120; i++){ printf("%hd \n", real_part[i]);}
		for(i = 5120; i < 6144; i++){ printf("%hd \n", real_part[i]);}
		for(i = 6144; i < 7168; i++){ printf("%hd \n", real_part[i]);}
		for(i = 7168; i < 8192; i++){ printf("%hd \n", real_part[i]);}
		for(i = 8192; i < 8192+1024; i++){ printf("%hd \n", real_part[i]);}*/
		
		
		rmsValueRaw = RMS(&real_part[0], fft_length);
		
		firflag = fir2(&real_part[0], HP_coeffs, &real_part[1024], dBufferer_ptr, 1024, 121);
		
	//	printf("%s \n \n","----------HP---------");
	//	for(i = 1024; i < 2048; i++){ printf("%hd \n", real_part[i]);}
		/*for(i = 2048-512; i < 3072-512; i++){ printf("%hd \n", real_part[i]);}
		for(i = 3072-512; i < 4096-512; i++){ printf("%hd \n", real_part[i]);}
		for(i = 4096-512; i < 5120-512; i++){ printf("%hd \n", real_part[i]);}
		for(i = 5120-512; i < 6144-512; i++){ printf("%hd \n", real_part[i]);}
		for(i = 6144-512; i < 7168-512; i++){ printf("%hd \n", real_part[i]);}
		for(i = 7168-512; i < 8192-512; i++){ printf("%hd \n", real_part[i]);}
		for(i = 8192-512; i < 8192+1024-512; i++){ printf("%hd \n", real_part[i]);}*/
		
		for(i = 1024; i < 2048; i++){
			if(real_part[i] < 0)
			{
				real_part[i] = real_part[i] * -1;
			}	
		}
	//	printf("%s \n \n","----------ABS---------");
	//	for(i = 1024; i < 2048; i++){ printf("%hd \n", real_part[i]);}
		/*for(i = 2048-512; i < 3072-512; i++){ printf("%hd \n", real_part[i]);}
		for(i = 3072-512; i < 4096-512; i++){ printf("%hd \n", real_part[i]);}
		for(i = 4096-512; i < 5120-512; i++){ printf("%hd \n", real_part[i]);}
		for(i = 5120-512; i < 6144-512; i++){ printf("%hd \n", real_part[i]);}
		for(i = 6144-512; i < 7168-512; i++){ printf("%hd \n", real_part[i]);}
		for(i = 7168-512; i < 8192-512; i++){ printf("%hd \n", real_part[i]);}
		for(i = 8192-512; i < 8192+1024-512; i++){ printf("%hd \n", real_part[i]);}*/
		
		firflag = fir2(&real_part[1024], lowpass, &real_part[2048], dBufferer_ptr, 1024, 121);
		
	//	printf("%s \n \n","----------LP---------");
	//	for(i = 1024+1024; i < 2048+1024; i++){ printf("%hd \n", real_part[i]);}
		/*for(i = 2048-1024; i < 3072-1024; i++){ printf("%hd \n", real_part[i]);}
		for(i = 3072-1024; i < 4096-1024; i++){ printf("%hd \n", real_part[i]);}
		for(i = 4096-1024; i < 5120-1024; i++){ printf("%hd \n", real_part[i]);}
		for(i = 5120-1024; i < 6144-1024; i++){ printf("%hd \n", real_part[i]);}
		for(i = 6144-1024; i < 7168-1024; i++){ printf("%hd \n", real_part[i]);}
		for(i = 7168-1024; i < 8192-1024; i++){ printf("%hd \n", real_part[i]);}
		for(i = 8192; i < 8192+1024-1024; i++){ printf("%hd \n", real_part[i]);}*/
		
			//ezdsp5535_waitusec(250000);
			//printf("%s \n", "STARTING");
		//for(i = 0; i < 1024; i++){ printf("%hd \n", real_part[i]);}
		for(i = 0; i < 1024; i++){
			real_part[8192+i] = real_part[i*8];
		}
		//printf("%s \n \n","----------DS---------");
		//for(i = 8192; i < 8192+1024; i++){ printf("%hd \n", real_part[i]);}
		
		//while( i < 8192) { printf("%d \n", real_part[i]); i++; }
	/*	for(i = 1024; i < 2048; i++){ printf("%hd \n", real_part[i]);}
		for(i = 2048; i < 3072; i++){ printf("%hd \n", real_part[i]);}
		for(i = 3072; i < 4096; i++){ printf("%hd \n", real_part[i]);}
		for(i = 4096; i < 5120; i++){ printf("%hd \n", real_part[i]);}
		for(i = 5120; i < 6144; i++){ printf("%hd \n", real_part[i]);}
		for(i = 6144; i < 7168; i++){ printf("%hd \n", real_part[i]);}
		for(i = 7168; i < 8192; i++){ printf("%hd \n", real_part[i]);}*/
		//for(loopCounter = 0; loopCounter < 1024; loopCounter++){real_part[loopCounter] = loopCounter;}
		//for(loopCounter = 0; loopCounter < 1024; loopCounter++){imaginary_part[loopCounter] = 0;} 
	
/*
 * 		TIME DOMAIN ANALYSIS BELOW
 *  ---------------------------------------------------------------------------------------------
 */	 

	rmsValue = RMS(&real_part[2048], fft_length);
	
	nrOfPeaks = peakDetect(&real_part[2048], &peakIndices[0], fft_length, rmsValueRaw*3);
	
	peakFreq = peaksPerRev(nrOfPeaks, rpm);
	fraction = 123;

		fractionBits = peakFreq % 1000;
		peakFreq /=1000;
		expBits = peakFreq % 10;
	
	//printf("%s %ld%s%ld \n"," Peaks per revolution:  ", expBits,".",fractionBits);
	
/*	
	for(i = 0; i < 15; i++){
		fraction += ((peakFreq >> (15-i) ) & 0x01) * homemadePOW(i);	
	}
	*/
	for(i = 0; i < 1024; i++){
			if(real_part[8192+i] > maxValue){
				maxValue = real_part[8192+i];
			}
		}
	

	//printf("%s %d %s %d \n","RMS:  " , rmsValue, "   PEAK:  ", maxValue );

/*
 * 	FFT FUNCTIONS BELOW
 *----------------------------------------------------------------------------------------------
 */
	fft_create_datapoint_array(&real_part[2048], fft_length, &fft_datapoints[0]); //Kommando der sammens�tter real_part og imaginary_part sammen til en array, fft_datapoints
	hwafft_br((Int32 *)&fft_datapoints[0], &data_br_buf[0],fft_length); //Kommando der bit reverser pladserne s� de havner i data_br_buf
	

	fft_save_location = hwafft_1024pts(&data_br_buf[0], &scratch_buf[0],0,1); //Sidste v�rdi er skalering (1 uden skalering, 0 med)

// ----------------------------------------------------------------------------------------------
	if(fft_save_location == 17857){ 								//Tjekker om FFT'en rent faktisk er blevet udf�rt
		printf("FFT NOT executed correctly");}						//Hvis ikke, surt show
	else {		
		if(fft_save_location == 0){ 								//Tjekker om dataen er i bitrev array
			fft_output_location = &data_br_buf[0];}
		else if(fft_save_location == 1){ 							//Tjekker om dataen er i scratch array
			fft_output_location = &scratch_buf[0];} 
		else {printf("Output array MIA \n");}						//Hvis ikke, surt show
	

		// This is utterly fucking retarded
	for(loopCounter = 0; loopCounter < fft_length; loopCounter++){ 							//For loop der splitter den FFT/IFFT transformered array op i reel og imagin�r del	
		real_part[loopCounter+4096] = (*(fft_output_location+loopCounter+1)) & 0x0000FFFF; 	//Imagin�r del
		real_freq[loopCounter] = (*(fft_output_location+loopCounter)) >> 16; 			//Reel del
				
		calculate_abs(real_freq[loopCounter], real_part[loopCounter+4096], &absolute_value[0], loopCounter);
		//printf("%ld \n", absolute_value[loopCounter]);
		}
//		for(loopCounter = 0; loopCounter < fft_length; loopCounter++){ 
//			printf("%d \n", real_part[loopCounter]);
//		}
	}
	//printf("%s \n \n","----------FFT---------");
//	for(i = 0; i < 1024; i++){ printf("%ld \n", absolute_value[i]);}
	CrestFactor = (10*maxValue)/rmsValueRaw;
	crestFrac = CrestFactor % 10;
	crestExp = CrestFactor / 10; 
	if(CrestFactor > 3)
	{
		// THIS IS ALSO UTTERLY FUCKING RETARDED
		errorFlag |= 0x01;
	}
	//printf("%s \n", "GFUJNERFEON");
	printErrorMessages(expBits, fractionBits, rmsValueRaw, rpm/60, nrOfPeaks*6, crestExp, crestFrac, maxValue);
  printf("Breakpoint reached \n");
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