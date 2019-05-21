void setup()
{
    Serial.begin(115200);
    Serial.println("Avvio nodo master");

    // Display
    setupDisplay();

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
        sensorDataPrintTo(sensorData, Serial);
#endif

#ifdef INIFLUXDB
        sendToInflux(&sensorData);
#endif
    }
}