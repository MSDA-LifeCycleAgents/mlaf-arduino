#pragma once

#include "MessageDispatcher.h"
#include "AID.h"
#include "AclMessage.h"
#include <list>
#include "Configuration.h"

class Agent{
  private:
    MessageDispatcher messageDispatcher;
    AID* aid;
    std::list<std::function< void()>> behaviours;
  
  public:
    virtual void setup() = 0;
    
    Agent(String name, int port){
      messageDispatcher.init(WiFi_ssid, WiFi_pass, port);

      aid = new AID(name, messageDispatcher.getIpAddress());
      aid->setPort(port);

      messageDispatcher.advertise(aid->getName(), mdns_description, "20171031-21:57:38:513000");
      addBehaviour([this]{
          messageDispatcher.fillQueue();
          messageDispatcher.sendCache();
        }); 
    }

    AID* getAID(){
      return aid;
    }

    void send(AclMessage* message){
      if(message->sender == NULL)
        message->sender = getAID();
        
      messageDispatcher.send(message);
    }

    AclMessage* receive(){
      return messageDispatcher.receive();
    }

    void doBehaviours(){
      for(std::function<void ()> behaviour : behaviours){
        behaviour();
      }
    }

    void addBehaviour(std::function< void()> behaviour){
      behaviours.push_back(behaviour);
    }
};


