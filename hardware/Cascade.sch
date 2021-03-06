EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 7 8
Title "Cascade"
Date "2019-11-15"
Rev "V1.0"
Comp "Delft University of Technology"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 4350 1450 0    50   Input ~ 0
V_STAGE_0
$Comp
L power:GND #PWR044
U 1 1 5C8B78BE
P 5000 2050
F 0 "#PWR044" H 5000 1800 50  0001 C CNN
F 1 "GND" H 5005 1877 50  0000 C CNN
F 2 "" H 5000 2050 50  0001 C CNN
F 3 "" H 5000 2050 50  0001 C CNN
	1    5000 2050
	1    0    0    -1  
$EndComp
Text HLabel 6400 1650 2    50   Input ~ 0
EN_STAGE_1
Text HLabel 7400 1950 0    50   Input ~ 0
VIN
Text HLabel 4350 2950 0    50   Input ~ 0
V_STAGE_1
$Comp
L power:GND #PWR045
U 1 1 5C8A9D08
P 5000 3550
F 0 "#PWR045" H 5000 3300 50  0001 C CNN
F 1 "GND" H 5005 3377 50  0000 C CNN
F 2 "" H 5000 3550 50  0001 C CNN
F 3 "" H 5000 3550 50  0001 C CNN
	1    5000 3550
	1    0    0    -1  
$EndComp
Text HLabel 6450 3150 2    50   Input ~ 0
EN_STAGE_2
Wire Wire Line
	5000 2850 5000 2650
Wire Wire Line
	5000 2650 5500 2650
Connection ~ 5000 2650
Wire Wire Line
	5000 2650 5000 2550
Text HLabel 4350 4650 0    50   Input ~ 0
V_STAGE_2
$Comp
L power:GND #PWR046
U 1 1 5C8AA1FC
P 5000 5250
F 0 "#PWR046" H 5000 5000 50  0001 C CNN
F 1 "GND" H 5005 5077 50  0000 C CNN
F 2 "" H 5000 5250 50  0001 C CNN
F 3 "" H 5000 5250 50  0001 C CNN
	1    5000 5250
	1    0    0    -1  
$EndComp
Text HLabel 6500 4850 2    50   Input ~ 0
EN_STAGE_3
Wire Wire Line
	5000 4550 5000 4350
Wire Wire Line
	5000 4350 5500 4350
Connection ~ 5000 4350
Wire Wire Line
	5000 4350 5000 4250
$Comp
L jaspers_lib:TS3A4751 U10
U 1 1 5C8B3D93
P 5900 1650
F 0 "U10" H 5900 1917 50  0000 C CNN
F 1 "TS3A4751" H 5900 1826 50  0000 C CNN
F 2 "Package_DFN_QFN:Texas_S-PVQFN-N14_ThermalVias" H 5800 800 50  0001 C CNN
F 3 "http://www.ti.com/general/docs/lit/getliterature.tsp?genericPartNumber=ts3a4751&fileType=pdf" H 5850 1650 50  0001 C CNN
	1    5900 1650
	1    0    0    -1  
$EndComp
$Comp
L jaspers_lib:TS3A4751 U10
U 2 1 5C8B3E1C
P 5900 3150
F 0 "U10" H 5900 3417 50  0000 C CNN
F 1 "TS3A4751" H 5900 3326 50  0000 C CNN
F 2 "Package_DFN_QFN:Texas_S-PVQFN-N14_ThermalVias" H 5800 2300 50  0001 C CNN
F 3 "http://www.ti.com/general/docs/lit/getliterature.tsp?genericPartNumber=ts3a4751&fileType=pdf" H 5850 3150 50  0001 C CNN
	2    5900 3150
	1    0    0    -1  
