#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <ArduinoJson.h>

// Wemos D1
// #define CE_PIN D2
// #define CSN_PIN D8

#define CE_PIN 4
#define CSN_PIN 3
#define DEBUG_SERIAL true
#define PAYLOAD_SIZE 128

struct SensorData
{
    float temperature;
    float humidity;
    float pressure;
};

const byte pipe[6] = "00001";

RF24 radio(CE_PIN, CSN_PIN);
SensorData sensorData;

void setup()
{
    Serial.begin(115200);
    Serial.println("Avvio nodo master");

    radio.begin();
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_MIN);
    radio.openReadingPipe(1, pipe);
    radio.setPayloadSize(PAYLOAD_SIZE);
    radio.startListening();

    if (!radio.isChipConnected())
    {
        // TODO fix with led blink
        Serial.println("Radio NOT CONNECTED");
    }

    Serial.println("Avviato");
}

void loop()
{
    if (radio.available())
    {
        radio.read(&sensorData, sizeof(sensorData));

        Serial.print("Temperature: ");
        Serial.print(sensorData.temperature);
        Serial.print(" - Humidity: ");
        Serial.print(sensorData.humidity);
        Serial.print(" - Pressure: ");
        Serial.println(sensorData.pressure);
    }
}
