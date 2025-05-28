/* 
 * Project l04_04_pixelFill
 * Author: Jamie Gavina
 * Date: 2/04/2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

#include "Particle.h"
#include <neopixel.h>
#include <Colors.h>

SYSTEM_MODE(SEMI_AUTOMATIC);

const int PIXELCOUNT = 16;
 
Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B);

int start;
int segment;

void pixelFill(int startPixel, int endPixel, int color);

void setup()
{
  pixel.begin();
  pixel.setBrightness(10);
  pixel.show();
}

void loop()
{
  // pixel.clear();
  for (segment = 0; segment < 7; segment++)  // segment starts at zero, less than 7
  { 
    start = 7 * segment;
    pixelFill(start, start + 6, rainbow[segment]);
  }
  delay(2000);
  pixel.show();
}

void pixelFill(int startPixel, int endPixel, int color)
{
  int litPixel;
  for (litPixel = startPixel; litPixel <= endPixel; litPixel++)
  {
    pixel.setPixelColor(litPixel, color);
  }
  pixel.show();
}
