void setupDisplay() {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.setTextWrap(false);
    display.dim(1);
    display.clearDisplay();
}