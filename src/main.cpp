#include <Arduino.h>

#define MG_SWITCH 2
#define BUZZER 6
#define VAR_RESISTOR A0
#define FULL_ANGLE 300
#define ADC_REF 5

unsigned long lastTimeMagnetDetected;
const unsigned long minDetectionInterval = 5000;// 5 seconds in milliseconds
const unsigned long maxDetectionInterval = 60000; // 1 minute in milliseconds 

void pinsInit() {
  pinMode(MG_SWITCH, INPUT);
  pinMode(VAR_RESISTOR, INPUT);
  pinMode(BUZZER, OUTPUT);
}

bool isNearMagnet(){
  int sensorVal = digitalRead(MG_SWITCH);
  if (sensorVal == HIGH){
    return true;
  }
  return false;
}

void setup() {
  Serial.begin(9600);
  pinsInit();
  lastTimeMagnetDetected = millis(); // Initialize with current time
}

float getAngle(){
  float voltage;
  int sensor_value = 5 - analogRead(VAR_RESISTOR);
  voltage = (float) sensor_value * ADC_REF / 1023;
  float degrees = (((voltage * FULL_ANGLE) / ADC_REF) + 298.53)/300;
  // float degrees = (voltage * FULL_ANGLE) / ADC_REF;
  return degrees;
}

unsigned long detectionInterval() {
    return minDetectionInterval + getAngle() * (maxDetectionInterval - minDetectionInterval);
}

void loop() {
  if (isNearMagnet()) {
    // If the magnet is detected, turn the buzzer off and update the last detection time
    digitalWrite(BUZZER, LOW);
    lastTimeMagnetDetected = millis();
  } else {
    // If the magnet is not detected, check if 10 seconds have passed
    if (millis() - lastTimeMagnetDetected >= detectionInterval()) {
      // If 10 seconds have passed without detecting the magnet, turn the buzzer on
      digitalWrite(BUZZER, HIGH);
    }
  }
}

