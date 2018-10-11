#include <Display.h>

// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();

// Initialize ILI9341 display
// Setup in User_Setup.h -> TFT_eSPI library
Display::Display()
{
    tft.begin();
    tft.setRotation(1);
}

void Display::printMeteoData(MeteoData data)
{
    int backgroundColor = TFT_WHITE;

    // Set text datum to middle centre
    tft.setTextDatum(MC_DATUM);
    tft.fillScreen(backgroundColor); // Clear screen

    tft.setFreeFont(FF1); // Select the font

    tft.setTextColor(TFT_RED, backgroundColor);
    tft.drawString("Teplota: " + String(data.shtTemperature) + "°C", 160, 20, GFXFF); // Print the string name of the font
    tft.setTextColor(TFT_BLUE, backgroundColor);
    tft.drawString("Vlhkost: " + String(data.shtHumidity) + "%", 160, 50, GFXFF); // Print the string name of the font

    tft.setTextColor(TFT_RED, backgroundColor);
    tft.drawString("Teplota loznice: " + String(data.bedroomTemperature) + "°C", 160, 100, GFXFF); // Print the string name of the font
    tft.setTextColor(TFT_BLUE, backgroundColor);
    tft.drawString("Vlhkost loznice: " + String(data.bedroomHumidity) + "%", 160, 130, GFXFF); // Print the string name of the font

    tft.setTextColor(TFT_RED, backgroundColor);
    tft.drawString("Teplota venku: " + String(data.outdoorTemperature) + "°C", 160, 180, GFXFF); // Print the string name of the font
    tft.setTextColor(TFT_BLUE, backgroundColor);
    tft.drawString("Vlhkost venku: " + String(data.outdoorHumidity) + "%", 160, 210, GFXFF); // Print the string name of the font
}
