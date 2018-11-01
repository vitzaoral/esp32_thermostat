#ifndef __JpegUtils_H
#define __JpegUtils_H

#include <TFT_eSPI.h>
#include <JPEGDecoder.h>

class JpegUtils
{
  public:
    JpegUtils(TFT_eSPI *tft);
    void jpegRender(int xpos, int ypos);
    void drawJpeg(const char *filename, int xpos, int ypos);

  private:
    TFT_eSPI *_tft;
};

#endif