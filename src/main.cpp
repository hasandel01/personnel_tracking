#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <FS.h>
#include <vector>
#include "Employee.h"

using namespace std;

const int RSSI_AT_ONE_METER = -40;
const float PATH_LOSS_EXPONENT = 2.0;
int scanTime = 5; // In seconds
BLEScan* pBLEScan;
const short LEDPIN = 32;
vector<Employee> employees;

float rssiToDistance(int rssi) {
    return pow(10, (RSSI_AT_ONE_METER - rssi) / (10 * PATH_LOSS_EXPONENT));
}

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        String macAddress = advertisedDevice.getAddress().toString().c_str();
        
        for (int i = 0; i < employees.size(); i++) {
            if (macAddress.equalsIgnoreCase(employees[i].getMacAddress().c_str())) {

                if (advertisedDevice.getRSSI() > -50) {
                    Serial.printf("Target device is found with RSSI: %d\n", advertisedDevice.getRSSI());
                    Serial.printf("Name: %s\n", employees[i].getName().c_str());
                    Serial.printf("Estimated Distance: %.2f meters\n", rssiToDistance(advertisedDevice.getRSSI()));
                    digitalWrite(LEDPIN, HIGH);
                    delay(1000);
                } else {
                    digitalWrite(LEDPIN, LOW);
                    delay(1000);
                }
            }
        }
    }
};


void saveEmployees() {
    File file = SPIFFS.open("/employees.json", "r");
    if (!file) {
        Serial.println("Failed to open file for reading");
        return;
    }

    JsonDocument doc;

    DeserializationError error = deserializeJson(doc, file);
    if (error) {
        Serial.println("Failed to read file, using default configuration");
        Serial.println(error.c_str());
        file.close();
        return;
    }

    file.close();

    JsonArray jsonArray = doc.as<JsonArray>();
    for (JsonObject employeeObj : jsonArray) {
        const char* name = employeeObj["name"];
        const char* mac_address = employeeObj["mac_address"];
        employees.push_back(Employee(name, mac_address));
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("Initializing...");
    BLEDevice::init("");
    pinMode(LEDPIN, OUTPUT);
    if (!SPIFFS.begin(true)) {
        Serial.println("An error has occurred while mounting SPIFFS");
        return;
    }
    saveEmployees();
    Serial.println("Setup complete, starting scan...");
}

void loop() {
    // Perform scan every `scanTime` seconds
    pBLEScan = BLEDevice::getScan(); // Create new scan
    if (pBLEScan == nullptr) {
        Serial.println("Failed to create BLE scan object.");
        return;
    }
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks()); // Anonymous constructor
    pBLEScan->setActiveScan(true); // Active scan uses more power, but gets results faster
    Serial.println("Starting BLE scan...");
    pBLEScan->start(scanTime, false);
    delay(scanTime * 1000);
}