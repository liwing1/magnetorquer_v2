EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text GLabel 1400 1725 0    50   Input ~ 0
ACCEL_MISO
Wire Wire Line
	1700 1725 1400 1725
Text GLabel 1400 1825 0    50   Input ~ 0
ACCEL_MOSI
Wire Wire Line
	1700 1825 1400 1825
Text GLabel 1400 1525 0    50   Input ~ 0
ACCEL_SCK
Wire Wire Line
	1700 1525 1400 1525
Text GLabel 1400 1625 0    50   Input ~ 0
ACCEL_SCS
Wire Wire Line
	1700 1625 1400 1625
$Comp
L power:GND #PWR0101
U 1 1 617A96E2
P 4200 2100
F 0 "#PWR0101" H 4200 1850 50  0001 C CNN
F 1 "GND" H 4205 1927 50  0000 C CNN
F 2 "" H 4200 2100 50  0001 C CNN
F 3 "" H 4200 2100 50  0001 C CNN
	1    4200 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 2025 4200 2025
Wire Wire Line
	4200 2025 4200 2100
$Comp
L power:GND #PWR0110
U 1 1 617AA90C
P 1600 2100
F 0 "#PWR0110" H 1600 1850 50  0001 C CNN
F 1 "GND" H 1605 1927 50  0000 C CNN
F 2 "" H 1600 2100 50  0001 C CNN
F 3 "" H 1600 2100 50  0001 C CNN
	1    1600 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 2025 1600 2100
Wire Wire Line
	1700 2025 1600 2025
Wire Wire Line
	4100 1925 4200 1925
Wire Wire Line
	4200 1925 4200 2025
Connection ~ 4200 2025
$Comp
L power:+3V3 #PWR0115
U 1 1 617AB4C6
P 4850 1650
F 0 "#PWR0115" H 4850 1500 50  0001 C CNN
F 1 "+3V3" H 4865 1823 50  0000 C CNN
F 2 "" H 4850 1650 50  0001 C CNN
F 3 "" H 4850 1650 50  0001 C CNN
	1    4850 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 1725 4850 1725
Wire Wire Line
	4100 1825 4850 1825
Wire Wire Line
	4850 1825 4850 1725
Connection ~ 4850 1725
Text GLabel 4200 1525 2    50   Input ~ 0
ACCEL_INT1
Wire Wire Line
	4850 1650 4850 1725
Wire Wire Line
	4200 1525 4100 1525
Text GLabel 4200 1625 2    50   Input ~ 0
ACCEL_INT2
Wire Wire Line
	4200 1625 4100 1625
$Comp
L accel:LIS2DH12TR U2
U 1 1 617B4502
P 1700 1525
F 0 "U2" H 2900 1912 60  0000 C CNN
F 1 "LIS2DH12TR" H 2900 1806 60  0000 C CNN
F 2 "LGA-12_STM" H 2900 1765 60  0001 C CNN
F 3 "" H 1700 1525 60  0000 C CNN
	1    1700 1525
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 1925 1600 1925
Wire Wire Line
	1600 1925 1600 2025
Connection ~ 1600 2025
Wire Notes Line
	750  2400 5075 2400
Wire Notes Line
	5075 2400 5075 1025
Wire Notes Line
	5075 1025 750  1025
Wire Notes Line
	750  1025 750  2400
$Comp
L gyros:L3GD20HTR U3
U 1 1 617B8687
P 1850 3875
F 0 "U3" H 2850 4262 60  0000 C CNN
F 1 "L3GD20HTR" H 2850 4156 60  0000 C CNN
F 2 "LGA-16_3X3X1_STM" H 2850 4115 60  0001 C CNN
F 3 "" H 1850 3875 60  0000 C CNN
	1    1850 3875
	1    0    0    -1  
$EndComp
Text Notes 2650 1000 0    50   ~ 0
Acelerômetro
Text GLabel 1750 4175 0    50   Input ~ 0
GYRO_MISO
Wire Wire Line
	1750 4175 1850 4175
Text GLabel 1750 4075 0    50   Input ~ 0
GYRO_MOSI
Wire Wire Line
	1750 4075 1850 4075
Text GLabel 1750 4275 0    50   Input ~ 0
GYRO_SCS
Wire Wire Line
	1750 4275 1850 4275
Text GLabel 1750 3975 0    50   Input ~ 0
GYRO_SCK
Wire Wire Line
	1750 3975 1850 3975
Wire Wire Line
	3850 4575 3950 4575
Wire Wire Line
	3850 4475 3950 4475
Connection ~ 3950 4575
Wire Wire Line
	3850 4175 3950 4175
Wire Wire Line
	3950 4175 3950 4275
Connection ~ 3950 4475
Wire Wire Line
	3950 4475 3950 4575
Wire Wire Line
	3850 4275 3950 4275
Connection ~ 3950 4275
Wire Wire Line
	3950 4275 3950 4375
Wire Wire Line
	3850 4375 3950 4375
Connection ~ 3950 4375
Wire Wire Line
	3950 4375 3950 4475
