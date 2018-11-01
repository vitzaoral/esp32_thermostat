#include <Arduino.h>
#include <InternetConnection.h>
#include <MeteoData.h>
#include <Thermostat.h>
#include <Display.h>
#include <Ticker.h>
#include <EEPROM.h>

InternetConnection connection;

// intervals in miliseconds
const int readMeteoDataInterval = 10000;
const int readOtherSensorsMeteoDataInterval = 30000;
const int sendDataToBlynkInterval = 60000;

void readMeteoData();
void readOtherSensorsMeteoData();
void sendDataToBlynk();
void controllThermostat();

Ticker timerReadMeteoData(readMeteoData, readMeteoDataInterval);
Ticker timerSendDataToBlynk(sendDataToBlynk, sendDataToBlynkInterval);
Ticker timerReadOtherSensorsMeteoData(readOtherSensorsMeteoData, readOtherSensorsMeteoDataInterval);
Ticker timerControllThermostat(controllThermostat, CONTROLL_THERMOSTAT_INTERVAL * 1000);

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
    // TODO kumbal
    Display::printSensorsMeteoData();
}

void controllThermostat() {
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

void startTimers()
{
    timerReadMeteoData.start();
    timerReadOtherSensorsMeteoData.start();
    timerSendDataToBlynk.start();
    timerControllThermostat.start();
}

void updateTimers()
{
    timerReadMeteoData.update();
    timerReadOtherSensorsMeteoData.update();
    timerSendDataToBlynk.update();
    timerControllThermostat.update();
}

void setup()
{
    // Initialize two bytes: 1. device status (enabled/disabled) and 2. required temperature
    EEPROM.begin(2);
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