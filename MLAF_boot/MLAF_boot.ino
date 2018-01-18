#include <NTPClient.h>
#include <WiFiUdp.h>

#include "AID.h"
#include "ExampleAgent.h"
#include "AgentPlatform.h"
#include "Configuration.h"
#include "sensor_agent.h"
#include "temp_sensor.h"
#include "arduino_adc_sensor.h"
#include "accelero_sensor.h"

WiFiUDP ntpUDP;
NTPClient ntp(ntpUDP);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting up agent");

  ntp.begin();

  //ExampleAgent* agent = new ExampleAgent("Agent47", 1234);
  auto ts = new TempSensor();
  auto as = new AcceleroSensor();
  auto adc = new ADCSensor(A0);
  auto agent = new SensorAgent(
    "NonCriticalSensorStuffAgent",
    "abc123xyz",
    "JADE",
    true,
    std::list<Sensor*>{ts,as,adc},
    ntp,
    1234);
  AgentPlatform::blockingStart(agent);
}

void loop() {

}


