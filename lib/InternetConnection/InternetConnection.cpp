#include "../../src/settings.cpp"
#include <BlynkSimpleEsp32.h>
#include "InternetConnection.h"

Settings settings;

const char *ssid = settings.ssid;
const char *password = settings.password;
const char *blynkAuth = settings.blynkAuth;

// number of attempts to connecting WIFI,API etc.
const int timeout = 10;

// Initialize WiFi connection. Return true if connection is successful.
bool InternetConnection::initialize(void)
{
    if (WiFi.SSID() != ssid)
    {
        WiFi.begin(ssid, password);
        WiFi.persistent(true);
        WiFi.setAutoConnect(true);
        WiFi.setAutoReconnect(true);
    }

    Serial.print("WiFi connecting to: ");
    Serial.println(ssid);
    int i = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        if (i == timeout)
        {
            Serial.println("Timeout on WiFi connection");
            return false;
        }
        i++;
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Wifi signal stregth: ");
    Serial.println(WiFi.RSSI());

    return true;
}

// Initialize connection to Blynk. Return true if connection is successful.
bool InternetConnection::initializeBlynk(void)
{
    Serial.println("WiFi connecting to Blynk");
    // TODO: tohle vola wifi.begin, neda se to nejak obejit?
    Blynk.config(blynkAuth);

    // timeout 3sec
    Blynk.connect(1000);
    Serial.println(Blynk.connected() ? "Blynk connected" : "Timeout on Blynk or no internet connection");
    return Blynk.connected();
}

void InternetConnection::runBlynk(void)
{
    Blynk.run();
}