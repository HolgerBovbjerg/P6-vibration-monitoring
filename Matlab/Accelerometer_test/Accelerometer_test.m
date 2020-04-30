%% Clear
clear 
clf;


%% System setup
fs = 48e3;          % Sample Rate (Hz)
 
%% Bearing
n = 7;         % Number of rolling element bearings
d = 0.004;      % Diameter of rolling elements 
p = 0.015;      % Pitch diameter of bearing
thetaDeg = 0;

fPin = 30;

bpfi = n*fPin/2*(1 + d/p*cosd(thetaDeg)); % Ballpass frequency, inner race
bpfo = n*fPin/2*(1 - d/p*cosd(thetaDeg)); % Ballpass frequency, outer race

load('accelerometer_data_2048_inner_race.mat');

%% Envelope
N=length(xBper);
envelope=lowpass(abs(highpass(xBper, 1000, fs)),1000, fs);

%% Figures 
figure(1)
% ax1 = tiledlayout(2,2);
% for i = 1:2
%     nexttile
    deltaf = fs/N;
    stem((0:N-1)*deltaf,abs(fft(envelope)/N))
    xlim([0 10*bpfi])
    xlabel('Frequency [Hz]')
    ylabel('Amplitude [g]')
    title([N + "-point FFT of envelope signal"])
%     N = N*2;
% end
