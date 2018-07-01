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

#include "base/common.h"
#include "semantics/Attribute.h"
#include "semantics/Handler.h"
#include "semantics/Type.h"

#include <boost/algorithm/string/predicate.hpp>

namespace s1
{
  namespace semantics
  {
    Attribute Attribute::Identify (const uc::String& attributeStr)
    {
      if (attributeStr.length() == 0)
        // Empty attribute? Bogus.
        return Attribute (Attribute::Unknown);
      
      if (boost::algorithm::equals (attributeStr, "length"))
        return Attribute (Attribute::arrayLength);
      else if (boost::algorithm::equals (attributeStr, "row"))
        return Attribute (Attribute::matrixRow);
      else if (boost::algorithm::equals (attributeStr, "col"))
        return Attribute (Attribute::matrixCol);
      else if (boost::algorithm::equals (attributeStr, "transpose"))
        return Attribute (Attribute::matrixTranspose);
      else if (boost::algorithm::equals (attributeStr, "invert"))
        return Attribute (Attribute::matrixInvert);
      
      // Attribute is swizzle
      if (attributeStr.length() > 4)
        // Attribute too long for a swizzle
        return Attribute (Attribute::Unknown);
      unsigned char attr[4];
      auto attributeStrBuf = attributeStr.data ();
      switch (attributeStrBuf[0])
      {
      case 'x':
      case 'y':
      case 'z':
      case 'w':
        {
          // XYZW style swizzle
          for (uc::String::size_type i = 0; i < attributeStr.length(); i++)
          {
            unsigned char comp;
            switch (attributeStrBuf[i])
            {
            case 'x': comp = 0; break;
            case 'y': comp = 1; break;
            case 'z': comp = 2; break;
            case 'w': comp = 3; break;
            default:
              // Unknown component
              return Attribute (Attribute::Unknown);
            }
            attr[i] = comp;
          }
        }
        break;
      case 'r':
      case 'g':
      case 'b':
      case 'a':
        {
          // RGBA style swizzle
          for (uc::String::size_type i = 0; i < attributeStr.length(); i++)
          {
            unsigned char comp;
            switch (attributeStrBuf[i])
            {
            case 'r': comp = 0; break;
            case 'g': comp = 1; break;
            case 'b': comp = 2; break;
            case 'a': comp = 3; break;
            default:
              // Unknown component
              return Attribute (Attribute::Unknown);
            }
            attr[i] = comp;
          }
        }
        break;
      default:
        // Unknown component
        return Attribute (Attribute::Unknown);
      }
      return Attribute (static_cast<unsigned char> (attributeStr.length()),
                        attr[0], attr[1], attr[2], attr[3]);
    }

    TypePtr Attribute::GetType (Handler* handler, Type* expressionType, const Attribute& attr)
    {
      TypePtr attrType;
      switch (attr.attrClass)
      {
      case Attribute::arrayLength:
        if (expressionType->GetTypeClass() == Type::Array)
          attrType = handler->CreateType (BaseType::UInt); // Type is fix
        break;
      case Attribute::matrixCol:
        if (expressionType->GetTypeClass() == Type::Matrix)
        {
          auto baseType = handler->CreateType (expressionType->GetVMBase());
          attrType = handler->CreateArrayType (
            handler->CreateVectorType (baseType, expressionType->GetMatrixTypeRows()));
        }
        break;
      case Attribute::matrixRow:
        if (expressionType->GetTypeClass() == Type::Matrix)
        {
          auto baseType = handler->CreateType (expressionType->GetVMBase());
          attrType = handler->CreateArrayType (
            handler->CreateVectorType (baseType, expressionType->GetMatrixTypeCols()));
        }
        break;
      case Attribute::matrixTranspose:
        if (expressionType->GetTypeClass() == Type::Matrix)
        {
          auto baseType = handler->CreateType (expressionType->GetVMBase());
          attrType = handler->CreateMatrixType (baseType, expressionType->GetMatrixTypeRows(), expressionType->GetMatrixTypeCols());
        }
        break;
      case Attribute::matrixInvert:
        if ((expressionType->GetTypeClass() == Type::Matrix)
            && (expressionType->GetMatrixTypeRows() == expressionType->GetMatrixTypeCols()))
          attrType = expressionType;
        break;
      case Attribute::vectorSwizzle:
        if (expressionType->GetTypeClass() == Type::Vector)
        {
          if (attr.swizzleCompNum == 1)
            // 1-component swizzles return the base type, not a 1-component vector
            attrType = handler->CreateType (expressionType->GetVMBase());
          else
          {
            auto baseType = handler->CreateType (expressionType->GetVMBase());
            attrType = handler->CreateVectorType (baseType, attr.swizzleCompNum);
          }
        }
        break;
      case Attribute::Unknown:
        break;
      }
      return attrType;
    }
  } // namespace semantics
} // namespace s1
