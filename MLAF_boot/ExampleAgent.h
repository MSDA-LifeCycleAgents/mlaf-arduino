#include "Agent.h"
#include "Behaviour.h"

class ExampleAgent : public Agent{
  public:
    ExampleAgent(String name, int port) : Agent(name, port){}

    void setup(){
      class MyBehaviour : public Behaviour {
        public:
          virtual void action(){
            AclMessage& message = receive();
            
            if(message != NULL){
              Serial.println(message.toString());
      
              AclMessage response(AGREE);
              response.sender = getAID();
              response.receiver = message.sender;
              response.content = "I received your message!";
              
              send(response);
            }
          }
      } behaviour;
      
      addBehaviour(behaviour);
    }
};

