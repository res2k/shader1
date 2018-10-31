/*
    Shader1
    Copyright (c) 2010-2018 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef SEMANTICS_SCOPE_H_
#define SEMANTICS_SCOPE_H_

#include "Base.h"
#include "Expression.h"
#include "Type.h"

#include "base/outcome.h"
#include "base/uc/String.h"
#include "semantics/Diagnostics_fwd.h"

#include <unordered_map>
#include <vector>

namespace s1
{
  namespace semantics
  {
    /**
     * Scope object, managing visibility of identifiers.
     */
    class Scope : public Base
    {
    protected:
      Scope* parent;

      typedef std::unordered_map<uc::String, NamePtr> IdentifierMap;
      IdentifierMap identifiers;

      std::vector<NameVariablePtr> newVars; // TODO: Needed for intermediate variable initialization
      std::vector<NameVariablePtr> varsInDeclOrder; // TODO: Needed for global var handling

      /// Returns \c true if identifier is not registered in this or any parent scope.
      bool CheckIdentifierUnique (const uc::String& identifier);
    public:
      Scope (Scope* parent);
      virtual ~Scope();

      /// Get level of the scope.
      virtual ScopeLevel GetLevel() const = 0;

      /**
       * Add a variable or constant.
       * \param diagnosticsHandler Diagnostics handler called in case of an issue.
       * \param type Type of variable.
       * \param identifier Identifier of variable.
       * \param initialValue Initial value of variable, can be a 0 pointer if none
       *  is given. Required for constants
       * \param constant Whether it is a constant variable.
       */
      NameVariablePtr AddVariable (SimpleDiagnostics& diagnosticsHandler,
                                   Type* type,
                                   const uc::String& identifier,
                                   Expression* initialValue,
                                   bool constant);

      std::vector<NameVariablePtr> FlushNewVars (); // TODO: Needed for intermediate variable initialization
      const std::vector<NameVariablePtr>& GetAllVars (); // TODO: Needed for global var handling

      /**
       * Add a type alias.
       * \param aliasedType Type to alias.
       * \param identifier Identifier of type alias.
       */
      virtual NameTypeAliasPtr AddTypeAlias (TypePtr aliasedType,
        const uc::String& identifier) = 0;

      /**
       * Add a function.
       * \param returnType Return type of function.
       * \param identifier Identifier of function.
       * \param params Formal parameters.
       * \returns Function object.
       */
      virtual FunctionPtr AddFunction (TypePtr returnType,
        const uc::String& identifier,
        const FunctionFormalParameters& params) = 0;

      typedef outcome::unchecked<NamePtr, Error> result_NamePtr;
      /// Resolve an identifier to a name
      result_NamePtr ResolveIdentifier (const uc::String& identifier);
    };
  } // namespace semantics
} // namespace s1

#endif // SEMANTICS_SCOPE_H_
