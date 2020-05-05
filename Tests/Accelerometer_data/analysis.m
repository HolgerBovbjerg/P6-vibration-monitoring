%% Clear
clear 
close all;

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
innerRing = readtable("C:\Users\holge\OneDrive - Aalborg Universitet\AAU - Elektronik og IT\6. semester\P6 - bachelor\P6-vibration-monitoring\Tests\Accelerometer_data\innerRing.csv", opts);


%% Clear temporary variables
clear opts

%% Envelope analysis
fs = 48e3;
N = 8192;
data = table2array(innerRing);
t = (0:length(data)-1)*1/fs;

envelope_data = lowpass(abs(highpass(data(1:N), 10000, fs)),1000, fs);
figure
plot(t,data);
figure
plot(t,envelope_data);
figure
plot(abs(fft(highpass(lowpass(abs(highpass(data(1:N), 1000, fs)),1000, fs),50,fs)))/N);


