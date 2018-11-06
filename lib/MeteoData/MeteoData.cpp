#include <MeteoData.h>

Adafruit_SHT31 sht31 = Adafruit_SHT31();

// static variables declaration
float MeteoData::shtTemperature;
float MeteoData::shtHumidity;
float MeteoData::outdoorTemperature;
float MeteoData::outdoorHumidity;
float MeteoData::bedroomTemperature;
float MeteoData::bedroomHumidity;
float MeteoData::pantryTemperature;
float MeteoData::pantryHumidity;

// Initialize and get meteorological data
void MeteoData::initialize(void)
{
    if (!sht31.begin(0x44))
    {
        Serial.println("Could not find a valid SHT31X sensor on address 0x44!");
    }
    else
    {
        Serial.println("Sht31 OK");
    }
}

// Set temperature/humidity data from thermostat senzor
void MeteoData::setData(void)
{
    shtTemperature = sht31.readTemperature();
    shtHumidity = sht31.readHumidity();

    if (isnan(shtTemperature))
    {
        Serial.println("Error during read temperature");
    }
    if (isnan(shtHumidity))
    {
        Serial.println("Error during read humidity");
    }

    Serial.print(shtTemperature);
    Serial.println(" °C");
    Serial.print(shtHumidity);
    Serial.println(" %");
}

void MeteoData::setOutdoorData(float temperature, float humidity)
{
    if (temperature && humidity)
    {
        outdoorTemperature = temperature;
        outdoorHumidity = humidity;
        Serial.print(temperature);
        Serial.println("°C - outdoor data temperature");
        Serial.print(humidity);
        Serial.println("% - outdoor data humidity");
    }
}

void MeteoData::setBedroomData(float temperature, float humidity)
{
    if (temperature && humidity)
    {
        bedroomTemperature = temperature;
        bedroomHumidity = humidity;
        Serial.print(temperature);
        Serial.println("°C - bedroom data temperature");
        Serial.print(humidity);
        Serial.println("% - bedroom data humidity");
    }
}

void MeteoData::setPantryData(float temperature, float humidity)
{
    if (temperature && humidity)
    {
        pantryTemperature = temperature;
        pantryHumidity = humidity;
        Serial.print(temperature);
        Serial.println("°C - pantry data temperature");
        Serial.print(humidity);
        Serial.println("% - pantry data humidity");
    }
}

// local sensor data are valid
bool MeteoData::shtDataAreValid(void)
{
    return shtTemperature <= 50.0 && shtTemperature > 0 &&
           shtHumidity <= 100.0 && shtHumidity >= 0.0;
}

// bedroom sensor data are valid
bool MeteoData::bedroomDataAreValid(void)
{
    return bedroomTemperature <= 50.0 && bedroomTemperature > 0 &&
           bedroomHumidity <= 100.0 && bedroomHumidity >= 0.0;
}

// pantry sensor data are valid
bool MeteoData::pantryDataAreValid(void)
{
    return pantryTemperature <= 50.0 && pantryTemperature > 0 &&
           pantryHumidity <= 100.0 && pantryHumidity >= 0.0;
}

// outdoor sensor data are valid
bool MeteoData::outdoorDataAreValid(void)
{
    return outdoorTemperature <= 50.0 && outdoorTemperature >= -40 &&
           outdoorHumidity <= 100.0 && outdoorHumidity >= 0.0;
}
