#pragma once

class AID{
  private:
    String name;
    String address;
    int port;
  
  public:
    AID(String _name, String _address){
      name = _name;
      address = _address;
      port = -1;
    }

    AID(String _name){
      name = _name;
      port = -1; 
    }

    AID(){}

    void setAddress(String _address){
      address = _address;
    }

    String getAddress() const{
      return address;
    }

    String getName() const{
      return name;
    }

    String setName(String _name){
      name = _name;
    }

    int getPort() const{
      return port;
    }

    void setPort(int _port){
      port = _port;
    }
};


