#ifndef __Display_H
#define __Display_H

#include <SPI.h>
#include <TFT_eSPI.h>
#include <Free_Fonts.h>
#include <MeteoData.h>

class Display
{
  public:
    Display();
    void printMeteoData();

  private:
    void setup(void);
};

#endif
