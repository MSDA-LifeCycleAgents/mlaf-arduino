#include "AID.h"
#include "ExampleAgent.h"
#include "AgentPlatform.h"
#include "Configuration.h"

void setup() {
  Serial.begin(115200);
  Serial.println("Starting up agent");

  ExampleAgent agent("Agent47", 1234);
  AgentPlatform::blockingStart(agent);
}

void loop() {

}


