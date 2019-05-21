#include <ESP8266WiFi.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <InfluxDb.h>

#define DEBUG_SERIAL true
#define PRINT_INFO

#define SSD1306_128_64
#define OLED_RESET 2

#define CE_PIN D2
#define CSN_PIN D8

#define INIFLUXDB
#define INFLUXDB_HOST "192.168.3.20"
#define INFLUXDB_DATABASE "garden"

struct SensorData
{
  float temperature;
  float humidity;
  float pressure;
  uint8_t sensorId;
  //double batteryVolt;
  //long batteryVcc;
};

// -----
// WI FI
// -----
const char *ssid = "<WIFI_SSID>";
const char *password = "<WIFI_PASSWORD>";

const byte pipe[5] = {'0', '0', '0', '0', '1'};

//const uint64_t slaveAddress[] = {0xA};

RF24 radio(CE_PIN, CSN_PIN);
Adafruit_SSD1306 display(OLED_RESET);
Influxdb influx(INFLUXDB_HOST);
SensorData sensorData;


void sensorDataPrintTo(SensorData data, Print &out)
{
    out.print("Sensor Data [");
    out.print("id: ");
    out.print(data.sensorId);
    out.print(", temp: ");
    out.print(data.temperature);
    out.print(", hum: ");
    out.print(data.humidity);
    out.print(", pres: ");
    out.print(data.pressure);
    out.println("]");
}
