clear 
clf;
fs = 20e3;          % Sample Rate (Hz)

%% System setup
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
n = 8;         % Number of rolling element bearings
d = 0.002;      % Diameter of rolling elements 
p = 0.010;      % Pitch diameter of bearing
thetaDeg = 15; % Contact angle in degrees

bpfi = n*fPin/2*(1 + d/p*cosd(thetaDeg)) % Ballpass frequency, inner race
fImpact = 3000;
tImpact = 0:1/fs:5e-3-1/fs;
xImpact = 0.4*sin(2*pi*fImpact*tImpact);
window  = kaiser(length(tImpact),40);

xImpactWindowed = xImpact.*window';

xComb = zeros(size(t));
xComb(1:round(fs/bpfi):end) = 1;
xBper = 0.33*conv(xComb,xImpactWindowed,'same');

figure(1)
plot(t,xBper)
xlim([0 0.05])
xlabel('Time (s)')
ylabel('Acceleration')
title('Impacts Due to Local Fault on the Inner Race of the Bearing')

vNoBFaultNoisy = vNoFault + randn(size(t))/5;
vBFaultNoisy = xBper + vNoFault + randn(size(t))/5;

figure(2)
plot(t,vBFaultNoisy)

figure(3)
pspectrum([vBFaultNoisy' vNoBFaultNoisy' ],fs,'FrequencyResolution',1,'FrequencyLimits',[0 10*bpfi])
legend('Damaged','Healthy')
title('Bearing Vibration Spectra')
grid off

harmImpact = (0:10)*bpfi;
[X,Y] = meshgrid(harmImpact,ylim);

hold on
plot(X/1000,Y,':k')
legend('Healthy','Damaged','BPFI harmonics')
xlim([0 10*bpfi]/1000)
hold off

figure(4)
pspectrum(vBFaultNoisy',fs,'FrequencyLimits',[0 10*bpfi], 'spectrogram', 'TimeResolution')
