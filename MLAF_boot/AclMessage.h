#pragma once
#include "Envelope.h"

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

class AclMessage{
  public:
    Envelope* envelope;
    Performative performative;
    AID* sender;
    AID* receiver;
    String content;
    String language;
    String ontology;
    String protocol;
    String conversationID;

    AclMessage(Performative _performative){
      performative = _performative;
    }

    AclMessage(int _performative){
      performative = static_cast<Performative>(_performative);
    }

    AclMessage(){}

    bool equals(const AclMessage& other) const{
      bool send_eq = sender->getName() == other.sender->getName() && sender->getAddress() == other.sender->getAddress();
      bool rec_eq = receiver->getName() == other.receiver->getName() && receiver->getAddress() == other.receiver->getAddress();
      bool str_eq = content == other.content && language == other.language && ontology == other.ontology && protocol == other.protocol && conversationID == other.conversationID;
      
      return send_eq && rec_eq && str_eq;
    }

    bool operator==( const AclMessage& other) const{
      return this->equals(other);
    }

    bool operator!=( const AclMessage& other) const{
      return !this->equals(other);
    }

    String toString(){
      // to do: acl representatie printen
      return "Message from " + sender->getName() + ", address: " + sender->getAddress() + " to " + receiver->getName() + ", address: " + receiver->getAddress();
    }

    static void destroy(AclMessage* message){
      delete message->sender;
      delete message->receiver;
      delete message->envelope;
      delete message;
      message = NULL;
    }
};

