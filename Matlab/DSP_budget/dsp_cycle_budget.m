%% Clear
clear

%% DSP setup
f_clock = 100e6;
Fs = 40e3;

cycles_per_sample = floor(f_clock/Fs);

% Nyquist filter

% High pass

% Demodulation

% Low pass

% FFT
FFT_framelength = 2^10;
cycles_per_frame = cycles_per_sample*FFT_framelength;
frequency_resolution = Fs/FFT_framelength;


% Alarm

% 

%% Cycles used
total_used = FFT_cycles(FFT_framelength);
%% Budget
leftover = cycles_per_frame - total_used;


