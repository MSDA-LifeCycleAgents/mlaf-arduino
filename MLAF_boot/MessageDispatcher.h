#include <list>
#include "AclMessage.h"
#include "Envelope.h"
#include "TcpSocket.h"

class MessageDispatcher{
  public:
    MessageDispatcher(){}
  
    void init(String wifi_ssid, String wifi_pass, int _port){
      socket.init(wifi_ssid, wifi_pass, _port);
    }

    String getIpAddress(){
      return socket.getIpAddressAsString();
    }

   void advertise(String name, String description, String timestamp){
      socket.advertise(name, description, timestamp);
   }
    
    AclMessage* receive(){
      AclMessage* result = NULL;
      for(auto message : messageQueue){
        result = message;
        messageQueue.remove(message);
        break;
      }
      return result;
    }
    
    void send(AclMessage* message){
      if(message->envelope == NULL)
        createEnvelope(message);
        
      cache.push_back(message);
    }

    void sendCache(){
      std::list<AclMessage*>::iterator i = cache.begin();

      while(i != cache.end()){
        if(socket.send(*i) > 0){
          i = cache.erase(i);
        }else{
          i++;
        }
      }
      
      for(auto message : cache){
        if(socket.send(message) > 0){
          cache.remove(message);
          delete message;
          message = NULL;
        }
      }
    }

    void fillQueue(){
      AclMessage* message = socket.listen();
      if(message != NULL){
        messageQueue.push_back(message);
      }
    }
  
  private:
    TcpSocket socket;
    std::list<AclMessage*> messageQueue;
    std::list<AclMessage*> cache;

    void createEnvelope(AclMessage* message){ 
      Envelope* env = new Envelope();
      env->to = message->receiver;
      env->from = message->sender;
      env->intendedReceiver = message->receiver;
      env->aclRepresentation = "fipa.acl.rep.string.std";
      env->payloadEncoding = "US-ASCII";
      message->envelope = env;
    }
};

