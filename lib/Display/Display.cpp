#include <Display.h>

// fonts
#define REGULAR_15 "NotoSansRegular15"
#define REGULAR_20 "NotoSansRegular20"
#define REGULAR_25 "NotoSansRegular25"
#define REGULAR_30 "NotoSansRegular30"
#define THIN_10 "NotoSansThin10"
#define BOLD_30 "NotoSansBold30"

// pictures
#define PIC_TEMPERATURE "/temperature.jpg"
#define PIC_HUMIDITY "/humidity.jpg"
#define PIC_BUTTON_PLUS "/button-plus.jpg"
#define PIC_BUTTON_MINUS "/button-minus.jpg"

#define GFXFF 1
#define BACKGROUND_COLOR TFT_BLACK

// temperature/humidity data y offsets
#define DATA_1_OFFSET_Y 0
#define DATA_2_OFFSET_Y 40
#define DATA_3_OFFSET_Y 80
#define DATA_4_OFFSET_Y 120

// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();

// Other components (progress bar)
GfxUi ui = GfxUi(&tft);

// app settings
Settings settingsDisplay;

// Drawing pictures
JpegUtils jpegUtils = JpegUtils(&tft);

void Display::testMissingFiles()
{
    bool font_missing = false;
    String fonts[] = {REGULAR_15, REGULAR_20, REGULAR_25, REGULAR_30, THIN_10, BOLD_30};
    for (int i = 0; i < 6; i++)
    {
        String fontPath = String("/") + fonts[i] + String(".vlw");
        if (SPIFFS.exists(fontPath) == false)
        {
            Serial.print("Missing: ");
            Serial.println(fontPath);
            font_missing = true;
        }
    }

    if (font_missing)
    {
        Serial.println("Font missing in SPIFFS, did you upload it?");
        while (1)
            yield();
    }
    else
        Serial.println("Fonts found OK.");

    // Test missing jpges
    bool jpg_missing = false;
    String pictures[] = {PIC_TEMPERATURE, PIC_HUMIDITY, PIC_BUTTON_PLUS, PIC_BUTTON_MINUS};
    for (int i = 0; i < 4; i++)
    {
        if (SPIFFS.exists(pictures[i]) == false)
        {
            Serial.print("Missing: ");
            Serial.println(pictures[i]);
            jpg_missing = true;
        }
    }

    if (jpg_missing)
    {
        Serial.println("Jpeg file missing in SPIFFS, did you upload it?");
        while (1)
            yield();
    }
    else
        Serial.println("Jpeg found OK.");
}

// Initialize ILI9341 display
// Setup in User_Setup.h -> TFT_eSPI library
void Display::initialize()
{
    tft.begin();
    tft.setRotation(1);

    if (!SPIFFS.begin())
    {
        Serial.println("SPIFFS initialisation failed! Maybe you forgot create file system and loaded files to ESP32.");
        while (1)
            yield(); // Stay here twiddling thumbs waiting
    }

    Serial.println("SPIFFS available!");

    Display::testMissingFiles();
    Display::welcomeScreen(true);
}

void Display::welcomeScreen(bool showLoading)
{
    tft.setTextDatum(MC_DATUM);
    tft.fillScreen(BACKGROUND_COLOR);

    tft.loadFont(BOLD_30);
    tft.setTextColor(TFT_WHITE, BACKGROUND_COLOR);
    tft.drawString("TERMOSTAT", 160, 40, GFXFF);

    if (showLoading)
    {
        tft.setTextColor(TFT_GREEN, BACKGROUND_COLOR);
        tft.loadFont(REGULAR_25);
        tft.drawString("... načítám ...", 160, 150, GFXFF);
    }

    tft.unloadFont();
}

void Display::printTempleate(int offsetY, String description)
{
    tft.setCursor(0, offsetY + 3);
    tft.print(description);
    jpegUtils.drawJpeg(PIC_TEMPERATURE, 95, offsetY + 5);
    jpegUtils.drawJpeg(PIC_HUMIDITY, 220, offsetY + 7);
}

