#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <BME280I2C.h>
#include <Wire.h>
#include <ArduinoJson.h>

#define CE_PIN 9
#define CSN_PIN 10
#define DEBUG_SERIAL true

const uint64_t pipe = 0xE8E8F0F0E1LL;

RF24 radio(CE_PIN, CSN_PIN);
BME280I2C bme;

void setup()
{
    Serial.begin(115200);

    radio.begin();
    radio.openWritingPipe(pipe);

    Wire.begin();
    bme.begin();
}

void loop()
{
    float temp(NAN), hum(NAN), pres(NAN);
    bme.read(pres, temp, hum);

    // json
    DynamicJsonDocument json(1024);
    json["temperature"] = temp;
    json["humidity"] = hum;
    json["pressure"] = pres;

#ifdef DEBUG_SERIAL
    serializeJson(json, Serial);
    Serial.println();
#endif

    size_t jsonLength = measureJson(json);
    char buffer[256];
    serializeJson(json, buffer);
    // radio.write(buffer, jsonLength);
}
