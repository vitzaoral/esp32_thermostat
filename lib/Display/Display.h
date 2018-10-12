#ifndef __Display_H
#define __Display_H

#include <SPI.h>
#include <TFT_eSPI.h>
#include "GfxUi.h"
#include <Free_Fonts.h>
#include <MeteoData.h>
#include <WiFi.h>

class Display
{
  public:
    static void initialize();
    static void welcomeScreen(bool);
    static void printWifiStatus();
    static void printMeteoData();
    static void printProgressBar(int);
};

#endif
