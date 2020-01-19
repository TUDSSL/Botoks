EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 8
Title "Solar Energy Harvester"
Date "2019-11-15"
Rev "V1.0"
Comp "Delft University of Technology"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	4550 3650 4550 3700
Connection ~ 4550 3700
Wire Wire Line
	4550 3350 4550 3300
$Comp
L pspice:DIODE D?
U 1 1 5CB7CD8A
P 5000 3300
AR Path="/5C8A716C/5CB7CD8A" Ref="D?"  Part="1" 
AR Path="/5C8B282B/5CB7CD8A" Ref="D?"  Part="1" 
AR Path="/5CB7CD8A" Ref="D?"  Part="1" 
AR Path="/5CAA653B/5CB7CD8A" Ref="D?"  Part="1" 
AR Path="/5CAEBABF/5CB7CD8A" Ref="D4"  Part="1" 
F 0 "D4" H 5000 3565 50  0000 C CNN
F 1 "BAS40-02V-V-G-08" H 5000 3474 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-523" H 5000 3300 50  0001 C CNN
F 3 "http://www.vishay.com/docs/82391/bas4002v.pdf" H 5000 3300 50  0001 C CNN
	1    5000 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 3300 4800 3300
$Comp
L power:GND #PWR075
U 1 1 5CB7D22E
P 4550 3800
F 0 "#PWR075" H 4550 3550 50  0001 C CNN
F 1 "GND" H 4555 3627 50  0000 C CNN
F 2 "" H 4550 3800 50  0001 C CNN
F 3 "" H 4550 3800 50  0001 C CNN
	1    4550 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 3800 4550 3700
Text HLabel 6100 3300 2    50   Input ~ 0
VSOLAR
$Comp
L Device:C C42
U 1 1 5CB8ECFD
P 5900 3500
F 0 "C42" H 6015 3546 50  0000 L CNN
F 1 "100uF" H 6015 3455 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 5938 3350 50  0001 C CNN
F 3 "~" H 5900 3500 50  0001 C CNN
	1    5900 3500
	1    0    0    -1  
$EndComp
$Comp
L Device:C C41
U 1 1 5CB8F73F
P 5500 3500
F 0 "C41" H 5615 3546 50  0000 L CNN
F 1 "22uF" H 5615 3455 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5538 3350 50  0001 C CNN
F 3 "~" H 5500 3500 50  0001 C CNN
	1    5500 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 3300 5500 3300
Wire Wire Line
	5900 3350 5900 3300
Connection ~ 5900 3300
Wire Wire Line
	5900 3300 6100 3300
Wire Wire Line
	5500 3350 5500 3300
Connection ~ 5500 3300
Wire Wire Line
	5500 3300 5900 3300
Wire Wire Line
	5500 3650 5500 3700
Wire Wire Line
	5900 3650 5900 3700
Wire Wire Line
	5500 3700 4550 3700
Wire Wire Line
	5900 3700 5500 3700
Connection ~ 5500 3700
$Comp
L jaspers_lib:KXOB25-02X8F SC1
U 1 1 5D1DD6B3
P 4550 3550
F 0 "SC1" H 4658 3646 50  0000 L CNN
F 1 "KXOB25-02X8F" H 4658 3555 50  0000 L CNN
F 2 "jaspers_footprints:KXOB25-02X8F" V 4350 4050 50  0001 C CNN
F 3 "~" V 4550 3610 50  0001 C CNN
	1    4550 3550
	1    0    0    -1  
$EndComp
$EndSCHEMATC
