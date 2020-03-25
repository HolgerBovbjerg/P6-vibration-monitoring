%% Clear
clear
close all
%% Import data
dataInner = load(fullfile('MFPT Fault Data Sets\4 - Seven Inner Race Fault Conditions\InnerRaceFault_vload_7.mat'));

% Data from faulty inner bearing
xInner = dataInner.bearing.gs;
% Sampling rate
Fs = dataInner.bearing.sr;
% Time vector
t = (0:length(xInner)-1)/Fs;

xInner = xInner + awgn(20*sin(2*pi*20*t'),25,'measured');

%% Plot raw data 
ax1 = tiledlayout(2,1);
figure(1)
nexttile
% Time domain
plot(t, xInner)
xlabel('Time, (s)')
ylabel('Acceleration (g)')
title('Raw Signal: Inner Race Fault')
xlim([0 0.1])

nexttile
% Frequency domain
[pInner,fpInner ] = pspectrum(xInner, Fs);
plot(fpInner, pow2db(pInner))
xlabel('Frequency [Hz]')
ylabel('Power Spectrum [dB]')
title('Raw Signal: Inner Race Fault')
legend('Power Spectrum')

%% Plot power sepctrum and BPFI comblines
% Ball pass frequency found from time series data
BPFI = 118.875; % [Hz]  

figure(2)
plot(fpInner, pow2db(pInner))
ncomb = 10; % Number of comblines
helperPlotCombs(ncomb, BPFI) % Matlab function to plot comblines
xlabel('Frequency [Hz]')
ylabel('Power Spectrum [dB]')
title('Raw Signal: Inner Race Fault')
legend('Power Spectrum', 'BPFI Harmonics')
xlim([0 1000])

%% High pass filter data
xInnerHPfiltered = highpass(xInner,1000,Fs);
figure(3)
ax2 = tiledlayout(2,1);
nexttile
plot(t,xInnerHPfiltered)
xlabel('Time, (s)')
ylabel('Acceleration (g)')
title('High pass filtered Signal: Inner Race Fault')
xlim([0 0.1])

nexttile
[pInnerHP,fpInnerHP ] = pspectrum(xInnerHPfiltered, Fs);
plot(fpInnerHP, pow2db(pInnerHP))
xlabel('Frequency [Hz]')
ylabel('Power Spectrum [dB]')
title('High pass filtered Signal: Inner Race Fault')
legend('Power Spectrum')
xlim([100 20000])

%% Demodulate
xInnerDemod = abs(xInnerHPfiltered);

figure(4)
ax3 = tiledlayout(2,1);
nexttile
plot(t,xInnerDemod)
xlabel('Time, (s)')
ylabel('Acceleration (g)')
title('Demodulated Signal: Inner Race Fault')
xlim([0 0.1])

nexttile
[pInnerDemod,fpInnerDemod ] = pspectrum(xInnerDemod, Fs);
plot(fpInnerDemod, pow2db(pInnerDemod))
helperPlotCombs(ncomb, BPFI) % Matlab function to plot comblines
xlabel('Frequency [Hz]')
ylabel('Power Spectrum [dB]')
title('Demodulated Signal: Inner Race Fault')
legend('Power Spectrum')
xlim([0 8000])

%% Low pass filter data
xInnerLPfiltered = lowpass(xInnerDemod,1000,Fs);
figure(5)
ax4 = tiledlayout(2,1);
nexttile
plot(t,xInnerLPfiltered)
xlabel('Time, (s)')
ylabel('Acceleration (g)')
title('Low pass filtered Signal: Inner Race Fault')
xlim([0 0.1])

nexttile
[pInnerLP,fpInnerLP ] = pspectrum(xInnerLPfiltered, Fs);
plot(fpInnerLP, pow2db(pInnerLP))
helperPlotCombs(ncomb, BPFI) % Matlab function to plot comblines
xlabel('Frequency [Hz]')
ylabel('Power Spectrum [dB]')
title('Low pass filtered Signal: Inner Race Fault')
legend('Power Spectrum')
xlim([0 1400])
ylim([-30 5])

%% Combined plot
figure(6)
ax5 = tiledlayout(4,2);

% Raw
nexttile
plot(t, xInner)
xlabel('Time, (s)')
ylabel('Acceleration (g)')
title('Raw Signal: Inner Race Fault')
xlim([0 0.1])

nexttile
% Frequency domain
[pInner,fpInner ] = pspectrum(xInner, Fs);
plot(fpInner, pow2db(pInner))
xlabel('Frequency [Hz]')
ylabel('Power Spectrum [dB]')
title('Raw Signal: Inner Race Fault')
legend('Power Spectrum')

% HP filtered
nexttile
plot(t,xInnerHPfiltered)
xlabel('Time, (s)')
ylabel('Acceleration (g)')
title('High pass filtered Signal: Inner Race Fault')
xlim([0 0.1])

nexttile
[pInnerHP,fpInnerHP ] = pspectrum(xInnerHPfiltered, Fs);
plot(fpInnerHP, pow2db(pInnerHP))
xlabel('Frequency [Hz]')
ylabel('Power Spectrum [dB]')
title('High pass filtered Signal: Inner Race Fault')
legend('Power Spectrum')
xlim([100 20000])

% Demodulated
nexttile
plot(t,xInnerDemod)
xlabel('Time, (s)')
ylabel('Acceleration (g)')
title('Demodulated Signal: Inner Race Fault')
xlim([0 0.1])

nexttile
[pInnerDemod,fpInnerDemod ] = pspectrum(xInnerDemod, Fs);
plot(fpInnerDemod, pow2db(pInnerDemod))
helperPlotCombs(ncomb, BPFI) % Matlab function to plot comblines
xlabel('Frequency [Hz]')
ylabel('Power Spectrum [dB]')
title('Demodulated Signal: Inner Race Fault')
legend('Power Spectrum')
xlim([0 8000])

% LP filtered
nexttile
plot(t,xInnerLPfiltered)
xlabel('Time, (s)')
ylabel('Acceleration (g)')
title('Low pass filtered Signal: Inner Race Fault')
xlim([0 0.1])

nexttile
[pInnerLP,fpInnerLP ] = pspectrum(xInnerLPfiltered, Fs);
plot(fpInnerLP, pow2db(pInnerLP))
helperPlotCombs(ncomb, BPFI) % Matlab function to plot comblines
xlabel('Frequency [Hz]')
ylabel('Power Spectrum [dB]')
title('Low pass filtered Signal: Inner Race Fault')
legend('Power Spectrum')
xlim([0 1400])
ylim([-30 5])


%% Spectrogram of envelope
% Highpass to remove DC-bias
[psp,fsp,tsp] = pspectrum(highpass(xInnerLPfiltered,50,Fs), Fs,'spectrogram',...
    'FrequencyResolution', 12,'FrequencyLimits',[0 600]);
figure(7)
waterfall(fsp,tsp,psp')
title('Envelope spectrogram')
xlabel('Frequency (Hz)')
ylabel('Time (seconds)')
view([30 45])