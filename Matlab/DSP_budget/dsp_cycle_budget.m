%% Clear
clear

%% DSP setup
f_clock = 100e6;
Fs = 0.4e3;
framelength = 2^10;
cycles_per_sample = floor(f_clock/Fs);
cycles_per_frame = cycles_per_sample*framelength;
frequency_resolution = Fs/framelength;


% FIR = inputsize * (3 + no_of_filter_coeffs/2)
nhFIR1 = 6;
% Sample by sample
nxFIR1 = 1;
FIR1_cycles = nxFIR1 * (3 + nhFIR1/2);

% Power
nxPower = 1024;
Power_cycles = nxPower-1+12;
% Low pass

% FFT
FFT_total = FFT_cycles(framelength);
% Alarm

% 

%% Cycles used
total_used = FFT_total + FIR1_cycles*framelength...
    + Power_cycles;
%% Budget
leftover = cycles_per_frame - total_used;


