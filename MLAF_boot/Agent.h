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

    /**
     * @brief      Initial setup method
     * 
     * This method is executed once, after the agent is created. Classes inheriting from Agent must implement this.
     */
    virtual void setup() = 0;
    
    /**
     * @brief      Constructor
     *
     * @param[in]  name  Agent Name
     * @param[in]  port  Agent TCP/IP port
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
     * @brief      Gets the AID.
     *
     * @return     A std::shared_ptr<AID> to the AID associated with this agent.
     */
    std::shared_ptr<AID> getAID(){
      return aid;
    }

    /**
     * @brief      Send an ACL message
     *
     * @param[in]  message  The message
     */
    void send(std::shared_ptr<AclMessage> message){
      if(!message->sender)
        message->sender = getAID();

      if(message->envelope && !message->envelope->from)
        message->envelope->from = getAID();
        
      messageDispatcher.send(message);
    }

    /**
     * @brief      Try to get a message. Non-blocking.
     *
     * @param[in]  _template  The template
     *
     * @return     NULL if there was no message received.
     */
    std::shared_ptr<AclMessage> receive(std::shared_ptr<MessageTemplate> _template = NULL){
      return messageDispatcher.receive(_template);
    }

    /**
     * @brief      Execute agent behaviours.
     */
    void doBehaviours(){
      for(std::function<void ()> behaviour : behaviours){
        behaviour();
      }
    }

    /**
     * @brief      Adds a behaviour.
     *
     * @param[in]  behaviour  The behaviour as a function pointer. Using lambdas is recommended.
     */
    void addBehaviour(std::function< void()> behaviour){
      behaviours.push_back(behaviour);
    }

    /**
     * @brief      Sets the default receiver.
     *
     * @param[in]  receiver  The receiver
     */
    void setDefaultReceiver(std::shared_ptr<AID> receiver){
      messageDispatcher.setDefaultEnvelopeReceiver(receiver);
    }
};

