EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 8
Title "Cascaded Hierarchical Remanence Timekeeper"
Date "2019-11-15"
Rev "V1.0"
Comp "Delft University of Technology"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 2100 2500 950  1050
U 5C8A716C
F0 "HRT2" 50
F1 "HRT.sch" 50
F2 "ADC0" I R 3050 2600 50 
F3 "ADC1" I R 3050 3100 50 
F4 "EN_TIME_0" I R 3050 2750 50 
F5 "EN_CHR_0" I R 3050 2850 50 
F6 "EN_CHR_1" I R 3050 3350 50 
F7 "EN_TIME_1" I R 3050 3250 50 
F8 "VIN" I L 2100 2700 50 
F9 "VCHARGE" I L 2100 2800 50 
F10 "CLK" I L 2100 3450 50 
F11 "EN_CAS_TIME_0" I R 3050 2950 50 
F12 "EN_CAS_TIME_1" I R 3050 3450 50 
$EndSheet
$Sheet
S 4000 1850 900  1000
U 5C8B7277
F0 "Cascade" 50
F1 "Cascade.sch" 50
F2 "V_STAGE_1" I L 4000 2150 50 
F3 "EN_STAGE_2" I L 4000 2550 50 
F4 "VIN" I R 4900 1950 50 
F5 "V_STAGE_2" I L 4000 2450 50 
F6 "EN_STAGE_3" I L 4000 2750 50 
F7 "V_STAGE_0" I L 4000 1950 50 
F8 "EN_STAGE_1" I L 4000 2250 50 
F9 "ENABLE" I R 4900 2750 50 
F10 "CLK" I R 4900 2650 50 
$EndSheet
Wire Wire Line
	4000 1950 3950 1950
Wire Wire Line
	4000 2150 3900 2150
Wire Wire Line
	3900 2150 3900 1750
Wire Wire Line
	3900 1750 3150 1750
Wire Wire Line
	4000 2750 3950 2750
Wire Wire Line
	3950 1950 3950 1250
Wire Wire Line
	3950 1250 3150 1250
Wire Wire Line
	3150 1250 3150 1300
Wire Wire Line
	3150 1300 3050 1300
Wire Wire Line
	3150 1300 3150 1350
Wire Wire Line
	3150 1350 3400 1350
Connection ~ 3150 1300
Wire Wire Line
	3250 1450 3050 1450
Wire Wire Line
	3050 1550 3250 1550
Wire Wire Line
	3250 2050 3050 2050
Wire Wire Line
	3400 1850 3150 1850
Wire Wire Line
	3150 1850 3150 1800
Wire Wire Line
	3050 1800 3150 1800
Connection ~ 3150 1800
Wire Wire Line
	3150 1800 3150 1750
Wire Wire Line
	4000 2250 3850 2250
Wire Wire Line
	3950 3450 3050 3450
Wire Wire Line
	3950 2750 3950 3450
Wire Wire Line
	4000 2450 3150 2450
Wire Wire Line
	3150 2450 3150 2600
Wire Wire Line
	3150 2600 3050 2600
Wire Wire Line
	3150 2600 3150 2650
Wire Wire Line
	3150 2650 3400 2650
Connection ~ 3150 2600
Wire Wire Line
	3050 2850 3250 2850
Wire Wire Line
	4000 2550 3900 2550
Wire Wire Line
	3900 2550 3900 2950
Wire Wire Line
	3400 3150 3150 3150
Wire Wire Line
	3150 3150 3150 3100
Wire Wire Line
	3150 3100 3050 3100
Wire Wire Line
	3050 3350 3250 3350
Wire Wire Line
	2100 2150 1550 2150
Wire Wire Line
	1550 3450 2100 3450
Text Label 8950 3450 0    50   ~ 0
VSUP
Wire Wire Line
	10150 5200 9650 5200
Text Label 9650 5200 0    50   ~ 0
LATCH_CLK
Wire Wire Line
	10150 5300 9650 5300
