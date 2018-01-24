#pragma once
#include <AclMessage.h>

enum MatchType{
  EQUALS, STARTS_WITH, ENDS_WITH
};

class MessageTemplate{
  // friend std::make_shared<MessageTemplate>
  private:
    Performative performative = UNSET;
    String ontology = "-1";
    MatchType ontologyMatchType = EQUALS;

    MessageTemplate(){}
  public:
    boolean match(std::shared_ptr<AclMessage> message){
      boolean performativeOK, ontologyOK = false;
      
      if(performative != UNSET)
        performativeOK = _matchPerformative(message, performative);
      else
        performativeOK = true;
        
      if(!ontology.equals("-1"))
        ontologyOK = _matchOntology(message, ontology, ontologyMatchType);
      else
        ontologyOK = true;

      return performativeOK && ontologyOK;
    }

    boolean _matchPerformative(std::shared_ptr<AclMessage> message, Performative performative){
      return message->performative == performative;
    }

    boolean _matchOntology(std::shared_ptr<AclMessage> message, String ontology, MatchType matchBy){
      switch(matchBy){
        case EQUALS: return message->ontology.equals(ontology);
        case STARTS_WITH: return message->ontology.startsWith(ontology);
        case ENDS_WITH: return message->ontology.endsWith(ontology);
      }
    }

    // factory
    
    static std::shared_ptr<MessageTemplate> matchPerformative(Performative _performative){
      auto _template = std::shared_ptr<MessageTemplate>(new MessageTemplate);
      
      _template->performative = _performative;
      return _template;
    }

    static std::shared_ptr<MessageTemplate> matchOntology(String _ontology, MatchType _match = EQUALS){
      auto _template = std::shared_ptr<MessageTemplate>(new MessageTemplate);
      _template->ontology = _ontology;
      _template->ontologyMatchType = _match;
      return _template;
    }

    static std::shared_ptr<MessageTemplate> matchPerformativeAndOntology(Performative _performative, String _ontology, MatchType _match = EQUALS){
      auto _template = std::shared_ptr<MessageTemplate>(new MessageTemplate);
      _template->performative = _performative;
      _template->ontology = _ontology;
      _template->ontologyMatchType = _match;
      return _template;
    }
};

