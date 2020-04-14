%% Clear
clear

%% DSP setup
f_clock = 2e6;
Fs = 48e3;

cycles_per_sample = floor(f_clock/Fs);

% FIR = inputsize * (3 + no_of_filter_coeffs/2)
nhFIR1 = 6;
% Sample by sample
nxFIR1 = 1;
FIR1_cycles = nxFIR1 * (3 + nhFIR1/2);



% High pass

% Demodulation

% Low pass

% FFT
FFT_framelength = 2^10;
cycles_per_frame = cycles_per_sample*FFT_framelength;
frequency_resolution = Fs/FFT_framelength;

FFT_total = FFT_cycles(FFT_framelength);
% Alarm

% 

%% Cycles used
total_used = FFT_total + FIR1_cycles*FFT_framelength;
%% Budget
leftover = cycles_per_frame - total_used;