Text Label 9650 5300 0    50   ~ 0
CASCADE_EN
Text Label 5300 1950 0    50   ~ 0
VIN
Text Label 1750 1400 0    50   ~ 0
VIN
Text Label 1750 2700 0    50   ~ 0
VIN
Text Label 1350 1500 0    50   ~ 0
VCHARGE
Text Label 1350 2800 0    50   ~ 0
VCHARGE
Text Label 5000 2650 0    50   ~ 0
LATCH_CLK
Text Label 1550 2150 0    50   ~ 0
LATCH_CLK
Text Label 1550 3450 0    50   ~ 0
LATCH_CLK
Text Label 4950 2750 0    50   ~ 0
CASCADE_EN
$Sheet
S 2100 1200 950  1050
U 5C8B282B
F0 "HRT1" 50
F1 "HRT.sch" 50
F2 "ADC0" I R 3050 1300 50 
F3 "ADC1" I R 3050 1800 50 
F4 "EN_TIME_0" I R 3050 1450 50 
F5 "EN_CHR_0" I R 3050 1550 50 
F6 "EN_CHR_1" I R 3050 2050 50 
F7 "EN_TIME_1" I R 3050 1950 50 
F8 "VIN" I L 2100 1400 50 
F9 "VCHARGE" I L 2100 1500 50 
F10 "CLK" I L 2100 2150 50 
F11 "EN_CAS_TIME_0" I R 3050 1650 50 
F12 "EN_CAS_TIME_1" I R 3050 2150 50 
$EndSheet
Wire Wire Line
	3050 1950 3250 1950
Wire Wire Line
	3050 2150 3850 2150
Wire Wire Line
	3850 2150 3850 2250
Wire Wire Line
	3050 2750 3250 2750
Wire Wire Line
	3050 2950 3900 2950
Wire Wire Line
	3050 3250 3250 3250
$Comp
L pspice:DIODE D?
U 1 1 5CA3E7D5
P 9600 3450
AR Path="/5C8A716C/5CA3E7D5" Ref="D?"  Part="1" 
AR Path="/5C8B282B/5CA3E7D5" Ref="D?"  Part="1" 
AR Path="/5CA3E7D5" Ref="D?"  Part="1" 
AR Path="/5CAA653B/5CA3E7D5" Ref="D1"  Part="1" 
F 0 "D1" H 9600 3715 50  0000 C CNN
F 1 "BAS40-02V-V-G-08" H 9600 3624 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-523" H 9600 3450 50  0001 C CNN
F 3 "http://www.vishay.com/docs/82391/bas4002v.pdf" H 9600 3450 50  0001 C CNN
	1    9600 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:C C20
U 1 1 5CA426BC
P 10600 3700
F 0 "C20" H 10715 3746 50  0000 L CNN
F 1 "10u" H 10715 3655 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 10638 3550 50  0001 C CNN
F 3 "~" H 10600 3700 50  0001 C CNN
	1    10600 3700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR025
U 1 1 5CA426C3
P 10600 3900
F 0 "#PWR025" H 10600 3650 50  0001 C CNN
F 1 "GND" V 10605 3772 50  0000 R CNN
F 2 "" H 10600 3900 50  0001 C CNN
F 3 "" H 10600 3900 50  0001 C CNN
	1    10600 3900
	-1   0    0    -1  
$EndComp
Wire Wire Line
	10600 3850 10600 3900
Wire Wire Line
	10600 3450 10600 3550
Text Label 10400 3450 0    50   ~ 0
VIN
Wire Wire Line
	9400 3450 8950 3450
Wire Wire Line
	1750 2700 2100 2700
Wire Wire Line
	1700 2800 1350 2800
Wire Wire Line
	1750 1400 2100 1400
Wire Wire Line
	1350 1500 1700 1500
Wire Wire Line
	4900 2650 5400 2650
Wire Wire Line
	4900 2750 5400 2750
Wire Wire Line
	4900 1950 5400 1950
