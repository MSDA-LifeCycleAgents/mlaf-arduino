#pragma once

#include "tinyxml2.h"

using namespace tinyxml2;

class MessageParser{
  public:
    MessageParser(){}
    
    String toXml(std::shared_ptr<AclMessage> message){
      XMLDocument doc;
      
      auto root = doc.NewElement("fipa-message");
      String perf = performativeToString(message->performative);
      char _perf[sizeof(perf) + 1];
      perf.toCharArray(_perf, sizeof(_perf));
      root->SetAttribute("communicative-act", _perf);

      Serial.println("Message sender name: " + message->sender->getName());
      auto sender = doc.NewElement("sender");
      sender->InsertEndChild(AidToXml(doc, message->sender));
      root->InsertEndChild(sender);

      auto receiver = doc.NewElement("receiver");
      receiver->InsertEndChild(AidToXml(doc, message->receiver));
      root->InsertEndChild(receiver);

      if(message->replyTo != NULL){
        auto replyTo = doc.NewElement("reply-to");
        replyTo->InsertEndChild(AidToXml(doc, message->replyTo));
        replyTo->InsertEndChild(replyTo);
      }

      addTag(doc, root, "content", message->content);
      addTag(doc, root, "language", message->language);
      addTag(doc, root, "ontology", message->ontology);
      addTag(doc, root, "protocol", message->protocol);
      addTag(doc, root, "conversation-id", message->conversationID);

      doc.InsertEndChild(root);

      if(message->envelope != NULL){
        int messageLength = getMessageLength(doc);
        auto env = message->envelope;
        env->payloadLength = messageLength;
        addEnvelopeToXml(doc, message->envelope);
      }

      doc.InsertFirstChild(doc.NewDeclaration());
      
      XMLPrinter printer;
      doc.Print(&printer);
      String result = printer.CStr();
      printer.ClearBuffer();
      result.replace("&lt;", "<");
      result.replace("&gt;", ">");
      return result;
    }

    std::shared_ptr<AclMessage> fromXml(String message){
      XMLDocument doc;
      doc.Parse(message.c_str());

      auto root = doc.FirstChildElement("fipa-message");
      String perfStr = root->Attribute("communicative-act");
      Performative performative = stringToPerformative(perfStr);
      
      auto xmlSender = root->FirstChildElement("sender");
      auto sender = xmlToAid(xmlSender);
      
      auto xmlReceiver = root->FirstChildElement("receiver");
      auto receiver = xmlToAid(xmlReceiver);

      auto xmlReplyTo = root->FirstChildElement("reply-to");
      auto replyTo = xmlReplyTo ? xmlToAid(xmlReplyTo) : NULL;

      auto xmlContent = root->FirstChildElement("content");
      String content = xmlContent ? xmlContent->GetText() : NULL;

      auto xmlLanguage = root->FirstChildElement("language");
      String language = xmlLanguage ? xmlLanguage->GetText() : NULL;

      auto xmlOntology = root->FirstChildElement("ontology");
      String ontology = xmlOntology ? xmlOntology->GetText() : NULL;

      auto xmlProtocol = root->FirstChildElement("protocol");
      String protocol = xmlProtocol ? xmlProtocol->GetText() : NULL;

      auto xmlConversationID = root->FirstChildElement("conversation-id");
      String conversationID = xmlConversationID ? xmlConversationID->GetText() : NULL;

      std::shared_ptr<Envelope> envelope = xmlToEnvelope(doc.FirstChildElement("envelope"));
      
      auto aclMessage = std::make_shared<AclMessage>(performative);
      aclMessage->sender = sender;
      aclMessage->receiver = receiver;
      aclMessage->replyTo = replyTo;
      aclMessage->content = content;
      aclMessage->language = language;
      aclMessage->ontology = ontology;
      aclMessage->protocol = protocol;
      aclMessage->conversationID = conversationID;
      aclMessage->envelope = envelope;
      
      return aclMessage;
    }
    
  private:
    int getMessageLength(XMLDocument& doc){
      XMLPrinter printer;
      doc.Print(&printer);
      String message = printer.CStr();
      printer.ClearBuffer();
      message.replace("&lt;", "<");
      message.replace("&gt;", ">");
      return message.length() + 1;
    }
  
