/*
 * Project myScale
 * Author: Jamie Gavina
 * Date: 11/08/2024
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include <Adafruit_MQTT.h>                     //install
#include "Adafruit_MQTT/Adafruit_MQTT_SPARK.h" // include
#include "Adafruit_MQTT/Adafruit_MQTT.h"       // include
#include "credentials.h"
#include "HX711.h" //install HX711A

/************ Global State (you don't need to change this!) ***   ***************/
TCPClient TheClient;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_SPARK mqtt(&TheClient, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/
// Setup Feeds to publish or subscribe
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Subscribe scale = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/scale");

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);
const int PIN1 = D6, PIN2 = D5, CALFACTOR = 562, SAMPLES = 10;
float rawData;

HX711 myScale(PIN1, PIN2);

/************Declare Functions*************/
void MQTT_connect();
bool MQTT_ping();

void setup()
{
  Serial.begin(9600);
  waitFor(Serial.isConnected, 1000);

  WiFi.connect();

  myScale.set_scale();
  delay(5000);
  myScale.tare();
  myScale.set_scale(CALFACTOR);
}

void loop()
{
  MQTT_connect();
  MQTT_ping();

  rawData = myScale.get_value(SAMPLES);
  delay(5000);
  scale.publish(rawData);

  // Serial.printf("weight: %f\n", weight*-1);
  // Serial.printf("rawdata: %f\n", rawData);
  // Serial.printf("calibration: %f\n",calibration);
  // Serial.printf("offset: %i\n", offset);
}

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