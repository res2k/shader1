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
#include "parser/Diagnostics.h"
#include "semantics/CommonSemanticsHandler.h"
#include "semantics/CommonType.h"

#include "base/format/Formatter.tpp"

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
    
    CommonSemanticsHandler::Attribute CommonSemanticsHandler::IdentifyAttribute (const uc::String& attributeStr)
    {
      if (attributeStr.length() == 0)
        // Empty attribute? Bogus.
        return Attribute (Attribute::Unknown);
      
      if (attributeStr == uc::String ("length"))
        return Attribute (Attribute::arrayLength);
      else if (attributeStr == uc::String ("row"))
        return Attribute (Attribute::matrixRow);
      else if (attributeStr == uc::String ("col"))
        return Attribute (Attribute::matrixCol);
      else if (attributeStr == uc::String ("transpose"))
        return Attribute (Attribute::matrixTranspose);
      else if (attributeStr == uc::String ("invert"))
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
    
    TypePtr
    CommonSemanticsHandler::GetAttributeType (CommonType* expressionType, const Attribute& attr)
    {
      TypePtr attrType;
      switch (attr.attrClass)
      {
      case Attribute::arrayLength:
        if (expressionType->typeClass == CommonType::Array)
          attrType = CreateType (BaseType::UInt); // Type is fix
        break;
      case Attribute::matrixCol:
        if (expressionType->typeClass == CommonType::Matrix)
          attrType = CreateArrayType (
            CreateVectorType (expressionType->avmBase, expressionType->matrixRows));
        break;
      case Attribute::matrixRow:
        if (expressionType->typeClass == CommonType::Matrix)
          attrType = CreateArrayType (
            CreateVectorType (expressionType->avmBase, expressionType->matrixCols));
        break;
      case Attribute::matrixTranspose:
        if (expressionType->typeClass == CommonType::Matrix)
          attrType = CreateMatrixType (expressionType->avmBase, expressionType->matrixRows, expressionType->matrixCols);
        break;
      case Attribute::matrixInvert:
        if ((expressionType->typeClass == CommonType::Matrix)
            && (expressionType->matrixRows == expressionType->matrixCols))
          attrType = expressionType;
        break;
      case Attribute::vectorSwizzle:
        if (expressionType->typeClass == CommonType::Vector)
        {
          if (attr.swizzleCompNum == 1)
            // 1-component swizzles return the base type, not a 1-component vector
            attrType = expressionType->avmBase;
          else
            attrType = CreateVectorType (expressionType->avmBase, attr.swizzleCompNum);
        }
        break;
      case Attribute::Unknown:
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
  
    bool CommonSemanticsHandler::CommonScope::CheckIdentifierUnique (const uc::String& identifier)
    {
      IdentifierMap::iterator ident = identifiers.find (identifier);
      if (ident != identifiers.end())
      {
        return false;
      }
      if (parent)
        return parent->CheckIdentifierUnique (identifier);
      return true;
    }
      
    CommonSemanticsHandler::CommonScope::CommonScope (CommonSemanticsHandler* handler,
                                                      CommonScope* parent,
                                                      ScopeLevel level)
     : handler (handler), parent (parent), level (level)
    {}

    NamePtr
    CommonSemanticsHandler::CommonScope::AddVariable (TypePtr type, const uc::String& identifier,
                                                      ExpressionPtr initialValue, bool constant)
    {
      if (!CheckIdentifierUnique (identifier))
      {
        // TODO: Error handling
        return NamePtr();
      }
      NamePtr newName (new CommonName (identifier, type, initialValue, constant));
      identifiers[identifier] = newName;
      return newName;
    }
      
    NamePtr
    CommonSemanticsHandler::CommonScope::AddTypeAlias (TypePtr aliasedType, const uc::String& identifier)
    {
      if (!CheckIdentifierUnique (identifier))
      {
        // TODO: Error handling
        return NamePtr();
      }
      NamePtr newName (new CommonName (identifier, Name::TypeAlias, aliasedType));
      identifiers[identifier] = newName;
      return newName;
    }
      
    FunctionPtr
    CommonSemanticsHandler::CommonScope::AddFunction (TypePtr returnType,
                                                      const uc::String& identifier,
                                                      const FunctionFormalParameters& params)
    {
      if (level >= ScopeLevel::Function)
      {
        // TODO: Error handling
        return FunctionPtr();
      }
      if (!CheckIdentifierUnique (identifier))
      {
        // TODO: Error handling
        return FunctionPtr();
      }
      NamePtr newName (new CommonName (identifier, Name::Function, returnType));
      identifiers[identifier] = newName;
      ScopePtr funcScope;
      funcScope = handler->CreateScope (this, ScopeLevel::Function);
      auto newBlock = handler->CreateBlock (funcScope);
      funcScope = ScopePtr();
      FunctionPtr newFunction (new CommonFunction (newBlock));
      return newFunction;
    }

    CommonSemanticsHandler::CommonScope::result_NamePtr
    CommonSemanticsHandler::CommonScope::ResolveIdentifier (const uc::String& identifier)
    {
      IdentifierMap::iterator ident = identifiers.find (identifier);
      if (ident != identifiers.end())
      {
        return ident->second;
      }
      if (parent)
        return parent->ResolveIdentifier (identifier);
      return parser::Error::IdentifierUndeclared;
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
