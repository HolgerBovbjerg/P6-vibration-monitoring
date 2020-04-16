;/*
; * Copyright (C) 2013 Texas Instruments Incorporated - http://www.ti.com/ 
; * 
; * 
; *  Redistribution and use in source and binary forms, with or without 
; *  modification, are permitted provided that the following conditions 
; *  are met:
; *
; *    Redistributions of source code must retain the above copyright 
; *    notice, this list of conditions and the following disclaimer.
; *
; *    Redistributions in binary form must reproduce the above copyright
; *    notice, this list of conditions and the following disclaimer in the 
; *    documentation and/or other materials provided with the   
; *    distribution.
; *
; *    Neither the name of Texas Instruments Incorporated nor the names of
; *    its contributors may be used to endorse or promote products derived
; *    from this software without specific prior written permission.
; *
; *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
; *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
; *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
; *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
; *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
; *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
; *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
; *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
; *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
; *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
; *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
; *
;*/

;***********************************************************
; Version 3.00.00                                           
;***********************************************************
; Processor:   C55xx
; Description: Radix-4/Radix-2 DIT no scale complex IFFT using normal input data
;    and bit-reversed twiddle table (length N/2, cosine/sine format)
;    
; Usage:    void cifft (DATA *x, ushort nx, NOSCALE);
;
; Benchmarks:
;   Cycles:
;     nx =  16:    231
;     nx =  32:    460
;     nx =  64:    977
;     nx = 128:   2153
;     nx = 256:   4798
;     nx = 512:  10707
;     nx =1024:  23748
;   Code Size (in bytes):  
;     .text            323
;     .const:twiddle  2048 (shared by cfft, cifft, unpack, unpacki)
;
; History:
; 	- 07/17/2003	C. Iovescu fixed the bug in the kernal.
;	- 07/17/2003	C. Iovescu changed the way the twiddle table is included
;       - 06/20/2012    Craig Leeds: removed .bss usage and general cleanup
;****************************************************************

;-----------------------------------------------------------------------
; Arguments passed to _fft
;
; AR0       ->    fftdata pointer
; T0        ->    fft size
;
;-----------------------------------------------------------------------



;//-----------------------------------------------------------------------------
;// Array declarations
;//-----------------------------------------------------------------------------

            .ref twiddle

                 
        .def _cifft_NOSCALE     ; make function visible to other fnct
        .cpl_on
        .arms_off               ; enable assembler for arms mode
        .mmregs
        .noremark    5579, 5573, 5684
  
; Stack frame
; -----------
RET_ADDR_SZ       .set 1            ;return address
REG_SAVE_SZ       .set 5            ;save-on-entry registers saved (T3, T2, AR5, AR6, AR7)
FRAME_SZ          .set 6            ;local variables
ARG_BLK_SZ        .set 0            ;argument block

PARAM_OFFSET      .set ARG_BLK_SZ + FRAME_SZ + REG_SAVE_SZ + RET_ADDR_SZ


; Local variables
; --------------      
       
       .asg    0, tempmem     ; (long) must be aligned on even
       .asg    2, tempmem1    ; (long) must be aligned on even
       .asg    4, data_ptr
       .asg    5, data_sz

;//-----------------------------------------------------------------------------
;// Register aliases
;//-----------------------------------------------------------------------------
                .asg        T3,groups            ; number of butterfly groups
                .asg        T1,index             ; addess adjustment
                .asg        AR7,bflies           ; number of butterflies/group
                .asg        AR4,k                ; outer loop counter
                .asg        AR2,a                ; top butterfly pointer
                .asg        AR3,b                ; bottom butterfly pointer
                .asg        AR5,temp             ; temp register        

        .text           
_cifft_NOSCALE:

;//-----------------------------------------------------------------------------
;// Conditional compile
;//-----------------------------------------------------------------------------
SCALED        .set        0                     ; SCALED = 0 - not scaled version
                                                ; SCALED = 1 - scaled version



;
; Save any save-on-entry registers that are used
;----------------------------------------------------------------
;
        PSH     T3, T2
     || BCLR    #ARMS, ST2_55          
        PSHBOTH XAR5
     || BSET    #FRCT, ST1_55          
        PSHBOTH XAR6
     || BSET    SATD
        PSHBOTH XAR7
;
; Allocate the local frame and argument block
;----------------------------------------------------------------
        AADD    #-(ARG_BLK_SZ + FRAME_SZ), SP
;
; Save entry values for later
;----------------------------------------------------------------

