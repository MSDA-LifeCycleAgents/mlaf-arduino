#pragma once
#include<memory>
#include "AID.h"

/**
 * \brief Envelope used in an ACL-message
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

    /**
     * \brief Creates a default Envelope
     */
    Envelope(){}

    /**
     * \brief Creates a human-readable string from the envelope
     * 
     * \return a human-readable envelope representation
     */
    String toString(){
      return "Message from " + from->getName() + ", address: " + from->getAddress() + " to " + to->getName() + ", address: " + to->getAddress();
    }
};

