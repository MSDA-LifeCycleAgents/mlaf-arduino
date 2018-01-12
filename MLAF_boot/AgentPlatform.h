#pragma once
#include "Agent.h"

class AgentPlatform{
  public:
    static void blockingStart(Agent agent){
      while(true){
        // to do: add logic for agent sleeping etc.
        agent.doBehaviours();
        delay(500);
      }
    }
};