;-----------------------------------------------------------------------
; FFT implementation
;
; The FFT is implemented in three different steps:
;
;  1) - a radix-4 stage with twiddle factors only equal to -1, 0 and 1
;  2) - a group of log2(FFT_SIZE)-3 radix-2 stages
;  3) - on out-of-place bit-reversal
;
;  No need to implement the last stage in a different way
;  to the other stages.
        
;-----------------------------------------------------------------------
              
;-----------------------------------------------------------------------
; First two stages computed as one radix-4 stage


    ; --------------------------------------------------------------        
        ; First in-place radix-4 stage for CIFFT
        ;
        ;        ----------
        ; a -----|   R    |----- a'
        ;            A
        ; b -----|   D    |----- b'
        ;            I
        ; c -----|   X    |----- c'
        ;  
        ; d -----|   4    |----- d'
        ;        ---------
        ;
        ; ar' =  (ar + cr) + (br + dr)
        ; ai' =  (ai + ci) + (bi + di) 
        ;
        ; br' =  (ar + cr) - (br + dr)
        ; bi' =  (ai + ci) - (bi + di)
        ;
        ; cr' =  (ar - cr) - (bi - di)
        ; ci' =  (ai - ci) + (br - dr)
        ;
        ; dr' =  (ar - cr) + (bi - di)
        ; di' =  (ai - ci) - (br - dr)
        ;        
        ; AR7, -> temp
        ; AR6  -> temp1
        ;
        ; AR0-> a
        ; AR1-> b
        ; AR2-> c
        ; AR3-> d
        ;
        ; AR5 = ar2
        ;
        ; T0 = 1
        ; T1 = 2
        ;       
        ;
        ; AC0-AC3 modified
        ; T1-T2 modified
        ;
        ; --------------------------------------------------------------        
                
    ; AR0 contains    #(fftdata)                    ; a

        AMAR    *AR0(T0), XAR2                      ; c: AR2 = #(fftdata + 2*2*FFT_SIZE/4)
        MOV     T0, *SP(data_sz)   ;
     || SFTS    T0, #-1                             ; T0 = FFT_SIZE/2
        MOV     AR0, *SP(data_ptr) ; 
     
        MOV     XSP, XAR7
        MOV     XSP, XAR6
 .if (tempmem1 != 0)
        ADD     #tempmem1, AR7                      ; AR7 = &tempmem1 (on stack)
 .endif
 .if (tempmem != 0)
        ADD     #tempmem, AR6                       ; AR6 = &tempmem (on stack)
 .endif
 
        AMAR    *AR0(T0), XAR1                      ; b: AR1 = #(fftdata + 1*2*FFT_SIZE/4)
        AMAR    *AR2(T0), XAR3                      ; d: AR3 = #(fftdata + 3*2*FFT_SIZE/4)

        SFTS    T0, #-1                             ; T0 = FFT_SIZE/4
        SUB     #2, T0                              ; T0 = FFT_SIZE/4 - 2
        
        MOV     #2, T1                                                         ; 
     || MOV     dbl(*AR0), AC0                      ; in (ar,ai)
       
        MOV     T0, BRC0
     || ADD     dual(*AR2), AC0, AC2                ; (ar+cr)/ ; (ai+ci) 
                            
    
        MOV     XAR2, XAR5 
        MOV     #1, T0
     
        MOV     dbl(*AR1) ,AC1                      ; in (br,bi)
     || MOV     AC2, dbl(*AR6)                      ; temp (ar+cr),(ai+ci)   
    
        SUB     dual(*AR3), AC1, AC2                ; (br-dr)/(bi-di)
        
        SUB     dual(*AR2+), AC0                    ; (ar-cr)/(ai-ci) 
     || MOV     HI(AC2),T3                          ; move (br-dr)
    
        ADD     dual(*AR3), AC1, AC3                ; (br+dr)/ (bi+di)
     || MOV     AC2, T2                             ; move (bi-di)        
                    
        ADD     dual(*AR6), AC3, AC0                ; (ar+cr) + (dr+br) = ar'
                                                    ; (ai+ci) + (di+bi)        = ai'
     || MOV     AC0, dbl(*AR7(T0))                  ; (ar-cr),(ai-ci) unaligned

          
        SUB     AC3, dual(*AR6), AC3                ; (ar+cr) - (dr+br) = br'
                                                    ; (ai+ci) - (di+bi)        = bi'
     || MOV     AC0, dbl(*AR0+)                     ; out (ar';ai')
         
        ADDSUB  T2, *AR7(T0), AC0                   ; (ar-cr) - (bi-di)        = cr' (low part)
                                                    ; (ar-cr) + (bi-di)        = dr' (high part)
        SUBADD  T3, *AR7, AC1                       ; (ai-ci) + (br-dr)        = ci' (low part)
                                                    ; (ai-ci) - (br-dr)        = di' (high part)
     || RPTBLOCAL r4_loop-1
                MOV     AC3, dbl(*AR1+)             ; out(br', bi')                                                                ; 
             || MOV     dbl(*AR0), AC3              ; in (ar,ai)
       
                MOV     pair(LO(AC0)), dbl(*AR5+)   ; out(cr',ci')
             || ADD     dual(*AR2), AC3, AC2        ; (ar+cr) 
                                                    ; (ai+ci)   
                MOV     dbl(*AR1), AC1              ; in (br,bi)
             || MOV     pair(HI(AC0)), dbl(*AR3+)   ; out (dr', di')
    
                SUB     dual(*AR3), AC1, AC2        ; (br-dr)/(bi-di) 
             || MOV     AC2, dbl(*AR6)              ; temp (ar+cr),(ai+ci)
    
        
                SUB     dual(*AR2+), AC3, AC0       ; (ar-cr)/(ai-ci) 
             || MOV     HI(AC2), T3                 ; move(br-dr)
    
                ADD     dual(*AR3), AC1, AC3        ; (br+dr)/(bi+di) 
             || MOV     AC2, T2                     ; move (bi-di)        
                    
        
                ADD     dual(*AR6), AC3, AC0        ; (ar+cr) + (dr+br) = ar'
                                                    ; (ai+ci) + (di+bi)        = ai' 
             || MOV     AC0, dbl(*AR7(T0))          ; (ar-cr),(ai-ci) unaligned
          
           
                SUB     AC3, dual(*AR6), AC3        ; (ar+cr) - (dr+br) = br'
                                                    ; (ai+ci) - (di+bi)        = bi'
             || MOV     AC0, dbl(*AR0+)             ; out (ar';ai')
         
                ADDSUB  T2, *AR7(T0), AC0           ; (ar-cr) - (bi-di)        = cr'
                                                    ; (ar-cr) + (bi-di)        = dr'
                SUBADD  T3, *AR7, AC1               ; (ai-ci) + (br-dr)        = ci'
                                                    ; (ai-ci) - (br-dr)        = di'