$EndComp
$Comp
L jaspers_lib:TS3A4751 U10
U 3 1 5C8B3E8C
P 5900 4850
F 0 "U10" H 5900 5117 50  0000 C CNN
F 1 "TS3A4751" H 5900 5026 50  0000 C CNN
F 2 "Package_DFN_QFN:Texas_S-PVQFN-N14_ThermalVias" H 5800 4000 50  0001 C CNN
F 3 "http://www.ti.com/general/docs/lit/getliterature.tsp?genericPartNumber=ts3a4751&fileType=pdf" H 5850 4850 50  0001 C CNN
	3    5900 4850
	1    0    0    -1  
$EndComp
$Comp
L jaspers_lib:TS3A4751 U10
U 5 1 5C8B3F11
P 7950 3800
F 0 "U10" H 7950 4067 50  0000 C CNN
F 1 "TS3A4751" H 7950 3976 50  0000 C CNN
F 2 "Package_DFN_QFN:Texas_S-PVQFN-N14_ThermalVias" H 7850 2950 50  0001 C CNN
F 3 "http://www.ti.com/general/docs/lit/getliterature.tsp?genericPartNumber=ts3a4751&fileType=pdf" H 7900 3800 50  0001 C CNN
	5    7950 3800
	1    0    0    -1  
$EndComp
$Comp
L jaspers_lib:TS3A4751 U10
U 4 1 5C8B3F93
P 7900 1950
F 0 "U10" H 7700 2100 50  0000 L CNN
F 1 "TS3A4751" H 8000 1750 50  0000 L CNN
F 2 "Package_DFN_QFN:Texas_S-PVQFN-N14_ThermalVias" H 7800 1100 50  0001 C CNN
F 3 "http://www.ti.com/general/docs/lit/getliterature.tsp?genericPartNumber=ts3a4751&fileType=pdf" H 7850 1950 50  0001 C CNN
	4    7900 1950
	1    0    0    -1  
$EndComp
Text Label 5900 2350 1    50   ~ 0
EN_CASCADE
Text Label 5900 3900 1    50   ~ 0
EN_CASCADE
Text Label 5900 5600 1    50   ~ 0
EN_CASCADE
Wire Wire Line
	7400 1950 7650 1950
Wire Wire Line
	8150 1950 8550 1950
Text Label 8350 1950 0    50   ~ 0
VINT
Text Label 5000 1100 1    50   ~ 0
VINT
Text Label 5000 2600 1    50   ~ 0
VINT
Text Label 5000 4300 1    50   ~ 0
VINT
Text Label 7900 2200 3    50   ~ 0
EN_CASCADE
Wire Wire Line
	7900 2150 7900 2500
$Comp
L power:GND #PWR051
U 1 1 5C8F122C
P 7950 4300
F 0 "#PWR051" H 7950 4050 50  0001 C CNN
F 1 "GND" H 7955 4127 50  0000 C CNN
F 2 "" H 7950 4300 50  0001 C CNN
F 3 "" H 7950 4300 50  0001 C CNN
	1    7950 4300
	1    0    0    -1  
$EndComp
Text HLabel 7950 3200 1    50   Input ~ 0
VIN
Wire Wire Line
	7950 3200 7950 3250
Wire Wire Line
	7850 4200 7850 4250
Wire Wire Line
	7850 4250 7950 4250
Wire Wire Line
	8050 4250 8050 4200
Wire Wire Line
	7950 4250 7950 4300
Connection ~ 7950 4250
Wire Wire Line
	7950 4250 8050 4250
Text HLabel 7100 2500 0    50   Input ~ 0
ENABLE
Wire Wire Line
	5000 1050 5000 1150
Wire Wire Line
	5000 1150 5500 1150
$Comp
L Device:C C30
U 1 1 5C9120DC
P 7400 3500
F 0 "C30" H 7515 3546 50  0000 L CNN
F 1 "100nF" H 7515 3455 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 7438 3350 50  0001 C CNN
F 3 "~" H 7400 3500 50  0001 C CNN
	1    7400 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7950 3250 7400 3250
Wire Wire Line
	7400 3250 7400 3350
