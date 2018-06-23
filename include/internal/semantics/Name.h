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

namespace s1
{
  namespace semantics
  {
    /**
     * A name refers to a variable, an attribute of it or an element.
     */
    struct Name : public Base
    {
      virtual ~Name() {}
      
      /// Type of name (function or variable/parameter)
      enum NameType { Function, Variable, TypeAlias };
      
      /// Get type of name
      virtual NameType GetType() = 0;
      
      /// Get aliased type
      virtual TypePtr GetAliasedType() = 0;

      /// Get name identifier
      virtual const uc::String& GetIdentifier () = 0;

      /// Return whether a variable name is a constant
      virtual bool IsConstantVariable () = 0;

      /// Return type of variable value
      virtual TypePtr GetValueType () = 0;
    };
  } // namespace semantics
} // namespace s1

#endif // SEMANTICS_NAME_H_
