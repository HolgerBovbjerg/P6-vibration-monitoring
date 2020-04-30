%% Clear
clear 
clf;


%% System setup
fs = 48e3;          % Sample Rate (Hz)

Np = 13;            % Number of teeth on pinion
Ng = 35;            % Number of teeth on gear

fPin = 30;        % Pinion (Input) shaft frequency (Hz)

fGear = fPin*Np/Ng; % Gear (Output) shaft frequency (Hz)


fMesh = fPin*Np;    % Gear Mesh frequency (Hz)

t = 0:1/fs:20-1/fs;

vfIn = 0.4*sin(2*pi*fPin*t);    % Pinion waveform     
vfOut = 0.2*sin(2*pi*fGear*t);  % Gear waveform
vMesh = sin(2*pi*fMesh*t);      % Gear-mesh waveform

vNoFault = vfIn + vfOut + vMesh;                          
%% Bearing
n = 7;         % Number of rolling element bearings
d = 0.004;      % Diameter of rolling elements 
p = 0.015;      % Pitch diameter of bearing
thetaDeg = 0;

bpfi = n*fPin/2*(1 + d/p*cosd(thetaDeg)); % Ballpass frequency, inner race
bpfo = n*fPin/2*(1 - d/p*cosd(thetaDeg)); % Ballpass frequency, outer race
fImpact = 5000;
tImpact = 0:1/fs:5e-3-1/fs;
xImpact = 0.4*sin(2*pi*fImpact*tImpact);
AImpact = 1;
window  = kaiser(length(tImpact),40);

xImpactWindowed = xImpact.*window';

xComb = zeros(size(t));
xComb(1:round(fs/bpfi):end) = 1;
xBper = AImpact*conv(xComb,xImpactWindowed,'same');
snr = 20;
xBper = awgn(xBper,snr);

%% Envelope
N=2^10;
figure(99)
ax1 = tiledlayout(2,2);
for i = 1:4
    nexttile
    deltaf = fs/N;
    stem((0:N-1)*deltaf,abs(fft(highpass(lowpass(abs(highpass(xBper(1:N), 1000, fs)),1000, fs),50,fs)))/N)
    xlim([0 10*bpfi])
    xlabel('Frequency [Hz]')
    ylabel('Amplitude [g]')
    title([N + "-point FFT of envelope signal"])
    N = N*2;
end
%% Figures 
figure(1)
plot(t,xBper)
xlim([0 0.05])
xlabel('Time (s)')
ylabel('Acceleration')
title('Impacts Due to Local Fault on the Inner Race of the Bearing')

vBNoFaultNoisy = awgn(vNoFault,45);
vBFaultNoisy = awgn(xBper + vNoFault,45);

figure(2)
plot(t,vBFaultNoisy)
xlim([0 0.05])
xlabel('Time (s)')
ylabel('Acceleration')

figure(3)
pspectrum([vBFaultNoisy' vBNoFaultNoisy' ],fs,'FrequencyResolution',1,'FrequencyLimits',[0 10*bpfi])
legend('Damaged','Healthy')
title('Bearing Vibration Spectra')
grid off

%% Envelope 
vBFaultNoisyHP = highpass(vBFaultNoisy,1000,fs);
vBNoFaultNoisyHP = highpass(vBNoFaultNoisy,1000,fs);
vBFaultNoisyAbs = abs(vBFaultNoisyHP);
vBNoFaultNoisyAbs = abs(vBNoFaultNoisyHP);
vBFaultNoisyEnv = lowpass(vBFaultNoisyAbs,500,fs);
vBNoFaultNoisyEnv = lowpass(vBNoFaultNoisyAbs,500,fs);

BPFIharmImpact = (0:10)*bpfi;
BPFOharmImpact = (0:10)*bpfo;
[xBPFI,yBPFI] = meshgrid(BPFIharmImpact,ylim);
[xBPFO,yBPFO] = meshgrid(BPFOharmImpact,ylim);

figure(4)
ax1 = tiledlayout(4,1);
nexttile
plot(t,vBFaultNoisy,t,vBNoFaultNoisy)
xlim([0 0.05])
nexttile
plot(t,vBFaultNoisyHP,t,vBNoFaultNoisyHP)
xlim([0 0.05])
nexttile
plot(t,vBFaultNoisyAbs,t,vBNoFaultNoisyAbs)
xlim([0 0.05])
nexttile
plot(t,vBFaultNoisyEnv,t,vBNoFaultNoisyEnv)
xlim([0 0.05])

figure(5)
ax1 = tiledlayout(4,1);
nexttile
pspectrum([vBFaultNoisy' vBNoFaultNoisy'], fs,'FrequencyResolution',1)
nexttile
pspectrum([vBFaultNoisyHP' vBNoFaultNoisyHP'], fs,'FrequencyResolution',1)
nexttile
pspectrum([vBFaultNoisyAbs' vBNoFaultNoisyAbs'], fs,'FrequencyResolution',1)
nexttile
pspectrum([vBFaultNoisyEnv' vBNoFaultNoisyEnv'], fs,'FrequencyResolution',1,'FrequencyLimits',[0 10*bpfi])
hold on 
plot(xBPFI*1e-3,yBPFI,':g')
plot(xBPFO*1e-3,yBPFO,':r')
legend('Damaged','Healthy','BPFI harmonics','BPFO harmonics')