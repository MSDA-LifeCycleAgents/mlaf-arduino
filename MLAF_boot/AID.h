#pragma once

/**
 * Agent identifier (AID)
 */
class AID{
private:
    String _name;
    String _address;
    int _port;
  
public:

    /**
     * \brief Constructs an empty AID
     * 
     * Values will be empty and port will be -1
     */
    AID() : AID("", "", -1)
    {}
    
    /**
     * \brief Constructs an AID
     * 
     * The address and port will be empty and -1 respectively
     * 
     * \param name the name
     */
    AID(String name) : AID(name, "", -1)
    {}

    /**
     * \brief Constructs an AID
     * 
     * The port will be -1
     * 
     * \param name the name
     * \param address the address
     */
    AID(String name, String address) : AID(name, address, -1)
    {}
    
    /**
     * \brief Constructs an AID
     * 
     * \param name the name
     * \param address the address
     * \param port the port
     */
    AID(String name, String address, int port) : _name{name}, _address{address}, _port{port}
    {}

    /**
     * Gets the address associated with this AID
     */
    String getAddress() const{
      return _address;
    }

    /**
     * Sets the address associated with this AID
     */
    void setAddress(String address){
      _address = address;
    }

    /**
     * Gets the name associated with this AID
     * 
     * \return the name
     */
    String getName() const{
      return _name;
    }

    /**
     * Sets the name associated with this AID
     * 
     * \param name the name
     */
    String setName(String name){
      _name = name;
    }

    /**
     * Gets the port associated with this AID
     * 
     * \return the port
     */
    int getPort() const{
      return _port;
    }

    /**
     * Sets the port associated with this AID
     * 
     * \param port the port
     */
    void setPort(int port){
      _port = port;
    }

    static AID* copy(AID* aid){
        AID* copy = new AID(aid->getName(), aid->getAddress());
        copy->setPort(aid->getPort());
        return copy;
    }
};