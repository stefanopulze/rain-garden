#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <BME280I2C.h>
#include <Wire.h>
#include "LowPower.h"

//define the pins used by the transceiver module
#define SS_PIN 10
#define RESET_PIN 9
#define DIO0_PIN 2
#define SOIL_PIN A2
#define LOW_POWER_ENABLED

BME280I2C bme;

struct SensorData
{
    float temperature;
    float humidity;
    float pressure;
    short soil;
};

void setup()
{
    //initialize Serial Monitor
    Serial.begin(115200);
    Serial.println("LoRa Sender");

    // Setup BME280
    Wire.begin();
    bme.begin();

    //setup LoRa transceiver module
    LoRa.setPins(SS_PIN, RESET_PIN, DIO0_PIN);

    //replace the LoRa.begin(---E-) argument with your location's frequency
    //433E6 for Asia
    //866E6 for Europe
    //915E6 for North America
    Serial.print("Checking SX1276");
    while (!LoRa.begin(866E6))
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println();
    Serial.println("SX1276 Found");
    // Change sync word (0xF3) to match the receiver
    // The sync word assures you don't get LoRa messages from other LoRa transceivers
    // ranges from 0-0xFF
    LoRa.setSyncWord(0xF3);
    Serial.println("LoRa Initializing OK!");
}

void loop()
{
    SensorData data;

    // Soil
    data.soil = analogRead(SOIL_PIN);
    delay(20); // delay in between reads for stability

    // BME
    float temp(NAN), hum(NAN), pres(NAN);
    bme.read(data.pressure, data.temperature, data.humidity);
    delay(20);

    printSensorData(data);

    Serial.print("Sending packet: ");

    //Send LoRa packet to receiver
    LoRa.beginPacket();
    LoRa.write((uint8_t *)&data, sizeof(data));
    // LoRa.print(data.humidity);
    // LoRa.print(data.humidity);
    // LoRa.print(data.pressure);
    // LoRa.print(data.soil);
    LoRa.endPacket();
    Serial.println("OK");
    delay(20);

#ifdef LOW_POWER_ENABLED
    for (int i = 0; i < 10; i++)
    {
        LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    }
#else
    delay(2000);
#endif
}

void printSensorData(SensorData data)
{
    Serial.print("SensorData[");
    Serial.print("temp: ");
    Serial.print(data.temperature);
    Serial.print(", hum: ");
    Serial.print(data.humidity);
    Serial.print(", pressure: ");
    Serial.print(data.pressure);
    Serial.print(", soil: ");
    Serial.print(data.soil);
    Serial.println("]");
}