void setupDisplay()
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextWrap(false);
}

void disegnaSchermataIniziale()
{
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 5);
  display.println("RAIN");

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 25);
  display.println("SENSOR");

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 45);
  display.println("V 0.1.1 - 2019");
  display.display();
}

void displayBootInfo(String info)
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 5);
  display.println("RAIN");
  display.setCursor(10, 25);
  display.println("SENSOR");

  display.setTextSize(1);
  display.setCursor(10, 45);
  display.println(info);
  display.setCursor(10, 55);
  display.println("V 0.1.1 - 2019");
  display.display();
}

void displaySensorData(SensorData* data, int rssi) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,5);
  display.print("Temp: ");
  display.println(data->temperature);

  display.setCursor(10,15);
  display.print("Humi: ");
  display.println(data->humidity);

  display.setCursor(10,25);
  display.print("Press: ");
  display.println(data->pressure);

  display.setCursor(10,35);
  display.print("Soil: ");
  display.println(data->soil);

  display.setCursor(10,45);
  display.print("RSSI: ");
  display.println(rssi);

  display.setCursor(10,55);
  display.print("Date: ");
  display.println(timeClient.getFormattedTime());

  display.display();
}