/* 
 * Project l04_03_neoStrip
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
float R = random(0,255);
float G = random(0,255);
float B = random(0,255);
int j;

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
  // Random color up
  for (int i = 0; i < PIXELCOUNT; i++)
  {
    pixel.setPixelColor(i, R, G, B);
    pixel.show();
    delay(DELAY);
    pixel.clear();
  }

  // Random color down
  for (int i = PIXELCOUNT; i >= 0; i--)
  {
    pixel.setPixelColor(i, R, G, B);
    pixel.show();
    delay(DELAY);
    pixel.clear();
  }

  // Pair maize and blue up
  for (int i = 0; i < PIXELCOUNT; i++)
  {
    pixel.setPixelColor(i, maize);
    pixel.show();
    delay(DELAY);
    pixel.clear();
    if (i < PIXELCOUNT - 1)
    {
      pixel.setPixelColor(i + 1, blue);
      pixel.show();
      delay(DELAY);
      pixel.clear();
    }
  }

  // Pair maize and blue down
  for (int i = PIXELCOUNT; i >= 0; i--)
  {
    pixel.setPixelColor(i, maize);
    pixel.show();
    delay(DELAY);
    if (i > 0)
    {
      pixel.setPixelColor(i - 1, blue);
      pixel.show();
      delay(DELAY);
      pixel.clear();
    }
  }

  // Pixels in different directions
  for (int i = 0; i < PIXELCOUNT; i++)
  {
    int j = (PIXELCOUNT - 1) - i;
    pixel.setPixelColor(i, blue);
    pixel.setPixelColor(j, violet);
    pixel.show();
    delay(100);
    pixel.clear();
  }

  // Pixels at different speeds
  for (int i = 0; i < PIXELCOUNT; i++)
  {
    int j = (2 / 3.0) * i;
    //int j = pow(i, 1.2);
    pixel.setPixelColor(i, blue);
    pixel.setPixelColor(j, violet);
    pixel.show();
    delay(100);
    pixel.clear();
  }

  for (int i = 0; i < PIXELCOUNT; i++)
  {
    j = i%7;
    
    pixel.setPixelColor(i, rainbow[j]);
 
  
    pixel.show();
    delay(100);
    j++;
  }

  for( int i = 0; i < PIXELCOUNT; i++){
    j= 0;
    if (j < 7){
      pixel.setPixelColor(i,rainbow[j]);
      pixel.show();
    }else{
      j = 0;
      pixel.setPixelColor(i,rainbow[j]);
      pixel.show();
    }
  }
  // pixel.clear();
  // if (j == 6){
  //   j = 0;
  // }
}