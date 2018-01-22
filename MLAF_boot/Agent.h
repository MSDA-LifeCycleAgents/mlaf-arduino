#pragma once

#include <list>
#include "MessageDispatcher.h"
#include "AID.h"
#include "AclMessage.h"
#include "Configuration.h"

/**
 * Abstract base class for Agents
 */
class Agent{
private:
    MessageDispatcher messageDispatcher;
    AID* aid;
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

        aid = new AID(name, messageDispatcher.getIpAddress(), port);

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
    AID* getAID(){
        return aid;
    }

    /**
     * \brief Send an ACL-message to the recipient
     * 
     * \param message the ACL-message to send
     */
    void send(AclMessage* message){
        if(!message->sender)
            message->sender = getAID();

        if(message->envelope && !message->envelope->from)
            message->envelope->from = getAID();

        messageDispatcher.send(message);
    }

    /**
     * \brief Receives an ACL-message
     * 
     * \return An ACL-message if there is one, otherwise null
     */
    AclMessage* receive(bool pop = true){
        return messageDispatcher.receive(pop);
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
};