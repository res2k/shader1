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
    TypePtr
    CommonHandler::GetAttributeType (Type* expressionType, const Attribute& attr)
    {
      TypePtr attrType;
      switch (attr.attrClass)
      {
      case semantics::Attribute::arrayLength:
        if (expressionType->GetTypeClass() == Type::Array)
          attrType = CreateType (BaseType::UInt); // Type is fix
        break;
      case semantics::Attribute::matrixCol:
        if (expressionType->GetTypeClass() == Type::Matrix)
          attrType = CreateArrayType (
            CreateVectorType (expressionType->GetAVMBase(), expressionType->GetMatrixTypeRows()));
        break;
      case semantics::Attribute::matrixRow:
        if (expressionType->GetTypeClass() == Type::Matrix)
          attrType = CreateArrayType (
            CreateVectorType (expressionType->GetAVMBase(), expressionType->GetMatrixTypeCols()));
        break;
      case semantics::Attribute::matrixTranspose:
        if (expressionType->GetTypeClass() == Type::Matrix)
          attrType = CreateMatrixType (expressionType->GetAVMBase(), expressionType->GetMatrixTypeRows(), expressionType->GetMatrixTypeCols());
        break;
      case semantics::Attribute::matrixInvert:
        if ((expressionType->GetTypeClass() == Type::Matrix)
            && (expressionType->GetMatrixTypeRows() == expressionType->GetMatrixTypeCols()))
          attrType = expressionType;
        break;
      case semantics::Attribute::vectorSwizzle:
        if (expressionType->GetTypeClass() == Type::Vector)
        {
          if (attr.swizzleCompNum == 1)
            // 1-component swizzles return the base type, not a 1-component vector
            attrType = expressionType->GetAVMBase();
          else
            attrType = CreateVectorType (expressionType->GetAVMBase(), attr.swizzleCompNum);
        }
        break;
      case semantics::Attribute::Unknown:
        break;
      }
      return attrType;
    }
    
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
