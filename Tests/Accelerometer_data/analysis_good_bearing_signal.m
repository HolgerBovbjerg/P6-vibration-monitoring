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
goodBearing = readtable("goodBearing.csv", opts);

%% Clear temporary variables
clear opts

%% Envelope analysis
fs = 48e3;
data = table2array(goodBearing);
N = length(data);
t = (0:N-1)./fs;
f = fs/N*(0:length(data)-1)';
Nyq = 20e3;
HP = 3000;
LP = 200;
envelope_data = lowpass(abs(highpass(lowpass(data(1:N),Nyq,fs), HP, fs)),LP, fs);
figure
plot(t,data,t,envelope_data);
figure
plot(f,abs(fft(envelope_data))./N);
xlim([0 500])


%%
figure
pspectrum([data envelope_data], fs,'FrequencyResolution', 16, 'FrequencyLimits',[0 1000])