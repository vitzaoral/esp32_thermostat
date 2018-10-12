#ifndef __InternetConnection_H
#define __InternetConnection_H

#include <ArduinoOTA.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <MeteoData.h>
#include <EEPROM.h>
#include <Display.h>

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
  void runBlynk();
  static void setStatusToBlynk(String, String);
  static void setIsHeatingToBlynk(bool);

private:
  float getFloatFromBlynkUrl(String, int);
};

#endif