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
thetaDeg = (0:89); % Contact angle in degrees

bpfi = fShaft'*(n/2*(1 + d/p*cosd(thetaDeg))); % Ballpass frequency, inner race
bpfo = fShaft'*(n/2*(1-d/p*cosd(thetaDeg))); % Ballpass frequency, outer race
bsf = fShaft'*(p/d*(1 - (d/p*cosd(thetaDeg)).^2)); % Ball spin frequency
ftf = fShaft'*(1/2*(1-d/p*cosd(thetaDeg))); % Fundamental train frequency

fig1 = figure(1)
ax = tiledlayout(1,2);
nexttile
plot(fShaft, bpfi(:,1), fShaft, bpfo(:,1), fShaft, bsf(:,1), fShaft, ftf(:,1));
grid on
legend('BPFI', 'BPFO', 'BSF', 'FTF', 'Location', 'Best');
xlabel('Shaft Frequency [Hz]');
ylabel('Characteristic Frequency [Hz]');
title(['0' char(176) ' contact angle'])
nexttile
plot(fShaft, bpfi(:,45), fShaft, bpfo(:,45), fShaft, bsf(:,45), fShaft, ftf(:,45));
grid on
title(['45' char(176) ' contact angle'])
legend('BPFI', 'BPFO', 'BSF', 'FTF', 'Location', 'Best');
xlabel('Shaft Frequency [Hz]');
ylabel('Characteristic Frequency [Hz]');

exportgraphics(fig1,'bearing_characteristic_frequencies.pdf','ContentType','vector')

figure(2)
surf(thetaDeg,fShaft,bpfi,'EdgeColor', 'none', 'FaceColor', 'r')
hold on
surf(thetaDeg,fShaft,bpfo,'EdgeColor', 'none', 'FaceColor', 'g')
surf(thetaDeg,fShaft,ftf,'EdgeColor', 'none', 'FaceColor', 'b')
surf(thetaDeg,fShaft,bsf,'EdgeColor', 'none', 'FaceColor', 'y')
    
xlabel(['Angle [' char(176) ']'])
ylabel('Shaft frequency [Hz]')
zlabel('Characteristic frequency [Hz]')
legend('BPFI','BPFO','FTF','BSF')



