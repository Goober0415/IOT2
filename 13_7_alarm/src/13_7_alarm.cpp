/*
 * Project alarm
 * Author: Jamie Gavina
 * Date: 11/21/2024
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

#include "Particle.h"
#include "Button.h"
#include "Colors.h"
#include "neopixel.h"

const int PIXELCOUNT = 16, STARTPIXEL = 0, ENCSWITCH = D15, HALL = D19;
int hallStatus, pix, lastBlinkTime, blinkMode;
bool onOff;

void pixelFill(int startPixel, int endPixel, int pixColor);
void pixelBlink(int startPixel, int endPixel, int pixColor);
Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B);
Button encButton(ENCSWITCH);

SYSTEM_MODE(AUTOMATIC);

void setup()
{
  Serial.begin(9600);
  waitFor(Serial.isConnected, 10000);

  pixel.begin();
  pixel.setBrightness(35);
  pixel.show();

  pinMode(ENCSWITCH, INPUT_PULLDOWN);
  pinMode(HALL, INPUT);
}

void loop()
{
  hallStatus = digitalRead(HALL);

  if (encButton.isClicked())
  {
    onOff = !onOff;
  }

  if (onOff == 0)
  {
    pixelFill(STARTPIXEL, PIXELCOUNT, blue); // BLUE when alarm disarmed
  }

  if (onOff == 1)
  {
    if (hallStatus == 0)
    {
      pixelFill(STARTPIXEL, PIXELCOUNT, green); // GREEN when armed and magnet detected
    }

    if (hallStatus == 1)
    {
      pixelBlink(STARTPIXEL, PIXELCOUNT, red); // Blinking RED when armed and magnet is not detected
    }
  }
  Serial.printf("Hall Sensor Status %i\n", hallStatus);
}

void pixelFill(int startPixel, int endPixel, int pixColor)
{
  for (pix = startPixel; pix <= endPixel; pix++)
  {
    pixel.setPixelColor(pix, pixColor);
  }
  pixel.show();
}

void pixelBlink(int startPixel, int endPixel, int pixColor)
{
  if ((millis() - lastBlinkTime) > 200)
  {
    blinkMode++;
    if (blinkMode == 3)
    {
      blinkMode = 1;
    }
    lastBlinkTime = millis();
  }

  if ((blinkMode == 1))
  {
    for (pix = startPixel; pix <= endPixel; pix++)
    {
      pixel.setPixelColor(pix, pixColor);
    }
    pixel.show();
  }

  if ((blinkMode == 2))
  {
    for (pix = startPixel; pix <= endPixel; pix++)
    {
      pixel.setPixelColor(pix, 0x000000);
    }
    pixel.show();
  }
}