function Hd = filter_design
%FILTER_DESIGN Returns a discrete-time filter object.

% MATLAB Code
% Generated by MATLAB(R) 9.8 and DSP System Toolbox 9.10.
% Generated on: 07-May-2020 10:36:55

% FIR Window Highpass filter designed using the FIR1 function.

% All frequency values are in Hz.
Fs = 48000;  % Sampling Frequency

N    = 120;      % Order
Fc   = 1000;     % Cutoff Frequency
flag = 'scale';  % Sampling Flag

% Create the window vector for the design algorithm.
win = blackman(N+1);

% Calculate the coefficients using the FIR1 function.
b  = fir1(N, Fc/(Fs/2), 'high', win, flag); % Create filter coeffeicients
Hd = dfilt.dffir(b); % design filter
% Set the arithmetic property.
set(Hd, 'Arithmetic', 'fixed', ...
    'CoeffWordLength', 16, ...
    'CoeffAutoScale', true, ...
    'Signed',         true, ...
    'InputWordLength', 16, ...
    'inputFracLength', 15, ...
    'FilterInternals',  'FullPrecision');
denormalize(Hd);


% [EOF]
