#pragma once

#include "tinyxml2.h"

using namespace tinyxml2;

/*
 * TO DO: Not handling Envelopes yet!!
 * 
 */

class MessageParser{
  public:
    MessageParser(){}
    
    String toXml(AclMessage message){
      XMLDocument doc;
      auto root = doc.NewElement("fipa-message");
      String perf = performativeToString(message.performative);
      root->SetAttribute("communicative-act", perf);
      doc.InsertEndChild(root);

      auto sender = doc.NewElement("sender");
      auto sender_name = doc.NewElement("name");
      sender_name->SetText(message.sender.getName());
      auto sender_addrss = doc.NewElement("addresses");
      auto sender_addr = doc.NewElement("address");
      sender_addr->SetText(message.sender.getAddress());
      sender_addrss->InsertEndChild(sender_addr);
      sender->InsertEndChild(sender_name);
      sender->InsertEndChild(sender_addrss);
      root->InsertEndChild(sender);

      auto content = doc.NewElement("content");
      content->SetText(message.content);
      root->InsertEndChild(content);
      
      XMLPrinter printer;
      doc.Print(&printer);
      return printer.CStr();
    }

    AclMessage fromXml(String message){
      XMLDocument doc;
      doc.Parse(message.c_str());

      auto root = doc.FirstChildElement("fipa-message");
      String perfStr = root->Attribute("communicative-act");
      Performative performative = stringToPerformative(perfStr);

      auto sender = root->FirstChildElement("sender");
      String sender_name = sender->FirstChildElement("name")->GetText();
      auto sender_addrss = sender->FirstChildElement("addresses");
      String sender_addr = sender_addrss->FirstChildElement("address")->GetText();

      String content = root->FirstChildElement("content")->GetText();
      
      AclMessage aclMessage(AGREE);
      AID aclSender(sender_name, sender_addr);
      aclMessage.sender = aclSender;
      aclMessage.content = content;
      
      return aclMessage;
    }
    
  private:  
    String performativeToString(Performative performative){
      return performatives[performative];
    }

    Performative stringToPerformative(String performative){
      Performative result;
      for(int i = 0; i < sizeof(performatives); i++){
        performative.toUpperCase();
        if(performative == performatives[i]){
          result = static_cast<Performative>(i);
          break;
        }
      }
      return result;
    }

    String performatives[20] = {"ACCEPT_PROPOSAL",
  "AGREE",
  "CFP",
  "CONFIRM",
  "DISCONFIRM",
  "FAILURE",
  "INFORM",
  "INFORM_IF",
  "INFORM_REF",
  "NOT_UNDERSTOOD",
  "PROPOSE",
  "QUERY_IF",
  "QUERY_REF",
  "REFUSE",
  "REJECT_PROPOSAL",
  "REQUEST",
  "REQUEST_WHEN",
  "SUBSCRIBE",
  "PROXY",
  "PROPAGATE"};
};

