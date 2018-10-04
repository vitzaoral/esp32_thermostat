#include <Arduino.h>
#include <InternetConnection.h>

InternetConnection connection;

// Connections to APIs are OK
bool apisAreConnected = false;

void initializeInternetConnection()
{
    if (connection.initialize())
    {
        apisAreConnected = connection.initializeBlynk();
    }
}

void setup()
{
    Serial.begin(9600);
    initializeInternetConnection();
}

void loop()
{
    // put your main code here, to run repeatedly:
}