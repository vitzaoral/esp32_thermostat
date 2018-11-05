#ifndef __MeteoData_H
#define __MeteoData_H

#include <Adafruit_SHT31.h>

class MeteoData
{
  public:

    static void initialize(void);
    static float shtTemperature;
    static float shtHumidity;
    static float outdoorTemperature;
    static float outdoorHumidity;
    static float bedroomTemperature;
    static float bedroomHumidity;
    static float pantryTemperature;
    static float pantryHumidity;

    static void setData(void);
    static void setOutdoorData(float, float);
    static void setBedroomData(float, float);
    static void setPantryData(float, float);
    static bool shtDataAreValid(void);
    static bool bedroomDataAreValid(void);
    static bool outdoorDataAreValid(void);
    static bool pantryDataAreValid(void);
};

#endif
