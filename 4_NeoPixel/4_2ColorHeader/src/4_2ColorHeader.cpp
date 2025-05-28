/* 
 * Project l04_02_colorHeader
 * Author: Jamie Gavina
 * Date: 2/4/2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

#include "Particle.h"
#include <neopixel.h>
#include <Colors.h>

SYSTEM_MODE(SEMI_AUTOMATIC);

const int PIXELCOUNT = 16;
const int DELAY = 50;

Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B);

void setup() {
  pixel.begin();
  pixel.setBrightness(10);
  pixel.show();
  delay(DELAY);
  pixel.clear();
}

 void loop()
{
  pixel.setPixelColor(0, red);
  pixel.setPixelColor(1, orange);
  pixel.setPixelColor(2, yellow);
  pixel.setPixelColor(3, green);
  pixel.setPixelColor(4, blue);
  pixel.setPixelColor(5, indigo);
  pixel.setPixelColor(6, violet);
  pixel.setPixelColor(7, red);
  pixel.setPixelColor(8, orange);
  pixel.setPixelColor(9, yellow);
  pixel.setPixelColor(10, green);
  pixel.setPixelColor(11, blue);
  pixel.setPixelColor(12, indigo);
  pixel.setPixelColor(13, violet);
  pixel.setPixelColor(14, red);
  pixel.setPixelColor(15, orange);
  pixel.setPixelColor(16, yellow);
  pixel.show();
}
