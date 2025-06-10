/*
 * Project GPSPublish
 * Author: Jamie Gavina
 * Date: 11/5/2024
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include <Adafruit_MQTT.h>
#include "Adafruit_MQTT/Adafruit_MQTT_SPARK.h"
#include "Adafruit_MQTT/Adafruit_MQTT.h"
#include "credentials.h"
#include "math.h"
#include "JsonParserGeneratorRK.h"

SYSTEM_MODE(SEMI_AUTOMATIC);

/************ Global State (you don't need to change this!) ***   ***************/
TCPClient TheClient;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_SPARK mqtt(&TheClient, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/
// Setup Feeds to publish or subscribe
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish GPS = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/GPS");

/************Declare Variables*************/
unsigned int last, lastTime;
int pubValue;
bool onOff;
int currentPosition;
int lastPosition;
int currentTime = millis();

struct GeoMap
{
  float lat = random(35120749 * 0.000001, 35139015 * 0.000001);
  float lon = random(-106786453 * 0.000001, -106810231 * 0.000001);
};

GeoMap myLoc;
GeoMap locations[13];

/************Declare Functions*************/
void MQTT_connect();
bool MQTT_ping();
void createEventPayLoad(GeoMap);

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

  myLoc.lat = 35.120606;
  myLoc.lon = -106.65818;
}

void loop()
{
  MQTT_connect();
  MQTT_ping();

  // this is our 'wait for incoming subscription packets' busy subloop
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(100)))
  {
    if ((currentTime - lastTime > 10000))
    {
      createEventPayLoad(myLoc);
    }
  }
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void createEventPayLoad(GeoMap) {
  JsonWriterStatic<256> jw; {

    JsonWriterAutoObject obj(&jw);

      jw.insertKeyValue("lat", myLoc.lat);
      jw.insertKeyValue("lon", myLoc.lon);
  }
      GPS.publish(jw.getBuffer());
    
}


bool MQTT_ping() {
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