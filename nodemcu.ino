#define BLYNK_TEMPLATE_ID "TMPL6pvilBnMW"
#define BLYNK_TEMPLATE_NAME "smart home"
#define BLYNK_AUTH_TOKEN "QWz6ra-H40xdQLgqq0BJPsa2dlApzqvN"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Keypad.h>

// WiFi and Blynk
char auth[] = "QWz6ra-H40xdQLgqq0BJPsa2dlApzqvN";
char ssid[] = "free";
char pass[] = "password";

// Pin Definitions for NodeMCU
#define DOOR_LOCK_RELAY_PIN D3  // GPIO0

// Keypad Configuration
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {D0, D1, D2, D4};    
byte colPins[COLS] = {D5, D6, D7, D8};    
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Door Lock Variables
String enteredCode = "";
String correctCode = "1111";
int incorrectAttempts = 0;
bool doorActive = false;

// Timing
unsigned long lockTimer = 0;
const unsigned long LOCK_DURATION = 5000; // 5 seconds unlock time

void setup() {
    pinMode(DOOR_LOCK_RELAY_PIN, OUTPUT);
    digitalWrite(DOOR_LOCK_RELAY_PIN, LOW);  

    Blynk.begin(auth, ssid, pass);
}

void loop() {
    Blynk.run();  

    char key = keypad.getKey();

    if (key) {
        if (key == '#') {
            checkPassword();
        }
        else if (key == '*') {
            enteredCode = "";
        }
        else if (enteredCode.length() < 4) {
            enteredCode += key;
        }
    }

    if (doorActive && (millis() - lockTimer >= LOCK_DURATION)) {
        resetDoor();
    }

    delay(50);
}

// Blynk Virtual Pin V4 - Door Unlock Button
BLYNK_WRITE(V4) {
    int buttonState = param.asInt();
    if (buttonState == 1) {
        activateDoor();
    }
}

void checkPassword() {
    if (enteredCode == correctCode) {
        activateDoor();
        incorrectAttempts = 0;
    } else {
        incorrectAttempts++;
        if (incorrectAttempts >= 3) {
            delay(5000);  // lock system temporarily
            incorrectAttempts = 0;
        }
    }
    enteredCode = "";
}

void activateDoor() {
    doorActive = true;
    lockTimer = millis();
    digitalWrite(DOOR_LOCK_RELAY_PIN, HIGH);  
}

void resetDoor() {
    doorActive = false;
    digitalWrite(DOOR_LOCK_RELAY_PIN, LOW);   
}
