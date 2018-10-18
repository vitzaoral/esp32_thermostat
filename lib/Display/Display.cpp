#include <Display.h>

// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();

// Other components (progress bar)
GfxUi ui = GfxUi(&tft);

static bool firstClear = true;

// Initialize ILI9341 display
// Setup in User_Setup.h -> TFT_eSPI library
void Display::initialize()
{
    tft.begin();
    tft.setRotation(1);
    Display::welcomeScreen(true);
}

void Display::welcomeScreen(bool showLoading)
{
    int backgroundColor = TFT_BLACK;

    tft.setTextDatum(MC_DATUM);
    tft.fillScreen(backgroundColor); // Clear screen

    tft.setFreeFont(FF23); // Select the font

    tft.setTextColor(TFT_WHITE, backgroundColor);
    tft.drawString("TERMOSTAT", 160, 40, GFXFF);

    if (showLoading)
    {
        tft.setFreeFont(FF17); // Select the font
        tft.drawString("... loading ...", 160, 150, GFXFF);
    }
}

void Display::printWifiStatus()
{
    int backgroundColor = TFT_BLACK;
    tft.fillScreen(backgroundColor);

    Display::welcomeScreen(false);

    if (WiFi.status() == WL_CONNECTED)
    {
        tft.setTextDatum(MC_DATUM);
        tft.setFreeFont(FS9);

        tft.setTextColor(TFT_GREEN, backgroundColor);
        tft.drawString("WiFi OK", 160, 140, GFXFF);
        tft.drawString("IP " + WiFi.localIP().toString(), 160, 160, GFXFF);
        tft.drawString("Signal " + String(WiFi.RSSI()) + "dBi", 160, 180, GFXFF);
    }
}

void Display::printProgressBar(int percentage)
{
    ui.drawProgressBar(10, 225, 300, 15, percentage, TFT_WHITE, TFT_BLUE);
}

void Display::printMeteoData()
{
    int backgroundColor = TFT_WHITE;

    // Set text datum to middle centre
    tft.setTextDatum(MC_DATUM);
    if (firstClear)
    {
        tft.fillScreen(backgroundColor); // Clear screen
        firstClear = false;
    }

    tft.setFreeFont(FF1); // Select the font

    tft.setTextColor(TFT_RED, backgroundColor);
    tft.drawString("Teplota: " + String(MeteoData::shtTemperature) + "°C", 160, 20, GFXFF); // Print the string name of the font
    tft.setTextColor(TFT_BLUE, backgroundColor);
    tft.drawString("Vlhkost: " + String(MeteoData::shtHumidity) + "%", 160, 50, GFXFF); // Print the string name of the font

    tft.setTextColor(TFT_RED, backgroundColor);
    tft.drawString("Teplota loznice: " + String(MeteoData::bedroomTemperature) + "°C", 160, 80, GFXFF); // Print the string name of the font
    tft.setTextColor(TFT_BLUE, backgroundColor);
    tft.drawString("Vlhkost loznice: " + String(MeteoData::bedroomHumidity) + "%", 160, 110, GFXFF); // Print the string name of the font

    tft.setTextColor(TFT_RED, backgroundColor);
    tft.drawString("Teplota venku: " + String(MeteoData::outdoorTemperature) + "°C", 160, 140, GFXFF); // Print the string name of the font
    tft.setTextColor(TFT_BLUE, backgroundColor);
    tft.drawString("Vlhkost venku: " + String(MeteoData::outdoorHumidity) + "%", 160, 170, GFXFF); // Print the string name of the font

    // TODO: nejak poresit define konstanty - do nejakeho souboru a linkovat vsude?
    int requiredTemp = EEPROM.read(2);
    Display::prinTargetTemperature(requiredTemp);
}

void Display::prinTargetTemperature(int targetTemperature)
{
    int backgroundColor = TFT_WHITE;

    // Set text datum to middle centre
    tft.setTextDatum(MC_DATUM);
    tft.setFreeFont(FF1); // Select the font

    tft.setTextColor(TFT_RED, backgroundColor);
    tft.drawString("Nastavena teplota: " + String(targetTemperature) + "°C", 160, 200, GFXFF);
}

void Display::printHeatingStatus(int color, String message)
{
    int backgroundColor = TFT_WHITE;
    // Set text datum to middle centre
    tft.setTextDatum(MC_DATUM);
    tft.setFreeFont(FF1); // Select the font

    tft.setTextColor(color, backgroundColor);
    tft.drawString("Stav: " + message, 160, 230, GFXFF);
}
