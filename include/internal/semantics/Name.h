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

#ifndef SEMANTICS_NAME_H_
#define SEMANTICS_NAME_H_

#include "Base.h"
#include "Scope.h"

#include <boost/optional.hpp>

namespace s1
{
  namespace semantics
  {
    /**
     * A name refers to a variable, an attribute of it or an element.
     */
    class Name : public Base
    {
    public:
      /// Type of name (function or variable/parameter)
      enum NameType { Function, Variable, TypeAlias };
    protected:
      Scope* ownerScope;

      uc::String identifier;
      NameType type;

      /* Variables/Constants: type of variable/constant
        * Functions: type of return value
        * Type aliases: aliased type
        */
      TypePtr valueType;
      // Variables/Constants: value
      ExpressionPtr varValue;
      // Distinguish between variable/constant
      bool varConstant;

      // Parameter info, if it's originally a function parameter
      boost::optional<Scope::FunctionFormalParameter> paramInfo;
    public:
      Name (Scope* ownerScope, const uc::String& identifier, NameType type, Type* typeOfName)
        : ownerScope (ownerScope), identifier (identifier), type (type), valueType (typeOfName) {}
      Name (Scope* ownerScope, const uc::String& identifier, Type* typeOfName,
            ExpressionPtr value, bool constant)
        : ownerScope (ownerScope), identifier (identifier), type (Variable), valueType (typeOfName),
          varValue (value), varConstant (constant) {}
      Name (Scope* ownerScope,
            const Scope::FunctionFormalParameter& param)
        :  Name (ownerScope, param.identifier, param.type.get(), param.defaultValue,
                 param.dir == Scope::dirIn)
      {
        paramInfo = param;
      }
      virtual ~Name() {}

      /// Get scope this name is declared in
      Scope* GetOwnerScope() const { return ownerScope; }

      /// Get type of name
      NameType GetType() { return type; }
      
      /// Get aliased type
      TypePtr GetAliasedType() { return type == TypeAlias ? valueType : TypePtr (); }

      /// Get initialization value for variables
      Expression* GetValue() const { return varValue.get(); }

      /// Get name identifier
      const uc::String& GetIdentifier () { return identifier; }

      /// Return whether a variable name is a constant
      bool IsConstantVariable () { return (type == Variable) && varConstant; }

      /// Return type of variable value
      TypePtr GetValueType () { return valueType; }

      /// Return parameter info, if name was originally a function parameter
      const Scope::FunctionFormalParameter* GetParamInfo() const { return paramInfo.get_ptr(); }
    };
  } // namespace semantics
} // namespace s1

#endif // SEMANTICS_NAME_H_