void Display::prepareTemplate()
{
    // clear display
    tft.fillScreen(BACKGROUND_COLOR);
    tft.setTextColor(TFT_WHITE, BACKGROUND_COLOR);
    tft.loadFont(REGULAR_25);

    printTempleate(DATA_1_OFFSET_Y, "Obývák");
    printTempleate(DATA_2_OFFSET_Y, "Ložnice");
    printTempleate(DATA_3_OFFSET_Y, "Kumbál");
    printTempleate(DATA_4_OFFSET_Y, "Venku");

    int offsetY = 160;
    tft.setCursor(0, offsetY + 3);
    tft.print("Topit na:");
    jpegUtils.drawJpeg(PIC_BUTTON_MINUS, 210, offsetY + 5);
    jpegUtils.drawJpeg(PIC_BUTTON_PLUS, 270, offsetY + 5);

    // Remove the font to recover memory used
    tft.unloadFont();
}

void Display::printWifiStatusAndPrepareTemplate()
{
    tft.fillScreen(BACKGROUND_COLOR);

    Display::welcomeScreen(false);
    tft.setTextDatum(MC_DATUM);
    tft.loadFont(REGULAR_20);
    int xPos = 160;

    if (WiFi.status() == WL_CONNECTED)
    {
        tft.drawString(settingsDisplay.version, xPos, 100, GFXFF);
        tft.setTextColor(TFT_GREEN, BACKGROUND_COLOR);
        tft.drawString("WiFi OK", xPos, 140, GFXFF);
        tft.drawString("IP " + WiFi.localIP().toString(), xPos, 170, GFXFF);
        tft.drawString("Signál " + String(WiFi.RSSI()) + "dBi", xPos, 200, GFXFF);
        tft.setTextColor(TFT_ORANGE, BACKGROUND_COLOR);

        delay(3000);
        Display::prepareTemplate();
    }
    else
    {
        tft.setTextColor(TFT_RED, BACKGROUND_COLOR);
        tft.drawString("WiFi připojení se nezdařilo!", xPos, 140, GFXFF);
    }

    tft.unloadFont();
}

void Display::printProgressBar(int percentage)
{
    ui.drawProgressBar(10, 225, 300, 15, percentage, TFT_WHITE, TFT_BLUE);
}

// Return the whole and the decimal number of the number
temperatureParts Display::getTEmperatureParts(float temperature)
{
    temperatureParts parts;
    float temp = temperature - (long)(temperature);
    long p = 1;
    for (int i = 0; i < 1; i++)
        p *= 10;
    long decimal = abs(p * temp);

    parts.temperature = (int)temperature;
    parts.decimal = decimal;
    return parts;
}

void Display::displayMeteoData(float temperature, int humidity, int offsetY)
{
    temperatureParts parts = Display::getTEmperatureParts(temperature);

    // temperature
    tft.loadFont(BOLD_30);
    tft.fillRect(115, offsetY, 90, 40, BACKGROUND_COLOR);
    tft.setCursor(120, offsetY);
    tft.setTextColor(TFT_ORANGE, BACKGROUND_COLOR);
    tft.print(parts.temperature);

    tft.loadFont(REGULAR_20);
    tft.print(".");
    tft.print(parts.decimal);
    tft.print("°C");

    // humidity
    tft.loadFont(BOLD_30);
    tft.fillRect(244, offsetY, 70, 40, BACKGROUND_COLOR);
    tft.setCursor(250, offsetY);
    tft.setTextColor(TFT_CYAN, BACKGROUND_COLOR);
    tft.print(humidity);
    tft.loadFont(REGULAR_20);
    tft.print("%");

    // release memory
    tft.unloadFont();
}

void Display::printSensorsMeteoData()
{
    if (MeteoData::bedroomDataAreValid())
    {
        displayMeteoData(MeteoData::bedroomTemperature, MeteoData::bedroomHumidity, DATA_2_OFFSET_Y);
    }
    if (MeteoData::pantryDataAreValid())
    {
        displayMeteoData(MeteoData::pantryTemperature, MeteoData::pantryHumidity, DATA_3_OFFSET_Y);
    }
    if (MeteoData::outdoorDataAreValid())
    {
        displayMeteoData(MeteoData::outdoorTemperature, MeteoData::outdoorHumidity, DATA_4_OFFSET_Y);
    }
}

