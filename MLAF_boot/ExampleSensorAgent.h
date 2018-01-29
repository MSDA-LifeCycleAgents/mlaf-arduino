#pragma once

#include "sensor_agent.h"
#include "temp_sensor.h"
#include "accelero_sensor.h"
#include "arduino_adc_sensor.h"

class ExampleSensorAgent : public SensorAgent{
  public:
    ExampleSensorAgent(const char* name, NTPClient& ntp, int port) : SensorAgent(name, ntp, port){}

    void setup(){
      addSensor(std::shared_ptr<Sensor>(new TempSensor()));
      addSensor(std::shared_ptr<Sensor>(new AcceleroSensor()));
      addSensor(std::shared_ptr<Sensor>(new ADCSensor(A0)));

      // optionals:
      setIdentifier("someidentifier");
      setTopic("sometopic");
      setMissedData(5); //missed data packages before agent is unregistered
      
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
//            }
//        });
    }
};

