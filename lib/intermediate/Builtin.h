#ifndef __BUILTIN_H__
#define __BUILTIN_H__

#include "intermediate/BuiltinFunction.h"
#include "intermediate/IntermediateGeneratorSemanticsHandler.h"

#include "ScopeImpl.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::Builtin
    {
      intermediate::BuiltinFunction what;
      TypePtr returnType;
      UnicodeString identifier;
      ScopeImpl::FunctionFormalParameters formalParameters;
    public:
      Builtin (intermediate::BuiltinFunction what,
	       const TypePtr& returnType,
	       const UnicodeString& identifier,
	       const ScopeImpl::FunctionFormalParameters& formalParameters)
       : what (what), returnType (returnType), identifier (identifier),
	 formalParameters (formalParameters)
      {}
      
      intermediate::BuiltinFunction GetBuiltinFunction () const { return what; }
      const TypePtr& GetReturnType () const { return returnType; }
      const UnicodeString& GetIdentifier() const { return identifier; }
      const ScopeImpl::FunctionFormalParameters& GetFormalParameters() const
      { return formalParameters; }
    };
  } // namespace intermediate
} // namespace s1

#endif // __BUILTIN_H__
