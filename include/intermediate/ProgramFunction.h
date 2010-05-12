#ifndef __INTERMEDIATE_PROGRAMFUNCTION_H__
#define __INTERMEDIATE_PROGRAMFUNCTION_H__

#include <unicode/unistr.h>

#include "parser/SemanticsHandler.h"

#include "forwarddecl.h"

namespace s1
{
  namespace intermediate
  {
    class ProgramFunction
    {
      UnicodeString identifier;
      parser::SemanticsHandler::TypePtr returnType;
      parser::SemanticsHandler::Scope::FunctionFormalParameters params;
      SequencePtr body;
    public:
      ProgramFunction (const UnicodeString& identifier,
		       const parser::SemanticsHandler::TypePtr& returnType,
		       const parser::SemanticsHandler::Scope::FunctionFormalParameters& params,
		       const parser::SemanticsHandler::BlockPtr& block);

      const UnicodeString& GetIdentifier() const
      { return identifier; }
      const parser::SemanticsHandler::TypePtr& GetReturnType() const
      { return returnType; }
      const parser::SemanticsHandler::Scope::FunctionFormalParameters& GetParams() const
      { return params; }
      SequencePtr GetBody() const
      { return body; }
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_PROGRAMFUNCTION_H__