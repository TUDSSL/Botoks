function [rc_result, offset_result] = hrt_fit(filename)

%% Initialize variables.
delimiter = ',';
startRow = 2;

formatSpec = '%s%f%f%f%[^\n\r]';

%% Open the text file.
fileID = fopen(filename,'r');

%% Read columns of data according to the format.
dataArray = textscan(fileID, formatSpec, 'Delimiter', delimiter, 'TextType', 'string', 'ReturnOnError', false, 'EndOfLine', '\r\n');

%% Close the text file.
fclose(fileID);

%% Allocate imported array to column variable names
ADC_VAL = dataArray{:, 1};
CONV_TIME = dataArray{:, 2};
TIME = dataArray{:, 4};

%% Clear temporary variables
clearvars filename delimiter startRow formatSpec fileID dataArray ans;

ADC_VAL = hex2dec(ADC_VAL);
ADC_VAL = ADC_VAL * (1.2 / 4095);

%% actual processing
 c = createFit(TIME,ADC_VAL);
 cooeffs = coeffvalues(c);

 rc_result = cooeffs(2);
 offset_result =  cooeffs(3) * (4095 / 1.2);

end

function [fitresult, gof] = createFit(counter, ADC_VAL1)
%% Fit: 'CHRT'.
[xData, yData] = prepareCurveData( counter, ADC_VAL1 );

% Set up fittype and options.
ft = fittype( 'a*exp(-x/b)+c', 'independent', 'x', 'dependent', 'y' );
opts = fitoptions( 'Method', 'NonlinearLeastSquares' );
opts.Display = 'Off';
opts.Lower = [1.0 -Inf];
opts.StartPoint = [0.777390816508421 0.383310947544083 0.662851372314787];
opts.Upper = [1.4 Inf];

% Fit model to data.
[fitresult, gof] = fit( xData, yData, ft, opts );
end
