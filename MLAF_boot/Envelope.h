#pragma once
#include<memory>
#include "AID.h"

/**
 * @brief      Class for message envelope.
 */
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

    /**
     * @brief      Returns a string representation of the object.
     *
     * @return     String representation of the object.
     */
    String toString(){
      return "Message from " + from->getName() + ", address: " + from->getAddress() + " to " + to->getName() + ", address: " + to->getAddress();
    }
};

