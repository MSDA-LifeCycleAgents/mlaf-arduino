#include <list>
#include "AclMessage.h"
#include "Envelope.h"
#include "TcpSocket.h"
#include "MessageTemplate.h"

/**
 * \brief Dispatches messages to and from this device
 */
class MessageDispatcher{
  public:
    /**
     * \brief Default constructor
     */
    MessageDispatcher(){}
    
    /**
     * \brief Initializes the Message Dispatcher
     * 
     * \param wifi_ssid the Wi-Fi SSID (name) to connect to
     * \param wifi_pass the password to use
     * \param port the port to use
     */
    void init(String wifi_ssid, String wifi_pass, int _port){
      socket.init(wifi_ssid, wifi_pass, _port);
    }

    /**
     * \brief Gets the current IP-address of this device
     * 
     * \return the current IP-addres as a String
     */
    String getIpAddress(){
      return socket.getIpAddressAsString();
    }

    /**
     * \brief Advertises the device on the network using mDNS
     * 
     * \param name the name to use
     * \param description the description to use
     * \param timestamp the timestamp to use
     */
    // TODO: determine which standard format to use for the timestamp
   void advertise(String name, String description, String timestamp){
      socket.advertise(name, description, timestamp);
   }
    
    /**
     * \brief Receives an ACL-message
     * 
     * \param msgTemplate the message template to compare to
     * 
     * \return An ACL-message if there is any, otherwise it returns an empty smart pointer
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
     * \brief Puts an ACL-message in the send-buffer
     * 
     * \param message the message to be stored
     */
    // TODO: determine if messages need to be send right away, or put in a buffer to be send
    void send(std::shared_ptr<AclMessage> message){
      if(!message->envelope)
        createEnvelope(message);
        
      cache.push_back(message);
    }

    /**
     * \brief Sends all ACL-messages stored in the cache
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
     * \brief Fills the message queue with a message, if there is any
     */
    void fillQueue(){
      std::shared_ptr<AclMessage> message = socket.listen();
      if(message != NULL){
        messageQueue.push_back(message);
      }
    }

    /**
     * \brief Sets the default envelope receiver (AID)
     * 
     * \param aid the default receiver
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
