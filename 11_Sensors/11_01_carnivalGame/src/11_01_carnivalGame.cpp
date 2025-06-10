/*
 * Project Carnival Game
 * Author: Jamie Gavina
 * Date:
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

#include "Particle.h"
#include "neopixel.h"

// Define the pins
const int PIEZO_PIN = A0;
const int TOTAL_PIXELS = 30;

int maxValue;
int pValue, m, voltage, i;
float pixelNum;

Adafruit_NeoPixel pixel(TOTAL_PIXELS, SPI1, WS2812B);

SYSTEM_MODE(SEMI_AUTOMATIC);

void setup()
{
  Serial.begin(9600);
  waitFor(Serial.isConnected, 10000);

  pixel.begin();
  pixel.show();
  delay(50);
  pixel.clear();
  pinMode(PIEZO_PIN, INPUT);
}

void loop()
{
  maxValue = 0;
  pixel.clear();
  pixel.show();
  for (i = 0; i < 100; i++)
  {
    int pValue = analogRead(PIEZO_PIN);
    // Serial.printf("pValue: %i\n", pValue);
    // Serial.printf("i = %i\n", i);
    if (pValue > maxValue)
    {
      maxValue = pValue;
      // Serial.printf("maxValue: %i\n", maxValue);
    }
    if (i == 99)
    {
      voltage = (pValue * 3.3) / 4096.00;
      pixelNum = voltage * 10;

      // Serial.printf("number of pixels %0.1f \n", pixelNum);
      for (int i = 0; i < pixelNum; i++)
      {
        pixel.setPixelColor(i, 0, 0, 255);
        pixel.setBrightness(10);
        pixel.show();
        delay(50);
      }
      pixel.clear();
    }
  }
}