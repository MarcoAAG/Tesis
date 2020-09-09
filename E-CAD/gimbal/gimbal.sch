EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Microchip_ATmega:ATmega328-AU U2
U 1 1 5F579F3D
P 1950 3850
F 0 "U2" H 1950 2261 50  0000 C CNN
F 1 "ATmega328-AU" H 1950 2170 50  0000 C CNN
F 2 "Package_QFP:TQFP-32_7x7mm_P0.8mm" H 1950 3850 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega328_P%20AVR%20MCU%20with%20picoPower%20Technology%20Data%20Sheet%2040001984A.pdf" H 1950 3850 50  0001 C CNN
	1    1950 3850
	1    0    0    -1  
$EndComp
$Comp
L power:+5C #PWR?
U 1 1 5F58815B
P 1450 1850
F 0 "#PWR?" H 1450 1700 50  0001 C CNN
F 1 "+5C" H 1465 2023 50  0000 C CNN
F 2 "" H 1450 1850 50  0001 C CNN
F 3 "" H 1450 1850 50  0001 C CNN
	1    1450 1850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F588945
P 1700 1850
F 0 "#PWR?" H 1700 1600 50  0001 C CNN
F 1 "GND" H 1705 1677 50  0000 C CNN
F 2 "" H 1700 1850 50  0001 C CNN
F 3 "" H 1700 1850 50  0001 C CNN
	1    1700 1850
	-1   0    0    1   
$EndComp
Wire Wire Line
	1450 2350 1950 2350
Wire Wire Line
	1700 2250 1300 2250
Wire Wire Line
	1300 2250 1300 5350
Wire Wire Line
	1300 5350 1950 5350
Wire Wire Line
	1700 1850 1700 2250
Connection ~ 1700 2250
$Comp
L power:+12V #PWR?
U 1 1 5F59FB84
P 10900 4600
F 0 "#PWR?" H 10900 4450 50  0001 C CNN
F 1 "+12V" H 10915 4773 50  0000 C CNN
F 2 "" H 10900 4600 50  0001 C CNN
F 3 "" H 10900 4600 50  0001 C CNN
	1    10900 4600
	1    0    0    -1  
$EndComp
$Comp
L Motor:Motor_Servo M1
U 1 1 5F5A7F1C
P 3750 1750
F 0 "M1" H 4082 1815 50  0000 L CNN
F 1 "Motor_Servo" H 4082 1724 50  0000 L CNN
F 2 "" H 3750 1560 50  0001 C CNN
F 3 "http://forums.parallax.com/uploads/attachments/46831/74481.png" H 3750 1560 50  0001 C CNN
	1    3750 1750
	1    0    0    -1  
$EndComp
$Comp
L Motor:Motor_Servo M2
U 1 1 5F5A9169
P 3750 2250
F 0 "M2" H 4082 2315 50  0000 L CNN
F 1 "Motor_Servo" H 4082 2224 50  0000 L CNN
F 2 "" H 3750 2060 50  0001 C CNN
F 3 "http://forums.parallax.com/uploads/attachments/46831/74481.png" H 3750 2060 50  0001 C CNN
	1    3750 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 1850 1450 2000
Wire Wire Line
	2550 4350 2700 4350
Wire Wire Line
	2700 4350 2700 1650
Wire Wire Line
	2700 1650 3450 1650
Wire Wire Line
	3450 2150 2750 2150
Wire Wire Line
	2750 2150 2750 4450
Wire Wire Line
	2750 4450 2550 4450
Wire Wire Line
	3450 2250 3300 2250
Wire Wire Line
	3300 2250 3300 2000
Wire Wire Line
	3300 1750 3450 1750
Wire Wire Line
	3450 2350 3350 2350
Wire Wire Line
	3350 2350 3350 1850
Wire Wire Line
	3350 1850 3450 1850
Wire Wire Line
	2600 2250 2600 2350
Wire Wire Line
	2600 2350 3350 2350
Connection ~ 3350 2350
Wire Wire Line
	1450 2000 3300 2000
Connection ~ 1450 2000
Wire Wire Line
	1450 2000 1450 2350
Connection ~ 3300 2000
Wire Wire Line
	3300 2000 3300 1750
$Comp
L MCU_Module:RaspberryPi-CM3+L U1
U 1 1 5F5B75D8
P 7150 4850
F 0 "U1" V 7104 1507 50  0000 R CNN
F 1 "Odroid" V 7195 1507 50  0000 R CNN
F 2 "" H 6550 8250 50  0001 C CNN
F 3 "https://www.raspberrypi.org/documentation/hardware/computemodule/datasheets/rpi_DATA_CM3plus_1p0.pdf" H 6550 8250 50  0001 C CNN
	1    7150 4850
	0    1    1    0   
$EndComp
Wire Wire Line
	1700 2250 2600 2250
Wire Wire Line
	10900 4600 10900 4750
Wire Wire Line
	10900 4750 10450 4750
$Comp
L power:GND #PWR?
U 1 1 5F5F3CEA
P 3850 5150
F 0 "#PWR?" H 3850 4900 50  0001 C CNN
F 1 "GND" H 3855 4977 50  0000 C CNN
F 2 "" H 3850 5150 50  0001 C CNN
F 3 "" H 3850 5150 50  0001 C CNN
	1    3850 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 5150 3850 4850
Wire Bus Line
	8850 3850 8850 1550
Wire Bus Line
	8850 1550 2050 1550
Wire Bus Line
	2050 1550 2050 2350
$Comp
L Connector_Generic:Conn_01x01 C1
U 1 1 5F602BCF
P 9450 3350
F 0 "C1" H 9530 3392 50  0000 L CNN
F 1 "OCAM" H 9530 3301 50  0000 L CNN
F 2 "" H 9450 3350 50  0001 C CNN
F 3 "~" H 9450 3350 50  0001 C CNN
	1    9450 3350
	1    0    0    -1  
$EndComp
Wire Bus Line
	8950 3850 8950 3350
Wire Bus Line
	8950 3350 9250 3350
Text Notes 7350 7500 0    50   ~ 0
Gimbal e-cad
Text Notes 8150 7650 0    50   ~ 0
01/08/2020
$EndSCHEMATC
