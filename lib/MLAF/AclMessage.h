#pragma once
#include <Envelope.h>

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
    Envelope* envelope;
    Performative performative;
    AID* sender;
    AID* receiver;
    AID* replyTo;
    String content;
    String language;
    String ontology;
    String protocol;
    String conversationID;

    AclMessage(Performative _performative){
      performative = _performative;
      sender = NULL;
      receiver = NULL;
      envelope = NULL;
      replyTo = NULL;
    }

    AclMessage(int _performative){
      performative = static_cast<Performative>(_performative);
      sender = NULL;
      receiver = NULL;
      envelope = NULL;
      replyTo = NULL;
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
      return "Message from " + sender->getName() + ", address: " + sender->getAddress() + " to " + receiver->getName() + ", address: " + receiver->getAddress();
    }

    AclMessage* createReply(Performative perf){
      AclMessage* response = new AclMessage(perf);
      if(replyTo != NULL){
        response->receiver = new AID(replyTo->getName(), replyTo->getAddress());
        response->receiver->setPort(replyTo->getPort());
      }else{
        response->receiver = new AID(sender->getName(), sender->getAddress());
        response->receiver->setPort(sender->getPort());
      }
      return response;
    }

    static void destroy(AclMessage* message){
      delete message->sender;
      delete message->receiver;
      delete message->envelope;
      delete message->replyTo;
      delete message;
      message = NULL;
    }
};
