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

### In Progress
* configuration via configuration.h (modeled after Merlin for 3d printers)
* Cummins-bus input (24v cummins)
* Torque Control for shifting (Via Cummins-bus)
* Transmission Temperature Sensor
* Transmission Pressure Sensor

### Future
1. 5v sensor inputs
2. Suggestions?

## Hardware
This project currently uses an Arduino Mega, Seeedstudio CAN-bus shield, and a few optoboards

[Arduino Mega(41.30USD)](https://store.arduino.cc/usa/mega-2560-r3?gclid=Cj0KCQjw_8mHBhClARIsABfFgpjTCKfoT6O_4BFl50Pk-M2Uob__o3zz4_DoIzSqRxK_8AcUW3GCW3gaAq1nEALw_wcB)

[Seeedstudio CAN-BUS Shield(27.00USD)](https://www.seeedstudio.com/CAN-BUS-Shield-V2.html)

[CCD dev board(25.00USD)](https://www.tindie.com/products/boundarycondition/ccdpci-bus-transceiver-development-board/)

[Optocoupler Isolation Board (6.99USD)](https://www.amazon.com/gp/product/B07GMHLL2M/ref=ppx_yo_dt_b_asin_title_o09_s00?ie=UTF8&psc=1)

This makes the hardware I'm currently using 100.29USD, not to shabby!  In the future it would be nice to have a custom PCM that combine all these things onto one board, but baby steps.

### Pins
#### Analog
0. Transmission Temperature Sensor
1. Transmission Pressure Sensor
2.
3.
4.
5.
6.
7.
8.
9.
10.
11.
12.
13.
14.
15.

#### Digital
0.
1.
2. CAN-BUS Shield INT (Output)
3. CAN-BUS INT (Cummins Bus Interrupt)
4.
5.
6. CS (Cummins Bus Chip Select)
7.
8. Speedometer Output
9. CAN-BUS Shield CS (Output)
10.
11. 
12. 
13. 
14. Nextion TX
15. Nextion RX
16.
17.
18. Speedometer Input (pwm)
19. Tachometer Input (pwm)
20.
21.
22.
23.
24. Left Blinker
25. Right Blinker
26. High Beams
27. Wait To Start
28. 4x4
29. Seat Belt Warning
30. Door Ajar Warning
31. Brake Light
32.
33.
34.
35.
36.
37.
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