Connection ~ 7950 3250
Wire Wire Line
	7950 3250 7950 3300
$Comp
L power:GND #PWR050
U 1 1 5C91374A
P 7400 3700
F 0 "#PWR050" H 7400 3450 50  0001 C CNN
F 1 "GND" H 7405 3527 50  0000 C CNN
F 2 "" H 7400 3700 50  0001 C CNN
F 3 "" H 7400 3700 50  0001 C CNN
	1    7400 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	7400 3650 7400 3700
$Comp
L Device:C C29
U 1 1 5C91C5A4
P 5500 4150
F 0 "C29" H 5615 4196 50  0000 L CNN
F 1 "100nF" H 5615 4105 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 5538 4000 50  0001 C CNN
F 3 "~" H 5500 4150 50  0001 C CNN
	1    5500 4150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR049
U 1 1 5C91DECF
P 5500 3950
F 0 "#PWR049" H 5500 3700 50  0001 C CNN
F 1 "GND" H 5505 3777 50  0000 C CNN
F 2 "" H 5500 3950 50  0001 C CNN
F 3 "" H 5500 3950 50  0001 C CNN
	1    5500 3950
	-1   0    0    1   
$EndComp
$Comp
L Device:C C28
U 1 1 5C91F7AB
P 5500 2450
F 0 "C28" H 5615 2496 50  0000 L CNN
F 1 "100nF" H 5615 2405 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 5538 2300 50  0001 C CNN
F 3 "~" H 5500 2450 50  0001 C CNN
	1    5500 2450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR048
U 1 1 5C91F7B2
P 5500 2250
F 0 "#PWR048" H 5500 2000 50  0001 C CNN
F 1 "GND" H 5505 2077 50  0000 C CNN
F 2 "" H 5500 2250 50  0001 C CNN
F 3 "" H 5500 2250 50  0001 C CNN
	1    5500 2250
	-1   0    0    1   
$EndComp
$Comp
L Device:C C27
U 1 1 5C920F29
P 5500 950
F 0 "C27" H 5615 996 50  0000 L CNN
F 1 "100nF" H 5615 905 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 5538 800 50  0001 C CNN
F 3 "~" H 5500 950 50  0001 C CNN
	1    5500 950 
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR047
U 1 1 5C920F30
P 5500 750
F 0 "#PWR047" H 5500 500 50  0001 C CNN
F 1 "GND" H 5505 577 50  0000 C CNN
F 2 "" H 5500 750 50  0001 C CNN
F 3 "" H 5500 750 50  0001 C CNN
	1    5500 750 
	-1   0    0    1   
$EndComp
Wire Wire Line
	5500 4350 5500 4300
Wire Wire Line
	5500 4000 5500 3950
Wire Wire Line
	5500 2650 5500 2600
Wire Wire Line
	5500 2300 5500 2250
Wire Wire Line
	5500 1100 5500 1150
Wire Wire Line
	5500 800  5500 750 
$Comp
L jaspers_lib:SN74AUP2G79DCU U11
U 1 1 5C93A58E
P 7500 2600
F 0 "U11" H 7500 2967 50  0000 C CNN
F 1 "SN74AUP2G79DCU" H 7550 2400 50  0000 C CNN
F 2 "Package_SO:VSSOP-8_2.4x2.1mm_P0.5mm" H 7300 2050 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/sn74aup2g79.pdf" H 7150 1900 50  0001 C CNN
	1    7500 2600
	1    0    0    -1  
$EndComp
$Comp
L jaspers_lib:SN74AUP2G79DCU U11
U 2 1 5C93A628
P 9750 2750
F 0 "U11" H 9750 3117 50  0000 C CNN
F 1 "SN74AUP2G79DCU" H 9750 3026 50  0000 C CNN
F 2 "Package_SO:VSSOP-8_2.4x2.1mm_P0.5mm" H 9550 2200 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/sn74aup2g79.pdf" H 9400 2050 50  0001 C CNN
	2    9750 2750
	1    0    0    -1  
