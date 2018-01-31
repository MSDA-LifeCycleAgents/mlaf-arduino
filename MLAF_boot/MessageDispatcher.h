#include <list>
#include "AclMessage.h"
#include "Envelope.h"
#include "TcpSocket.h"
#include "MessageTemplate.h"

class MessageDispatcher{
  public:
    MessageDispatcher(){}
    
    /**
     * @brief      Initialize the message dispatcher
     *
     * @param[in]  wifi_ssid  The wifi ssid
     * @param[in]  wifi_pass  The wifi pass
     * @param[in]  _port      The TCP/IP port
     */
    void init(String wifi_ssid, String wifi_pass, int _port){
      socket.init(wifi_ssid, wifi_pass, _port);
    }

    /**
     * @brief      Gets the ip address.
     *
     * @return     The ip address.
     */
    String getIpAddress(){
      return socket.getIpAddressAsString();
    }

   /**
    * @brief      Advertises the device on the network using mDNS
    *
    * @param[in]  name         The name
    * @param[in]  description  The description
    * @param[in]  timestamp    The timestamp
    */
   void advertise(String name, String description, String timestamp){
      socket.advertise(name, description, timestamp);
   }
    
    /**
     * @brief      Receives an ACLmessage
     *
     * @param[in]  _template  The template to use
     *
     * @return     Return an ACLmessage if there are any available, otherwise return a nullptr.
     */
    std::shared_ptr<AclMessage> receive(std::shared_ptr<MessageTemplate> _template){
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
    
    /**
     * @brief      Put a message in the send queue
     *
     * @param[in]  message  The message
     */
    void send(std::shared_ptr<AclMessage> message){
      if(!message->envelope)
        createEnvelope(message);
        
      cache.push_back(message);
    }

    /**
     * @brief      Send all the message in the queue
     */
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


    /**
     * @brief      Listen for new message, and put it in the recv queue
     */
    void fillQueue(){
      std::shared_ptr<AclMessage> message = socket.listen();
      if(message != NULL){
        messageQueue.push_back(message);
      }
    }

    /**
     * @brief      Sets the default envelope receiver.
     *
     * @param[in]  aid   The aid
     */
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
