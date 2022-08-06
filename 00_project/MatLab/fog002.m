%% clear up
clear all; clc; close all;
format bank
%format short

%% ble services and characteristics
% blelist;  %look for bluetooth low energy devices
b = ble("FOG Nano");
c = characteristic(b, "180F", "2A19");

%% daten aufnehmen

%data2 = zeros(1000, 33);
x_limit = 250;

j = 1;
while(1)
    tic % duration feedback start
    value = read(c);
    value = char(value);
    %value = erase(value, ';');
    %data = strsplit(value, ',');
    data = str2num(value);
    data = data(2:end);

    if j == 1
        data2(j,:) = data;
        data3(j,:) = data; %test select selective type
    end

    if size(data,2) == size(data2,2)
        data2(j,:) = data;
        data3(j,:) = data;



        if j <= x_limit
            plot(data2)
            hold on 
            plot(data3(j,2:3))
            plot(data3(j,4:6))
        else
            plot(data2(end - x_limit:end, :))
        end
        % ylim([-100 100])
        xlim([0 x_limit])
        grid on
        drawnow %limitrate
        j = j + 1;
        toc % duration feedback
    end
end