Text HLabel 9650 5200 0    50   Input ~ 0
LATCH_CLK
Text HLabel 9650 5300 0    50   Input ~ 0
CAS_EN
Text HLabel 8950 3450 0    50   Input ~ 0
VSUP
Text HLabel 1950 5800 2    50   Input ~ 0
ADC0
Text HLabel 1950 6500 2    50   Input ~ 0
ADC1
Text HLabel 3350 5800 2    50   Input ~ 0
ADC2
Text HLabel 3350 6500 2    50   Input ~ 0
ADC3
Text HLabel 3250 1450 2    50   Input ~ 0
EN_TIME_0
Text HLabel 3250 1550 2    50   Input ~ 0
EN_CHR_0
Text HLabel 3250 1950 2    50   Input ~ 0
EN_TIME_1
Text HLabel 3250 2750 2    50   Input ~ 0
EN_TIME_2
Text HLabel 3250 3250 2    50   Input ~ 0
EN_TIME_3
Text HLabel 3250 2050 2    50   Input ~ 0
EN_CHR_1
Text HLabel 3250 2850 2    50   Input ~ 0
EN_CHR_2
Text HLabel 3250 3350 2    50   Input ~ 0
EN_CHR_3
$Comp
L Device:R R36
U 1 1 5D116282
P 1850 1500
F 0 "R36" H 1920 1546 50  0000 L CNN
F 1 "10k" H 1920 1455 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 1780 1500 50  0001 C CNN
F 3 "~" H 1850 1500 50  0001 C CNN
	1    1850 1500
	0    1    1    0   
$EndComp
$Comp
L Device:R R37
U 1 1 5D116CF8
P 1850 2800
F 0 "R37" H 1920 2846 50  0000 L CNN
F 1 "100" H 1920 2755 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 1780 2800 50  0001 C CNN
F 3 "~" H 1850 2800 50  0001 C CNN
	1    1850 2800
	0    1    1    0   
$EndComp
Wire Wire Line
	2100 2800 2000 2800
Wire Wire Line
	2100 1500 2000 1500
$Comp
L jaspers_lib:TS3A4751 U3
U 1 1 5D4D3DFC
P 1600 5800
F 0 "U3" H 1600 6067 50  0000 C CNN
F 1 "TS3A4751" H 1600 5976 50  0000 C CNN
F 2 "Package_DFN_QFN:Texas_S-PVQFN-N14_ThermalVias" H 1500 4950 50  0001 C CNN
F 3 "http://www.ti.com/general/docs/lit/getliterature.tsp?genericPartNumber=ts3a4751&fileType=pdf" H 1550 5800 50  0001 C CNN
	1    1600 5800
	-1   0    0    -1  
$EndComp
$Comp
L jaspers_lib:TS3A4751 U3
U 2 1 5D4D7164
P 1600 6500
F 0 "U3" H 1600 6767 50  0000 C CNN
F 1 "TS3A4751" H 1600 6676 50  0000 C CNN
F 2 "Package_DFN_QFN:Texas_S-PVQFN-N14_ThermalVias" H 1500 5650 50  0001 C CNN
F 3 "http://www.ti.com/general/docs/lit/getliterature.tsp?genericPartNumber=ts3a4751&fileType=pdf" H 1550 6500 50  0001 C CNN
	2    1600 6500
	-1   0    0    -1  
$EndComp
$Comp
L jaspers_lib:TS3A4751 U3
U 4 1 5D4D8532
P 3000 5800
F 0 "U3" H 3000 6067 50  0000 C CNN
F 1 "TS3A4751" H 3000 5976 50  0000 C CNN
F 2 "Package_DFN_QFN:Texas_S-PVQFN-N14_ThermalVias" H 2900 4950 50  0001 C CNN
F 3 "http://www.ti.com/general/docs/lit/getliterature.tsp?genericPartNumber=ts3a4751&fileType=pdf" H 2950 5800 50  0001 C CNN
	4    3000 5800
	-1   0    0    -1  
