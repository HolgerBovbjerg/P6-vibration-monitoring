%% Clear
clear

%% Filter design specs
samplingFrequency = 48e3;
centerFrequency = 21e3;
transitionWidth = 3e3; 
passbandRipple = 1; % passband ripple in dB
stopbandAttenuation = 45; % attenuation in dB

designSpec = fdesign.lowpass('Fp,Fst,Ap,Ast',...
    centerFrequency-transitionWidth/2, ...
    centerFrequency+transitionWidth/2, ...
    passbandRipple,stopbandAttenuation, ...
    samplingFrequency);
LPF = design(designSpec,'equiripple','SystemObject',true)

fvtool(LPF) % Visualise filter

%% Fixed point parameters
rng default
inputWordLength = 16;
fixedPointInput = fi(randn(100,1),true,inputWordLength);
floatingPointInput = double(fixedPointInput);
floatingPointOutput = LPF(floatingPointInput);

release(LPF)
fullPrecisionOutput = LPF(fixedPointInput);
norm(floatingPointOutput-double(fullPrecisionOutput),'inf')

fvtool(LPF)

measure(LPF)