void Display::printLocalMeteoData()
{
    if (MeteoData::shtDataAreValid())
    {
        displayMeteoData(MeteoData::shtTemperature, MeteoData::shtHumidity, DATA_1_OFFSET_Y);
    }

    int requiredTemp = EEPROM.read(EEPROM_TARGET_HEATING_TEMPERATURE_ADDRESS);
    Display::prinTargetTemperature(requiredTemp);
}

void Display::prinTargetTemperature(int targetTemperature)
{
    int xPos = 120;
    int yPos = 160;
    tft.setCursor(xPos, yPos);
    tft.loadFont(BOLD_30);
    tft.fillRect(xPos, yPos, 70, 40, BACKGROUND_COLOR);
    tft.setTextColor(TFT_GREENYELLOW, BACKGROUND_COLOR);
    tft.print(targetTemperature);
    tft.print("°C");

    tft.unloadFont();
}

void Display::printHeatingStatus(int color, String message)
{
    tft.fillRect(0, 200, 320, 40, BACKGROUND_COLOR);
    tft.loadFont(BOLD_30);
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(color, BACKGROUND_COLOR);
    tft.drawString(message, 160, 220, 1);

    tft.unloadFont();
}

void Display::checkDisplayClicked()
{
    uint16_t x = 0, y = 0;
    boolean pressed = tft.getTouch(&x, &y);
    if (pressed)
    {
        // plus / minus temperature buttons
        if (y >= 35 && y <= 70)
        {
            if (x >= 200 && x <= 245)
            {
                // temperature minus
                Serial.print(" MINUS ");
                Display::setTargetTemperatureToEEPROMAndDisplay(false);
            }
            else if (x >= 260 && x <= 310)
            {
                // temperature plus
                Serial.print(" PLUS ");
                Display::setTargetTemperatureToEEPROMAndDisplay(true);
            }
        }
        else if (y <= 20 && x >= 75 && x <= 245)
        {
            // heating switch on/off
            Serial.print(" HEATING ON/OFF ");
            Display::setHeatingEnableToEEPROMAndDisplay();
        }

        // Serial.print(String(x));
        // Serial.print(" ");
        // Serial.println(String(y));
    }
}

// TODO: vice stejnych fci, jak definovat na jednom miste??
void setToEEPROM2(int address, int value)
{
    EEPROM.write(address, value);
    EEPROM.commit();
}

void Display::setTargetTemperatureToEEPROMAndDisplay(bool plus)
{
    int actualTemp = EEPROM.read(EEPROM_TARGET_HEATING_TEMPERATURE_ADDRESS);
    int newTemperature = plus ? actualTemp + 1 : actualTemp - 1;

    if (newTemperature <= MAX_TEMPERATURE_FROM_DISPLAY && newTemperature >= MIN_TEMPERATURE_FROM_DISPLAY)
    {
        setToEEPROM2(EEPROM_TARGET_HEATING_TEMPERATURE_ADDRESS, newTemperature);
        setToEEPROM2(EEPROM_TARGET_HEATING_TEMPERATURE_DISPLAY_SET_ADDRESS, 1);
        Display::prinTargetTemperature(newTemperature);
    }
}

void Display::setHeatingEnableToEEPROMAndDisplay()
{
    bool heatingEnabled = EEPROM.read(EEPROM_ENABLED_DISABLED_HEATING_ADDRESS);
    bool newValue = !heatingEnabled;
    setToEEPROM2(EEPROM_ENABLED_DISABLED_HEATING_ADDRESS, newValue ? 1 : 0);
    setToEEPROM2(EEPROM_ENABLED_DISABLED_HEATING_DISPLAY_SET_ADDRESS, 1);

    if (newValue)
    {
        Display::printHeatingStatus(TFT_GREEN, String("ZAPNUTO (čeká)"));
    }
    else
    {
        Display::printHeatingStatus(TFT_RED, String("VYPNUTO (čeká)"));
    }
}
