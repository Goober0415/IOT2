/* 
 * Project webhook
 * Author: Jamie Dowden-Duarte
 * Date: 11/4/2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

#include "Particle.h"
#include "credentials.h"

const char *EVENT_NAME = "TomTomMuseums";
const float LAT=35.17869, LON= -106.56395;

void subscriptionHandler(const char *event, const char *data);

SYSTEM_MODE(AUTOMATIC);

void setup() {
  String subscriptionName=String::format("%s/%s/",System.deviceID().c_str(),EVENT_NAME);
  Particle.subscribe(subscriptionName,subscriptionHandler,MY_DEVICES);
  Serial.printf("Subscribing to %s\n",subscriptionName.c_str());  
}

void loop(){
  Particle.publish(EVENT_NAME, String::format("{\"lat42\":%0.5f,\"lon42\":%0.5f}",LAT,LON),PRIVATE);
}

void subscriptionHandler(const char *event, const char *data) {
  String museumName;
  float lat, lon;
  JSONValue outerObj = JSONValue::parseCopy(data);
  JSONObjectIterator iter(outerObj);
  while(iter.next()){
    if (iter.name() == "name") museumName = (const char *)iter.value().toString();
    if (iter.name() == "muslat") lat=iter.value().toDouble();
    if (iter.name() == "muslon") lon= iter.value().toDouble();
  }  
  Serial.printf("Museums found at %s (%0.6f, %0.6f)\n",museumName.c_str(),lat,lon);
}