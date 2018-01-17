#include "Agent.h"

class ExampleAgent : public Agent{
  public:
    ExampleAgent(String name, int port) : Agent(name, port){}

    void setup(){
      addBehaviour([this]{
          // receive the incoming message
          AclMessage* message; 
          


            // you can either create a new message or create a reply
            AclMessage* response = message->createReply(INFORM);
            response->content = "I received your message!";
            
            // send the message 
            // (be carefull with printing the sender, it will be set later)
            send(response);

            // don't forget to collect your garbage
            //AclMessage::destroy(message);
          
        });
    }
};


