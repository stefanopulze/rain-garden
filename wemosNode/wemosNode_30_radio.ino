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