#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <BME280I2C.h>
#include <Wire.h>
#include "printf.h"
// #include <ArduinoJson.h>
// #include "printf.h"

#define CE_PIN 7
#define CSN_PIN 8
#define DEBUG_SERIAL false
#define STATUS_PIN 43

const byte pipe[5] = {'0', '0', '0', '0', '1'};

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
  printf_begin();
  Serial.println("Avvio sensor node");
  pinMode(STATUS_PIN, OUTPUT);
  digitalWrite(STATUS_PIN, LOW);

  radio.begin();
  radio.setAutoAck(true);   // Ensure autoACK is enabled
  radio.enableAckPayload(); // Allow optional ack payloads
  radio.setRetries(3, 5);   // delay, count
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.openWritingPipe(pipe);
  radio.stopListening();

  radio.printDetails();

  if (!radio.isChipConnected())
  {
    Serial.println("Radio NOT CONNECTED");
    digitalWrite(STATUS_PIN, HIGH);
  }

  // radio.printDetails();

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
  byte ackByte;

  bool rslt;
  rslt = radio.write(&data, sizeof(data));
  if (rslt)
  {
    if (radio.isAckPayloadAvailable())
    {
      radio.read(&ackByte, 1);
      Serial.print("Acknowledge received: ");
      Serial.println(ackByte);
    }
    else
    {
      Serial.println("Acknowledge but no data ");
    }

    digitalWrite(STATUS_PIN, LOW);
  }
  else
  {
    Serial.println("Tx failed");
    digitalWrite(STATUS_PIN, HIGH);
  }

#ifdef DEBUG_SERIAL
  Serial.print("Struct data size: ");
  Serial.println(sizeof(data));
#endif

  delay(3000);
}
