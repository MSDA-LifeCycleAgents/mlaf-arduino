#pragma once
#include "Agent.h"

/**
 * \brief A platform to run agents on
 */
class AgentPlatform{
  public:
    /**
     * \brief Starts an agent
     * 
     * NOTE: this is a blocking function
     * 
     * \param the agent to run
     */
    static void blockingStart(Agent* agent){
      agent->setup();
      while(true){
        // to do: add logic for agent sleeping etc.
        agent->doBehaviours();
        delay(500);
      }
    }
};

