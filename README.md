# OPCM (Open Source Power-train Control Module)

Have an older vehicle without a PCM?  Maybe a Fummins with a 24 valve or common rail swap. This project aims to add OBD2 over CAN-bus to any vehicle, and support different combinations of inputs and outputs over time.

Right away I'm working on input via CCD and standard sensors.  I already have output over OBD2 and a standard Tachometer and Speedometer output working with 12 volt PWM.

There is a LOT of room for improvement, and I'm just getting started here.

## Features
### Working
* 12v PWM Speedometer output
* 12v PWM Tachometer output
* Basic OBD2 via CAN-BUS

### Future
1. CCD input
2. configuration via configuration.h (modeled after Merlin for 3d printers)
3. 5v sensor inputs
4. Suggestions?

## Hardware
This project currently uses an Arduino Mega, Seeedstudio CAN-bus shield, and a few optoboards

[Arduino Mega(41.30USD)](https://store.arduino.cc/usa/mega-2560-r3?gclid=Cj0KCQjw_8mHBhClARIsABfFgpjTCKfoT6O_4BFl50Pk-M2Uob__o3zz4_DoIzSqRxK_8AcUW3GCW3gaAq1nEALw_wcB)

[Seeedstudio CAN-BUS Shield(27.00USD)](https://www.seeedstudio.com/CAN-BUS-Shield-V2.html)

[CCD dev board(25.00USD)](https://www.tindie.com/products/boundarycondition/ccdpci-bus-transceiver-development-board/)

[Optocoupler Isolation Board (6.99USD)](https://www.amazon.com/gp/product/B07GMHLL2M/ref=ppx_yo_dt_b_asin_title_o09_s00?ie=UTF8&psc=1)

This makes the hardware I'm currently using 100.29USD, not to shabby!  In the future it would be nice to have a custom PCM that combine all these things onto one board, but baby steps.

### Pins
#### Analog
0.
1. 
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
2. CAN-BUS Shield INT
3.
4.
5.
6.
7.
8. Speedometer Output
9. CAN-BUS Shield CS
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
50.
51.
52.
53.

ICSP Pins
* CAN-BUS Shield MOSI
* CAN-BUS Shield MISO
* CAN-BUS Shield SCK