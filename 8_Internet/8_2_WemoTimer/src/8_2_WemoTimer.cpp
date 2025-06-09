/*
 * Project WemoTimer
 * Author: Jamie Gavina
 * Date: 10/23/2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

#include "Particle.h"
#include "Button.h"
#include "wemo.h"

SYSTEM_MODE(MANUAL);

const int MYWEMO = 3;
const int WEMOTAKEOVER = 4;
Button BUTTON(D15);
bool wemowrite;
bool buttonState;
int currentTime = millis();
int lastSecond = 10000;

void onOff();

void setup()
{
  Serial.begin(9600);
  waitFor(Serial.isConnected, 1500);

  WiFi.on();
  WiFi.clearCredentials();
  WiFi.setCredentials("IoTNetwork");

  WiFi.connect();
  while (WiFi.connecting())
  {
    Serial.printf(".");
  }
  Serial.printf("\n\n");
  // wemo isn't attached to a pin so no pinMode
}

void loop()
{
  if (BUTTON.isClicked())
  {
    Serial.printf("on/off\n");
    buttonState = !buttonState;
  }
  onOff();
}

void onOff()
{
  if (buttonState == FALSE)
  {
    wemoWrite(MYWEMO, HIGH);
  }
  else
  {
    if ((currentTime - lastSecond) > 20000)
    {
      wemoWrite(MYWEMO, LOW);
    }
  }
}