#pragma once
#include <memory>

/**
 * \brief Agent identifier (AID)
 */
class AID{
  private:
    String name;
    String address;
    int port;
  
  public:
    /**
     * \brief Constructs an AID
     * 
     * The port will be -1
     * 
     * \param name the name
     * \param address the address
     */
    AID(String _name, String _address){
      name = _name;
      address = _address;
      port = -1;
    }

    /**
     * \brief Constructs an AID
     * 
     * The address and port will be empty and -1 respectively
     * 
     * \param name the name
     */
    AID(String _name){
      name = _name;
      port = -1;
    }

    /**
     * \brief Constructs an empty AID
     * 
     * Values will be empty and port will be -1
     */
    AID(){}

    /**
     * \brief Sets the address associated with this AID
     * 
     * \param address the new address
     */
    void setAddress(String _address){
      address = _address;
    }

    /**
     * \brief Gets the address associated with this AID
     * 
     * \return the current address
     */
    String getAddress() const{
      return address;
    }

    /**
     * \brief Gets the name associated with this AID
     * 
     * \return the current name
     */
    String getName() const{
      return name;
    }

    /**
     * \brief Sets the name associated with this AID
     * 
     * \param name the new name
     */
    String setName(String _name){
      name = _name;
    }

    /**
     * \brief Gets the port associated with this AID
     * 
     * \return the current port
     */
    int getPort() const{
      return port;
    }

    /**
     * \brief Sets the port associated with this AID
     * 
     * \param port the new port
     */
    void setPort(int _port){
      port = _port;
    }

    /**
     * \brief Creates a copy of the given AID
     * 
     * \param aid the AID to create a copy of
     */
    static std::shared_ptr<AID> copy(std::shared_ptr<AID> aid){
      auto copy = std::make_shared<AID>(aid->getName(), aid->getAddress());
      copy->setPort(aid->getPort());
      return copy;
    }
};

