#ifndef __MeteoData_H
#define __MeteoData_H

#include <Adafruit_SHT31.h>

class MeteoData
{
  public:
    MeteoData();
    float shtTemperature;
    float shtHumidity;

    void setData(void);
    bool dataAreValid(void);
};

#endif
