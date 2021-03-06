#pragma once
#include "Agent.h"

/**
 * @brief      Class for agent platform.
 */
class AgentPlatform{
  public:
    static void blockingStart(Agent* agent){
      agent->setup();
      while(true){
        // to do: add logic for agent sleeping etc.
        agent->doBehaviours();
        delay(500);
      }
    }
};

