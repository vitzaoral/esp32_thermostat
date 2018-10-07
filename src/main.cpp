#include <Arduino.h>
#include <InternetConnection.h>
#include <MeteoData.h>
#include <Display.h>
#include <Ticker.h>

InternetConnection connection;
MeteoData meteoData;
Display display;

const int readMeteoDataInterval = 10000;
void readMeteoData();
Ticker timerReadMeteoData(readMeteoData, readMeteoDataInterval);

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
    meteoData.setData();
    display.printMeteoData(meteoData);
}

void startTimers()
{
    timerReadMeteoData.start();
}

void updateTimers()
{
    timerReadMeteoData.update();
}

void setup()
{
    Serial.begin(9600);
    initializeInternetConnection();
    startTimers();
}

void loop()
{
    updateTimers();
    connection.runBlynk();
}