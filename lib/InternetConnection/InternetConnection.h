#ifndef __InternetConnection_H
#define __InternetConnection_H

#include "../../src/settings.cpp"
#include <ArduinoOTA.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <MeteoData.h>
#include <EEPROM.h>
#include <Display.h>
#include <Thermostat.h>

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
  
  static void setTargetTemperatureToBlynk(int, bool);
  static void setStatusToBlynk(String, int);
  static void setIsHeatingToBlynk(bool);

private:
  float getFloatFromBlynkUrl(String, int);
};

#endif