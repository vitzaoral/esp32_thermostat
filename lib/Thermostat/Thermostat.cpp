#include <Thermostat.h>

int timer = 0;

void Thermostat::initialize()
{
    pinMode(RELAY_PIN, OUTPUT);
}

// Set thermostat ON/OFF and temperature
ThermostatStatus Thermostat::controllThermostat()
{
    ThermostatStatus status;

    MeteoData::setData();

    if (MeteoData::dataAreValid())
    {
        // heating is enabled
        if (EEPROM.read(1) == true)
        {
            int requiredTemperature = EEPROM.read(2);
            if (requiredTemperature >= 10 && requiredTemperature <= 25 && MeteoData::shtTemperature <= requiredTemperature)
            {
                // if heating is OFF
                if (digitalRead(RELAY_PIN) == LOW)
                {
                    digitalWrite(RELAY_PIN, HIGH);
                    status = {String("Heating ON"), TFT_GREEN, true};

                    // start heating, set timer to 0 seconds
                    timer = 0;
                }
                else
                {
                    // TODO dodat nejaky max count po ktery muze topit, treba 30min apod.
                    // heating is ON, add seconds heating timer
                    timer += CONTROLL_THERMOSTAT_INTERVAL;
                    status = {String("Heating ON"), TFT_GREEN, true};
                }
            }
            else
            {
                // if heating is ON, should turn OFF
                if (digitalRead(RELAY_PIN) == HIGH)
                {
                    // look to timer, turn off after minute
                    if (timer >= HEATING_BREAK_TIME_SECONDS)
                    {
                        digitalWrite(RELAY_PIN, LOW);
                        status = {String("Heating OFF"), TFT_BLUE, false};
                        timer = 0;
                    }
                    else
                    {
                        // count to 1 minute
                        timer += CONTROLL_THERMOSTAT_INTERVAL;
                        status = {String("Heating (waiting to OFF)"), TFT_GREENYELLOW, true};
                    }
                }
                else
                {
                    // heating was OFF
                    digitalWrite(RELAY_PIN, LOW);
                    status = {String("Heating OFF"), TFT_BLUE, false};
                    timer = 0;
                }
            }
        }
        else
        {
            digitalWrite(RELAY_PIN, LOW);
            status = {String("Heating not enabled"), TFT_RED, false};
            timer = 0;
        }
    }
    else
    {
        digitalWrite(RELAY_PIN, LOW);
        status = {String("Temperature data are invalid, heating OFF"), TFT_RED, false};
        timer = 0;
    }

    Display::printHeatingStatus(status.color, status.message);
    return status;
}