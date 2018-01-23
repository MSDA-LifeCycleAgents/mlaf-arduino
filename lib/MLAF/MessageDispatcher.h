#include <list>
#include <AclMessage.h>
#include <Envelope.h>
#include <TcpSocket.h>
#include <MessageTemplate.h>

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
    
    std::shared_ptr<AclMessage> receive(MessageTemplate* _template){
      std::shared_ptr<AclMessage> result = NULL;
      for(auto message : messageQueue){
        if(_template == NULL || _template->match(message)){
          result = message;
          messageQueue.remove(message);
          break;
        }
      }
      return result;
    }
    
    void send(std::shared_ptr<AclMessage> message){
      if(!message->envelope)
        createEnvelope(message);
        
      cache.push_back(message);
    }

    void sendCache(){
      std::list<std::shared_ptr<AclMessage>>::iterator i = cache.begin();

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
          message.reset();
        }
      }
    }

    void fillQueue(){
      std::shared_ptr<AclMessage> message = socket.listen();
      if(message != NULL){
        messageQueue.push_back(message);
      }
    }

    void setDefaultEnvelopeReceiver(std::shared_ptr<AID> aid){
      defaultEnvelopeReceiver = aid;
    }
  
  private:
    TcpSocket socket;
    std::shared_ptr<AID> defaultEnvelopeReceiver;
    std::list<std::shared_ptr<AclMessage>> messageQueue;
    std::list<std::shared_ptr<AclMessage>> cache;

    void createEnvelope(std::shared_ptr<AclMessage> message){
      auto env = std::make_shared<Envelope>();
      env->to = defaultEnvelopeReceiver ? defaultEnvelopeReceiver : message->receiver;
      env->from = message->sender;
      env->intendedReceiver = message->receiver;
      env->aclRepresentation = "fipa.acl.rep.string.std";
      env->payloadEncoding = "US-ASCII";
      message->envelope = env;
    }
};
