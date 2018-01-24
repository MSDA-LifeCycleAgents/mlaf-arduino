#pragma once

#include <list>
#include <memory>
#include "sensor.h"
#include "tinyxml2.h"
#include <NTPClient.h>

using namespace tinyxml2;

class SensorAgent : public Agent{
  private:
    std::shared_ptr<AID> proxyAgent;
    std::shared_ptr<AID> receiverAgent;
    std::list<std::shared_ptr<sensor>> _sensors;
    const char* _name;
    const char* _identifier;
    const char* _topic;
    bool _toDecisionAgent = true;
    NTPClient &ntp;
  public:
    /**
     * Constructs a sensor agent
     * 
     * \param name the name of the sensor agent
     * \param sensors a list of the sensors associated with the agent
    */
    SensorAgent(const char* name, NTPClient &ntp, int port)
        : Agent(name, port), _name{name}, ntp(ntp)
    {  
      proxyAgent = NULL;
      receiverAgent = NULL;
      
      addBehaviour([this] {
            getNTP().update();
            
            std::list<std::shared_ptr<Sensor>> queue;
            for (auto& sensor : _sensors) {
                if (sensor->needsUpdate(getNTP().getEpochTime())) {
                    queue.push_back(sensor);
                }
            }
            if (receiverAgent != NULL) {
                if(queue.empty())
                  return;
                  
                auto msg = std::make_shared<AclMessage>(INFORM);
                msg->receiver = receiverAgent;
                msg->content = String(toXML(queue));
                msg->ontology = "sensor-agent-reading";
                send(msg);
            }else{
              // TODO: cache values until decision agent exists
            }
        });

        // handshake behaviour
        addBehaviour([this] { 
          auto message = receive(MessageTemplate::matchPerformativeAndOntology(REQUEST, "sensor-agent-register", STARTS_WITH));
          if(!message)
            return;

          proxyAgent = AID::copy(message->envelope->from);
          setDefaultReceiver(proxyAgent);
            
          auto response = message->createReply(SUBSCRIBE);
          response->content = createInstructionSet();
          response->ontology = message->ontology;
          
          auto env = std::make_shared<Envelope>();
          env->to = proxyAgent;
          env->from = response->sender;
          env->intendedReceiver = response->receiver;
          env->aclRepresentation = "fipa.acl.rep.string.std";
          env->payloadEncoding = "US-ASCII";
          response->envelope = env;
          
          Serial.println("Sending instructionset");
          send(response);

        });

       addBehaviour([this] {
         std::shared_ptr<AclMessage> message = receive(MessageTemplate::matchPerformativeAndOntology(CONFIRM, "sensor-agent-register", STARTS_WITH));
          if(!message || !message->replyTo)
            return;
            
            receiverAgent = AID::copy(message->replyTo);
          });
    }

    std::shared_ptr<AID> getProxyAgent(){
      return proxyAgent;
    }

    std::shared_ptr<AID> getReceiverAgent(){
      return receiverAgent;
    }

    NTPClient& getNTP(){
      return ntp;
    }

    void setIdentifier(const char* id){
      _identifier = id;
    }

    void setTopic(const char* topicName){
      _topic = topicName;
      if(_topic)
        _toDecisionAgent = false;
    }

    void addSensor(std::shared_ptr<Sensor> sensor){
      _sensors.push_back(sensor);
    }

    // Functions
    const char* createInstructionSet()
    {
        XMLDocument doc;

        auto instr = doc.NewElement("instructions");
        doc.InsertEndChild(instr);
        auto ident = doc.NewElement("identifier");
        ident->SetText(_identifier);
        instr->InsertEndChild(ident);

        // Messaging
        auto msg = doc.NewElement("messaging");
        instr->InsertEndChild(msg);
        auto topic = doc.NewElement("topic");
        msg->InsertEndChild(topic);
        auto tName = doc.NewElement("name");
        tName->SetText(_topic);
        topic->InsertEndChild(tName);
        auto toDA = doc.NewElement("directToDecisionAgent");
        toDA->SetText(_toDecisionAgent);
        msg->InsertEndChild(toDA);
        auto xSensors = doc.NewElement("sensors");
        instr->InsertEndChild(xSensors);
        
        // Sensors
        for (auto& sensor : _sensors)
        {
            auto s = doc.NewElement("sensor");
            s->SetAttribute("id", sensor->getName());// TODO: replace spaces?
            xSensors->InsertEndChild(s);
            auto label = doc.NewElement("label");
            label->SetText(sensor->getName());
            s->InsertEndChild(label);
            auto sMin = doc.NewElement("min");
            sMin->SetText(sensor->getMinVal());
            s->InsertEndChild(sMin);
            auto sMax = doc.NewElement("max");
            sMax->SetText(sensor->getMaxVal());
            s->InsertEndChild(sMax);

            auto unit = doc.NewElement("unit");
            unit->SetText(sensor->getUnit());
            s->InsertEndChild(unit);
            auto interval = doc.NewElement("intervalinseconds");
            interval->SetText(sensor->getInterval());
            s->InsertEndChild(interval);
            
            auto backups = doc.NewElement("amountOfBackupMeasurements");
            backups->SetText(sensor->getNrBackupMeasurements());
            s->InsertEndChild(backups);
        }

        auto fallback = doc.NewElement("fallback");
        instr->InsertEndChild(fallback);
        auto via = doc.NewElement("via");
        via->SetText("ScreenAgent"); // TODO: replace with proper agent
        fallback->InsertEndChild(via);
        auto to = doc.NewElement("to");
        fallback->InsertEndChild(to);

        XMLPrinter printer;
        doc.Print(&printer);
        return printer.CStr();
    }

    /**
     * Converts the selected sensors values to an XML representation.
     * This includes the name of the sensor and its values.
     * 
     * \param sensors a list of selected sensors
     * 
     * \return an XML representation of the sensors
    */
    const char* toXML(std::list<std::shared_ptr<Sensor>> sensors)
    {
        XMLDocument doc;
        doc.InsertEndChild(doc.NewDeclaration());
        auto root = doc.NewElement("sensorreading");
        doc.InsertEndChild(root);

        auto timestamp = doc.NewElement("timestamp");
        long unsigned int time = ntp.getEpochTime();
        String time_str = String(time);
        timestamp->SetText(time_str.c_str());
        root->InsertEndChild(timestamp);
        
        auto sroot = doc.NewElement("sensors");
        root->InsertEndChild(sroot);
        for (auto sensor : sensors)
        {            
            auto s = doc.NewElement("sensor");
            s->SetAttribute("id", sensor->getName());
            sroot->InsertEndChild(s);
            for (auto& pair : sensor->getData())
            {
                auto sm = doc.NewElement("measurement");
                sm->SetAttribute("id", pair.first);
                auto sv = doc.NewElement("value");
                sv->SetText(pair.second);
                sm->InsertEndChild(sv);
                s->InsertEndChild(sm);
            }
        }

        XMLPrinter printer;
        doc.Print(&printer);
        return printer.CStr();
    }
};
