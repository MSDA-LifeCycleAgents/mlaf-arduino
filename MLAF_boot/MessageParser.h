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

      // parse envelope
      
      auto root = doc.NewElement("fipa-message");
      String perf = performativeToString(message.performative);
      root->SetAttribute("communicative-act", perf);
      doc.InsertEndChild(root);
      
      auto sender = doc.NewElement("sender");
      sender->InsertEndChild(AidToXml(doc, message.sender));
      root->InsertEndChild(sender);

      auto receiver = doc.NewElement("receiver");
      receiver->InsertEndChild(AidToXml(doc, message.receiver));
      root->InsertEndChild(receiver);
      
      addTag(doc, root, "content", message.content);
      addTag(doc, root, "language", message.language);
      addTag(doc, root, "ontology", message.ontology);
      addTag(doc, root, "protocol", message.protocol);
      addTag(doc, root, "conversationID", message.conversationID);
      
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

      auto xmlSender = root->FirstChildElement("sender");
      AID sender = xmlToAid(xmlSender);
      
      auto xmlReceiver = root->FirstChildElement("receiver");
      AID receiver = xmlToAid(xmlReceiver);
      
      String content = root->FirstChildElement("content")->GetText();
      String language = root->FirstChildElement("language")->GetText();
      String ontology = root->FirstChildElement("ontology")->GetText();
      String protocol = root->FirstChildElement("protocol")->GetText();
      String conversationID = root->FirstChildElement("conversationID")->GetText();
      
      AclMessage aclMessage(AGREE);
      aclMessage.sender = sender;
      aclMessage.receiver = receiver;
      aclMessage.content = content;
      aclMessage.language = language;
      aclMessage.ontology = ontology;
      aclMessage.protocol = protocol;
      aclMessage.conversationID = conversationID;
      
      // parse envelope
      
      return aclMessage;
    }
    
  private:
    XMLElement* AidToXml(XMLDocument& doc, AID aid){
      auto agentIdentifier = doc.NewElement("agent-identifier");
      auto agent_name = doc.NewElement("name");
      agent_name->SetText(aid.getName());
      auto agent_addrss = doc.NewElement("addresses");
      auto agent_addr = doc.NewElement("address");
      agent_addr->SetText(aid.getAddress());

      agent_addrss->InsertEndChild(agent_addr);
      agentIdentifier->InsertEndChild(agent_name);
      agentIdentifier->InsertEndChild(agent_addrss);

      return agentIdentifier;
    }

    AID xmlToAid(XMLElement* element){
      auto agentIdentifier = element->FirstChildElement("agent-identifier");
      String agent_name = agentIdentifier->FirstChildElement("name")->GetText();
      auto agent_addrss = agentIdentifier->FirstChildElement("addresses");
      String agent_addr = agent_addrss->FirstChildElement("address")->GetText();

      AID aid(agent_name, agent_addr);
      return aid;
    }

    void addTag(XMLDocument& doc, XMLElement* element, const char* tagName, String value){
      auto tag = doc.NewElement(tagName);
      tag->SetText(value);
      element->InsertEndChild(tag);
    }
    
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

