/* 
 * Project myProject
 * Author: Your Name
 * Date: 
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"

const char *EVENT_NAME = "GetWeatherData";
unsigned int lastTime;
const float lat=35.0045, lon=-106.6465;  //update to your favorite location
float tempC,temp2;

void subscriptionHandler(const char *event, const char *data);

SYSTEM_MODE(AUTOMATIC); //must be in automatic for Particle publish/subscribe
SYSTEM_THREAD(ENABLED);

void setup() {
  Serial.begin(9600);
  waitFor(Serial.isConnected,15000);

  String subscriptionName = String::format("%s/%s/", System.deviceID().c_str(), EVENT_NAME);
  Particle.subscribe(subscriptionName, subscriptionHandler, MY_DEVICES);
  Serial.printf("Subscribing to %s\n", subscriptionName.c_str());
}

void loop() {
    if((millis() - lastTime) > 60000) {
        Serial.printf("\n\nTime = %i\n",millis());
        Particle.publish(EVENT_NAME, "", PRIVATE);
        Particle.publish(EVENT_NAME, String::format("{\"lat\":%0.5f,\"lon\":%0.5f}", lat, lon), PRIVATE);
        lastTime = millis();
    }
}

void subscriptionHandler(const char *event, const char *data) {
    JSONValue outerObj = JSONValue::parseCopy(data);
    JSONObjectIterator iter(outerObj);
    while(iter.next()) {
            Serial.printf("key=%s value=%s\n",(const char *) iter.name(),(const char *) iter.value().toString());
            if(iter.name()=="temp") {
                tempC = iter.value().toDouble();
            }
    }
    Serial.printf("Temperature = %0.2f, %0.2f\n",tempC);
}