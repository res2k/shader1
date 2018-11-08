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
      Scope* parent;
    protected:
      ScopeLevel level;

      typedef std::unordered_map<uc::String, NamePtr> IdentifierMap;
      IdentifierMap identifiers;

      std::vector<NameVariablePtr> newVars; // TODO: Needed for intermediate variable initialization
      std::vector<NameVariablePtr> varsInDeclOrder; // TODO: Needed for global var handling

      std::vector<BaseFunctionPtr> functionsInDeclOrder; // TODO: Used in global scope, to detect functions

      /// Returns \c true if identifier is not registered in this or any parent scope.
      bool CheckIdentifierUnique (const uc::String& identifier);

      typedef outcome::unchecked<NameFunctionPtr, Error> result_NameFunctionPtr;
      /// Return NameFunction if identifier is a function, error code otherwise
      result_NameFunctionPtr CheckIdentifierIsFunction (const uc::String& identifier);
    public:
      Scope (Scope* parent, ScopeLevel level);
      virtual ~Scope();

      /// Get parent scope
      Scope* GetParent() const { return parent; }
      /// Get level of the scope.
      ScopeLevel GetLevel() const { return level; }

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
       * \param diagnosticsHandler Diagnostics handler called in case of an issue.
       * \param aliasedType Type to alias.
       * \param identifier Identifier of type alias.
       */
      NameTypeAliasPtr AddTypeAlias (SimpleDiagnostics& diagnosticsHandler,
                                     Type* aliasedType,
                                     const uc::String& identifier);

      /**
       * Add a function.
       * \param returnType Return type of function.
       * \param identifier Identifier of function.
       * \param params Formal parameters.
       * \param funcScope Scope for function parameters.
       * \param funcBlock Function block.
       * \returns Function object.
       */
      FunctionPtr AddFunction (SimpleDiagnostics& diagnosticsHandler,
                               Type* returnType, const uc::String& identifier,
                               const FunctionFormalParameters& params,
                               Scope* funcScope, Block* funcBlock);
      virtual FunctionPtr AddFunction (TypePtr returnType,
                                       const uc::String& identifier,
                                       const FunctionFormalParameters& params) = 0; // FIXME: Put into Handler?

      std::vector<BaseFunctionPtr> GetFunctions () const; // TODO: Used in global scope, to detect functions

      typedef outcome::unchecked<NamePtr, Error> result_NamePtr;
      /// Resolve an identifier to a name
      result_NamePtr ResolveIdentifier (const uc::String& identifier);
    };
  } // namespace semantics
} // namespace s1

#endif // SEMANTICS_SCOPE_H_
