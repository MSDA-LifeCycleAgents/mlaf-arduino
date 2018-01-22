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

    Envelope(
        AID* to,
        AID* from,
        AID* receiver,
        String aclRepr = "fipa.acl.rep.string.std",
        String date = "",
        String payloadEncoding = "US-ASCII",
        long payloadLength = 0)
        : to{to}, from{from}, intendedReceiver{receiver},
        aclRepresentation{aclRepresentation},
        date{date},
        payloadEncoding{payloadEncoding},
        payloadLength{payloadLength}
    {}

    String toString(){
      return "Message from " + from->getName() + ", address: " + from->getAddress() + " to " + to->getName() + ", address: " + to->getAddress();
    }
};

