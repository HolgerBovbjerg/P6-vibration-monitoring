%% Clear
clear 

%% Setup the Import Options and import the data
opts = delimitedTextImportOptions("NumVariables", 1);

% Specify range and delimiter
opts.DataLines = [1, Inf];
opts.Delimiter = " ";

% Specify column names and types
opts.VariableNames = "VarName1";
opts.VariableTypes = "double";

% Specify file level properties
opts.ExtraColumnsRule = "ignore";
opts.EmptyLineRule = "read";
opts.ConsecutiveDelimitersRule = "join";
opts.LeadingDelimitersRule = "ignore";

% Import the data
innerRing = readtable("innerRing.csv", opts);

%% Clear temporary variables
clear opts

LPnum = importdata('LPnum.mat');
LPnum = bitsll(LPnum,15);


%% Envelope analysis
fs = 48e3;
data = table2array(innerRing);
N = length(data);
t = (0:N-1)*1/fs;
f = fs/N*(0:length(data)-1)';
Nyq = 2e3;
HP = 500;
LP = 500;
envelope_dataLP = lowpass(data,Nyq,fs);
envelope_dataHP = highpass(envelope_dataLP, HP, fs);
envelope_dataABS = abs(envelope_dataHP);
envelope_data = filter(LPnum,1,envelope_dataABS);
envelope_data = bitsra(envelope_data,15);
%% Spectrum
figure
pspectrum([data envelope_data], fs,'FrequencyResolution', 16, 'FrequencyLimits',[0 10*LP])

%% Figures
figure
plot(t,data,t,envelope_data);
figure
plot(f,abs(fft(envelope_data))./N);
xlim([0 500])

