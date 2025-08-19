#include <Servo.h>

// Pin Definitions
#define IR_SENSOR_PIN 5
#define RAIN_SENSOR_PIN 2
#define IR_SERVO_PIN 9
#define RAIN_SERVO_PIN 10

// Servo Motors
Servo irServo;        // For IR sensor (garage door)
Servo rainServo;      // For rain sensor (rain cover)

// Timing Variables for IR Sensor
unsigned long detectionTime = 0;
bool objectDetected = false;
bool servoMoved = false;

// Rain Sensor Variables
bool rainDetected = false;

void setup() {
    pinMode(IR_SENSOR_PIN, INPUT);
    pinMode(RAIN_SENSOR_PIN, INPUT);

    irServo.attach(IR_SERVO_PIN);
    rainServo.attach(RAIN_SERVO_PIN);

    // Set servos to initial positions
    irServo.write(0);      // IR servo at 0 degrees
    rainServo.write(0);    // Rain servo at 0 degrees (open position)
}

void loop() {
    // Read sensor values
    int irValue = digitalRead(IR_SENSOR_PIN);
    int rainValue = digitalRead(RAIN_SENSOR_PIN);

    // === IR SENSOR LOGIC ===
    if (irValue == LOW && !objectDetected) {
        objectDetected = true;
        servoMoved = true;
        detectionTime = millis();
        irServo.write(90);  // Rotate to 90 degrees
    }

    if (servoMoved && (millis() - detectionTime >= 5000)) {
        irServo.write(0);   // Return to 0 degrees
        servoMoved = false;
        objectDetected = false;
    }

    // === RAIN SENSOR LOGIC ===
    if (rainValue == LOW && !rainDetected) {
        rainDetected = true;
        rainServo.write(180);  // Close rain cover
    }
    else if (rainValue == HIGH && rainDetected) {
        rainDetected = false;
        rainServo.write(0);    // Open rain cover
    }

    delay(50);  // Small delay for stability
}
