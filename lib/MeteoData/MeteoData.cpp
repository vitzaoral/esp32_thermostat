#include <MeteoData.h>

// Initialize and get meteorological data
MeteoData::MeteoData()
{
    Adafruit_SHT31 sht31 = Adafruit_SHT31();
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
        Serial.println("°C - outdoor data temperature");
        Serial.print(humidity);
        Serial.println("% - outdoor data humidity");
    }
}

bool MeteoData::dataAreValid(void)
{
    return shtTemperature <= 50.0 && shtTemperature >= 0 &&
           shtHumidity <= 100.0 && shtHumidity >= 0.0;
}
