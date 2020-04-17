clc;
clear all;
clf;
data = load('dataX_PD5.txt');
% data1 = load('dataY_P1.txt');
% data2 = load('dataY_P2.txt');
% data3 = load('dataY_P3.txt');
x = zeros(size(data));
R = zeros(size(data));
for i = [1 : length(data)]
  x(i) = i;
endfor
plot(x,data,"linewidth", 2);
hold on;
% plot(x,data1,"linewidth", 2);
% hold on;
% plot(x,data2,"linewidth", 2);
% hold on;
% plot(x,data3,"linewidth", 2);
% hold on;
plot(x,R,"linewidth", 3);
grid on;
xlabel ("Muestras");
ylabel ("Error");
title ("Control en Pitch");
% h = legend ({"P=3.2"}, "P=2.6", "P=2","P=1.7");
% legend (h, "location", "northeastoutside");
%  set (h, "fontsize", 10);