$EndComp
$Comp
L jaspers_lib:SN74AUP2G79DCU U11
U 3 1 5C93A6DB
P 8550 3700
F 0 "U11" H 8300 4050 50  0000 L CNN
F 1 "SN74AUP2G79DCU" H 8700 3300 50  0000 L CNN
F 2 "Package_SO:VSSOP-8_2.4x2.1mm_P0.5mm" H 8350 3150 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/sn74aup2g79.pdf" H 8200 3000 50  0001 C CNN
	3    8550 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C31
U 1 1 5C93A922
P 9000 3550
F 0 "C31" H 9115 3596 50  0000 L CNN
F 1 "100nF" H 9115 3505 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 9038 3400 50  0001 C CNN
F 3 "~" H 9000 3550 50  0001 C CNN
	1    9000 3550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR053
U 1 1 5C93A92B
P 9000 3750
F 0 "#PWR053" H 9000 3500 50  0001 C CNN
F 1 "GND" H 9005 3577 50  0000 C CNN
F 2 "" H 9000 3750 50  0001 C CNN
F 3 "" H 9000 3750 50  0001 C CNN
	1    9000 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 3700 9000 3750
Wire Wire Line
	7950 3250 8550 3250
Wire Wire Line
	9000 3250 9000 3400
Wire Wire Line
	8550 3300 8550 3250
Connection ~ 8550 3250
Wire Wire Line
	8550 3250 9000 3250
$Comp
L power:GND #PWR052
U 1 1 5C945F8F
P 8550 4300
F 0 "#PWR052" H 8550 4050 50  0001 C CNN
F 1 "GND" H 8555 4127 50  0000 C CNN
F 2 "" H 8550 4300 50  0001 C CNN
F 3 "" H 8550 4300 50  0001 C CNN
	1    8550 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	8550 4100 8550 4300
Text HLabel 7100 2600 0    50   Input ~ 0
CLK
Wire Wire Line
	7100 2600 7250 2600
Wire Wire Line
	7100 2500 7250 2500
Wire Wire Line
	7750 2500 7900 2500
$Comp
L power:GND #PWR054
U 1 1 5C9565CA
P 9400 2800
F 0 "#PWR054" H 9400 2550 50  0001 C CNN
F 1 "GND" H 9405 2627 50  0000 C CNN
F 2 "" H 9400 2800 50  0001 C CNN
F 3 "" H 9400 2800 50  0001 C CNN
	1    9400 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	9500 2650 9400 2650
Wire Wire Line
	9400 2650 9400 2750
Wire Wire Line
	9500 2750 9400 2750
Connection ~ 9400 2750
Wire Wire Line
	9400 2750 9400 2800
NoConn ~ 10000 2650
Wire Wire Line
	5400 1650 5650 1650
Wire Wire Line
	5400 4850 5650 4850
Wire Wire Line
	5400 3150 5650 3150
Wire Wire Line
	6150 1650 6400 1650
Wire Wire Line
	6150 3150 6450 3150
Wire Wire Line
	6150 4850 6500 4850
Wire Wire Line
	5900 5050 5900 5600
Wire Wire Line
	5900 3350 5900 3900
Wire Wire Line
	5900 1850 5900 2350
Wire Wire Line
	5000 1350 5000 1150
Connection ~ 5000 1150
$Comp
L jaspers_lib:MCP1810T-12 U7
U 1 1 5D4F817B
P 2400 6200
F 0 "U7" H 2400 6565 50  0000 C CNN
F 1 "MCP1810T-12" H 2400 6474 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 2250 5400 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/20005623B.pdf" H 2350 5550 50  0001 C CNN
	1    2400 6200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R15
U 1 1 5D4F8606
P 2900 6300
F 0 "R15" H 2970 6346 50  0000 L CNN
F 1 "9.1M" H 2970 6255 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 2830 6300 50  0001 C CNN
F 3 "~" H 2900 6300 50  0001 C CNN
	1    2900 6300
	1    0    0    -1  
