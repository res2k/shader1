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
#include "semantics/CommonSemanticsHandler.h"
#include "semantics/CommonType.h"

namespace s1
{
  namespace semantics
  {
    CommonType*
    CommonSemanticsHandler::GetHigherPrecisionType (CommonType* t1, CommonType* t2)
    {
      if (t1->IsPrecisionHigherEqual (*t2))
        return t1;
      else if (t2->IsPrecisionHigherEqual (*t1))
        return t2;
      return nullptr;
    }
      
    BaseType CommonSemanticsHandler::DetectNumericType (const uc::String& numericStr)
    {
      if (numericStr.startsWith ("0x") || numericStr.startsWith ("0X"))
      {
        // Hex number: always unsigned int
        return BaseType::UInt;
      }
      if ((numericStr.indexOf ('.') != uc::String::npos)
        || (numericStr.indexOf ('e') != uc::String::npos)
        || (numericStr.indexOf ('E') != uc::String::npos))
      {
        // Contains '.', 'e' or 'E': must be float number
        return BaseType::Float;
      }
      // Can only be an integer
      return numericStr.startsWith ("-") ? BaseType::Int : BaseType::UInt;
    }
    
    TypePtr
    CommonSemanticsHandler::GetAttributeType (CommonType* expressionType, const Attribute& attr)
    {
      TypePtr attrType;
      switch (attr.attrClass)
      {
      case semantics::Attribute::arrayLength:
        if (expressionType->typeClass == CommonType::Array)
          attrType = CreateType (BaseType::UInt); // Type is fix
        break;
      case semantics::Attribute::matrixCol:
        if (expressionType->typeClass == CommonType::Matrix)
          attrType = CreateArrayType (
            CreateVectorType (expressionType->avmBase, expressionType->matrixRows));
        break;
      case semantics::Attribute::matrixRow:
        if (expressionType->typeClass == CommonType::Matrix)
          attrType = CreateArrayType (
            CreateVectorType (expressionType->avmBase, expressionType->matrixCols));
        break;
      case semantics::Attribute::matrixTranspose:
        if (expressionType->typeClass == CommonType::Matrix)
          attrType = CreateMatrixType (expressionType->avmBase, expressionType->matrixRows, expressionType->matrixCols);
        break;
      case semantics::Attribute::matrixInvert:
        if ((expressionType->typeClass == CommonType::Matrix)
            && (expressionType->matrixRows == expressionType->matrixCols))
          attrType = expressionType;
        break;
      case semantics::Attribute::vectorSwizzle:
        if (expressionType->typeClass == CommonType::Vector)
        {
          if (attr.swizzleCompNum == 1)
            // 1-component swizzles return the base type, not a 1-component vector
            attrType = expressionType->avmBase;
          else
            attrType = CreateVectorType (expressionType->avmBase, attr.swizzleCompNum);
        }
        break;
      case semantics::Attribute::Unknown:
        break;
      }
      return attrType;
    }
    
    typedef TypePtr TypePtr;
    
    TypePtr CommonSemanticsHandler::CreateType (BaseType type)
    {
      return TypePtr (new CommonType (type));
    }
    
    TypePtr CommonSemanticsHandler::CreateSamplerType (SamplerType dim)
    {
      return TypePtr (new CommonType (dim));
    }
    
    TypePtr CommonSemanticsHandler::CreateArrayType (TypePtr baseType)
    {
      return TypePtr (new CommonType (baseType));
    }
    
    TypePtr CommonSemanticsHandler::CreateVectorType (TypePtr baseType,
                                                      unsigned int components)
    {
      return TypePtr (new CommonType (baseType, components));
    }
    
    TypePtr CommonSemanticsHandler::CreateMatrixType (TypePtr baseType,
                                                      unsigned int columns,
                                                      unsigned int rows)
    {
      return TypePtr (new CommonType (baseType, columns, rows));
    }
  
    ScopePtr CommonSemanticsHandler::CreateScope (ScopePtr parentScope,
                                                             ScopeLevel scopeLevel)
    {
      return ScopePtr (new CommonScope (this,
        get_static_ptr<CommonScope> (parentScope),
        scopeLevel));
    }
    
  } // namespace semantics
} // namespace s1
