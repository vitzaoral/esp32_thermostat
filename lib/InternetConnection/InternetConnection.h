#ifndef __InternetConnection_H
#define __InternetConnection_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <MeteoData.h>

class InternetConnection
{
public:
  bool initialize(void);
  bool initializeBlynk(void);
  bool sendDataToBlynk(MeteoData);
  void setOutdoorMeteoData(MeteoData &);
  void setBedroomMeteoData(MeteoData);
  //void setStatusToBlynk(bool, String, int);
  void runBlynk();

private:
  float getFloatFromBlynkUrl(String, int);
};

#endif