#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <BME280I2C.h>
#include <Wire.h>
// #include <ArduinoJson.h>
// #include "printf.h"

#define CE_PIN 7
#define CSN_PIN 8
// #define DEBUG_SERIAL false
#define PAYLOAD_SIZE 128

const byte pipe[6] = "00001";

struct SensorData
{
    float temperature;
    float humidity;
    float pressure;
};

RF24 radio(CE_PIN, CSN_PIN);
BME280I2C bme;

void setup()
{
    Serial.begin(115200);
    Serial.println("Avvio sensor node");

    radio.begin();
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_MIN);
    radio.openWritingPipe(pipe);
    radio.setPayloadSize(PAYLOAD_SIZE);
    radio.stopListening();

    if (!radio.isChipConnected())
    {
        Serial.println("Radio NOT CONNECTED");
    }

    Wire.begin();
    bme.begin();
}

void loop()
{
    float temp(NAN), hum(NAN), pres(NAN);
    bme.read(pres, temp, hum);
    delay(200);

    SensorData data;
    data.pressure = pres;
    data.humidity = hum;
    data.temperature = temp;

    radio.write(&data, sizeof(data));

#ifdef DEBUG_SERIAL
    Serial.print("Struct data size: ");
    Serial.println(sizeof(data));
#endif

    delay(3000);
}
