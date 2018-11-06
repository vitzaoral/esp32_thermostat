#include <Arduino.h>
#include <InternetConnection.h>
#include <MeteoData.h>
#include <Thermostat.h>
#include <Display.h>
#include <Ticker.h>
#include <EEPROM.h>

InternetConnection connection;

// intervals in miliseconds
const int readMeteoDataInterval = 28523;
const int readOtherSensorsMeteoDataInterval = 65123;
const int sendDataToBlynkInterval = 60321;
const int checkDisplayClickedInterval = 43;
const int checkWasTemperatureDisplaySetInterval = 15321;
const int checkWasHeatingDisplaySetInterval = 15800;

void readMeteoData();
void readOtherSensorsMeteoData();
void sendDataToBlynk();
void controllThermostat();
void checkDisplayClicked();
void checkWasTemperatureDisplaySet();
void checkWasHeatingDisplaySet();

Ticker timerReadMeteoData(readMeteoData, readMeteoDataInterval);
Ticker timerSendDataToBlynk(sendDataToBlynk, sendDataToBlynkInterval);
Ticker timerReadOtherSensorsMeteoData(readOtherSensorsMeteoData, readOtherSensorsMeteoDataInterval);
Ticker timerControllThermostat(controllThermostat, CONTROLL_THERMOSTAT_INTERVAL * 1000);
Ticker timerCheckDisplayClicked(checkDisplayClicked, checkDisplayClickedInterval);
Ticker timerCheckWasTemperatureDisplaySet(checkWasTemperatureDisplaySet, checkWasTemperatureDisplaySetInterval);
Ticker timerCheckWasHeatingDisplaySet(checkWasHeatingDisplaySet, checkWasHeatingDisplaySetInterval);

// Connections to APIs are OK
bool apisAreConnected = false;

void initializeInternetConnection()
{
    if (connection.initialize())
    {
        apisAreConnected = connection.initializeBlynk();
        connection.initializeOTA();
    }
    Display::printWifiStatusAndPrepareTemplate();
}

void readMeteoData()
{
    MeteoData::setData();
    Display::printLocalMeteoData();
}

void readOtherSensorsMeteoData()
{
    connection.setOutdoorMeteoData();
    connection.setBedroomMeteoData();
    connection.setPantryMeteoData();
    Display::printSensorsMeteoData();
}

void checkWasTemperatureDisplaySet()
{
    if (EEPROM.read(EEPROM_TARGET_HEATING_TEMPERATURE_DISPLAY_SET_ADDRESS) == true)
    {
        int targetTemperature = EEPROM.read(EEPROM_TARGET_HEATING_TEMPERATURE_ADDRESS);
        InternetConnection::setTargetTemperatureToBlynk(targetTemperature, true);
        Serial.print("Temperature setted on display to: ");
        Serial.println(targetTemperature);

        EEPROM.write(EEPROM_TARGET_HEATING_TEMPERATURE_DISPLAY_SET_ADDRESS, 0);
        EEPROM.commit();
    }
}

void checkWasHeatingDisplaySet()
{
    if (EEPROM.read(EEPROM_ENABLED_DISABLED_HEATING_DISPLAY_SET_ADDRESS) == true)
    {
        bool enabled = EEPROM.read(EEPROM_ENABLED_DISABLED_HEATING_ADDRESS);
        InternetConnection::setHeatingEnabledDisabledToBlynk(enabled);
        Serial.print("Heating enabled/disabled setted on display to: ");
        Serial.println(enabled);

        EEPROM.write(EEPROM_ENABLED_DISABLED_HEATING_DISPLAY_SET_ADDRESS, 0);
        EEPROM.commit();
    }
}

void controllThermostat()
{
    ThermostatStatus status = Thermostat::controllThermostat();

    InternetConnection::setStatusToBlynk(status.message, status.color);
    InternetConnection::setIsHeatingToBlynk(status.isHeating);
}

void sendDataToBlynk()
{
    if (apisAreConnected)
    {
        bool successBlynk = connection.sendDataToBlynk();

        if (successBlynk)
        {
            Serial.println("Data was sent to Blynk");
        }
        else
        {
            Serial.println("No internet connection, try initialize connection");
            apisAreConnected = false;
            initializeInternetConnection();
        }
    }
    else
    {
        initializeInternetConnection();
    }
}

void checkDisplayClicked()
{
    Display::checkDisplayClicked();
}

void startTimers()
{
    timerReadMeteoData.start();
    timerReadOtherSensorsMeteoData.start();
    timerSendDataToBlynk.start();
    timerControllThermostat.start();
    timerCheckDisplayClicked.start();
    timerCheckWasTemperatureDisplaySet.start();
    timerCheckWasHeatingDisplaySet.start();
}

void updateTimers()
{
    timerReadMeteoData.update();
    timerReadOtherSensorsMeteoData.update();
    timerSendDataToBlynk.update();
    timerControllThermostat.update();
    timerCheckDisplayClicked.update();
    timerCheckWasTemperatureDisplaySet.update();
    timerCheckWasHeatingDisplaySet.update();
}

void setup()
{
    // Initialize 10 addresses:
    // 1. device status (enabled/disabled)
    // 2. required temperature
    // 3. was setted targe temperature from display
    // 4. was setted enabled/disabled heating from display
    EEPROM.begin(10);
    Serial.begin(9600);

    // initialize components
    Display::initialize();
    MeteoData::initialize();
    Thermostat::initialize();

    // try connect to internet
    initializeInternetConnection();

    // start process in timers
    startTimers();
}

void loop()
{
    updateTimers();
    connection.runBlynk();
    connection.handleOTA();
}