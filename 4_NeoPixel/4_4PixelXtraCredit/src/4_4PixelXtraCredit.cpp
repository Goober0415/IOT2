/* 
 * Project l04_05_pixelCTRLextraCredit
 * Author: Jamie Gavina
 * Date: 2/04/2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

#include "Particle.h"
#include "Colors.h"
#include "neopixel.h"
#include "Button.h"

SYSTEM_MODE(SEMI_AUTOMATIC);

const int PIXELCOUNT = 46;
const int BUTTON = D3;
const int POTENTIOMETER = A2;

Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B);

int start;
int segment = analogRead(POTENTIOMETER)/89;
bool isPressed;
int randomNumber = rainbow[random(0, 6)];

void pixelFill(int startPixel, int endPixel, int color);


void setup()
{
  pixel.begin();
  pixel.setBrightness(10);
}

void loop()
{
  // pixel.clear();
  for (segment = 0; segment < PIXELCOUNT; segment++)
  { // segment starts at zero, less than 7
    start = 7 * segment;
    pixelFill(start, start + 6, rainbow[segment]);
  }
  delay(2000);
  pixel.show();
}

void pixelFill(int startPixel, int endPixel, int color)
{
  int litPixel;
  isPressed = digitalRead(BUTTON);
  for (litPixel = startPixel; litPixel <= endPixel; litPixel++)
  {
    pixel.setPixelColor(litPixel, color);
    pixel.show();
  }

   if (isPressed = true)
  {
    pixel.setPixelColor(litPixel, randomNumber);
    pixel.show();
  }
}
