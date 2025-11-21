/*
 * Project highStriker
 * Author: Jamie Dowden-Duarte
 * Date: 11/6/2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "neopixel.h"
#include "colors.h"

int piezoVal, lastPiezo, maxStrike;
const int PIEZOPIN = A0;
const float PIXELCOUNT = 46;
float strikeVolt;
const float MAXPIEZO = 4095.0;
int neoPos;
float startPixel, endPixel, currentTime, lastSecond;
int i, j;

void pixelFill(int startPixel, int endPixel, int color);

Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B);

SYSTEM_MODE(AUTOMATIC);

void setup()
{
  pinMode(PIEZOPIN, INPUT);
  Serial.begin(9600);
  waitFor(Serial.isConnected, 10000);

  pixel.begin();
  pixel.setBrightness(25);
  pixel.show();
}

void loop()
{
  maxStrike = 0;
  lastSecond = millis();

  delay(100);

  while ((millis() - lastSecond) < 1000)
  {
    piezoVal = analogRead(PIEZOPIN);

    if (piezoVal > maxStrike)
    {
      maxStrike = piezoVal;
      startPixel = 0;
      endPixel = (maxStrike * (PIXELCOUNT / MAXPIEZO));
    }

    Serial.printf("End Pixel= %f, Max Strike=%i\n", endPixel, maxStrike);
  }
  pixelFill(startPixel, endPixel, teal);
  //  Serial.printf("Piezo Reading= %i, Piezo Voltage= %f\n",piezoVal, strikeVolt);
}

void pixelFill(int startPixel, int endPixel, int color)
{
  for (j = startPixel; j <= endPixel; j++)
  {
    pixel.setPixelColor(j, color);
    pixel.show();
    delay(150);
  }
  delay(200);
  pixel.clear();
  pixel.show();
}
