clc;
clear all;
clf;
data = load('data9.txt');
x = zeros(size(data));
R = zeros(size(data));
for i = [1 : length(data)]
  x(i) = i;
endfor
plot(x,data,"linewidth", 2);
hold on;
plot(x,R,"linewidth", 3);
grid on;
xlabel ("Muestras");
ylabel ("Error");
title ("Control en Pitch");
h = legend ({"Control PI"}, "Referencia");
legend (h, "location", "northeastoutside");