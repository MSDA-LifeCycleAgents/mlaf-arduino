#pragma once

#include "sensor_agent.h"
#include "temp_sensor.h"
#include "accelero_sensor.h"
#include "arduino_adc_sensor.h"

/**
 * \brief An exampe implementation of the SensorAgent
 */
class ExampleSensorAgent : public SensorAgent{
  public:
    /**
     * \brief Creates an ExampleSensorAgent
     * 
     * \param name the name to use
     * \param port the port to use
     * \param ntp the NTP-client to use
     */
    ExampleSensorAgent(const char* name, NTPClient& ntp, int port) : SensorAgent(name, ntp, port){}

    /**
     * \brief Initial setup function
     */
    void setup(){
      addSensor(std::shared_ptr<Sensor>(new TempSensor()));
      addSensor(std::shared_ptr<Sensor>(new AcceleroSensor()));
      addSensor(std::shared_ptr<Sensor>(new ADCSensor(A0)));

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
//            }
//        });
    }
};