$EndComp
$Comp
L jaspers_lib:TS3A4751 U3
U 3 1 5D4D997E
P 3000 6500
F 0 "U3" H 3000 6767 50  0000 C CNN
F 1 "TS3A4751" H 3000 6676 50  0000 C CNN
F 2 "Package_DFN_QFN:Texas_S-PVQFN-N14_ThermalVias" H 2900 5650 50  0001 C CNN
F 3 "http://www.ti.com/general/docs/lit/getliterature.tsp?genericPartNumber=ts3a4751&fileType=pdf" H 2950 6500 50  0001 C CNN
	3    3000 6500
	1    0    0    -1  
$EndComp
$Comp
L jaspers_lib:TS3A4751 U3
U 5 1 5D4DA986
P 4500 6300
F 0 "U3" H 4730 6396 50  0000 L CNN
F 1 "TS3A4751" H 4730 6305 50  0000 L CNN
F 2 "Package_DFN_QFN:Texas_S-PVQFN-N14_ThermalVias" H 4400 5450 50  0001 C CNN
F 3 "http://www.ti.com/general/docs/lit/getliterature.tsp?genericPartNumber=ts3a4751&fileType=pdf" H 4450 6300 50  0001 C CNN
	5    4500 6300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 5800 3350 5800
Wire Wire Line
	3250 6500 3350 6500
Wire Wire Line
	1850 6500 1950 6500
Wire Wire Line
	1850 5800 1950 5800
Text HLabel 2350 6950 3    50   Input ~ 0
EN_ADC
Wire Wire Line
	1600 6000 1600 6100
Wire Wire Line
	1600 6100 2350 6100
Wire Wire Line
	3000 6100 3000 6000
Wire Wire Line
	3000 6700 3000 6800
Wire Wire Line
	3000 6800 2700 6800
Wire Wire Line
	1600 6800 1600 6700
Wire Wire Line
	2350 6100 2350 6800
Connection ~ 2350 6100
Wire Wire Line
	2350 6100 3000 6100
Connection ~ 2350 6800
Wire Wire Line
	2350 6800 1600 6800
Wire Wire Line
	2350 6800 2350 6950
Wire Wire Line
	4400 6700 4400 6750
Wire Wire Line
	4400 6750 4500 6750
Wire Wire Line
	4600 6750 4600 6700
Wire Wire Line
	4500 6750 4500 6800
Connection ~ 4500 6750
Wire Wire Line
	4500 6750 4600 6750
$Comp
L power:GND #PWR041
U 1 1 5D4FA1C2
P 4500 6800
F 0 "#PWR041" H 4500 6550 50  0001 C CNN
F 1 "GND" V 4505 6672 50  0000 R CNN
F 2 "" H 4500 6800 50  0001 C CNN
F 3 "" H 4500 6800 50  0001 C CNN
	1    4500 6800
	-1   0    0    -1  
$EndComp
Text Label 4500 5750 1    50   ~ 0
VIN
Wire Wire Line
	4500 5800 4500 5700
Text Label 900  6500 0    50   ~ 0
INT_ADC1
Text Label 900  5800 0    50   ~ 0
INT_ADC0
Text Label 2400 5800 0    50   ~ 0
INT_ADC2
Text Label 2400 6500 0    50   ~ 0
INT_ADC3
Wire Wire Line
	1350 5800 900  5800
Wire Wire Line
	1350 6500 900  6500
Wire Wire Line
	2400 6500 2750 6500
Wire Wire Line
	2400 5800 2750 5800
$Comp
L Device:R R2
U 1 1 5D5232C5
P 2700 7000
F 0 "R2" H 2770 7046 50  0000 L CNN
F 1 "10M" H 2770 6955 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 2630 7000 50  0001 C CNN
F 3 "~" H 2700 7000 50  0001 C CNN
	1    2700 7000
	-1   0    0    1   
$EndComp
Wire Wire Line
	2700 6850 2700 6800
Connection ~ 2700 6800
Wire Wire Line
	2700 6800 2350 6800
