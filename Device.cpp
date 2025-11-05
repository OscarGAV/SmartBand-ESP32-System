#include "Device.h"
#include <Wire.h>

Device::Device() 
  : edgeServiceClient(),
    pulseSimulator() {
    heartRateLog = HeartRateLog::getInstance();
}

void Device::setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n\n");
    Serial.println("========================================");
    Serial.println("Smart Band Edge Device - Starting");
    Serial.println("========================================");
    Serial.println("Initializing components...");
    
    Wire.begin(SDA_PIN, SCL_PIN);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    
    Serial.println("I2C initialized");
    Serial.println("LED pin configured");
    
    edgeServiceClient.connectWifi();
    edgeServiceClient.setSmartBandId(SMART_BAND_ID);
    
    pulseSimulator.begin(PULSE_PIN);
    Serial.println("Pulse sensor initialized");
    
    Serial.println("========================================");
    Serial.println("Device Ready - Starting measurements");
    Serial.println("========================================\n");
}

void Device::loop() {
    float voltage = pulseSimulator.readVoltage();
    int heartRate = calculateHeartRate(voltage);

    Serial.println("----------------------------------------");
    Serial.print("Voltage Reading: ");
    Serial.print(voltage, 3);
    Serial.println(" V");
    Serial.print("Calculated Heart Rate: ");
    Serial.print(heartRate);
    Serial.println(" bpm");
    
    sendDataToBackend(heartRate);
    controlLed(heartRate);
    
    delay(500);
}

void Device::sendDataToBackend(int heartRate) {
    edgeServiceClient.sendHeartRateData(heartRate);
}

int Device::calculateHeartRate(float voltage) {
    int heartRate = (voltage / 3.3) * 675;
    return heartRate;
}

void Device::controlLed(int heartRate) {
    if (heartRate < MIN_HEART_RATE || heartRate > MAX_HEART_RATE) {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("LED STATUS: ON - Heart rate out of normal range");
    } else {
        digitalWrite(LED_PIN, LOW);
        Serial.println("LED STATUS: OFF - Heart rate normal");
    }
}