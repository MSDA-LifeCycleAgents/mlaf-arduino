#pragma once
#include<memory>
#include "AID.h"

class Envelope{
  public:
    std::shared_ptr<AID> to;
    std::shared_ptr<AID> from;
    std::shared_ptr<AID> intendedReceiver;
    String aclRepresentation;
    String date;
    String payloadEncoding;
    long payloadLength;

    Envelope(){}

    String toString(){
      return "Message from " + from->getName() + ", address: " + from->getAddress() + " to " + to->getName() + ", address: " + to->getAddress();
    }
};

