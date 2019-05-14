#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <ArduinoJson.h>

#define CE_PIN 9
#define CSN_PIN 10
#define DEBUG_SERIAL true

const uint64_t pipe = 0xE8E8F0F0E1LL;

RF24 radio(CE_PIN, CSN_PIN);

void setup()
{
    Serial.begin(115200);

    radio.begin();
    radio.openReadingPipe(0, pipe);
    radio.startListening();
}

void loop()
{
    if (radio.available())
    {
        unsigned char buffer[1024];
        radio.read(buffer, sizeof(buffer));

        DynamicJsonDocument json(1024);
        deserializeJson(json, buffer);

        //serializeJson(json, &Serial);
    }
}
