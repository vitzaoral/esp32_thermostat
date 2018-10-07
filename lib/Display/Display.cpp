#include "Display.h"

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
    // Set text datum to middle centre
    tft.setTextDatum(MC_DATUM);
    tft.fillScreen(TFT_BLACK); // Clear screen
    
    tft.setFreeFont(FF22);     // Select the font
    tft.setTextColor(TFT_BLUE, TFT_BLACK);
    tft.drawString("Vlhkost: " + String(data.shtHumidity) + "%", 160, 60, GFXFF); // Print the string name of the font
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.drawString("Teplota: " + String(data.shtTemperature) + "°C", 160, 120, GFXFF); // Print the string name of the font
    tft.setFreeFont(FSB18); 
    tft.setTextColor(TFT_PINK, TFT_BLACK);
    tft.drawString("Vitame Marusku", 160, 180, GFXFF); // Print the string name of the font
    tft.drawString("v nasem kralovstvi!", 160, 210, GFXFF); // Print the string name of the font
}
