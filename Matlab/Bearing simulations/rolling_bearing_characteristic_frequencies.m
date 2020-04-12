%% clear
clear
clear figures
%% Setup
Fs = 48e3;
fShaft = (0:3700)/60;        % Pinion bearing shaft frequency (Hz)
t = 0:1/Fs:20-1/Fs;

%% Bearing
n = 8;         % Number of rolling element bearings
d = 0.002;      % Diameter of rolling elements 
p = 0.010;      % Pitch diameter of bearing
thetaDeg = 45; % Contact angle in degrees

bpfi = fShaft.*(n/2*(1 + d/p*cosd(thetaDeg))); % Ballpass frequency, inner race
bpfo = fShaft.*(n/2*(1-d/p*cosd(thetaDeg))); % Ballpass frequency, outer race
bsf = fShaft.*(p/d*(1 - (d/p*cosd(thetaDeg))^2)); % Ball spin frequency
ftf = fShaft.*(1/2*(1-d/p*cosd(thetaDeg))); % Fundamental train frequency

plot(fShaft, bpfi, fShaft, bpfo, fShaft, bsf, fShaft, ftf);
grid on
legend('BPFI', 'BPFO', 'BSF', 'FTF', 'Location', 'Best');
xlabel('Shaft Frequency [Hz]');
ylabel('Characteristic Frequency [Hz]');

