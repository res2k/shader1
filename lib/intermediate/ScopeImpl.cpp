#include "base/common.h"

#include "BlockImpl.h"
#include "Builtin.h"
#include "ScopeImpl.h"

#include "parser/Exception.h"
#include "ExpressionImpl.h"
#include "NameImpl.h"

#include <boost/make_shared.hpp>
#include <limits.h>

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
								 ScopeLevel level,
								 const TypePtr& funcReturnType)
     : handler (handler), parent (parent), level (level), funcReturnType (funcReturnType)
    {}

    void IntermediateGeneratorSemanticsHandler::ScopeImpl::AddParameter (const FunctionFormalParameter& param)
    {
      // TODO: Keep? Or allow params shadow global vars?
      CheckIdentifierUnique (param.identifier);
      NamePtr newName (new NameImpl (shared_from_this(), param.identifier, 
				     boost::static_pointer_cast<TypeImpl> (param.type),
				     param.defaultValue,
				     param.dir == dirIn,
				     param.dir == dirOut));
      identifiers[param.identifier] = newName;
      newVars.push_back (newName);
      
      if ((param.dir & dirOut) != 0)
	outputParams.push_back (param.identifier);
    }

    NamePtr IntermediateGeneratorSemanticsHandler::ScopeImpl::AddVariable (TypePtr type, const UnicodeString& identifier,
									   ExpressionPtr initialValue, bool constant)
    {
      CheckIdentifierUnique (identifier);
      NamePtr newName (new NameImpl (shared_from_this(), identifier, 
				     boost::static_pointer_cast<TypeImpl> (type), initialValue, constant));
      identifiers[identifier] = newName;
      newVars.push_back (newName);
      return newName;
    }
      
    NamePtr IntermediateGeneratorSemanticsHandler::ScopeImpl::AddTypeAlias (TypePtr aliasedType, const UnicodeString& identifier)
    {
      CheckIdentifierUnique (identifier);
      NamePtr newName (new NameImpl (shared_from_this(), identifier, Name::TypeAlias, 
				     boost::static_pointer_cast<TypeImpl> (aliasedType)));
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
				      boost::shared_ptr<TypeImpl> ()));
	identifiers[identifier] = newName;
      }
      
      ScopePtr funcScope;
      funcScope = handler->CreateScope (shared_from_this(), Function, returnType);
      boost::shared_ptr<ScopeImpl> funcScopeImpl (boost::static_pointer_cast<ScopeImpl> (funcScope));
      for (FunctionFormalParameters::const_iterator param (params.begin());
	   param != params.end();
	   ++param)
      {
	funcScopeImpl->AddParameter (*param);
      }
      BlockPtr newBlock (handler->CreateBlock (funcScope));
      funcScope = ScopePtr();
      
      FunctionInfoVector& functions = this->functions[identifier];
      FunctionInfoPtr funcInfo (boost::make_shared<FunctionInfo> ());
      funcInfo->originalIdentifier = identifier;
      // Decorate identifier with type info (so each overload gets a unique name)
      UnicodeString identifierDecorated (identifier);
      identifierDecorated.append ("$");
      for (FunctionFormalParameters::const_iterator param (params.begin());
	   param != params.end();
	   ++param)
      {
	std::string decorationString;
	char dirStr[2] = { 0, 0 };
	dirStr[0] = param->dir + '0';
	decorationString.append (dirStr);
	TypeImplPtr typeImpl (boost::static_pointer_cast<TypeImpl> (param->type));
	decorationString.append (handler->GetTypeString (typeImpl));
	identifierDecorated.append (decorationString.c_str());
      }
      funcInfo->identifier = identifierDecorated;
      funcInfo->returnType = returnType;
      funcInfo->params = params;
      funcInfo->block = newBlock;
      functions.push_back (funcInfo);
      
      functionsInDeclOrder.push_back (funcInfo);
      
      if (handler->IsEntryFunction (identifier))
      {
	boost::shared_ptr<BlockImpl> blockImpl (boost::static_pointer_cast<BlockImpl> (newBlock));
	blockImpl->GenerateGlobalVarInitialization();
      }	
      
      return newBlock;
    }

    NamePtr IntermediateGeneratorSemanticsHandler::ScopeImpl::ResolveIdentifier (const UnicodeString& identifier)
    {
      NameImplPtr name (ResolveIdentifierInternal (identifier));
      if (!name)
	throw parser::Exception (parser::IdentifierUndeclared);
      return name;
    }
    
    IntermediateGeneratorSemanticsHandler::NameImplPtr
    IntermediateGeneratorSemanticsHandler::ScopeImpl::ResolveIdentifierInternal (const UnicodeString& identifier)
    {
      IdentifierMap::iterator ident = identifiers.find (identifier);
      if (ident != identifiers.end())
      {
	return boost::static_pointer_cast<NameImpl> (ident->second);
      }
      if (parent)
	return parent->ResolveIdentifierInternal (identifier);
      return NameImplPtr ();
    }
    
    void IntermediateGeneratorSemanticsHandler::ScopeImpl::AddBuiltinFunction (const BuiltinPtr& builtin)
    {
      if (level >= Function)
	throw parser::Exception (parser::DeclarationNotAllowedInScope);
      
      const UnicodeString& identifier = builtin->GetIdentifier();
      const FunctionFormalParameters& params = builtin->GetFormalParameters();
      NamePtr funcName (CheckIdentifierIsFunction (identifier));
      if (funcName == NamePtr ())
      {
	NamePtr newName (boost::make_shared<NameImpl> (shared_from_this(),
						       identifier, Name::Function,
						       TypeImplPtr ()));
	identifiers[identifier] = newName;
      }
      
      FunctionInfoVector& functions = this->functions[identifier];
      FunctionInfoPtr funcInfo (boost::make_shared<FunctionInfo> ());
      funcInfo->originalIdentifier = identifier;
      // Decorate identifier with type info (so each overload gets a unique name)
      UnicodeString identifierDecorated (identifier);
      identifierDecorated.append ("$");
      for (FunctionFormalParameters::const_iterator param (params.begin());
	   param != params.end();
	   ++param)
      {
	identifierDecorated.append (handler->GetTypeString (boost::static_pointer_cast<TypeImpl> (param->type)).c_str());
      }
      funcInfo->identifier = identifierDecorated;
      funcInfo->returnType = builtin->GetReturnType();
      funcInfo->params = params;
      funcInfo->builtin = builtin;
      functions.push_back (funcInfo);
      
      functionsInDeclOrder.push_back (funcInfo);
    }
    
    IntermediateGeneratorSemanticsHandler::ScopeImpl::FunctionInfoVector
    IntermediateGeneratorSemanticsHandler::ScopeImpl::GetFunctions () const
    {
      return functionsInDeclOrder;
    }

    IntermediateGeneratorSemanticsHandler::ScopeImpl::FunctionInfoVector
    IntermediateGeneratorSemanticsHandler::ScopeImpl::CollectOverloadCandidates (const NamePtr& functionName, const ExpressionVector& params) const
    {
      boost::shared_ptr<NameImpl> nameImpl (boost::static_pointer_cast<NameImpl> (functionName));
      
      FunctionInfoVector vec;
      FunctionsMap::const_iterator funcIt = functions.find (nameImpl->identifier);
      if (funcIt != functions.end())
      {
	// First, look for an exact parameters type match
	for (FunctionInfoVector::const_iterator vecIt = funcIt->second.begin();
	     vecIt != funcIt->second.end();
	     ++vecIt)
	{
	  if (params.size() > (*vecIt)->params.size()) continue;
	  
	  bool abort = false;
	  size_t formal = 0, actual = 0;
	  for (; actual < params.size(); formal++)
	  {
        // Only consider user-specified parameters for matching
        if ((*vecIt)->params[formal].paramType != ptUser) continue;

	    boost::shared_ptr<ExpressionImpl> exprImpl (boost::static_pointer_cast<ExpressionImpl> (params[actual]));
	    TypeImplPtr paramType (exprImpl->GetValueType ());
	    TypeImplPtr formalParamType (boost::static_pointer_cast<TypeImpl> ((*vecIt)->params[formal].type));
	    // No exact type match? Skip
	    if (!paramType->IsEqual (*formalParamType))
	    {
	      abort = true;
	      break;
	    }
	    actual++;
	  }
	  if (abort) continue;
	  for (; formal < (*vecIt)->params.size(); formal++)
	  {
        // Only consider user-specified parameters for matching
        if ((*vecIt)->params[formal].paramType != ptUser) continue;

	    // Leftover parameter + no default value? Skip
	    if (!(*vecIt)->params[formal].defaultValue)
	    {
	      abort = true;
	      break;
	    }
	  }
	  if (abort) continue;
	  
	  vec.push_back (*vecIt);
	}
	
	// Second, look for a lossless parameters type match
	if (vec.size() == 0)
	{
	  for (FunctionInfoVector::const_iterator vecIt = funcIt->second.begin();
	      vecIt != funcIt->second.end();
	      ++vecIt)
	  {
	    if (params.size() > (*vecIt)->params.size()) continue;
	    
	    bool abort = false;
	    size_t formal = 0, actual = 0;
	    for (; actual < params.size(); formal++)
	    {
          // Only consider user-specified parameters for matching
          if ((*vecIt)->params[formal].paramType != ptUser) continue;

	      boost::shared_ptr<ExpressionImpl> exprImpl (boost::static_pointer_cast<ExpressionImpl> (params[actual]));
	      TypeImplPtr paramType (exprImpl->GetValueType ());
	      TypeImplPtr formalParamType (boost::static_pointer_cast<TypeImpl> ((*vecIt)->params[formal].type));
	      bool match;
	      if ((*vecIt)->params[formal].dir & dirOut)
		// Output parameters must _always_ match exactly
		match = paramType->IsEqual (*formalParamType);
	      else
		// Input parameters can match losslessy
		match = paramType->CompatibleLossless (*formalParamType);
	      // No type match? Skip
	      if (!match)
	      {
		abort = true;
		break;
	      }
	      actual++;
	    }
	    if (abort) continue;
	    for (; formal < (*vecIt)->params.size(); formal++)
	    {
          // Only consider user-specified parameters for matching
          if ((*vecIt)->params[formal].paramType != ptUser) continue;

	      // Leftover parameter + no default value? Skip
	      if (!(*vecIt)->params[formal].defaultValue)
	      {
		abort = true;
		break;
	      }
	    }
	    if (abort) continue;
	    
	    vec.push_back (*vecIt);
	  }
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

    std::vector<NamePtr> IntermediateGeneratorSemanticsHandler::ScopeImpl::GetAllVars ()
    {
      std::vector<NamePtr> ret;
      for (IdentifierMap::const_iterator ident (identifiers.begin());
	   ident != identifiers.end();
	   ++ident)
      {
	if (ident->second->GetType() == Name::Variable) ret.push_back (ident->second);
      }
      return ret;
    }
    
    int IntermediateGeneratorSemanticsHandler::ScopeImpl::DistanceToScope (const boost::shared_ptr<ScopeImpl>& scope)
    {
      if (!scope) return INT_MAX;
      
      boost::shared_ptr<ScopeImpl> parentScope (this->parent);
      int n = 0;
      while (parentScope)
      {
	if (parentScope == scope) return n;
	parentScope = parentScope->parent;
	n++;
      }
      return -1;
    }
    
  } // namespace intermediate
} // namespace s1
