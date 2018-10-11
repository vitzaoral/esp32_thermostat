#include <Thermostat.h>

int timer = 0;

void Thermostat::initialize()
{
    pinMode(relayPinAddress, OUTPUT);
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
                if (digitalRead(relayPinAddress) == LOW)
                {
                    digitalWrite(relayPinAddress, HIGH);
                    status = {(char *)"Heating ON", (char *)"#00FF00", true};

                    // start heating, set timer to 0 seconds
                    timer = 0;
                }
                else
                {
                    // TODO do nejake constanty tech 10 sekund
                    // TODO dodat nejaky max count po ktery muze topit, treba 30min apod.
                    // heating is ON, add 10 seconds according to readMetheoDataDisplayDataControllThermostatInterval in main.cpp
                    timer += 10;
                    status = {(char *)"Heating ON", (char *)"#00FF00", true};
                }
            }
            else
            {
                // if heating is ON, should turn OFF
                if (digitalRead(relayPinAddress) == HIGH)
                {
                    // look to timer, turn off after minute
                    if (timer >= heatingBreakTimeInSeconds)
                    {
                        digitalWrite(relayPinAddress, LOW);
                        status = {(char *)"Heating OFF", (char *)"#FF0000", false};
                        timer = 0;
                    }
                    else
                    {
                        // count to 1 minute
                        timer += 10;
                        status = {(char *)"Heating (waiting to OFF)", (char *)"#00FF00", true};
                    }
                }
                else
                {
                    // heating was OFF
                    digitalWrite(relayPinAddress, LOW);
                    status = {(char *)"Heating OFF", (char *)"#FF0000", false};
                    timer = 0;
                }
            }
        }
        else
        {
            digitalWrite(relayPinAddress, LOW);
            status = {(char *)"Heating not enabled", (char *)"#FF0000", false};
            timer = 0;
        }
    }
    else
    {
        digitalWrite(relayPinAddress, LOW);
        status = {(char *)"Temperature data are invalid, heating OFF", (char *)"#FF0000", false};
        timer = 0;
    }

    return status;
}