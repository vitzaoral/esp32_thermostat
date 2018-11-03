#ifndef __InternetConnection_H
#define __InternetConnection_H

#include <ArduinoOTA.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <MeteoData.h>
#include <EEPROM.h>
#include <Display.h>

// address 1 - enable/disable heating
#define EEPROM_ENABLED_DISABLED_HEATING_ADDRESS 1
// address 2 - target temperature
#define EEPROM_TARGET_HEATING_TEMPERATURE_ADDRESS 2

class InternetConnection
{
public:
  bool initialize();
  bool initializeBlynk(void);
  void initializeOTA(void);
  void handleOTA(void);
  bool sendDataToBlynk();
  void setOutdoorMeteoData();
  void setBedroomMeteoData();
  void setPantryMeteoData();
  void runBlynk();
  static void setStatusToBlynk(String, int);
  static void setIsHeatingToBlynk(bool);

private:
  float getFloatFromBlynkUrl(String, int);
};

#endif