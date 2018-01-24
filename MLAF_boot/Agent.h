#pragma once

#include "AID.h"
#include "AclMessage.h"
#include <list>
#include "Configuration.h"
#include "MessageTemplate.h"
#include "MessageDispatcher.h"

class Agent{
  private:
    MessageDispatcher messageDispatcher;
    std::shared_ptr<AID> aid;
    std::list<std::function< void()>> behaviours;
  
  public:
    virtual void setup() = 0;
    
    Agent(String name, int port){
      messageDispatcher.init(WiFi_ssid, WiFi_pass, port);

      aid = std::make_shared<AID>(name, messageDispatcher.getIpAddress());
      aid->setPort(port);

      messageDispatcher.advertise(aid->getName(), mdns_description, "20171031-21:57:38:513000");
      addBehaviour([this]{
          messageDispatcher.fillQueue();
          messageDispatcher.sendCache();
        });
    }

    std::shared_ptr<AID> getAID(){
      return aid;
    }

    void send(std::shared_ptr<AclMessage> message){
      if(!message->sender)
        message->sender = getAID();

      if(message->envelope && !message->envelope->from)
        message->envelope->from = getAID();
        
      messageDispatcher.send(message);
    }

    std::shared_ptr<AclMessage> receive(std::shared_ptr<MessageTemplate> _template = NULL){
      return messageDispatcher.receive(_template);
    }

    void doBehaviours(){
      for(std::function<void ()> behaviour : behaviours){
        behaviour();
      }
    }

    void addBehaviour(std::function< void()> behaviour){
      behaviours.push_back(behaviour);
    }

    void setDefaultReceiver(std::shared_ptr<AID> receiver){
      messageDispatcher.setDefaultEnvelopeReceiver(receiver);
    }
};

