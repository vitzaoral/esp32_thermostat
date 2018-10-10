#include "../../src/settings.cpp"
#include <BlynkSimpleEsp32.h>
#include "InternetConnection.h"

// Blynk Pins:
// V1 - shtTemperature
// V2 - shtHumidity
// V3 - WiFi IP address
// V4 - WiFi rssi

Settings settings;

const char *ssid = settings.ssid;
const char *password = settings.password;
const char *blynkAuth = settings.blynkAuth;

// number of attempts to connecting WIFI,API etc.
const int timeout = 10;

// Enable/disable thermostat, set value to EEPROM to address 1
BLYNK_WRITE(V0)
{
    // param.asInt() ? setToEEPROM(1, true) : setToEEPROM(1, false);
    // InternetConnection::callThermostatControll();
    param.asInt() ? digitalWrite(16, HIGH) : digitalWrite(16, LOW);
    Serial.println("BLYNK_WRITE");
}

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
        delay(1000);
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

bool InternetConnection::sendDataToBlynk(MeteoData meteoData)
{
    if (Blynk.connected())
    {
        Blynk.virtualWrite(V1, meteoData.shtTemperature);
        Blynk.virtualWrite(V2, meteoData.shtHumidity);
        Blynk.virtualWrite(V3, WiFi.localIP().toString());
        Blynk.virtualWrite(V4, WiFi.RSSI());
        return true;
    }
    else
    {
        return false;
    }
}

float InternetConnection::getFloatFromBlynkUrl(String blynkAuth, int virtualBlynkPin)
{
    if ((WiFi.status() == WL_CONNECTED))
    {
        HTTPClient http;
        http.begin("http://blynk-cloud.com:8080/" + blynkAuth + "/get/V" + virtualBlynkPin);
        int httpCode = http.GET();

        if (httpCode == 200)
        {
            String payload = http.getString();
            payload.replace("[", "");
            payload.replace("]", "");
            payload.replace("\"", "");
            http.end();
            return payload.toFloat();
        }
        else
        {
            Serial.print("Error on HTTP request, httpCode: ");
            Serial.println(httpCode);
            http.end();
        }
    }
    return NAN;
}

void InternetConnection::setOutdoorMeteoData(MeteoData &meteoData)
{
    float temperature = getFloatFromBlynkUrl(String(settings.blynkAuthOutdoor), V16);
    float humidity = getFloatFromBlynkUrl(String(settings.blynkAuthOutdoor), V17);
    meteoData.setOutdoorData(temperature, humidity);
}

void InternetConnection::setBedroomMeteoData(MeteoData &meteoData)
{
    float temperature = getFloatFromBlynkUrl(String(settings.blynkAuthBedroom), V1);
    float humidity = getFloatFromBlynkUrl(String(settings.blynkAuthBedroom), V2);
    meteoData.setBedroomData(temperature, humidity);
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