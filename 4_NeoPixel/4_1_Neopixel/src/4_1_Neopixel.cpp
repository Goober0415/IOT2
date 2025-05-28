/* 
 * Project l04_01_neoPixel
 * Author: Jamie Gavina
 * Date: 2/03/2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */


#include "Particle.h"
#include <neopixel.h>

SYSTEM_MODE(SEMI_AUTOMATIC);

const int PIXELCOUNT = 16;
const int DELAY = 50;

Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B);

void setup() {
 pixel.begin();
 pixel.setBrightness(10);
 pixel.clear();
}


void loop()
{
  for (int i = 0; i < PIXELCOUNT; i++)
  {
    pixel.setPixelColor(i, 0, 45, 0); 
    pixel.show();
    delay(DELAY);
  }

  pixel.clear();
  pixel.show();
}
