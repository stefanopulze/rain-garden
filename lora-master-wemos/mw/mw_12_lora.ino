void setupLora() {
    Serial.println("Setup LoRa ");
     //setup LoRa transceiver module
    LoRa.setPins(SS_PIN, RESET_PIN, DIO0_PIN);

    //replace the LoRa.begin(---E-) argument with your location's frequency
    //433E6 for Asia
    //866E6 for Europe
    //915E6 for North America
    while (!LoRa.begin(866E6))
    {
        Serial.println(".");
        delay(500);
    }
    // Change sync word (0xF3) to match the receiver
    // The sync word assures you don't get LoRa messages from other LoRa transceivers
    // ranges from 0-0xFF
    LoRa.setSyncWord(0xF3);

    Serial.println("LoRa Initializing OK!");
}