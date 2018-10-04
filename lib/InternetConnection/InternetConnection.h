#ifndef __InternetConnection_H
#define __InternetConnection_H

#include <WiFi.h>
//#include <MeteoData.h>

class InternetConnection
{
  public:
    bool initialize(void);
    bool initializeBlynk(void);
    //void sendDataToBlynk(MeteoData);
    //void setStatusToBlynk(bool, String, int);
    void runBlynk();
};

#endif