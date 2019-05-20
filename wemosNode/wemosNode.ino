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

// -----
// WI FI
// -----
const char *ssid = "<WIFI_SSID>";
const char *password = "<WIFI_PASSWORD>";

struct SensorData
{
    float temperature;
    float humidity;
    float pressure;
    uint8_t sensorId;
    //double batteryVolt;
    //long batteryVcc;
};

const byte pipe[5] = {'0', '0', '0', '0', '1'};
//const uint64_t slaveAddress[] = {0xA};

RF24 radio(CE_PIN, CSN_PIN);
Adafruit_SSD1306 display(OLED_RESET);
SensorData sensorData;
Influxdb influx(INFLUXDB_HOST);

void setupWifiConnection()
{
    WiFi.begin(ssid, password); // Connect to the network
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println(" ...");

    int i = 0;
    while (WiFi.status() != WL_CONNECTED)
    { // Wait for the Wi-Fi to connect
        delay(1000);
        Serial.print(++i);
        Serial.print(' ');
    }

    Serial.println('\n');
    Serial.println("Connection established!");
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());
}

void setupRadio()
{
    radio.begin();
    radio.enableAckPayload(); // Allow optional ack payloads
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_MIN);

    // TODO read sensors addres from SPIFFS and loop to open pipe
    radio.openReadingPipe(1, pipe);
    radio.startListening();

    if (!radio.isChipConnected())
    {
        // TODO fix with led blink
        Serial.println("Radio NOT CONNECTED");
    }
}

void setupInfluxdb()
{
    influx.setDb(INFLUXDB_DATABASE);
}

#ifdef PRINT_INFO
void sensorDataDebug(SensorData data, Print &out)
{
    out.print("Sensor Data[");
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
#endif

void sendToInflux(SensorData *sensorData);

void setup()
{
    Serial.begin(115200);
    Serial.println("Avvio nodo master");

    // Display
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.setTextWrap(false);
    display.dim(1);
    display.clearDisplay();

    // Wifi
    setupWifiConnection();

    // Influxdb
    setupInfluxdb();

    // Radio
    setupRadio();

    //disegnaTesto("Setup", "Avviato", "...");
}

void loop()
{
    if (radio.available())
    {
        byte ack = 0x1;
        radio.read(&sensorData, sizeof(sensorData));
        radio.writeAckPayload(1, &ack, sizeof(ack));

#ifdef PRINT_INFO
        sensorDataDebug(sensorData, Serial);
#endif

        //disegnaSensorData(sensorData);
#ifdef INIFLUXDB
        sendToInflux(&sensorData);
#endif
    }
}

InfluxData buildInfluxData(String measure, float value)
{
    InfluxData row(measure);
    row.addTag("sensor", "sensor-01");
    row.addValue("value", value);
    return row;
}

void sendToInflux(SensorData *sensorData)
{
    InfluxData temperature = buildInfluxData("temperature", sensorData->temperature);
    influx.prepare(temperature);

    InfluxData humidity = buildInfluxData("humidity", sensorData->humidity);
    influx.prepare(humidity);

    InfluxData pressure = buildInfluxData("pressure", sensorData->pressure);
    influx.prepare(pressure);

    // writes all prepared measurements with a single request into db.
    boolean success = influx.write();
}
