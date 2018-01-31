#pragma once
#include <memory>
#include "Envelope.h"

enum Performative{
  UNSET,
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

class AclMessage{
  public:
    std::shared_ptr<Envelope> envelope;
    Performative performative;
    std::shared_ptr<AID> sender;
    std::shared_ptr<AID> receiver;
    std::shared_ptr<AID> replyTo;
    String content;
    String language;
    String ontology;
    String protocol;
    String conversationID;

    /**
     * @brief      Constructor
     *
     * @param[in]  _performative  The performative
     */
    AclMessage(Performative _performative){
      performative = _performative;
      sender = NULL;
      receiver = NULL;
      envelope = NULL;
      replyTo = NULL;
    }

    /**
     * @brief      Constructor
     *
     * @param[in]  _performative  The performative (number from list)
     */
    AclMessage(int _performative){
      performative = static_cast<Performative>(_performative);
      sender = NULL;
      receiver = NULL;
      envelope = NULL;
      replyTo = NULL;
    }

    /**
     * @brief    Constructor
     */
    AclMessage(){}


    /**
     * @brief      Test if a message is equal to another message
     *
     * @param[in]  other  The other message
     *
     */
    bool equals(const AclMessage& other) const{
      bool send_eq = sender->getName() == other.sender->getName() && sender->getAddress() == other.sender->getAddress();
      bool rec_eq = receiver->getName() == other.receiver->getName() && receiver->getAddress() == other.receiver->getAddress();
      bool str_eq = content == other.content && language == other.language && ontology == other.ontology && protocol == other.protocol && conversationID == other.conversationID;
      
      return send_eq && rec_eq && str_eq;
    }

    /**
     * @brief      Test if a message is equal to another message
     *
     * @param[in]  other  The other message
     *
     */
    bool operator==( const AclMessage& other) const{
      return this->equals(other);
    }


    /**
     * @brief      Test if a messsage is not equal to another message
     *
     * @param[in]  other The other message
     *
     */
    bool operator!=( const AclMessage& other) const{
      return !this->equals(other);
    }

    /**
     * @brief      Returns a string representation of the ACLMessage.
     *
     * @return     String representation of the ACLMessage.
     */
    String toString(){
      return "Message from " + sender->getName() + ", address: " + sender->getAddress() + " to " + receiver->getName() + ", address: " + receiver->getAddress();
    }

    /**
     * @brief      Creates a reply.
     *
     * @param[in]  perf  The Performative
     *
     * @return     A std::shared_ptr<AclMessage> to the generated reply message
     */
    std::shared_ptr<AclMessage> createReply(Performative perf){
      auto response = std::make_shared<AclMessage>(perf);
      if(replyTo != NULL){
        response->receiver = std::make_shared<AID>(replyTo->getName(), replyTo->getAddress());
        response->receiver->setPort(replyTo->getPort());
      }else{
        response->receiver = std::make_shared<AID>(sender->getName(), sender->getAddress());
        response->receiver->setPort(sender->getPort());
      }
      return response;
    }
};