Wire Wire Line
	3950 3875 3850 3875
Wire Wire Line
	3850 3975 3950 3975
Wire Wire Line
	3950 3975 3950 3875
$Comp
L Device:C C12
U 1 1 617C9EAA
P 4175 4275
F 0 "C12" H 4290 4321 50  0000 L CNN
F 1 "10n" H 4290 4230 50  0000 L CNN
F 2 "" H 4213 4125 50  0001 C CNN
F 3 "~" H 4175 4275 50  0001 C CNN
	1    4175 4275
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 4075 4175 4075
Wire Wire Line
	4175 4075 4175 4125
Wire Wire Line
	4175 4425 4175 4575
Wire Wire Line
	4175 4575 3950 4575
Text GLabel 1750 4375 0    50   Input ~ 0
GYRO_INT1
Wire Wire Line
	1750 4375 1850 4375
Text GLabel 1750 4475 0    50   Input ~ 0
GYRO_INT2
Wire Wire Line
	1750 4475 1850 4475
$Comp
L Device:C C11
U 1 1 617CF487
P 975 4175
F 0 "C11" H 1090 4221 50  0000 L CNN
F 1 "100n" H 1090 4130 50  0000 L CNN
F 2 "" H 1013 4025 50  0001 C CNN
F 3 "~" H 975 4175 50  0001 C CNN
	1    975  4175
	1    0    0    -1  
$EndComp
Wire Wire Line
	975  3875 975  4025
Wire Wire Line
	975  3875 1850 3875
$Comp
L power:GND #PWR0116
U 1 1 617D0435
P 975 4475
F 0 "#PWR0116" H 975 4225 50  0001 C CNN
F 1 "GND" H 980 4302 50  0000 C CNN
F 2 "" H 975 4475 50  0001 C CNN
F 3 "" H 975 4475 50  0001 C CNN
	1    975  4475
	1    0    0    -1  
$EndComp
Wire Wire Line
	975  4475 975  4325
$Comp
L power:+3V3 #PWR0117
U 1 1 617D148F
P 975 3800
F 0 "#PWR0117" H 975 3650 50  0001 C CNN
F 1 "+3V3" H 990 3973 50  0000 C CNN
F 2 "" H 975 3800 50  0001 C CNN
F 3 "" H 975 3800 50  0001 C CNN
	1    975  3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	975  3800 975  3875
Connection ~ 975  3875
$Comp
L Device:C C13
U 1 1 617D4806
P 4550 4275
F 0 "C13" H 4665 4321 50  0000 L CNN
F 1 "100n" H 4665 4230 50  0000 L CNN
F 2 "" H 4588 4125 50  0001 C CNN
F 3 "~" H 4550 4275 50  0001 C CNN
	1    4550 4275
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 3875 4950 3875
$Comp
L power:GND #PWR0118
U 1 1 617D480E
P 4950 4650
F 0 "#PWR0118" H 4950 4400 50  0001 C CNN
F 1 "GND" H 4955 4477 50  0000 C CNN
F 2 "" H 4950 4650 50  0001 C CNN
F 3 "" H 4950 4650 50  0001 C CNN
	1    4950 4650
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR0119
U 1 1 617D4815
P 5050 3825
F 0 "#PWR0119" H 5050 3675 50  0001 C CNN
F 1 "+3V3" H 5065 3998 50  0000 C CNN
F 2 "" H 5050 3825 50  0001 C CNN
F 3 "" H 5050 3825 50  0001 C CNN
	1    5050 3825
	1    0    0    -1  
$EndComp
Connection ~ 4550 3875
Wire Wire Line
	4550 3875 3950 3875
Wire Wire Line
	4550 3875 4550 4125
Connection ~ 3950 3875
$Comp
L Device:C C14
U 1 1 617D9B43
P 4950 4275
F 0 "C14" H 5065 4321 50  0000 L CNN
F 1 "10n" H 5065 4230 50  0000 L CNN
F 2 "" H 4988 4125 50  0001 C CNN
F 3 "~" H 4950 4275 50  0001 C CNN
	1    4950 4275
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 4125 4950 3875
Connection ~ 4950 3875
Wire Wire Line
	4950 3875 5050 3875
Wire Wire Line
	4950 4425 4950 4575
Wire Wire Line
	4950 4575 4550 4575
Wire Wire Line
	4550 4425 4550 4575
Connection ~ 4550 4575
Wire Wire Line
	4950 4575 4950 4650
Wire Wire Line
	4550 4575 4175 4575
Connection ~ 4175 4575
Wire Wire Line
	5050 3825 5050 3875
Connection ~ 4950 4575
Text GLabel 1750 4575 0    50   Input ~ 0
GYRO_DEN
Wire Wire Line
	1750 4575 1850 4575
Wire Notes Line
	825  4875 5250 4875
Wire Notes Line
	5250 3375 825  3375
Wire Notes Line
	825  3375 825  4875
Wire Notes Line
	5250 3375 5250 4875
Text Notes 2700 3350 0    50   ~ 0
Giroscópio
$EndSCHEMATC