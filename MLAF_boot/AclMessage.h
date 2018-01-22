#pragma once
#include "Envelope.h"

/**
 * Performatives used in agent communication
 */
enum Performative{
  ACCEPT_PROPOSAL,
  AGREE,
  //CANCEL,
  CFP,
  CONFIRM,
  DISCONFIRM,
  FAILURE,
  INFORM,
  INFORM_IF,
  INFORM_REF,
  NOT_UNDERSTOOD,
  PROPOSE,
  QUERY_IF,
  QUERY_REF,
  REFUSE,
  REJECT_PROPOSAL,
  REQUEST,
  REQUEST_WHEN,
  SUBSCRIBE,
  PROXY,
  PROPAGATE
};

/**
 * An Agent Communication Language message (ACL-message)
 */
class AclMessage{
public:
    Envelope* envelope = nullptr;
    AID* sender = nullptr;
    AID* receiver = nullptr;
    AID* replyTo = nullptr;
    Performative performative;
    String content;
    String language;
    String ontology;
    String protocol;
    String conversationID;

    // TODO: determine if we need a default ACL-message, and if so, which performative should be used
    AclMessage(){}

    /**
     * \brief Constructs an ACL-message
     * 
     * \param performative the enum-based performative to use
     */
    AclMessage(Performative performative) : performative{performative}
    {}

    ~AclMessage()
    {
        if (sender) {
            delete sender;
            sender = nullptr;
        }
        if (receiver) {
            delete receiver;
            receiver = nullptr;
        }
        if (replyTo) {
            delete replyTo;
            replyTo = nullptr;
        }
        if (envelope) {
            delete envelope;
            envelope = nullptr;
        }
    }

    bool equals(const AclMessage& other) const{
        return sender->getName() == other.sender->getName()
            && sender->getAddress() == other.sender->getAddress()
            && receiver->getName() == other.receiver->getName()
            && receiver->getAddress() == other.receiver->getAddress()
            && content == other.content
            && language == other.language
            && ontology == other.ontology
            && protocol == other.protocol
            && conversationID == other.conversationID;
    }

    bool operator==( const AclMessage& other) const{
        return this->equals(other);
    }

    bool operator!=( const AclMessage& other) const{
        return !this->equals(other);
    }

    String toString(){
        return "Message from " + sender->getName() + ", address: " + sender->getAddress() + " to " + receiver->getName() + ", address: " + receiver->getAddress();
    }

    /**
     * \brief Creates a reply message
     * 
     * Automatically creates a reply message based on the <i>reply to</i> field
     * 
     * \param perf the performative to use
     * 
     * \return An ACL-message with a receiver field
     */
    AclMessage* createReply(Performative perf){
        AclMessage* response = new AclMessage(perf);
        if(replyTo){
            response->receiver = new AID(replyTo->getName(), replyTo->getAddress(), replyTo->getPort());
        }else{
            response->receiver = new AID(sender->getName(), sender->getAddress(), sender->getPort());
        }
        return response;
    }
};