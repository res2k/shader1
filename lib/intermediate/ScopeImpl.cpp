// Must be first; otherwise, link errors may occur
#include "base/hash_UnicodeString.h"

#include "ScopeImpl.h"

#include "parser/Exception.h"
#include "NameImpl.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace intermediate
  {
    typedef IntermediateGeneratorSemanticsHandler::NamePtr NamePtr;
    typedef IntermediateGeneratorSemanticsHandler::BlockPtr BlockPtr;
    
    void IntermediateGeneratorSemanticsHandler::ScopeImpl::CheckIdentifierUnique (const UnicodeString& identifier)
    {
      IdentifierMap::iterator ident = identifiers.find (identifier);
      if (ident != identifiers.end())
      {
	throw parser::Exception (parser::IdentifierAlreadyDeclared);
      }
      if (parent)
	parent->CheckIdentifierUnique (identifier);
    }
    
    NamePtr IntermediateGeneratorSemanticsHandler::ScopeImpl::CheckIdentifierIsFunction (const UnicodeString& identifier)
    {
      IdentifierMap::iterator ident = identifiers.find (identifier);
      if ((ident != identifiers.end()) && (ident->second->GetType() != Name::Function))
      {
	throw parser::Exception (parser::IdentifierAlreadyDeclared);
      }
      if ((ident != identifiers.end()) && (ident->second)) return ident->second;
      if (parent)
	return parent->CheckIdentifierIsFunction (identifier);
      return NamePtr ();
    }
      
    IntermediateGeneratorSemanticsHandler::ScopeImpl::ScopeImpl (IntermediateGeneratorSemanticsHandler* handler,
								 const boost::shared_ptr<ScopeImpl>& parent,
								 ScopeLevel level)
     : handler (handler), parent (parent), level (level)
    {}

    NamePtr IntermediateGeneratorSemanticsHandler::ScopeImpl::AddVariable (TypePtr type, const UnicodeString& identifier,
									   ExpressionPtr initialValue, bool constant)
    {
      CheckIdentifierUnique (identifier);
      NamePtr newName (new NameImpl (shared_from_this(), identifier, 
				     boost::shared_static_cast<TypeImpl> (type), initialValue, constant));
      identifiers[identifier] = newName;
      newVars.push_back (newName);
      return newName;
    }
      
    NamePtr IntermediateGeneratorSemanticsHandler::ScopeImpl::AddTypeAlias (TypePtr aliasedType, const UnicodeString& identifier)
    {
      CheckIdentifierUnique (identifier);
      NamePtr newName (new NameImpl (shared_from_this(), identifier, Name::TypeAlias, 
				     boost::shared_static_cast<TypeImpl> (aliasedType)));
      identifiers[identifier] = newName;
      return newName;
    }
      
    BlockPtr IntermediateGeneratorSemanticsHandler::ScopeImpl::AddFunction (TypePtr returnType,
									    const UnicodeString& identifier,
									    const FunctionFormalParameters& params)
    {
      if (level >= Function)
	throw parser::Exception (parser::DeclarationNotAllowedInScope);
      NamePtr funcName (CheckIdentifierIsFunction (identifier));
      if (funcName == NamePtr ())
      {
	NamePtr newName (boost::make_shared<NameImpl> (shared_from_this(), identifier, Name::Function,
				      boost::shared_static_cast<TypeImpl> (returnType)));
	identifiers[identifier] = newName;
      }
      
      ScopePtr funcScope;
      funcScope = handler->CreateScope (shared_from_this(), Function);
      BlockPtr newBlock (handler->CreateBlock (funcScope));
      funcScope = ScopePtr();
      
      FunctionInfoInternalVector& functions = this->functions[identifier];
      FunctionInfoInternal funcInfo;
      funcInfo.returnType = returnType;
      funcInfo.params = params;
      funcInfo.block = newBlock;
      functions.push_back (funcInfo);
      
      return newBlock;
    }

    NamePtr IntermediateGeneratorSemanticsHandler::ScopeImpl::ResolveIdentifier (const UnicodeString& identifier)
    {
      IdentifierMap::iterator ident = identifiers.find (identifier);
      if (ident != identifiers.end())
      {
	return ident->second;
      }
      if (parent)
	return parent->ResolveIdentifier (identifier);
      throw parser::Exception (parser::IdentifierUndeclared);
    }
    
    IntermediateGeneratorSemanticsHandler::ScopeImpl::FunctionInfoVector
    IntermediateGeneratorSemanticsHandler::ScopeImpl::GetFunctions () const
    {
      FunctionInfoVector vec;
      for (FunctionsMap::const_iterator funcIt = functions.begin();
	   funcIt != functions.end();
	   ++funcIt)
      {
	for (FunctionInfoInternalVector::const_iterator vecIt = funcIt->second.begin();
	     vecIt != funcIt->second.end();
	     ++vecIt)
	{
	  FunctionInfo func;
	  func.identifier = funcIt->first;
	  func.block = vecIt->block;
	  func.params = vecIt->params;
	  func.returnType = vecIt->returnType;
	  vec.push_back (func);
	}
      }
      return vec;
    }
    
    std::vector<NamePtr> IntermediateGeneratorSemanticsHandler::ScopeImpl::FlushNewVars ()
    {
      std::vector<NamePtr> ret (newVars);
      newVars.erase (newVars.begin(), newVars.end());
      return ret;
    }
  } // namespace intermediate
} // namespace s1
