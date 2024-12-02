#include <LedControl.h>
#include <Ultrasonic.h>

// Pin Definitions
const int trigPin = X;
const int echoPin = Y;
const int switchModePin = A;
const int switchUnitPin = B;
const int ledMetric = C;
const int ledImperial = D;

Ultrasonic ultrasonic(trigPin, echoPin);
LedControl lc = LedControl(Z, W, V, 2); // DIN, CLK, CS, number of devices

bool modeSpeed = false;  // false = distance, true = speed
bool metricUnits = true; // true = metric, false = imperial

void setup() {
    pinMode(switchModePin, INPUT_PULLUP);
    pinMode(switchUnitPin, INPUT_PULLUP);
    pinMode(ledMetric, OUTPUT);
    pinMode(ledImperial, OUTPUT);

    // Initialize MAX7219
    lc.shutdown(0, false);
    lc.shutdown(1, false);
    lc.setIntensity(0, 8);
    lc.setIntensity(1, 8);
    lc.clearDisplay(0);
    lc.clearDisplay(1);
}

void loop() {
    long distance = ultrasonic.read();
    if (!modeSpeed) {
        // Display Distance
        displayDistance(distance);
    } else {
        // Calculate and Display Speed
        static long previousDistance = distance;
        static long previousTime = millis();
        long currentTime = millis();
        long deltaTime = currentTime - previousTime;
        
        // Speed = Distance / Time (basic calculation)
        float speed = (distance - previousDistance) / (deltaTime / 1000.0);
        displaySpeed(speed);

        previousDistance = distance;
        previousTime = currentTime;
    }

    // Handle Switch Inputs
    if (digitalRead(switchModePin) == LOW) {
        modeSpeed = !modeSpeed;
        delay(300);  // Debouncing
    }
    
    if (digitalRead(switchUnitPin) == LOW) {
        metricUnits = !metricUnits;
        delay(300);  // Debouncing
    }

    // Update LED Indicators
    digitalWrite(ledMetric, metricUnits ? HIGH : LOW);
    digitalWrite(ledImperial, metricUnits ? LOW : HIGH);
}

void displayDistance(long distance) {
    if (!metricUnits) {
        distance *= 3.28084;  // Convert to feet if imperial
    }
    // Display on LED matrix
    lc.clearDisplay(0);
    lc.setDigit(0, 0, distance % 10, false);
    lc.setDigit(0, 1, (distance / 10) % 10, false);
    // ... Add code to handle large numbers if needed
}

void displaySpeed(float speed) {
    if (!metricUnits) {
        speed *= 3.28084;  // Convert to feet per second if imperial
    }
    // Display on LED matrix
    lc.clearDisplay(0);
    lc.setDigit(0, 0, (int)speed % 10, false);
    lc.setDigit(0, 1, ((int)speed / 10) % 10, false);
}
