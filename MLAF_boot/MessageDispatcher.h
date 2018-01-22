#pragma once

#include <list>
#include "AclMessage.h"
#include "Envelope.h"
#include "TcpSocket.h"

/**
 * 
 */
class MessageDispatcher{
public:
    MessageDispatcher(){}
  
    void init(String wifi_ssid, String wifi_pass, int port){
        _socket.init(wifi_ssid, wifi_pass, port);
    }

    String getIpAddress(){
        return _socket.getIpAddressAsString();
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
        _socket.advertise(name, description, timestamp);
    }
    
    /**
     * \brief Receives an ACL-message
     * 
     * \param pop pops the message of the stack if <i>true</i>
     * 
     * \return An ACL-message if there is any, otherwise it returns a nullptr
     */
    AclMessage* receive(bool pop){
        if (!_messageQueue.empty()){
            auto message = _messageQueue.front();
            if (pop)
                _messageQueue.pop_front();
        }
        return nullptr;
    }
    
    /**
     * \brief Puts an ACL-message in the send-buffer
     * 
     * \param message the message to be stored
     */
    // TODO: determine if messages need to be send right away, or put in a buffer to be send
    void send(AclMessage* message){
        if(!message->envelope)
            message->envelope = new Envelope(message->receiver, message->sender, message->receiver);
        
        _cache.push_back(message);
    }

    /**
     * \brief Sends all ACL-messages stored in the cache
     */
    void sendCache(){
        std::list<AclMessage*>::iterator i = _cache.begin();

        while(i != _cache.end()){
            if(_socket.send(*i) > 0){
                i = _cache.erase(i);
            }else{
                i++;
            }
        }
        
        for(auto message : _cache){
            if(_socket.send(message) > 0){
                _cache.remove(message);
                delete message;
                message = nullptr;
            }
        }
    }

    /**
     * \brief Fills the message queue with a message, if there is any
     */
    void fillQueue(){
        AclMessage* message = _socket.listen();
        if(message){
            _messageQueue.push_back(message);
        }
    }
  
private:
    TcpSocket _socket;
    std::list<AclMessage*> _messageQueue;
    std::list<AclMessage*> _cache;
};