#pragma once
#include <AclMessage.h>

enum MatchType{
  EQUALS, STARTS_WITH, ENDS_WITH
};

class MessageTemplate{
  private:
    Performative performative = UNSET;
    String ontology = "-1";
    MatchType ontologyMatchType = EQUALS;

    MessageTemplate(){}
  public:
    boolean match(AclMessage* message){
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

    boolean _matchPerformative(AclMessage* message, Performative performative){
      return message->performative == performative;
    }

    boolean _matchOntology(AclMessage* message, String ontology, MatchType matchBy){
      switch(matchBy){
        case EQUALS: return message->ontology.equals(ontology);
        case STARTS_WITH: return message->ontology.startsWith(ontology);
        case ENDS_WITH: return message->ontology.endsWith(ontology);
      }
    }

    // factory
    
    static MessageTemplate* matchPerformative(Performative _performative){
      MessageTemplate* _template = new MessageTemplate();
      _template->performative = _performative;
      return _template;
    }

    static MessageTemplate* matchOntology(String _ontology, MatchType _match = EQUALS){
      MessageTemplate* _template = new MessageTemplate();
      _template->ontology = _ontology;
      _template->ontologyMatchType = _match;
      return _template;
    }

    static MessageTemplate* matchPerformativeAndOntology(Performative _performative, String _ontology, MatchType _match = EQUALS){
      MessageTemplate* _template = new MessageTemplate();
      _template->performative = _performative;
      _template->ontology = _ontology;
      _template->ontologyMatchType = _match;
      return _template;
    }
};

