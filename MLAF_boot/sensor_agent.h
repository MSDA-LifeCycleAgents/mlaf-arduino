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
    std::list<std::shared_ptr<Sensor>> _sensors;
    String _name;
    String _identifier;
    String _topic;
    String _fallbackVia;
    String _fallbackTo;
    int _aomdp;
    bool _toDecisionAgent = true;
    NTPClient &ntp;
  public:
    /**
     * Constructs a sensor agent
     * 
     * \param name the name of the sensor agent
     * \param sensors a list of the sensors associated with the agent
    */
    SensorAgent(String name, NTPClient &ntp, int port)
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
            if (receiverAgent){
                if(queue.empty())
                  return;
                  
                auto msg = std::make_shared<AclMessage>(INFORM);
                msg->receiver = receiverAgent;
                msg->content = toXML(queue);
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
          if(response->sender)
            Serial.println("Instructionset sender: " + response->sender->getName());
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

    void setIdentifier(String id){
      _identifier = id;
    }

    void setTopic(String topicName){
      _topic = topicName;
      if(_topic)
        _toDecisionAgent = false;
    }

    void setFallbackVia(String via){
        _fallbackVia = via;
    }

    void setFallbackTo(String to){
        _fallbackTo = to;
    }

    void setMissedData(const int missedData){
      _aomdp = missedData;
    }

    void addSensor(std::shared_ptr<Sensor> sensor){
      _sensors.push_back(sensor);
    }

    // Functions
    String createInstructionSet()
    {
        XMLDocument doc;

        auto instr = doc.NewElement("instructions");
        doc.InsertEndChild(instr);
        auto ident = doc.NewElement("identifier");
        ident->SetText(_identifier.c_str());
        instr->InsertEndChild(ident);

        // Messaging
        auto msg = doc.NewElement("messaging");
        instr->InsertEndChild(msg);
        if(!_toDecisionAgent){
          auto topic = doc.NewElement("topic");
          msg->InsertEndChild(topic);
          auto tName = doc.NewElement("name");
          tName->SetText(_topic.c_str());
          topic->InsertEndChild(tName);
        }
        auto toDA = doc.NewElement("directToDecisionAgent");
        toDA->SetText(_toDecisionAgent);
        msg->InsertEndChild(toDA);
        auto aomdp = doc.NewElement("amountOfMissedDataPackages");
        aomdp->SetText(_aomdp);
        
        auto xSensors = doc.NewElement("sensors");
        instr->InsertEndChild(xSensors);
        
        // Sensors
        for (auto& sensor : _sensors)
        {
            auto s = doc.NewElement("sensor");
            s->SetAttribute("id", sensor->getId());
            xSensors->InsertEndChild(s);
            auto label = doc.NewElement("label");
            label->SetText(sensor->getName());
            s->InsertEndChild(label);
            auto interval = doc.NewElement("intervalinseconds");
            interval->SetText(sensor->getInterval());
            s->InsertEndChild(interval);
            auto unit = doc.NewElement("unit");
            unit->SetText(sensor->getUnit());
            s->InsertEndChild(unit);
            
            auto mCollection = doc.NewElement("measurements");
            s->InsertEndChild(mCollection);
            // Measurement structure (meta-data)
            for (auto& m : sensor->getMeasurementsMetadata())
            {
                auto xMeasurement = doc.NewElement("measurement");
                xMeasurement->SetAttribute("id", m.Id.c_str());
                mCollection->InsertEndChild(xMeasurement);
                auto sMin = doc.NewElement("min");
                sMin->SetText(m.Min);
                xMeasurement->InsertEndChild(sMin);
                auto sMax = doc.NewElement("max");
                sMax->SetText(m.Max);
                xMeasurement->InsertEndChild(sMax);
                
                auto xPlans = doc.NewElement("plans");
                xMeasurement->InsertEndChild(xPlans);
                for (auto& p : m.Plans)
                {
                    auto xp = doc.NewElement("plan");
                    xPlans->InsertEndChild(xp);

                    auto pBelow = doc.NewElement("below");
                    if (p.PlanThresholdType & PlanType::Below)
                        pBelow->SetText(p.ThresholdBelow);
                    xp->InsertEndChild(pBelow);
                    auto pAbove = doc.NewElement("above");
                    if (p.PlanThresholdType & PlanType::Above)
                        pAbove->SetText(p.ThresholdAbove);
                    xp->InsertEndChild(pAbove);

                    auto pMsg = doc.NewElement("message");
                    pMsg->SetText(p.Message.c_str());
                    xp->InsertEndChild(pMsg);
                    auto pVia = doc.NewElement("via");
                    pVia->SetText(p.Via.c_str());
                    xp->InsertEndChild(pVia);
                    auto pTo = doc.NewElement("to");
                    pTo->SetText(p.To.c_str());
                    xp->InsertEndChild(pTo);
                    auto pLimit = doc.NewElement("limit");
                    pLimit->SetText(p.Limit);
                    xp->InsertEndChild(pLimit);
                }
            }

            
            auto backups = doc.NewElement("amountOfBackupMeasurements");
            backups->SetText(sensor->getNrBackupMeasurements());
            s->InsertEndChild(backups);
        }

        auto fallback = doc.NewElement("fallback");
        instr->InsertEndChild(fallback);
        auto fMsg = doc.NewElement("message");
        String fallbackMsg = "This is the fallback message. " + _identifier + " is unregistered";
        fMsg->SetText(fallbackMsg.c_str()); 
        fallback->InsertEndChild(fMsg);
        auto fVia = doc.NewElement("via");
        fVia->SetText(_fallbackVia.c_str());
        fallback->InsertEndChild(fVia);
        auto fTo = doc.NewElement("to");
        fTo->SetText(_fallbackTo.c_str());
        fallback->InsertEndChild(fTo);

        XMLPrinter printer(0, true);
        doc.Print(&printer);
        return String(printer.CStr());
    }

    /**
     * Converts the selected sensors values to an XML representation.
     * This includes the name of the sensor and its values.
     * 
     * \param sensors a list of selected sensors
     * 
     * \return an XML representation of the sensors
    */
    String toXML(std::list<std::shared_ptr<Sensor>> sensors)
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

        XMLPrinter printer(0, true);
        doc.Print(&printer);
        return String(printer.CStr());
    }
};
