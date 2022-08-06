%% clean up
%clearvars -except datalogger20190101000141 datalogger1
clear all
close all
clc

%% importing data
filename = 'datalogger_20220804_181315.txt';
datalogger1 = importfile(filename);
%datalogger1 = datalogger20190101000141;

%% collecting data information / measurment types
% getting precision value
precision = split(datalogger1(1)); %first line contains precision
precision = str2double(precision(3)); %converting string to double


% getting measurement types and indices 
data_info = split(datalogger1(2), ", "); %splitting after pattern
data_info = data_info(1:end-1); %removing last empty ""

% making str array for table and plot information
buffer = extractBefore(data_info, ":"); %extracting m_types
buffer = erase(buffer, "m_");
buffer = buffer(2:end);

data_info4table(1) = "time in sec";
index = 2;
for c = 1:size(buffer,1)
    data_info4table(index) = buffer(c) + " x";
    index = index + 1;
    data_info4table(index) = buffer(c) + " y";
    index = index + 1;
    data_info4table(index) = buffer(c) + " z";
    index = index + 1;
end

% removing unnecessary variables
clear buffer
clear index
clear c




%% moving measurment data to data_array
disp("moving all values to data_array...");
data_array = datalogger1(5:end); % move all m_values to data_array

disp("erasing all semicolons...");
data_array = erase(data_array, ";"); %erasing ";"

disp("splitting values...");
data_array = split(data_array, ","); %spliting at ","

disp("converting values to double...");
data_array = str2double(data_array); %converting to double values



%% cutting
% cutting off some values from the end
start_cutoff = 1;
end_cutoff = 1;

disp("cutting the off values...");
data_array = data_array(start_cutoff:end - end_cutoff,:); % stop of fog simulation



%% time

disp("changing time to start from zero....");
data_array(:,1) = data_array(:,1) - data_array(1,1); %time start is zero
data_array(:,1) = data_array(:,1)/1000; %time display in seconds



%% data normalization
%data_array(:,2:end) = normalize(data_array(:,2:end), 'range'); %data 0 to 1



%% data apply precision
% disp("applying precision");
% data_array(:,2:end) = data_array(:,2:end) * 10.^precision;


%% converting from array to table with information of measurment type
data_table = array2table(data_array, 'VariableNames',data_info4table);



%% plotting data
plot(data_table, data_info4table(1), data_info4table(2:end))
ylabel("Movement data")
legend
title("not FOG simulation data") % \nbno1 left knee \nbno2 left ankle \nbno3 right knee \nbno4 right ankle")



%% data augmentation / inverting data
% data_array_inverted = data_array; 
% %inverting all values exept the time row
% data_array_inverted(:,2:end) = (-1)*data_array_inverted(:,2:end); 

%hold on 
%plot(data_array_inverted(:,1),data_array_inverted(:,2),'r',data_array(:,1),data_array(:,2),'b')


%% data augmentation / streching data

%% data augmentation / damping 
% damp_factor = 0.9;
% 
% data_array_damped = data_array;
% data_array_damped(:,2:end) = data_array_damped(:,2:end)*damp_factor;
% clear damp_factor



%% data augmentation / damping 
% amp_factor = 1.1;
% 
% data_array_amplified = data_array;
% data_array_amplified(:,2:end) = data_array_amplified(:,2:end)*amp_factor;
% clear amp_factor


%% export data
filename = 'export_nfog_data220804_172020.txt';
writetable(data_table,filename,'Delimiter',',','WriteRowNames',true);
%%type tabledata2.txt