$Comp
L power:GND #PWR024
U 1 1 5D525E53
P 2700 7200
F 0 "#PWR024" H 2700 6950 50  0001 C CNN
F 1 "GND" V 2705 7072 50  0000 R CNN
F 2 "" H 2700 7200 50  0001 C CNN
F 3 "" H 2700 7200 50  0001 C CNN
	1    2700 7200
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2700 7150 2700 7200
$Comp
L power:GND #PWR?
U 1 1 5D563335
P 10550 5350
AR Path="/5D563335" Ref="#PWR?"  Part="1" 
AR Path="/5C8A716C/5D563335" Ref="#PWR?"  Part="1" 
AR Path="/5C8B282B/5D563335" Ref="#PWR?"  Part="1" 
AR Path="/5CAA653B/5C8B282B/5D563335" Ref="#PWR?"  Part="1" 
AR Path="/5CAA653B/5C8A716C/5D563335" Ref="#PWR?"  Part="1" 
AR Path="/5CAA653B/5D563335" Ref="#PWR0103"  Part="1" 
F 0 "#PWR0103" H 10550 5100 50  0001 C CNN
F 1 "GND" H 10555 5177 50  0000 C CNN
F 2 "" H 10550 5350 50  0001 C CNN
F 3 "" H 10550 5350 50  0001 C CNN
	1    10550 5350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5D56333B
P 10550 5150
AR Path="/5C8B282B/5D56333B" Ref="R?"  Part="1" 
AR Path="/5C8A716C/5D56333B" Ref="R?"  Part="1" 
AR Path="/5CAA653B/5C8B282B/5D56333B" Ref="R?"  Part="1" 
AR Path="/5CAA653B/5C8A716C/5D56333B" Ref="R?"  Part="1" 
AR Path="/5CAA653B/5D56333B" Ref="R4"  Part="1" 
F 0 "R4" H 10620 5196 50  0000 L CNN
F 1 "10M" H 10620 5105 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 10480 5150 50  0001 C CNN
F 3 "~" H 10550 5150 50  0001 C CNN
	1    10550 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	10550 5300 10550 5350
Text Label 10550 4950 1    50   ~ 0
LATCH_CLK
Wire Wire Line
	10550 5000 10550 4550
Text Label 3200 1350 0    50   ~ 0
INT_ADC0
Text Label 3200 1850 0    50   ~ 0
INT_ADC1
Text Label 3200 2650 0    50   ~ 0
INT_ADC2
Text Label 3200 3150 0    50   ~ 0
INT_ADC3
$Comp
L jaspers_lib:S-1313A23-N4T1U3 U?
U 1 1 5D5F98C2
P 9350 1650
AR Path="/5D5F98C2" Ref="U?"  Part="1" 
AR Path="/5CAA653B/5D5F98C2" Ref="U18"  Part="1" 
F 0 "U18" H 9350 2015 50  0000 C CNN
F 1 "S-1313A22-N4T1U3" H 9350 1924 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SC-82AB" H 9200 850 50  0001 C CNN
F 3 "https://www.ablic.com/en/doc/datasheet/voltage_regulator/S1313_E.pdf" H 9300 1000 50  0001 C CNN
	1    9350 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	9700 1550 9900 1550
$Comp
L power:GND #PWR?
U 1 1 5D5F98CF
P 9350 2100
AR Path="/5D5F98CF" Ref="#PWR?"  Part="1" 
AR Path="/5CAA653B/5D5F98CF" Ref="#PWR039"  Part="1" 
F 0 "#PWR039" H 9350 1850 50  0001 C CNN
F 1 "GND" H 9355 1927 50  0000 C CNN
F 2 "" H 9350 2100 50  0001 C CNN
F 3 "" H 9350 2100 50  0001 C CNN
	1    9350 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	9350 2100 9350 2050
$Comp
L Device:C C?
U 1 1 5D5F98D6
P 9900 1800
AR Path="/5CAEB38A/5D5F98D6" Ref="C?"  Part="1" 
AR Path="/5D5F98D6" Ref="C?"  Part="1" 
AR Path="/5CAA653B/5D5F98D6" Ref="C44"  Part="1" 
F 0 "C44" H 10015 1846 50  0000 L CNN
F 1 "100nF" H 10015 1755 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 9938 1650 50  0001 C CNN
F 3 "~" H 9900 1800 50  0001 C CNN
	1    9900 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	9900 1650 9900 1550
