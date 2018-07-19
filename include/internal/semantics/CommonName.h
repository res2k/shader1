/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef SEMANTICS_COMMONNAME_H_
#define SEMANTICS_COMMONNAME_H_

#include "base/uc/String.h"

#include "Name.h"
#include "Scope.h"

#include <boost/optional.hpp>

namespace s1
{
  namespace semantics
  {
    class CommonName : public Name
    {
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
      CommonName (Scope* ownerScope, const uc::String& identifier, NameType type, Type* typeOfName)
        : ownerScope (ownerScope), identifier (identifier), type (type), valueType (typeOfName) {}
      CommonName (Scope* ownerScope, const uc::String& identifier, Type* typeOfName,
                  ExpressionPtr value, bool constant)
        : ownerScope (ownerScope), identifier (identifier), type (Variable), valueType (typeOfName),
          varValue (value), varConstant (constant) {}
      CommonName (Scope* ownerScope,
                  const Scope::FunctionFormalParameter& param)
        : CommonName (ownerScope, param.identifier, param.type.get(), param.defaultValue,
                      param.dir == Scope::dirIn)
      {
        paramInfo = param;
      }

      Scope* GetOwnerScope() const { return ownerScope; }

      NameType GetType() { return type; }
      TypePtr GetAliasedType()
      { return type == TypeAlias ? valueType : semantics::TypePtr (); }
      Expression* GetValue() const { return varValue.get(); }
      const uc::String& GetIdentifier () { return identifier; }
      bool IsConstantVariable () { return (type == Variable) && varConstant; }
      TypePtr GetValueType () { return valueType; }

      const semantics::Scope::FunctionFormalParameter* GetParamInfo() const { return paramInfo.get_ptr(); }
    };
  } // namespace semantics
} // namespace s1

#endif // SEMANTICS_COMMONNAME_H_
