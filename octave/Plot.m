clc;
clear all;
clf;
data = load('dataY.txt');
x = zeros(size(data));
R = zeros(size(data));
for i = [1 : length(data)]
  x(i) = i;
endfor
plot(x,data,"linewidth", 3);
hold on;
plot(x,R,"linewidth", 3);
grid on;
xlabel ("Muestras");
ylabel ("Error");
title ("Control en Pitch");
h = legend ({"Control PD"}, "Referencia");
legend (h, "location", "northeastoutside");