%% Read data

rawData = csvread('raw3.csv');
HPdata = csvread('hp3.csv');
ABSdata = csvread('abs3.csv');
LPData = csvread('lp3.csv');
FFTData = csvread('fft3.csv');

figure 
tiledlayout(5,1)
nexttile
plot(rawData)
nexttile
plot(HPdata)
nexttile
plot(ABSdata)
nexttile
plot(LPData)
nexttile
plot(FFTData)


%%
rawData = csvread('raw3.csv');
NYQnum = importdata('NYQnum.mat');
LPnum = importdata('LP_coefficients.mat');
HPnum = importdata('HP_coefficients.mat');
%LPnum=round(LPnum*2^15);
data = filter(NYQnum,1,rawData);
%HPnum = round(HPnum*2^15);
[envelope_dataHP, zf] = filter(HPnum,1,data);

envelope_dataABS = abs(envelope_dataHP);

envelope_dataLP = filter(LPnum,1,envelope_dataABS);

envelope_FFT = fft(envelope_dataLP(1:1024));
figure
tiledlayout(5,1)
nexttile
plot(rawData)
nexttile
plot(envelope_dataHP)
nexttile
plot(envelope_dataABS)
nexttile
plot(envelope_dataLP)
nexttile
plot(abs(envelope_FFT(2:824)))

%% HP
t = (1:1024)*(8192/48000)/1024
tl = tiledlayout(1,2)
title(tl,'After HP-filter has been applied')
nexttile
plot(t,envelope_dataHP)
title("MATLAB")
xlabel("Time [s]")
ylabel("Amplitude")
nexttile
plot(t,HPdata)
title("DSP")
xlabel("Time [s]")
ylabel("Amplitude")

saveas(tl,'afterHP.png')

%% LP
t = (1:1024)*(8192/48000)/1024
tl = tiledlayout(1,2)
title(tl,'After LP-filter has been applied')
nexttile
plot(t,envelope_dataLP)
title("MATLAB")
xlabel("Time [s]")
ylabel("Amplitude")
nexttile
plot(t,LPData)
title("DSP")
xlabel("Time [s]")
ylabel("Amplitude")

saveas(tl,'afterLP.png')

%% Raw data
t = (1:1024)*(8192/48000)/1024
raw = plot(t,rawData)
title("Raw data")
xlabel("Time [s]")
ylabel("Amplitude")
saveas(raw,'rawData.png')

%% Abs
t = (1:1024)*(8192/48000)/1024
tl = tiledlayout(1,2)
title(tl,'Absolute values after HP filtering')
nexttile
plot(t,envelope_dataABS)
title("MATLAB")
xlabel("Time [s]")
ylabel("Amplitude")
nexttile
plot(t,ABSdata)
title("DSP")
xlabel("Time [s]")
ylabel("Amplitude")

saveas(tl,'afterABS.png')


%% FFT
t = (1:1023)*(24000/8192)*8
tl = tiledlayout(1,2)
title(tl,'FFT on envelope data')
nexttile
plot(t,abs(envelope_FFT(2:1024)))
title("MATLAB")
xlabel("Frequency [Hz]")
ylabel("Amplitude")
ylim([-10000 10*10^4]);
nexttile
plot(t,FFTData(2:1024))
title("DSP")
xlabel("Frequency [Hz]")
ylabel("Amplitude")
ylim([-10000 10*10^4]);
saveas(tl,'afterFFT.png')


%% FFT
t = (1:100)*(24000/8192)*8
tl = tiledlayout(1,2)
title(tl,'FFT on envelope data - first 100 samples')
nexttile
plot(t,abs(envelope_FFT(2:101)))
title("MATLAB")
xlabel("Frequency [Hz]")
ylabel("Amplitude")
ylim([-10000 10*10^4]);
nexttile
plot(t,FFTData(1:100))
title("DSP")
xlabel("Frequency [Hz]")
ylabel("Amplitude")
ylim([-10000 10*10^4]);
saveas(tl,'zoomedFFT.png')
