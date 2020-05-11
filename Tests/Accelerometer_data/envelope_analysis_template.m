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


NYQnum = importdata('NYQnum.mat');
LPnum = importdata('LPnum.mat');
HPnum = importdata('HPnum.mat');

data = table2array(innerRing);
N = length(data);
fs = 48e3;
t = (0:N-1)*1/fs;
f = fs/N*(0:length(data)-1)';

Nyq = 20e3;
HP = 1000;
LP = 1000;

% envelope_dataLP = lowpass(data, Nyq, fs);
% envelope_dataHP = highpass(envelope_dataLP, HP, fs);
% envelope_dataABS = abs(envelope_dataHP);
% envelope_data = lowpass(envelope_dataABS, LP, fs);


data = filter(NYQnum,1,data);

envelope_dataHP = filter(HPnum,1,data);

envelope_dataABS = abs(envelope_dataHP);

envelope_data = filter(LPnum,1,envelope_dataABS);

%plot(envelope_data)
pspectrum(envelope_data, fs,'FrequencyResolution', 16, 'FrequencyLimits',[0 LP])
