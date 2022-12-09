# OPCM (Open Source Power-train Control Module)

Have an older vehicle without a PCM?  Maybe a Fummins with a 24 valve or common rail swap. This project aims to add OBD2 over CAN-bus to any vehicle, and support different combinations of inputs and outputs over time.

There is a LOT of room for improvement, and I'm just getting started here.

## Features
### Working
* 12v PWM Speedometer input (with optoisolator)
* 5-24v 60-2 camshaft sensor tachometer input (with optoisolator)
* 12v PWM Speedometer output (with optoisolator)
* 12v PWM Tachometer output (with optoisolator, needs input)
* Basic OBD2 via CAN-BUS output (speedometer and tachometer)
* Transmission Temperature Sensor
* Transmission Pressure Sensor
* Nextion output (custom displays)
* configuration via configuration.h (modeled after Merlin for 3d printers)
* Cummins-bus input (24v cummins)

### In Progress
* Torque Control for shifting (Via Cummins-bus)

### Future
1. 5v sensor inputs
2. Suggestions?

## Hardware
This project currently uses a Teensy 4.1, 2 CAN-bus transceivers(to send data), and a few optoisolators

[Teensy 4.1(31.50USD)](https://www.pjrc.com/store/teensy41.html)

[SN65HVD230 CAN Board(4.99USD)](https://www.waveshare.com/sn65hvd230-can-board.htm)

[ADS1115 16-Bit ADC - 4 Channel with Programmable Gain Amplifier(14.95USD)](https://www.adafruit.com/product/1085)

This makes the hardware I'm currently using 56.43USD, not to shabby!  In the future it would be nice to have a custom PCM that combine all these things onto one board, but baby steps.

Also, a Nextion display can be as much as 150USD, but totally worth it for me :)

### Pins
0. Cummins Bus RX
1. Cummins Bus TX
2. Speedometer Output
3.
4. Tachometer Input/Output
5. Speedometer Input
6.
7. Nextion RX(RX2)
8. Nextion TX(TX2)
9.
10.
11.
12.
13.
14.
15.
16.
17.
18. 
19. 
20.
21.
22.
23.
24. 
25. 
26. 
27. 
28. 
29. 
30. 
31. 
32.
33.
34.
35.
36.
37. Left Blinker
38.
39.
40.
41.
42.
43.
44. Tachometer Output
45.
46.
47.
48.
49.
50. MISO (Master In Slave Out Cummins Bus)
51. MOSI (Master Out Slave In Cummins Bus)
52. SCK (Serial Clock Cummins Bus)
53. SS (Slave Select Cummins Bus)

ICSP Pins
* CAN-BUS Shield MOSI (Output)
* CAN-BUS Shield MISO (Output)
* CAN-BUS Shield SCK (Output)