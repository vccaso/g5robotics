# Arduino projects

## Ultrasonicspeed

### Components:
- 1 x Ultrasonic Sensor (HC-SR04 or similar): For measuring distance.
- 2 x MAX7219 LED Matrix: For displaying data (distance, speed, units).
- 2 x Pushbutton Switch: To switch between modes and units.
- 2 x LED: To indicate units (e.g., one LED for meters/kilometers and the other for feet/miles).
- 1 x Arduino Board: Arduino Uno or similar.
- Resistors (for the pushbuttons and LEDs, typically 220 ohms for LEDs).
- Jumper wires and Breadboard for prototyping.
- Pin Configuration:
- Ultrasonic Sensor (HC-SR04):
- Trigger Pin: Digital Pin X (you can choose an available pin).
- Echo Pin: Digital Pin Y.
- MAX7219 LED Matrices (Daisy-chained):
- DIN: Digital Pin Z.
- CLK: Digital Pin W.
- CS: Digital Pin V.
- Pushbutton Switches:
- Switch 1 (Mode Change): Digital Pin A.
- Switch 2 (Unit Change): Digital Pin B.
- LEDs:
- LED 1 (Meters/Kilometers): Digital Pin C.
- LED 2 (Feet/Miles): Digital Pin D.

###  Functionality Breakdown:
Mode Selection:

    Use Switch 1 to toggle between two modes:
    Distance Mode: Display the distance to the target.
    Speed Mode: Display the speed of the target. Speed can be calculated based on the difference in distance measurements over time.
    Unit Selection:

Use Switch 2 to toggle between units:
    Distance: Meters/Kilometers (Metric) or Feet/Miles (Imperial).
    Speed: Meters per second (m/s) or Feet per second (ft/s).
    The corresponding LED will light up to indicate the current unit system.

Displaying Data:
    Use the MAX7219 LED matrices to show the calculated distance or speed values. The first matrix can show the number, and the second matrix can show the units (e.g., "m", "ft", "km", "mph").

###  Example Workflow:
Start in Distance Mode using Metric Units (default).
The user presses Switch 1 to toggle to Speed Mode.
Pressing Switch 2 toggles the units between Metric and Imperial (e.g., meters/kilometers to feet/miles).
The LEDs change to indicate the current unit system (e.g., LED 1 for Metric, LED 2 for Imperial).
The MAX7219 matrices continuously display the calculated data (distance in meters or feet, speed in meters per second or feet per second).

###  Arduino Code Overview:
Ultrasonic Sensor: Measure the time between sending and receiving the ultrasonic wave to calculate the distance.
Mode Logic: Use the first switch to alternate between calculating/displaying distance or speed.
Unit Conversion: Based on the selected unit system, convert the distance or speed accordingly.
LED Indicators: Light up the corresponding LED based on the selected unit system.