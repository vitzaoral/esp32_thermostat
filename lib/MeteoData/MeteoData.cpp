#include "MeteoData.h"

Adafruit_SHT31 sht31 = Adafruit_SHT31();

// Initialize and get meteorological data
MeteoData::MeteoData()
{
    if (!sht31.begin(0x44))
    {
        Serial.println("Could not find a valid SHT31X sensor on address 0x44!");
    }
}

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

bool MeteoData::dataAreValid(void)
{
    return shtTemperature <= 50.0 && shtTemperature >= 0 &&
           shtHumidity <= 100.0 && shtHumidity >= 0.0;
}
