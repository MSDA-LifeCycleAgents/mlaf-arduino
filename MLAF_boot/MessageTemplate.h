#pragma once
#include "AclMessage.h"

/**
 * \brief Enum to match a given ontology in a specified manner
 */
enum MatchType{
  EQUALS, STARTS_WITH, ENDS_WITH
};

/**
 * \brief Allows messages to be matched based on their performative and ontology
 */
class MessageTemplate{
  // friend std::make_shared<MessageTemplate>
  private:
    Performative performative = UNSET;
    String ontology = "-1";
    MatchType ontologyMatchType = EQUALS;

    MessageTemplate(){}
  public:
    /**
     * \brief Matches an ACL-message based on their performative and ontology
     * 
     * \param message the message to match
     * 
     * \return <i>true</i> if both the performative and ontology match, <i>false</i> otherwise
     */
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

    /**
     * \brief Matches an ACL-message based on the given performative
     * 
     * \param message the message to compare to
     * \param performative the performative to check against
     * 
     * \return <i>true</i> if the performative matches the one in the message, <i>false</i> otherwise
     */
    boolean _matchPerformative(std::shared_ptr<AclMessage> message, Performative performative){
      return message->performative == performative;
    }

    /**
     * \brief Matches an ACL-message based on the given ontology
     * 
     * \param message the message to compare to
     * \param ontology the ontology to check against
     * \param matchBy the way to match the ontology against the message
     * 
     * \return <i>true</i> if the ontology matches the one in the message, <i>false</i> otherwise
     */    boolean _matchOntology(std::shared_ptr<AclMessage> message, String ontology, MatchType matchBy){
      switch(matchBy){
        case EQUALS: return message->ontology.equals(ontology);
        case STARTS_WITH: return message->ontology.startsWith(ontology);
        case ENDS_WITH: return message->ontology.endsWith(ontology);
      }
    }

    // factory
    
    /**
     * \brief Creates a MessageTemplate based on a given performative
     * 
     * \param performative the performative to use
     * 
     * \return a MessageTemplate based on the performative
     */
    static std::shared_ptr<MessageTemplate> matchPerformative(Performative _performative){
      auto _template = std::shared_ptr<MessageTemplate>(new MessageTemplate);
      
      _template->performative = _performative;
      return _template;
    }

    /**
     * \brief Creates a MessageTemplate based on a given ontology
     * 
     * \param ontology the ontology to use
     * \param match the way to match the ontology against other messages
     * 
     * \return a MessageTemplate based on the ontology
     */
    static std::shared_ptr<MessageTemplate> matchOntology(String _ontology, MatchType _match = EQUALS){
      auto _template = std::shared_ptr<MessageTemplate>(new MessageTemplate);
      _template->ontology = _ontology;
      _template->ontologyMatchType = _match;
      return _template;
    }

    /**
     * \brief Creates a MessageTemplate based on a given performative and ontology
     * 
     * \param performative the performative to use
     * \param ontology the ontology to use
     * \param match the way to match the ontology against other messages
     * 
     * \return a MessageTemplate based on the performative and ontology
     */
    static std::shared_ptr<MessageTemplate> matchPerformativeAndOntology(Performative _performative, String _ontology, MatchType _match = EQUALS){
      auto _template = std::shared_ptr<MessageTemplate>(new MessageTemplate);
      _template->performative = _performative;
      _template->ontology = _ontology;
      _template->ontologyMatchType = _match;
      return _template;
    }
};

