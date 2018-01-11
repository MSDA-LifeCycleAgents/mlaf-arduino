#include <list>
#include "AclMessage.h"
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
    
    AclMessage& receive(){
      AclMessage result;
      for(auto& message : messageQueue){
        result = message;
        messageQueue.remove(message);
        break;
      }
      return result;
    }
    
    void send(AclMessage message){
      cache.push_back(message);
    }

    void sendCache(){
      for(auto& message : cache){
        if(socket.send(message) > 0){
          cache.remove(message);
        }
      }
    }

    void fillQueue(){
      AclMessage message = socket.listen();
      if(message != NULL){
        messageQueue.push_back(message);
      }
    }
  
  private:
    TcpSocket socket;
    std::list<AclMessage> messageQueue;
    std::list<AclMessage> cache;
};
