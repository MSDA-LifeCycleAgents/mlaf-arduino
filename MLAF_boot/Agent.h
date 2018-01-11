#pragma once

#include "MessageDispatcher.h"
#include "AID.h"
#include "AclMessage.h"
#include "Behaviour.h"
#include <list>

class Agent{
  private:
    MessageDispatcher messageDispatcher;
    AID aid;
    std::list<Behaviour> behaviours;
  
  public:
    virtual void setup();
    
    Agent(String name, int port){
      messageDispatcher.init("It Hurts When IP", "GlobalWarmingIsAMyth300AlGore", port);

      AID _aid(name, messageDispatcher.getIpAddress());
      aid = _aid;

      messageDispatcher.advertise(aid.getName(), "Dit is een omschrijving", "20171031-21:57:38:513000");
      addBehaviour(sendAndReceiveBehaviour);
      
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
      for(Behaviour behaviour : behaviours){
        behaviour.action();
      }
    }

    class SendAndReceiveBehaviour : public Behaviour{
      public:
        virtual void action(){
          messageDispatcher.fillQueue();
          messageDispatcher.sendCache();
        }
    } sendAndReceiveBehaviour;

    void addBehaviour(Behaviour behaviour){
      behaviours.push_back(behaviour);
    }
};

