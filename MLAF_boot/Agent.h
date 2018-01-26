#pragma once

#include "AID.h"
#include "AclMessage.h"
#include <list>
#include "Configuration.h"
#include "MessageTemplate.h"
#include "MessageDispatcher.h"

/**
 * \brief Abstract base class for Agents
 */
class Agent{
  private:
    MessageDispatcher messageDispatcher;
    std::shared_ptr<AID> aid;
    std::list<std::function< void()>> behaviours;
  
  public:
    /**
     * \brief Initial setup function
     * 
     * Required function used in the setup procedure once
     */
    virtual void setup() = 0;
    
    /**
     * \brief Constructs an Agent
     * 
     * \param name the nane of the agent
     * \param port the port to listen to
     */
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

    /**
     * \brief Gets the AID associated with this agent
     * 
     * \return the AID
     */
    std::shared_ptr<AID> getAID(){
      return aid;
    }

    /**
     * \brief Send an ACL-message to the recipient
     * 
     * \param message the ACL-message to send
     */
    void send(std::shared_ptr<AclMessage> message){
      if(!message->sender)
        message->sender = getAID();

      if(message->envelope && !message->envelope->from)
        message->envelope->from = getAID();
        
      messageDispatcher.send(message);
    }

    /**
     * \brief Receives an ACL-message
     * 
     * \param msgTemplate the message template to compare to
     * 
     * \return An ACL-message if there is one, otherwise nullptr
     */
    std::shared_ptr<AclMessage> receive(std::shared_ptr<MessageTemplate> _template = NULL){
      return messageDispatcher.receive(_template);
    }

    /**
     * \brief Processes all behaviours in order of addition
     */
    void doBehaviours(){
      for(std::function<void ()> behaviour : behaviours){
        behaviour();
      }
    }

    /**
     * \brief Adds behaviour to be executed
     * 
     * \param behaviour the function to executed
     */
    void addBehaviour(std::function< void()> behaviour){
      behaviours.push_back(behaviour);
    }

    /**
     * \brief Sets the default receiver (AID)
     * 
     * \param aid the receiver to use
     */
    void setDefaultReceiver(std::shared_ptr<AID> receiver){
      messageDispatcher.setDefaultEnvelopeReceiver(receiver);
    }
};

