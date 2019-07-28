void printSensorData(SensorData data);

void setup()
{
    Serial.begin(115200);
    Serial.println("Avvio nodo master");

    // Display
    setupDisplay();

#ifdef INFLUXDB_ENABLE
    displayBootInfo("Connecting...");
    // Wifi
    setupWifiConnection();
    displayBootInfo(WiFi.localIP().toString());

    // Influxdb
    setupInfluxdb();
#else
    displayBootInfo("Boot completed");
#endif

    setupNtp();

    setupLora();

    // disegnaTesto("Setup", "Avviato", "...");
}

void loop()
{
    while (!timeClient.update())
    {
        timeClient.forceUpdate();
    }

    // // try to parse packet
    int packetSize = LoRa.parsePacket();
    if (packetSize)
    {
        SensorData data;

        // received a packet
        Serial.print("Receving LoRa packet with size: ");
        Serial.print(packetSize);

        // read packet
        LoRa.readBytes((uint8_t *)&data, packetSize);

        Serial.print(" with RSSI: ");
        Serial.println(LoRa.packetRssi());

        displaySensorData(&data, LoRa.packetRssi());

#ifdef PRINT_DATA_INFO
        printSensorData(data);
#endif

#ifdef INFLUXDB_ENABLE
        sendToInflux(&data);
#endif
    }
}

void printSensorData(SensorData data)
{
    Serial.print("SensorData[");
    Serial.print("temp: ");
    Serial.print(data.temperature);
    Serial.print(", hum: ");
    Serial.print(data.humidity);
    Serial.print(", pressure: ");
    Serial.print(data.pressure);
    Serial.print(", soil: ");
    Serial.print(data.soil);
    Serial.println("]");
}