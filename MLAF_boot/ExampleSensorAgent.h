#pragma once

#include "sensor_agent.h"
#include "temp_sensor.h"
#include "accelero_sensor.h"
#include "arduino_adc_sensor.h"

class ExampleSensorAgent : public SensorAgent{
  public:
    ExampleSensorAgent(const char* name, NTPClient& ntp, int port) : SensorAgent(name, ntp, port){}

    void setup(){
      addSensor(new TempSensor());
      addSensor(new AcceleroSensor());
      addSensor(new ADCSensor(A0));

      // optionals:
      setIdentifier("someidentifier");
      setTopic("sometopic");
      
//      addBehaviour([this] {    
//          AclMessage* message = receive();
//
//          if (message) {
//              Serial.println("Received: " + message->toString());
//
//              AclMessage* response = message->createReply(INFORM);
//              response->content = "I received your message!";
//
//              send(response);
//              AclMessage::destroy(message);
//            }
//        });
    }
};

