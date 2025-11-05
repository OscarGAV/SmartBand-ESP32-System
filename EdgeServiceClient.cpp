#include "EdgeServiceClient.h"
#include "HeartRateLog.h"
#include "config.h"

EdgeServiceClient::EdgeServiceClient()
  : ssid(WIFI_SSID), password(WIFI_PASSWORD), backendUrl(EDGE_BACKEND_URL), smartBandId(SMART_BAND_ID) {}

void EdgeServiceClient::connectWifi() {
    Serial.println("========================================");
    Serial.println("WiFi Connection Starting");
    Serial.println("========================================");
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        Serial.print(".");
        delay(500);
        attempts++;
    }
    Serial.println();
    
    if(WiFi.status() == WL_CONNECTED) {
        Serial.println("WiFi Connected Successfully");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        Serial.println("========================================");
    } else {
        Serial.println("Failed to connect to WiFi");
        Serial.println("========================================");
    }
}

void EdgeServiceClient::setSmartBandId(int id) {
    smartBandId = id;
    Serial.println("========================================");
    Serial.print("Smart Band ID configured: ");
    Serial.println(smartBandId);
    Serial.println("========================================");
}

void EdgeServiceClient::sendHeartRateData(int heartRate) {
    if (!isConnected()) {
        Serial.println("WARNING: WiFi not connected. Attempting reconnection...");
        connectWifi();
    }

    HeartRateLog* heartRateLog = HeartRateLog::getInstance();
    heartRateLog->setHeartRate(heartRate);
    heartRateLog->setSmartBandId(smartBandId);
    
    String jsonData = "{\"smartBandId\":" + String(heartRateLog->getSmartBandId()) + 
                  ",\"pulse\":\"" + String(heartRateLog->getHeartRate()) + "\"}";

    Serial.println("\n========================================");
    Serial.println("Sending Heart Rate Data to Edge API");
    Serial.println("========================================");
    Serial.print("Backend URL: ");
    Serial.println(backendUrl);
    Serial.print("Smart Band ID: ");
    Serial.println(smartBandId);
    Serial.print("Heart Rate: ");
    Serial.print(heartRate);
    Serial.println(" bpm");
    Serial.print("JSON Payload: ");
    Serial.println(jsonData);
    Serial.println("----------------------------------------");

    client_http.begin(backendUrl);
    client_http.addHeader("Content-Type", "application/json");

    Serial.println("Sending HTTP POST request...");
    int httpCode = client_http.POST(jsonData);

    if (httpCode > 0) {
        String payload = client_http.getString();
        Serial.println("========================================");
        Serial.println("Response from Edge API");
        Serial.println("========================================");
        Serial.print("HTTP Status Code: ");
        Serial.println(httpCode);
        Serial.print("Response Body: ");
        Serial.println(payload);
        Serial.println("========================================\n");
    } else {
        Serial.println("========================================");
        Serial.println("ERROR: Failed to send data");
        Serial.println("========================================");
        Serial.print("Error Code: ");
        Serial.println(httpCode);
        Serial.print("Error Message: ");
        Serial.println(client_http.errorToString(httpCode));
        Serial.println("========================================\n");
    }

    client_http.end();
}

bool EdgeServiceClient::isConnected() {
  return WiFi.status() == WL_CONNECTED;
}