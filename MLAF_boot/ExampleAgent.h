#include "Agent.h"

class ExampleAgent : public Agent{
  public:
    ExampleAgent(String name, int port) : Agent(name, port){}

    void setup(){
      addBehaviour([this]{
          AclMessage* message = receive();
          
          if(message != NULL){
            Serial.println("Received: " + message->toString());
            
            AclMessage* response = new AclMessage(REQUEST);
            response->sender = getAID();
            response->receiver = message->sender;
            response->content = "I received your message!";
            
            Serial.println("Sending: " + response->toString());
            send(response);

            AclMessage::destroy(message);
          }
        });
    }
};

