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

      message.envelope.payloadLength = getMessageLength(doc);
      addEnvelopeToXml(doc, message.envelope);
      
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

      Envelope envelope = xmlToEnvelope(doc.FirstChildElement("envelope"));
      
      AclMessage aclMessage(AGREE);
      aclMessage.sender = sender;
      aclMessage.receiver = receiver;
      aclMessage.content = content;
      aclMessage.language = language;
      aclMessage.ontology = ontology;
      aclMessage.protocol = protocol;
      aclMessage.conversationID = conversationID;
      aclMessage.envelope = envelope;
      
      return aclMessage;
    }
    
  private:
    int getMessageLength(XMLDocument& doc){
      XMLPrinter printer;
      doc.Print(&printer);
      String message = printer.CStr();
      return message.length();
    }
  
    void addEnvelopeToXml(XMLDocument& doc, Envelope& envelope){
      // to do     -> 
      
      auto envelopeElement = doc.NewElement("envelope");
      auto root = doc.NewElement("params");
      root->SetAttribute("index", "1");

      auto from = doc.NewElement("from");
      from->InsertEndChild(AidToXml(doc, envelope.from));
      root->InsertEndChild(from);

      auto to = doc.NewElement("to");
      to->InsertEndChild(AidToXml(doc, envelope.to));
      root->InsertEndChild(to);

      auto intendedReceiver = doc.NewElement("intendedReceiver");
      intendedReceiver->InsertEndChild(AidToXml(doc, envelope.intendedReceiver));
      root->InsertEndChild(intendedReceiver);

      addTag(doc, root, "acl-representation", envelope.aclRepresentation);
      addTag(doc, root, "payload-encoding", envelope.payloadEncoding);
      addTag(doc, root, "payload-length", String(envelope.payloadLength));
      addTag(doc, root, "date", envelope.date);
      
      envelopeElement->InsertEndChild(root);
      doc.InsertFirstChild(envelopeElement);
    }
  
    Envelope xmlToEnvelope(XMLElement* element){
      Envelope envelope;

      auto root = element->FirstChildElement("params");
      
      auto xmlTo = root->FirstChildElement("to");
      AID to = xmlToAid(xmlTo);

      auto xmlFrom = root->FirstChildElement("from");
      AID from = xmlToAid(xmlFrom);

      String aclRepresentation = root->FirstChildElement("acl-representation")->GetText();
      String payloadEncoding = root->FirstChildElement("payload-encoding")->GetText();
      String payloadLength = root->FirstChildElement("payload-length")->GetText();
      String date = root->FirstChildElement("date")->GetText();

      envelope.to = to;
      envelope.from = from;
      envelope.aclRepresentation = aclRepresentation;
      envelope.payloadEncoding = payloadEncoding;
      envelope.payloadLength = payloadLength.toInt();
      envelope.date = date;

      return envelope;
    }
  
    XMLElement* AidToXml(XMLDocument& doc, AID aid){
      auto agentIdentifier = doc.NewElement("agent-identifier");
      auto agent_name = doc.NewElement("name");
      agent_name->SetText(aid.getName());
      auto agent_addrss = doc.NewElement("addresses");
      auto agent_addr = doc.NewElement("address");
      auto addr_url = doc.NewElement("url");
      addr_url->SetText(aid.getAddress());

      agent_addr->InsertEndChild(addr_url);
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

