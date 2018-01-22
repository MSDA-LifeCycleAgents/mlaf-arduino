#pragma once
#include "AID.h"

class Envelope{
  public:
    AID* to;
    AID* from;
    AID* intendedReceiver;
    String aclRepresentation;
    String date;
    String payloadEncoding;
    long payloadLength;

    Envelope(){}

    String toString(){
      return "Message from " + from->getName() + ", address: " + from->getAddress() + " to " + to->getName() + ", address: " + to->getAddress();
    }
};

