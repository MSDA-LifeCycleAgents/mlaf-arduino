#pragma once

#include "MessageDispatcher.h"
#include "AID.h"
#include "AclMessage.h"
#include <list>

class Agent{
  private:
    MessageDispatcher messageDispatcher;
    AID aid;
    std::list<std::function< void()>> behaviours;
  
  public:
    virtual void setup(){}
    
    Agent(String name, int port){
      messageDispatcher.init("It Hurts When IP", "GlobalWarmingIsAMyth300AlGore", port);

      AID _aid(name, messageDispatcher.getIpAddress());
      aid = _aid;

      messageDispatcher.advertise(aid.getName(), "Dit is een omschrijving", "20171031-21:57:38:513000");
      addBehaviour([this]{
          messageDispatcher.fillQueue();
          messageDispatcher.sendCache();
        });
      
      setup();
    }

    AID getAID(){
      return aid;
    }

    void send(AclMessage message){
      messageDispatcher.send(message);
    }

    AclMessage& receive(){
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

