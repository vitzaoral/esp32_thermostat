#include "GfxUi.h"

GfxUi::GfxUi(TFT_eSPI *tft)
{
    _tft = tft;
}

void GfxUi::drawProgressBar(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint8_t percentage, uint16_t frameColor, uint16_t barColor)
{
    if (percentage == 0)
    {
        _tft->fillRoundRect(x0, y0, w, h, 3, TFT_BLACK);
    }
    uint8_t margin = 2;
    uint16_t barHeight = h - 2 * margin;
    uint16_t barWidth = w - 2 * margin;
    _tft->drawRoundRect(x0, y0, w, h, 3, frameColor);
    _tft->fillRect(x0 + margin, y0 + margin, barWidth * percentage / 100.0, barHeight, barColor);
}



