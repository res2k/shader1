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

#include "base/common.h"
#include "base/format/Formatter.h"
#include "base/format/uc_String.h"
#include "base/intrusive_ptr.h"
#include "semantics/Attribute.h"
#include "semantics/CommonScope.h"
#include "semantics/CommonHandler.h"
#include "semantics/Type.h"

namespace s1
{
  namespace semantics
  {
    typedef TypePtr TypePtr;
    
    TypePtr CommonHandler::CreateType (BaseType type)
    {
      return TypePtr (new Type (type));
    }
    
    TypePtr CommonHandler::CreateSamplerType (SamplerType dim)
    {
      return TypePtr (new Type (dim));
    }
    
    TypePtr CommonHandler::CreateArrayType (TypePtr baseType)
    {
      return TypePtr (new Type (baseType));
    }
    
    TypePtr CommonHandler::CreateVectorType (TypePtr baseType,
                                                      unsigned int components)
    {
      return TypePtr (new Type (baseType, components));
    }
    
    TypePtr CommonHandler::CreateMatrixType (TypePtr baseType,
                                                      unsigned int columns,
                                                      unsigned int rows)
    {
      return TypePtr (new Type (baseType, columns, rows));
    }
  
    ScopePtr CommonHandler::CreateScope (ScopePtr parentScope,
                                                             ScopeLevel scopeLevel)
    {
      return ScopePtr (new CommonScope (this,
        get_static_ptr<CommonScope> (parentScope),
        scopeLevel));
    }
    
  } // namespace semantics
} // namespace s1
