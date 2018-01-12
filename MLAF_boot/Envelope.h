#pragma once
#include "AID.h"

class Envelope{
  public:
    AID to;
    AID from;
    AID intendedReceiver;
    String aclRepresentation;
    String date;
    String payloadEncoding;
    int payloadLength;

    Envelope(){}
};