r4_loop:
     
    MOV     AC3, dbl(*AR1+)                         ; out(br', bi')          
    MOV     pair(LO(AC0)), dbl(*AR5+)               ; out(cr',ci')       
    MOV     pair(HI(AC0)), dbl(*AR3+)               ; out (dr', di')      

;-----------------------------------------------------------------------
; radix-2 stages (stages 3->log2(FFT_SIZE) )

        ; main initialization
        ; --------------------
        
       MOV     *SP(data_ptr), AR0                   ; AR0 = #fftdata
       
            ; 1 - circular buffer initializations
        
            ; modify ST2 to select linear or circular addressing modes
       OR       #0x57 , mmap(ST2_55)  ; circular AR0, AR1, AR2, AR4 and AR6

            ; circular buffer starting addresses
        ADD     #2, AR0, AR1
        MOV     AR0, mmap(BSA23)       ; P  leg
        MOV     AR0, mmap(BSA45)       ; P' leg    (in-place butterflies)
        MOV     AR1, mmap(BSA01)       ; Q  leg
        MOV     AR1, mmap(BSA67)       ; Q' leg    (in-place butterflies)
                            
            ; circular buffer sizes 
        MOV     *SP(data_sz), T1       ; T1 =  #FFT_SIZE
        SUB     #1, T1, T0
        SFTS    T0, #1        
        MOV     T0, mmap(BK03)        ; BK03 = (2*FFT_SIZE-2), 
        MOV     T0, mmap(BK47)        ; BK47 = (2*FFT_SIZE-2), 

            ; 2 - register initializations
        
        MOV     T1, T0
        SFTS    T0, #-1
        SFTS    T0, #-1          ; 2*number of groups for stage 3    
        MOV     T0, AR5          ; 2*number of groups for stage 3    
        MOV     #2, AR7          ; 1/2 * number of butterflies per group for stage 3
       
        AMOV    #twiddle+2, XAR3 ; not modified during iteration of the stages
                                 ; n=0 processed outside inner loop
        MOV     XAR0, XAR1       ; data page = data page of indata
        MOV     XAR0, XAR2       ; data page = data page of indata
        MOV     XAR0, XAR4       ; data page = data page of indata (in-place butterflies)
        MOV     XAR0, XAR6       ; data page = data page of indata (in-place butterflies)
        MOV     XAR3, XCDP       ; data page = data page of twiddles
        
