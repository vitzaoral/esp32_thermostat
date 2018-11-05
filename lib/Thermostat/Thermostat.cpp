#include <Thermostat.h>

int timer = 0;
String oldValue = "";

void Thermostat::initialize()
{
    pinMode(RELAY_PIN, OUTPUT);
}

// Set thermostat ON/OFF and temperature
ThermostatStatus Thermostat::controllThermostat()
{
    ThermostatStatus status;

    if (MeteoData::shtDataAreValid())
    {
        // heating is enabled
        if (EEPROM.read(EEPROM_ENABLED_DISABLED_HEATING_ADDRESS) == true)
        {
            int requiredTemperature = EEPROM.read(EEPROM_TARGET_HEATING_TEMPERATURE_ADDRESS);
            if (requiredTemperature >= 10 && requiredTemperature <= 25 && MeteoData::shtTemperature <= requiredTemperature)
            {
                // if heating is OFF
                if (digitalRead(RELAY_PIN) == LOW)
                {
                    digitalWrite(RELAY_PIN, HIGH);
                    status = {String("TOPÍ SE"), TFT_GREEN, true};

                    // start heating, set timer to 0 seconds
                    timer = 0;
                }
                else
                {
                    // TODO dodat nejaky max count po ktery muze topit, treba 30min apod.
                    // heating is ON, add seconds heating timer
                    timer += CONTROLL_THERMOSTAT_INTERVAL;
                    status = {String("TOPÍ SE"), TFT_GREEN, true};
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
                        status = {String("NETOPÍ SE"), TFT_BLUE, false};
                        timer = 0;
                    }
                    else
                    {
                        // count to 1 minute
                        timer += CONTROLL_THERMOSTAT_INTERVAL;
                        status = {String("VYPNUTÍ ZA ") + String(HEATING_BREAK_TIME_SECONDS - timer) + "s", TFT_YELLOW, true};
                    }
                }
                else
                {
                    // heating was OFF
                    digitalWrite(RELAY_PIN, LOW);
                    status = {String("NETOPÍ SE"), TFT_BLUE, false};
                    timer = 0;
                }
            }
        }
        else
        {
            // heating NOT ENABLED
            digitalWrite(RELAY_PIN, LOW);
            status = {String("VYPNUTO"), TFT_RED, false};
            timer = 0;
        }
    }
    else
    {
        // Temperature data are invalid, heating OFF
        digitalWrite(RELAY_PIN, LOW);
        status = {String("! ŠPATNÁ DATA !"), TFT_RED, false};
        timer = 0;
    }

    // show only change (prevent display blinking)
    if (oldValue != status.message)
    {
        Display::printHeatingStatus(status.color, status.message);
    }

    oldValue = status.message;
    return status;
}