    void addEnvelopeToXml(XMLDocument& doc, std::shared_ptr<Envelope> envelope){
      auto envelopeElement = doc.NewElement("envelope");
      auto root = doc.NewElement("params");
      root->SetAttribute("index", "1");
 
      auto from = doc.NewElement("from");
      from->InsertEndChild(AidToXml(doc, envelope->from));
      root->InsertEndChild(from);

      auto to = doc.NewElement("to");
      to->InsertEndChild(AidToXml(doc, envelope->to));
      root->InsertEndChild(to);

      auto intendedReceiver = doc.NewElement("intended-receiver");
      if(envelope->intendedReceiver)
        intendedReceiver->InsertEndChild(AidToXml(doc, envelope->intendedReceiver));
        
      root->InsertFirstChild(intendedReceiver);

      if(envelope->aclRepresentation)
        addTag(doc, root, "acl-representation", envelope->aclRepresentation);
      if(envelope->payloadEncoding)
        addTag(doc, root, "payload-encoding", envelope->payloadEncoding);
      
      addTag(doc, root, "payload-length", String(envelope->payloadLength));
      if(envelope->date && !envelope->date.equals(""))
        addTag(doc, root, "date", envelope->date);
      
      envelopeElement->InsertEndChild(root);
      doc.InsertFirstChild(envelopeElement);
    }
  
    std::shared_ptr<Envelope> xmlToEnvelope(XMLElement* element){
      auto envelope = std::make_shared<Envelope>();

      auto root = element->FirstChildElement("params");

      auto xmlTo = root->FirstChildElement("to");
      auto to = xmlToAid(xmlTo);

      auto xmlFrom = root->FirstChildElement("from");
      auto from = xmlToAid(xmlFrom);

      String aclRepresentation = root->FirstChildElement("acl-representation")->GetText();
      String payloadLength = root->FirstChildElement("payload-length")->GetText();
      String date = root->FirstChildElement("date")->GetText();

      envelope->to = to;
      envelope->from = from;
      envelope->aclRepresentation = aclRepresentation;
      envelope->payloadLength = payloadLength.toInt();
      envelope->date = date;

      return envelope;
    }
  
    XMLElement* AidToXml(XMLDocument& doc, std::shared_ptr<AID> aid){
      auto agentIdentifier = doc.NewElement("agent-identifier");
      
      auto agent_name = doc.NewElement("name");
      String name = aid->getName();

      char _name[name.length() + 1];
      name.toCharArray(_name, sizeof(_name));
      agent_name->SetText(_name);
      
      auto agent_addrss = doc.NewElement("addresses");
      auto addr_url = doc.NewElement("url");
      String url = "tcp://" + aid->getAddress() + ":" + aid->getPort();
      char _url[url.length() + 1];
      url.toCharArray(_url, sizeof(_url));
      addr_url->SetText(_url);

      agent_addrss->InsertEndChild(addr_url);
      agentIdentifier->InsertEndChild(agent_name);
      agentIdentifier->InsertEndChild(agent_addrss);

      return agentIdentifier;
    }

    std::shared_ptr<AID> xmlToAid(XMLElement* element){
      auto agentIdentifier = element->FirstChildElement("agent-identifier");
      if(!agentIdentifier->FirstChildElement("name") || !agentIdentifier->FirstChildElement("addresses"))
        return NULL;
      
      String agent_name = agentIdentifier->FirstChildElement("name")->GetText();
      auto agent_addrss = agentIdentifier->FirstChildElement("addresses");      
      String agent_addr = "";

      for(auto addr = agent_addrss->FirstChildElement("url"); addr != NULL; addr = addr->NextSiblingElement("url"))
      {
        String addr_text = addr->GetText();
        if(addr_text.startsWith("tcp")){
          agent_addr = addr_text;
          break;
        } 
      }

      if(agent_addr.equals("") || agent_addr == NULL){
        int index = agent_name.indexOf('@');
        agent_addr = agent_name.substring(index + 1);
      }

      int portIndex = agent_addr.lastIndexOf(':');
      String portStr = agent_addr.substring(portIndex + 1);
      agent_addr.remove(portIndex);
      if(agent_addr.startsWith("tcp://")){
        agent_addr.remove(0, 6);
      }

      auto aid = std::make_shared<AID>(agent_name, agent_addr);
      aid->setPort(portStr.toInt());
      return aid;
    }

    void addTag(XMLDocument& doc, XMLElement* element, const char* tagName, String value){
      auto tag = doc.NewElement(tagName);
      char _value[value.length() + 1];
      value.toCharArray(_value, sizeof(_value));
      tag->SetText(_value);
      element->InsertEndChild(tag);
    }
    
    String performativeToString(Performative performative){
      return performatives[performative - 1];
    }

    Performative stringToPerformative(String performative){
      Performative result;
      for(int i = 0; i < sizeof(performatives); i++){
        performative.toUpperCase();
        if(performative.equals(performatives[i])){
          result = static_cast<Performative>(i + 1);
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