stage:    ; begining of a new stage

        ; stage initialization
        ; --------------------

        ; 1 - loop counter updates
        
        ; butterfly counter update
        SFTS    AR7, #1
     || MOV     #0, AR4 
        SUB     #2, AR7, AR0
        MOV     AR0, BRC1        ;1 butterflies executed out of the loop

            ; group counter update
        SFTS    AR5, #-1
        SUB     #1, AR5, AR0     
        MOV     AR0, BRC0         

            ; stage "counter" update (T0=0 <=> last stage)
        MOV     AR0, T0

            ; 2 - pointer modifier update
                            
            ; T1 is the pointer modifier                    
        SFTS    T1, #-1        

            ; 3 - index updates                  
            ; AR0 -> Qr[0]
            ; AR1 -> Qi[0]
            ; AR2 -> Pr[0]
            ; AR4 -> Pr'[0]  (performed above to use parallelism)
            ; AR6 -> Qr'[0]
 
        SFTS    AR0, #1
        ADD     #1, AR0, AR1
        AMOV    #0, AR2
     || MOV     AR0, AR6 
        
group:  ; begining of a new group

        ; note:
        ; - circular buffers are used for data pointers so that no
        ;   initialization of data pointers is required before a new
        ;   iteration of a group
        ; - twiddle pointer (CDP) is updated inside the loop
        ; - same twiddle table as for FFT: (wr, wi) = (cos, -sin)
        
        RPTBLOCAL r2_loop1-1        
                ; AC2=Qr[0]:Qi[0] 
                MOV     dbl(*(AR0+T1)), AC2
             || ADD     T1, AR1       
        
                ; AC0 = Pr[0]+Qr[0]:Pi[0]+Qi[0]
                ;     = Pr'[0]:Pi'[0]
                ; CDP = #twiddle
                ADD     dual(*AR2), AC2, AC0 
             || MOV     AR3, CDP             ; #(twiddle+1) has to be loaded in CDP because
                                             ; n=0 is processed outside inner loop 

                ; AC1 = Pr[0]-Qr[0]:Pi[0]-Qi[0]
                ;     = Qr'[0]:Qi'[0]
                ; store P'[0]

                SUB     AC2, dual(*(AR2+T1)), AC1    
             || MOV     AC0, dbl(*(AR4+T1))               
                                                                                                                                                                                        
                RPTBLOCAL r2_loop2 -1

                        ; AC2=Qr[n]*cos                                    
                        ; AC3=Qi[n]*cos
                        MPY     *AR0, *CDP+, AC2               
                     :: MPY     *AR1, *CDP+, AC3                           
        
                        ; AC3=Qi[n]*cos+Qr[n]*sin   
                        ; AC2=Qr[n]*cos-Qi[n]*sin                                           

                        MACR    *(AR0+T1), *CDP+, AC3           ; 
                     :: MASR    *(AR1+T1), *CDP+, AC2   ; 
            
                        ; AC2=(Qr[n]*cos-Qi[n]*sin):(Qi[n]*cos+Qr[n]*sin)

                        OR      AC3 << #-16, AC2    
                        ; hi(AC0)=Pr[n]+(Qr[n]*cos-Qi[n]*sin)=Pr'[n]
                        ; lo(AC0)=Pi[n]+(Qi[n]*cos+Qr[n]*sin)=Pi'[n]
                        ; store Qr'[n-1]:Qi'[n-1]

                        ADD     dual(*AR2), AC2, AC0
                     || MOV     AC1, dbl(*(AR6+T1)) 
    
                        ; hi(AC1)=Pr[n]-(Qr[n]*cos-Qi[n]*sin)=Qr'[n] 
                        ; lo(AC1)=Pi[n]-(Qi[n]*cos+Qr[n]*sin)=Qi'[n]
                        ; store Pr'[n]:Pi'[n]

                        SUB     AC2, dual(*(AR2+T1)), AC1
                     || MOV     AC0, dbl(*(AR4+T1)) 
r2_loop2:                                       ; end of butterfly loop
        
                MOV     AC1, dbl(*(AR6+T1))     ; store last Qr':Qi'
r2_loop1:                                       ; end of group loop
        
        BCC     stage, T0 != #0                 ; end of stage loop               
                       
end_benchmark:

;Context restore 
;
; Deallocate the local frame and argument block
;----------------------------------------------------------------
        AADD    #+(ARG_BLK_SZ + FRAME_SZ), SP
        AND     #~0x01ff, mmap(ST2_55)  ; restore linear AR0, AR1, AR2, AR4 and AR6
        POPBOTH XAR7
     || BSET    #ARMS, ST2_55          
        POPBOTH XAR6
     || BCLR    #FRCT, ST1_55          
        POPBOTH XAR5
     || BCLR    SATD
        POP     T3, T2
        RET 

        .end         
        






