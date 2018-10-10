#include <Arduino.h>
#include <InternetConnection.h>
#include <MeteoData.h>
#include <Thermostat.h>
#include <Display.h>
#include <Ticker.h>

InternetConnection connection;
MeteoData meteoData;
Display display;
Thermostat thermostat;

const int readMeteoDataInterval = 10000;
const int readOutdoorMeteoDataInterval = 30000;
const int sendDataToBlynkInterval = 60000;

void readMeteoData();
void readOutdoorMeteoData();
void sendDataToBlynk();
Ticker timerReadMeteoData(readMeteoData, readMeteoDataInterval);
Ticker timerSendDataToBlynk(sendDataToBlynk, sendDataToBlynkInterval);
Ticker timerReadOutdoorMeteoData(readOutdoorMeteoData, readOutdoorMeteoDataInterval);

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

void readOutdoorMeteoData()
{
    connection.setOutdoorMeteoData(meteoData);
    display.printMeteoData(meteoData);
}

void sendDataToBlynk()
{
    if (apisAreConnected)
    {
        bool successBlynk = connection.sendDataToBlynk(meteoData);

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
    timerReadOutdoorMeteoData.start();
    timerSendDataToBlynk.start();
}

void updateTimers()
{
    timerReadMeteoData.update();
    timerReadOutdoorMeteoData.update();
    timerSendDataToBlynk.update();
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