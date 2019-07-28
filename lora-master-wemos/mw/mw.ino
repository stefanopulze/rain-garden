#include <ESP8266WiFi.h>
#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <InfluxDb.h>
#include <WiFiUdp.h>

// https://github.com/PaulStoffregen/Time
// #include "TimeLib.h"
#include <NTPClient.h>

// #include <Chrono.h>

#define PRINT_DATA_INFO

// Display
Adafruit_SSD1306 display(128, 64);

// SX1276
#define SS_PIN D8
#define RESET_PIN D4
#define DIO0_PIN D0

// InfluxDb
#define INFLUXDB_ENABLE
#ifdef INFLUXDB_ENABLE
#define INFLUXDB_HOST "192.168.3.90"
#define INFLUXDB_DATABASE "garden"
Influxdb influx(INFLUXDB_HOST);

// WIFI
const char *ssid = "<wifi_seed>";
const char *password = "<wifi_password>";
#endif

// NTP
#define DEBUG_NTPClient
WiFiUDP udp;
NTPClient timeClient(udp);

struct SensorData
{
    float temperature;
    float humidity;
    float pressure;
    short soil;
};