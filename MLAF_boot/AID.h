#pragma once
#include <memory>



/**
 * @brief      Agent Identifier (AID)
 */
class AID{
  private:
    String name;
    String address;
    int port;
  
  public:

    /**
     * @brief      Constructor
     *
     * @param[in]  _name     Agent Name
     * @param[in]  _address  Agent Network Address
     */
    AID(String _name, String _address){
      name = _name;
      address = _address;
      port = -1;
    }

    /**
     * @brief      Constructor
     *
     * @param[in]  _name  Agent Name
     */
    AID(String _name){
      name = _name;
      port = -1;
    }


    /**
     * @brief      Default Constructor
     */
    AID(){}
    

    /**
     * @brief      Sets the address.
     *
     * @param[in]  _address  The address
     */
    void setAddress(String _address){
      address = _address;
    }


    /**
     * @brief      Gets the address.
     *
     * @return     The address.
     */
    String getAddress() const{
      return address;
    }


    /**
     * @brief      Gets the name.
     *
     * @return     The name.
     */
    String getName() const{
      return name;
    }


    /**
     * @brief      Sets the name.
     *
     * @param[in]  _name  The name
     */
    void setName(String _name){
      name = _name;
    }

    /**
     * @brief      Gets the port.
     *
     * @return     The port.
     */
    int getPort() const{
      return port;
    }

    /**
     * @brief      Sets the port.
     *
     * @param[in]  _port  The port
     */
    void setPort(int _port){
      port = _port;
    }

    /**
     * @brief      Create a copy
     *
     * @param[in]  aid   The aid
     *
     * @return     std::shared_ptr<AID> to the generated copy
     */
    static std::shared_ptr<AID> copy(std::shared_ptr<AID> aid){
      auto copy = std::make_shared<AID>(aid->getName(), aid->getAddress());
      copy->setPort(aid->getPort());
      return copy;
    }
};