Wire Wire Line
	9900 1950 9900 2050
Wire Wire Line
	9900 2050 9350 2050
Connection ~ 9350 2050
Wire Wire Line
	9350 2050 9350 2000
Text Label 9800 1550 0    50   ~ 0
VCHARGE
Text HLabel 8400 1550 0    50   Input ~ 0
VCHARGE_IN
Wire Wire Line
	8400 1550 9000 1550
$Comp
L power:GND #PWR?
U 1 1 5D5FF8A3
P 8800 2100
AR Path="/5D5FF8A3" Ref="#PWR?"  Part="1" 
AR Path="/5C8A716C/5D5FF8A3" Ref="#PWR?"  Part="1" 
AR Path="/5C8B282B/5D5FF8A3" Ref="#PWR?"  Part="1" 
AR Path="/5CAA653B/5C8B282B/5D5FF8A3" Ref="#PWR?"  Part="1" 
AR Path="/5CAA653B/5C8A716C/5D5FF8A3" Ref="#PWR?"  Part="1" 
AR Path="/5CAA653B/5D5FF8A3" Ref="#PWR032"  Part="1" 
F 0 "#PWR032" H 8800 1850 50  0001 C CNN
F 1 "GND" H 8805 1927 50  0000 C CNN
F 2 "" H 8800 2100 50  0001 C CNN
F 3 "" H 8800 2100 50  0001 C CNN
	1    8800 2100
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5D5FF8AD
P 8800 1900
AR Path="/5C8B282B/5D5FF8AD" Ref="R?"  Part="1" 
AR Path="/5C8A716C/5D5FF8AD" Ref="R?"  Part="1" 
AR Path="/5CAA653B/5C8B282B/5D5FF8AD" Ref="R?"  Part="1" 
AR Path="/5CAA653B/5C8A716C/5D5FF8AD" Ref="R?"  Part="1" 
AR Path="/5CAA653B/5D5FF8AD" Ref="R11"  Part="1" 
F 0 "R11" H 8870 1946 50  0000 L CNN
F 1 "10M" H 8870 1855 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 8730 1900 50  0001 C CNN
F 3 "~" H 8800 1900 50  0001 C CNN
	1    8800 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	8800 2050 8800 2100
Wire Wire Line
	8600 1700 8800 1700
Wire Wire Line
	8800 1750 8800 1700
Connection ~ 8800 1700
Wire Wire Line
	8800 1700 9000 1700
Text HLabel 8600 1700 0    50   Input ~ 0
VCHARGE_EN
$Comp
L Device:C C?
U 1 1 5D7A1911
P 5350 6300
AR Path="/5CAA653B/5C8B7277/5D7A1911" Ref="C?"  Part="1" 
AR Path="/5CAA653B/5D7A1911" Ref="C45"  Part="1" 
F 0 "C45" H 5465 6346 50  0000 L CNN
F 1 "100nF" H 5465 6255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 5388 6150 50  0001 C CNN
F 3 "~" H 5350 6300 50  0001 C CNN
	1    5350 6300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 5700 5350 5700
Wire Wire Line
	5350 5700 5350 6150
Connection ~ 4500 5700
Wire Wire Line
	4500 5700 4500 5600
Wire Wire Line
	5350 6450 5350 6750
Wire Wire Line
	5350 6750 4600 6750
Connection ~ 4600 6750
Wire Wire Line
	9800 3450 10600 3450
Text Notes 8050 950  0    98   Italic 20
Charging supply
Wire Notes Line
	11100 2650 7750 2650
Wire Notes Line
	7750 4550 500  4550
Wire Notes Line
	7750 550  7750 4550
Text Notes 850  1000 0    98   Italic 20
Timekeeper
Text Notes 3900 7350 0    50   ~ 0
Gate MCU GPIO to prevent backfeeding
$EndSCHEMATC
