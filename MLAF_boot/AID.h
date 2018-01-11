#pragma once

class AID{
  private:
    String name;
    String address;
  
  public:
    AID(String _name, String _address){
      name = _name;
      address = _address;
    }

    AID(String _name){
      name = _name;
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
};

