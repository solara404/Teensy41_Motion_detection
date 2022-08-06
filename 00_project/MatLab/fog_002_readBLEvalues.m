%% clear up
clear all; clc; close all;
format bank
%format short

%% ble services and characteristics
blelist;  %look for bluetooth low energy devices
b = ble("FOG Nano")
c = characteristic(b, "180F", "2A19")
subscribe(c); %makes reading values much faster

%% daten aufnehmen

%data2 = zeros(1000, 33);
x_limit = 100;

j = 1;


while(1)
    tic % for mesuring time
    value = read(c);
    value = char(value);
    %value = erase(value, ';');
    %data = strsplit(value, ',');
    data = str2num(value);
    data = data(2:end);

    if j == 1
        data2(j,:) = data;
    end

    if size(data,2) == size(data2,2)
        data2(j,:) = data;


%% method 1 plot 
        if j <= x_limit
            plot(data2)
        else
            plot(data2(end - x_limit:end, :))
        end

        ylim([-1500 2000])
        xlim([0 x_limit])
        grid on
        drawnow %limitrate

        j = j + 1;
        toc %for mesuring time
    end
end

%unsubscribe(c); 
%clear all