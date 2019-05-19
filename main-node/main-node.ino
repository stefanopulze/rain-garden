#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "printf.h"

#define SSD1306_128_32
// #define SSD1306_128_64
#define OLED_RESET A2

#define CE_PIN 4
#define CSN_PIN 3
#define DEBUG_SERIAL true

struct SensorData
{
    float temperature;
    float humidity;
    float pressure;
};

const byte pipe[5] = {'0', '0', '0', '0', '1'};

RF24 radio(CE_PIN, CSN_PIN);
Adafruit_SSD1306 display(OLED_RESET);
SensorData sensorData;

void setup()
{
    Serial.begin(115200);
    printf_begin();
    Serial.println("Avvio nodo master");

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    // // non va a capo ma esce dallo schermo se riga troppo lunga
    display.setTextWrap(false);
    // // 0 luminosità massima, 1 leggermente inferiore
    display.dim(1);
    // svuotaDisplay();

    disegnaSchermataIniziale();

    radio.begin();
    radio.enableAckPayload(); // Allow optional ack payloads
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_HIGH);
    radio.openReadingPipe(1, pipe);
    radio.startListening();
     radio.writeAckPayload(1, 1, 1);

    radio.printDetails();

    if (!radio.isChipConnected())
    {
        // TODO fix with led blink
        Serial.println("Radio NOT CONNECTED");
    }

    disegnaTesto("Setup", "Avviato", "...");
}

void loop()
{
    if (radio.available())
    {
        byte ackByte;

        radio.read(&sensorData, sizeof(sensorData));
        radio.writeAckPayload(1, 0x02, 1);

        disegnaSensorData(sensorData);

        Serial.print("Temperature: ");
        Serial.print(sensorData.temperature);
        Serial.print(" - Humidity: ");
        Serial.print(sensorData.humidity);
        Serial.print(" - Pressure: ");
        Serial.println(sensorData.pressure);
    }
}

void svuotaDisplay()
{
    display.clearDisplay();
    display.display();
}

void disegnaSensorData(SensorData data)
{
    display.clearDisplay();
    display.display();

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10, 5);
    display.print("Temp:");
    display.println(data.temperature);

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10, 15);
    display.print("Humi:");
    display.println(data.humidity);

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10, 25);
    display.print("Press:");
    display.println(data.pressure);

    display.display();
}

void disegnaSchermataIniziale()
{
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10, 5);
    display.println("ENERGY");

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10, 15);
    display.println("MONITOR");

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10, 45);
    display.println("V 0.1.6 - 2018");
    display.display();
}

void disegnaTesto(String line1, String line2, String line3)
{
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10, 5);
    display.println(line1);

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10, 15);
    display.println(line2);

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10, 45);
    display.println(line3);
    display.display();
}