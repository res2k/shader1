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

namespace s1
{
  namespace semantics
  {
    struct CommonName : public Name
    {
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

      CommonName (const uc::String& identifier, NameType type, semantics::TypePtr typeOfName)
        : identifier (identifier), type (type), valueType (typeOfName) {}
      CommonName (const uc::String& identifier, TypePtr typeOfName,
                  ExpressionPtr value, bool constant)
        : identifier (identifier), type (Variable), valueType (typeOfName),
          varValue (value), varConstant (constant) {}

      NameType GetType() { return type; }
      TypePtr GetAliasedType()
      { return type == TypeAlias ? valueType : semantics::TypePtr (); }
      Expression* GetValue() const { return varValue.get(); }
      const uc::String& GetIdentifier () { return identifier; }
      bool IsConstantVariable () { return (type == Variable) && varConstant; }
      TypePtr GetValueType () { return valueType; }
    };
  } // namespace semantics
} // namespace s1

#endif // SEMANTICS_COMMONNAME_H_
