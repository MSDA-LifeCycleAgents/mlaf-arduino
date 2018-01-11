#include "Agent.h"

class ExampleAgent : public Agent{
  public:
    ExampleAgent(String name, int port) : Agent(name, port){}

    void setup(){
      addBehaviour([this]{
          AclMessage& message = receive();
          
          if(message != NULL){
            Serial.println(message.toString());
    
            AclMessage response(AGREE);
            response.sender = getAID();
            response.receiver = message.sender;
            response.content = "I received your message!";
            
            send(response);
          }
        });
    }
};

