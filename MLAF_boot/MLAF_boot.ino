#include <NTPClient.h>
#include <WiFiUdp.h>

#include "ExampleAgent.h"
#include "AgentPlatform.h"
#include "Configuration.h"
#include "ExampleSensorAgent.h"

WiFiUDP ntpUDP;
NTPClient ntp(ntpUDP);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting up agent");

  ntp.begin();

  auto agent = new ExampleSensorAgent("NonCriticalSensorStuffAgent", ntp, 1234);
  AgentPlatform::blockingStart(agent);
}

void loop() {

}