$EndComp
$Comp
L Device:R R16
U 1 1 5D4F88CC
P 2900 6700
F 0 "R16" H 2970 6746 50  0000 L CNN
F 1 "2.4M" H 2970 6655 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 2830 6700 50  0001 C CNN
F 3 "~" H 2900 6700 50  0001 C CNN
	1    2900 6700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 6100 2900 6100
Wire Wire Line
	2900 6100 2900 6150
Connection ~ 2900 6100
Wire Wire Line
	2900 6450 2900 6500
$Comp
L Device:C C18
U 1 1 5D50141D
P 3550 6300
F 0 "C18" H 3665 6346 50  0000 L CNN
F 1 "2.2uF" H 3665 6255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 3588 6150 50  0001 C CNN
F 3 "~" H 3550 6300 50  0001 C CNN
	1    3550 6300
	1    0    0    -1  
$EndComp
$Comp
L Device:C C17
U 1 1 5D501C09
P 3300 6750
F 0 "C17" H 3415 6796 50  0000 L CNN
F 1 "DNP" H 3415 6705 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 3338 6600 50  0001 C CNN
F 3 "~" H 3300 6750 50  0001 C CNN
	1    3300 6750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3550 6100 3550 6150
Wire Wire Line
	2900 6100 3550 6100
Wire Wire Line
	3300 6600 3300 6500
Wire Wire Line
	3300 6500 2900 6500
Connection ~ 2900 6500
Wire Wire Line
	2900 6500 2900 6550
Text Label 3100 6500 0    50   ~ 0
VREF
Wire Wire Line
	3550 6450 3550 6950
Wire Wire Line
	3550 6950 3300 6950
Wire Wire Line
	3300 6950 3300 6900
Wire Wire Line
	2900 6850 2900 6950
Wire Wire Line
	2900 6950 3300 6950
Connection ~ 3300 6950
Wire Wire Line
	2400 6650 2400 6950
Wire Wire Line
	2400 6950 2900 6950
Connection ~ 2900 6950
$Comp
L Device:C C15
U 1 1 5D50FB8C
P 1350 6350
F 0 "C15" H 1465 6396 50  0000 L CNN
F 1 "2.2uF" H 1465 6305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 1388 6200 50  0001 C CNN
F 3 "~" H 1350 6350 50  0001 C CNN
	1    1350 6350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2050 6250 2000 6250
Wire Wire Line
	1350 6200 1350 6100
Wire Wire Line
	1350 6500 1350 6950
Wire Wire Line
	1350 6950 2400 6950
Connection ~ 2400 6950
Text Label 1500 6100 2    50   ~ 0
VINT
$Comp
L power:GND #PWR042
U 1 1 5D51EEA5
P 2400 7000
F 0 "#PWR042" H 2400 6750 50  0001 C CNN
F 1 "GND" H 2405 6827 50  0000 C CNN
F 2 "" H 2400 7000 50  0001 C CNN
F 3 "" H 2400 7000 50  0001 C CNN
	1    2400 7000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 6950 2400 7000
$Comp
L jaspers_lib:TLV3691IDCK U8
U 1 1 5D52BBC1
P 5000 1650
F 0 "U8" H 5444 1696 50  0000 L CNN
F 1 "TLV3691IDCK" H 5444 1605 50  0000 L CNN
F 2 "jaspers_footprints:Texas_R-PDSO-G5" H 4950 1200 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tlv3691.pdf" H 5150 1850 50  0001 C CNN
	1    5000 1650
	1    0    0    -1  
$EndComp
$Comp
L jaspers_lib:TLV3691IDCK U9
U 1 1 5D52C95B
P 5000 3150
F 0 "U9" H 5444 3196 50  0000 L CNN
F 1 "TLV3691IDCK" H 5444 3105 50  0000 L CNN
F 2 "jaspers_footprints:Texas_R-PDSO-G5" H 4950 2700 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tlv3691.pdf" H 5150 3350 50  0001 C CNN
	1    5000 3150
	1    0    0    -1  
