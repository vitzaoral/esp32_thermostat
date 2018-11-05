#include <BlynkSimpleEsp32.h>
#include <InternetConnection.h>

// Blynk Pins:
// V0 - enabled/disabled thermostat
// V1 - shtTemperature
// V2 - shtHumidity
// V3 - WiFi IP address
// V4 - WiFi rssi
// V5 - target temperature setted from/to Blynk app (text)
// V10 - target temperature setted from/to Blynk app (slider)
// V6 - heating status
// V7 - is heating graph value (true / false)

void setToEEPROM(int address, int value)
{
    // address 1 - enable/disable heating
    // address 2 - target temperature
    // address 3 - set target temperature from device to Blynk (temperature was set by click to display)
    EEPROM.write(address, value);
    EEPROM.commit();
}

void callThermostatControllAndSetStatusToBlynk()
{
    ThermostatStatus status = Thermostat::controllThermostat();

    InternetConnection::setStatusToBlynk(status.message, status.color);
    InternetConnection::setIsHeatingToBlynk(status.isHeating);
}

// Enable/disable thermostat, set value to EEPROM to address 1
BLYNK_WRITE(V0)
{
    setToEEPROM(EEPROM_ENABLED_DISABLED_HEATING_ADDRESS, param.asInt());
    callThermostatControllAndSetStatusToBlynk();
}

// Set temperature slider, write back to blynk to confirm show
BLYNK_WRITE(V10)
{
    int requiredTemp = param.asInt();
    InternetConnection::setTargetTemperatureToBlynk(requiredTemp, false);
    Serial.println("Target Temperature is " + String(requiredTemp) + "°C");
    setToEEPROM(EEPROM_TARGET_HEATING_TEMPERATURE_ADDRESS, requiredTemp);
    callThermostatControllAndSetStatusToBlynk();
    Display::prinTargetTemperature(requiredTemp);
}

////////////////////////// INTERNET CONNECTION PART //////////////////////////

Settings settings;

const char *ssid = settings.ssid;
const char *password = settings.password;
const char *blynkAuth = settings.blynkAuth;

// in attemps
const int timeout = 20;

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
        Display::printProgressBar(i * 5);
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Wifi signal stregth: ");
    Serial.println(WiFi.RSSI());

    return true;
}

bool InternetConnection::sendDataToBlynk()
{
    if (Blynk.connected())
    {
        Blynk.virtualWrite(V1, MeteoData::shtTemperature);
        Blynk.virtualWrite(V2, MeteoData::shtHumidity);
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
        http.setReuse(true);
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

void InternetConnection::setOutdoorMeteoData()
{
    float temperature = getFloatFromBlynkUrl(String(settings.blynkAuthOutdoor), V16);
    float humidity = getFloatFromBlynkUrl(String(settings.blynkAuthOutdoor), V17);
    MeteoData::setOutdoorData(temperature, humidity);
}

void InternetConnection::setBedroomMeteoData()
{
    float temperature = getFloatFromBlynkUrl(String(settings.blynkAuthBedroom), V1);
    float humidity = getFloatFromBlynkUrl(String(settings.blynkAuthBedroom), V2);
    MeteoData::setBedroomData(temperature, humidity);
}

void InternetConnection::setPantryMeteoData()
{
    float temperature = getFloatFromBlynkUrl(String(settings.blynkAuthPantry), V1);
    float humidity = getFloatFromBlynkUrl(String(settings.blynkAuthPantry), V2);
    MeteoData::setPantryData(temperature, humidity);
}

// Initialize connection to Blynk. Return true if connection is successful.
bool InternetConnection::initializeBlynk(void)
{
    Serial.println("WiFi connecting to Blynk");
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

// Static method - send message status to Blynk
void InternetConnection::setStatusToBlynk(String status, int color)
{
    String colorValue = "";
    switch (color)
    {
    case TFT_GREEN:
        colorValue = String("#00FF00");
        break;
    case TFT_BLUE:
        colorValue = String("#0080FF");
        break;
    case TFT_YELLOW:
        colorValue = String("#D9E650");
        break;
    case TFT_RED:
        colorValue = String("#FF0000");
        break;
    default:
        break;
    }

    Blynk.virtualWrite(V6, status);
    Blynk.setProperty(V6, "color", colorValue);
}

// From display to Blynk
void InternetConnection::setTargetTemperatureToBlynk(int temperature, bool setSlider)
{
    Blynk.virtualWrite(V5, temperature);
    if (setSlider)
    {
        Blynk.virtualWrite(V10, temperature);
    }
}

// Send isHeating status to Blynk
void InternetConnection::setIsHeatingToBlynk(bool isHeating)
{
    Blynk.virtualWrite(V7, isHeating ? 1 : 0);
}

void InternetConnection::initializeOTA(void)
{
    ArduinoOTA.setHostname(settings.hostNameOTA);
    ArduinoOTA.setPassword(settings.passwordOTA);
    ArduinoOTA.begin();
}

// Run OTA in loop
void InternetConnection::handleOTA(void)
{
    ArduinoOTA.handle();
}