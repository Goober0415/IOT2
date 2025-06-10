/*
 * Project L09_01_SubscribePublish
 * Description: Starter Code
 * Author: Brian Rashap
 * Edited by: Jamie Gavina
 * Date: 10-MAR-2023
 * Date: 04-NOV-2024
 * IoT14
 */
#include "Particle.h"
#include <Adafruit_MQTT.h>
#include "Adafruit_MQTT/Adafruit_MQTT_SPARK.h"
#include "Adafruit_MQTT/Adafruit_MQTT.h"
#include "credentials.h"
#include "math.h"

/************ Global State (you don't need to change this!) ***   ***************/
TCPClient TheClient;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_SPARK mqtt(&TheClient, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/
// Setup Feeds to publish or subscribe
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Subscribe buttonOnOff = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/buttonOnOff");
Adafruit_MQTT_Publish randomNumber = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/randomNumber");
Adafruit_MQTT_Subscribe slidePosition = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/slidePosition");
/************Declare Variables*************/
unsigned int last, lastTime;
int pubValue, buttonState;
const int LED = D7;
const int YELLOW = D16;
bool onOff;
int currentPosition;
int lastPosition;

/************Declare Functions*************/
void MQTT_connect();
bool MQTT_ping();

SYSTEM_MODE(SEMI_AUTOMATIC);

void setup()
{
  Serial.begin(9600);
  waitFor(Serial.isConnected, 10000);

  // Connect to Internet but not Particle Cloud
  WiFi.on();
  WiFi.connect();
  while (WiFi.connecting())
  {
    Serial.printf(".");
  }
  Serial.printf("\n\n");

  // Setup MQTT subscription
  mqtt.subscribe(&buttonOnOff);
  mqtt.subscribe(&slidePosition);
  pinMode(LED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
}

void loop()
{
  MQTT_connect();
  MQTT_ping();

  // this is our 'wait for incoming subscription packets' busy subloop
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(100)))
  {
    if (subscription == &buttonOnOff)
    {
      buttonState = atoi((char *)buttonOnOff.lastread);
      if (buttonState == 0)
      {
        digitalWrite(LED, LOW);
      }
      if (buttonState == 1)
      {
        digitalWrite(LED, HIGH);
      }
    }
    unsigned int currentTime = millis();
    digitalWrite(YELLOW, HIGH);
    currentPosition = atoi((char *)slidePosition.lastread);
    if (subscription != &slidePosition)
    {
      analogWrite(YELLOW, currentPosition);
      lastPosition = currentPosition;
    }

    if ((currentTime - lastTime > 6000))
    {
      if (mqtt.Update())
      {
        pubValue = rand();
        randomNumber.publish(pubValue);
        Serial.printf("Publishing %i \n", pubValue);
      }
      lastTime = millis();
    }
  }
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect()
{
  int8_t ret;

  // Return if already connected.
  if (mqtt.connected())
  {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  while ((ret = mqtt.connect()) != 0)
  { // connect will return 0 for connected
    Serial.printf("Error Code %s\n", mqtt.connectErrorString(ret));
    Serial.printf("Retrying MQTT connection in 5 seconds...\n");
    mqtt.disconnect();
    delay(5000); // wait 5 seconds and try again
  }
  Serial.printf("MQTT Connected!\n");
}

bool MQTT_ping()
{
  static unsigned int last;
  bool pingStatus;

  if ((millis() - last) > 120000)
  {
    Serial.printf("Pinging MQTT \n");
    pingStatus = mqtt.ping();
    if (!pingStatus)
    {
      Serial.printf("Disconnecting \n");
      mqtt.disconnect();
    }
    last = millis();
  }
  return pingStatus;
}