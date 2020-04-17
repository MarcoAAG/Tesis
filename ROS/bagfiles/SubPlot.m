clc;
clear all;
clf;
data = load('dataX_PD2.txt');
xx = zeros(size(data));
R = zeros(size(data));
for i = [1 : length(data)]
  xx(i) = i;
endfor

%----------------------------------------------------------------
ax(1) = subplot (221);
set (ax(1), "tag", "1");
plot(xx,data,"linewidth", 2);
hold on;
plot(xx,R,"linewidth", 3);
grid on;
xlabel ("Muestras");
ylabel ("Error");
title ("Control en Yaw");
h = legend ({"P=1.3"}, "D=10");
legend (h, "location", "northeastoutside");
set (h, "fontsize", 10);
%----------------------------------------------------------------
data = load('dataX_PD3.txt');
xx = zeros(size(data));
R = zeros(size(data));
for i = [1 : length(data)]
  xx(i) = i;
endfor 
ax(2) = subplot (222);
set (ax(2), "tag", "1");
plot(xx,data,"linewidth", 2);
hold on;
plot(xx,R,"linewidth", 3);
grid on;
xlabel ("Muestras");
ylabel ("Error");
title ("Control en Yaw");
h = legend ({"P=1.3"}, "D=25");
legend (h, "location", "northeastoutside");
set (h, "fontsize", 10);
%----------------------------------------------------------------
data = load('dataX_PD4.txt');
xx = zeros(size(data));
R = zeros(size(data));
for i = [1 : length(data)]
  xx(i) = i;
endfor
ax(3) = subplot (223);
set (ax(3), "tag", "1");
plot(xx,data,"linewidth", 2);
hold on;
plot(xx,R,"linewidth", 3);
grid on;
xlabel ("Muestras");
ylabel ("Error");
title ("Control en Yaw");
h = legend ({"P=1.3"}, "D=35");
legend (h, "location", "northeastoutside");
set (h, "fontsize", 10);
%----------------------------------------------------------------
data = load('dataX_PD5.txt');
xx = zeros(size(data));
R = zeros(size(data));
for i = [1 : length(data)]
  xx(i) = i;
endfor
ax(4) = subplot (224);
set (ax(4), "tag", "1");
plot(xx,data,"linewidth", 2);
hold on;
plot(xx,R,"linewidth", 3);
grid on;
xlabel ("Muestras");
ylabel ("Error");
title ("Control en Yaw");
h = legend ({"P=1.3"}, "D=50.3");
legend (h, "location", "northeastoutside");
set (h, "fontsize", 10);