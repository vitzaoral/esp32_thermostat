#include <Arduino.h>
#include <InternetConnection.h>
#include <MeteoData.h>
#include <Thermostat.h>
#include <Display.h>
#include <Ticker.h>
#include <EEPROM.h>

InternetConnection connection;
Display display;

const int readMeteoDataInterval = 10000;
const int readOtherSensorsMeteoDataInterval = 30000;
const int sendDataToBlynkInterval = 60000;

void readMeteoData();
void readOtherSensorsMeteoData();
void sendDataToBlynk();
Ticker timerReadMeteoData(readMeteoData, readMeteoDataInterval);
Ticker timerSendDataToBlynk(sendDataToBlynk, sendDataToBlynkInterval);
Ticker timerReadOtherSensorsMeteoData(readOtherSensorsMeteoData, readOtherSensorsMeteoDataInterval);

// Connections to APIs are OK
bool apisAreConnected = false;

void initializeInternetConnection()
{
    if (connection.initialize())
    {
        apisAreConnected = connection.initializeBlynk();
    }
}

void readMeteoData()
{
    MeteoData::setData();
    display.printMeteoData();
}

void readOtherSensorsMeteoData()
{
    connection.setOutdoorMeteoData();
    connection.setBedroomMeteoData();
    display.printMeteoData();
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
}

void updateTimers()
{
    timerReadMeteoData.update();
    timerReadOtherSensorsMeteoData.update();
    timerSendDataToBlynk.update();
}

void setup()
{
    // Initialize two bytes: 1. device status (enabled/disabled) and 2. required temperature
    EEPROM.begin(2);
    Serial.begin(9600);
    MeteoData::initialize();
    Thermostat::initialize();
    initializeInternetConnection();
    startTimers();
}

void loop()
{
    updateTimers();
    connection.runBlynk();
}