$EndComp
$Comp
L jaspers_lib:TLV3691IDCK U17
U 1 1 5D52D9D7
P 5000 4850
F 0 "U17" H 5444 4896 50  0000 L CNN
F 1 "TLV3691IDCK" H 5444 4805 50  0000 L CNN
F 2 "jaspers_footprints:Texas_R-PDSO-G5" H 4950 4400 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tlv3691.pdf" H 5150 5050 50  0001 C CNN
	1    5000 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 5150 5000 5250
Wire Wire Line
	5000 3450 5000 3550
Wire Wire Line
	5000 1950 5000 2050
$Comp
L Device:C C43
U 1 1 5D52EF03
P 4450 4850
F 0 "C43" H 4565 4896 50  0000 L CNN
F 1 "DNP" H 4565 4805 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 4488 4700 50  0001 C CNN
F 3 "~" H 4450 4850 50  0001 C CNN
	1    4450 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 4950 4750 4950
Wire Wire Line
	4750 4950 4750 5050
Wire Wire Line
	4750 5050 4450 5050
Wire Wire Line
	4450 5050 4450 5000
Wire Wire Line
	4750 4750 4800 4750
Wire Wire Line
	4750 4650 4750 4750
Wire Wire Line
	4750 4650 4450 4650
Wire Wire Line
	4450 4650 4450 4700
Wire Wire Line
	4350 4650 4450 4650
Connection ~ 4450 4650
Text Label 4500 5050 0    50   ~ 0
VREF
$Comp
L Device:C C39
U 1 1 5D54903C
P 4450 3150
F 0 "C39" H 4565 3196 50  0000 L CNN
F 1 "DNP" H 4565 3105 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 4488 3000 50  0001 C CNN
F 3 "~" H 4450 3150 50  0001 C CNN
	1    4450 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 3250 4750 3250
Wire Wire Line
	4750 3250 4750 3350
Wire Wire Line
	4750 3350 4450 3350
Wire Wire Line
	4450 3350 4450 3300
Wire Wire Line
	4750 3050 4800 3050
Wire Wire Line
	4750 2950 4750 3050
Wire Wire Line
	4750 2950 4450 2950
Wire Wire Line
	4450 2950 4450 3000
Wire Wire Line
	4350 2950 4450 2950
Connection ~ 4450 2950
Text Label 4500 3350 0    50   ~ 0
VREF
$Comp
L Device:C C21
U 1 1 5D54CECB
P 4450 1650
F 0 "C21" H 4565 1696 50  0000 L CNN
F 1 "DNP" H 4565 1605 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 4488 1500 50  0001 C CNN
F 3 "~" H 4450 1650 50  0001 C CNN
	1    4450 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 1750 4750 1750
Wire Wire Line
	4750 1750 4750 1850
Wire Wire Line
	4750 1850 4450 1850
Wire Wire Line
	4450 1850 4450 1800
Wire Wire Line
	4750 1550 4800 1550
Wire Wire Line
	4750 1450 4750 1550
Wire Wire Line
	4750 1450 4450 1450
Wire Wire Line
	4450 1450 4450 1500
Wire Wire Line
	4350 1450 4450 1450
Connection ~ 4450 1450
Text Label 4500 1850 0    50   ~ 0
VREF
Wire Wire Line
	1350 6100 2000 6100
Wire Wire Line
	2000 6250 2000 6100
Connection ~ 2000 6100
Wire Wire Line
	2000 6100 2050 6100
Text Notes 2700 2400 0    50   ~ 0
Trip at the end of usable capacitor voltage range.\nThen start the discharging of the next tier.
Wire Notes Line
	500  5500 4350 5500
Wire Notes Line
	4350 5500 4350 7800
Text Notes 700  5800 0    98   Italic 20
Reference voltage
$EndSCHEMATC
