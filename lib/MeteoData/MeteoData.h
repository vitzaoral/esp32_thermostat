#ifndef __MeteoData_H
#define __MeteoData_H

#include <Adafruit_SHT31.h>

class MeteoData
{
  public:
    MeteoData();
    float shtTemperature;
    float shtHumidity;
    float outdoorTemperature;
    float outdoorHumidity;
    float bedroomTemperature;
    float bedroomHumidity;

    void setData(void);
    void setOutdoorData(float, float);
    void setBedroomData(float, float);
    bool dataAreValid(void);
};

#endif
