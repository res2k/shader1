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
#include "parser/CommonSemanticsHandler.h"
#include "parser/Diagnostics.h"

#include "base/format/Formatter.tpp"

namespace s1
{
  namespace parser
  {
    bool CommonSemanticsHandler::CommonType::CompatibleLossless (const CommonType& to) const
    {
      if (typeClass != to.typeClass) return false;
      switch (typeClass)
      {
      case Base:
        if ((base == semantics::BaseType::Void) || (to.base == semantics::BaseType::Void))
          // Void is not even compatible to itself
          return false;
        if (((base == semantics::BaseType::Int) || (base == semantics::BaseType::UInt))
            && ((to.base == semantics::BaseType::Int) || (to.base == semantics::BaseType::UInt)))
          // int and unsigned int are assumed assignable without precision loss
          return true;
        if (((base == semantics::BaseType::Int) || (base == semantics::BaseType::UInt))
            && (to.base == semantics::BaseType::Float))
          // assignment from int or unsigned int to float is assumed assignable without precision loss
          return true;
        // Otherwise, types must be equal
        return base == to.base;
      case Sampler:
        // Samplers have to be equal
        return sampler == to.sampler;
      case Array:
        // Array assignments are compatible when the contained members are
        return static_cast<CommonType*> (avmBase.get())->CompatibleLossless (
          *(static_cast<CommonType*> (to.avmBase.get())));
      case Vector:
        // Vectors: base types must be compatible and both types have the same number of components
        return static_cast<CommonType*> (avmBase.get())->CompatibleLossless (
          *(static_cast<CommonType*> (to.avmBase.get())))
          && (vectorDim == to.vectorDim);
      case Matrix:
        // Matrices: base types must be compatible and both types have the same number of rows/cols
        return static_cast<CommonType*> (avmBase.get())->CompatibleLossless (
          *(static_cast<CommonType*> (to.avmBase.get())))
          && (matrixCols == to.matrixCols)
          && (matrixRows == to.matrixRows);
      }
      S1_ASSERT_NOT_REACHED (false);
    }
    
    bool CommonSemanticsHandler::CommonType::CompatibleLossy (const CommonType& to) const
    {
      // Lossless compatibility implies lossy compatibility ;)
      if (CompatibleLossless (to)) return true;
      
      if (typeClass != to.typeClass) return false;
      switch (typeClass)
      {
      case Base:
        if ((base == semantics::BaseType::Float)
            && ((to.base == semantics::BaseType::Int) || (to.base == semantics::BaseType::UInt)))
          // assignment from float to int or unsigned int possible with precision loss
          return true;
      default:
        break;
      }
      return false;
    }
    
    bool CommonSemanticsHandler::CommonType::IsEqual (const CommonType& other) const
    {
      if (typeClass != other.typeClass) return false;
      switch (typeClass)
      {
      case Base:
        return base == other.base;
      case Sampler:
        return sampler == other.sampler;
      case Array:
        return static_cast<CommonType*> (avmBase.get())->IsEqual (
          *(static_cast<CommonType*> (other.avmBase.get())));
      case Vector:
        return static_cast<CommonType*> (avmBase.get())->IsEqual (
          *(static_cast<CommonType*> (other.avmBase.get())))
          && (vectorDim == other.vectorDim);
      case Matrix:
        return static_cast<CommonType*> (avmBase.get())->IsEqual (
          *(static_cast<CommonType*> (other.avmBase.get())))
          && (matrixCols == other.matrixCols)
          && (matrixRows == other.matrixRows);
      }
      S1_ASSERT_NOT_REACHED (false);
    }

    bool CommonSemanticsHandler::CommonType::IsPrecisionHigherEqual (const CommonType& other) const
    {
      if (typeClass != other.typeClass) return false;
      switch (typeClass)
      {
      case Base:
        // Void is not even of equal precision to itself...
        if ((base == semantics::BaseType::Void) || (other.base == semantics::BaseType::Void))
          return false;
        // Treat 'int' as higher precision as 'unsigned int' (b/c 'int' has a sign)
        if ((base == semantics::BaseType::Int)
            && ((other.base == semantics::BaseType::Int) || (other.base == semantics::BaseType::UInt)))
          return true;
        // Float is considered higher precision than int or unsigned int (has fractions)
        if ((base == semantics::BaseType::Float)
            && ((other.base == semantics::BaseType::Int) || (other.base == semantics::BaseType::UInt)))
          // assignment from int or unsigned int to float is assumed assignable without precision loss
          return true;
        // Otherwise, types must be equal
        return base == other.base;
      case Sampler:
        // Samplers have to be equal
        return sampler == other.sampler;
      case Array:
        // Array type is higher/equal prec when the contained members are
        return static_cast<CommonType*> (avmBase.get())->IsPrecisionHigherEqual (
          *(static_cast<CommonType*> (other.avmBase.get())));
      case Vector:
        // Vectors: base type is higher/equal prec if both types have the same number of components
        return static_cast<CommonType*> (avmBase.get())->IsPrecisionHigherEqual (
          *(static_cast<CommonType*> (other.avmBase.get())))
          && (vectorDim == other.vectorDim);
      case Matrix:
        // Matrices: base type is higher/equal prec if both types have the same number of rows/cols
        return static_cast<CommonType*> (avmBase.get())->IsPrecisionHigherEqual (
          *(static_cast<CommonType*> (other.avmBase.get())))
          && (matrixCols == other.matrixCols)
          && (matrixRows == other.matrixRows);
      }
      S1_ASSERT_NOT_REACHED (false);
    }

    DECLARE_STATIC_FORMATTER(FormatArray, "{0}[]");
    DECLARE_STATIC_FORMATTER(FormatVector, "{0}{1}");
    DECLARE_STATIC_FORMATTER(FormatMatrix, "{0}{1}x{2}");

    uc::String CommonSemanticsHandler::CommonType::ToString() const
    {
      switch (typeClass)
      {
      case Base:
        {
          switch (base)
          {
            case semantics::BaseType::Invalid: return uc::String ("INVALID");
            case semantics::BaseType::Void: return uc::String ("void");
            case semantics::BaseType::Bool: return uc::String ("bool");
            case semantics::BaseType::Int: return uc::String ("int");
            case semantics::BaseType::UInt: return uc::String ("unsigned int");
            case semantics::BaseType::Float: return uc::String ("float");
          }
        }
        break;
      case Sampler:
        {
          switch (sampler)
          {
            case semantics::SamplerType::_1D: return uc::String ("sampler1D");
            case semantics::SamplerType::_2D: return uc::String ("sampler2D");
            case semantics::SamplerType::_3D: return uc::String ("sampler3D");
            case semantics::SamplerType::CUBE: return uc::String ("samplerCUBE");
          }
        }
        break;
      case Array:
        {
          uc::String s;
          FormatArray (s, static_cast<CommonType*> (avmBase.get())->ToString());
          return s;
        }
      case Vector:
        {
          uc::String s;
          FormatVector (s, static_cast<CommonType*> (avmBase.get())->ToString(), vectorDim);
          return s;
        }
      case Matrix:
        {
          uc::String s;
          FormatMatrix (s, static_cast<CommonType*> (avmBase.get())->ToString(), matrixCols, matrixRows);
          return s;
        }
      }
      S1_ASSERT_NOT_REACHED (uc::String());
    }
    
    boost::shared_ptr<CommonSemanticsHandler::CommonType>
    CommonSemanticsHandler::GetHigherPrecisionType (
      const boost::shared_ptr<CommonType>& t1, const boost::shared_ptr<CommonType>& t2)
    {
      if (t1->IsPrecisionHigherEqual (*t2))
        return t1;
      else if (t2->IsPrecisionHigherEqual (*t1))
        return t2;
      return boost::shared_ptr<CommonSemanticsHandler::CommonType> ();
    }
      
    semantics::BaseType CommonSemanticsHandler::DetectNumericType (const uc::String& numericStr)
    {
      if (numericStr.startsWith ("0x") || numericStr.startsWith ("0X"))
      {
        // Hex number: always unsigned int
        return semantics::BaseType::UInt;
      }
      if ((numericStr.indexOf ('.') != uc::String::npos)
        || (numericStr.indexOf ('e') != uc::String::npos)
        || (numericStr.indexOf ('E') != uc::String::npos))
      {
        // Contains '.', 'e' or 'E': must be float number
        return semantics::BaseType::Float;
      }
      // Can only be an integer
      return numericStr.startsWith ("-") ? semantics::BaseType::Int : semantics::BaseType::UInt;
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
    
    semantics::TypePtr
    CommonSemanticsHandler::GetAttributeType (const boost::shared_ptr<CommonType>& expressionType,
                                              const Attribute& attr)
    {
      semantics::TypePtr attrType;
      switch (attr.attrClass)
      {
      case Attribute::arrayLength:
        if (expressionType->typeClass == CommonType::Array)
          attrType = CreateType (semantics::BaseType::UInt); // Type is fix
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
    
    typedef semantics::TypePtr TypePtr;
    
    TypePtr CommonSemanticsHandler::CreateType (semantics::BaseType type)
    {
      return TypePtr (new CommonType (type));
    }
    
    TypePtr CommonSemanticsHandler::CreateSamplerType (semantics::SamplerType dim)
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
                                                      const boost::shared_ptr<CommonScope>& parent,
                                                      ScopeLevel level)
     : handler (handler), parent (parent), level (level)
    {}

    semantics::NamePtr
    CommonSemanticsHandler::CommonScope::AddVariable (TypePtr type, const uc::String& identifier,
                                                      semantics::ExpressionPtr initialValue, bool constant)
    {
      if (!CheckIdentifierUnique (identifier))
      {
        // TODO: Error handling
        return semantics::NamePtr();
      }
      semantics::NamePtr newName (new CommonName (identifier, type, initialValue, constant));
      identifiers[identifier] = newName;
      return newName;
    }
      
    semantics::NamePtr
    CommonSemanticsHandler::CommonScope::AddTypeAlias (TypePtr aliasedType, const uc::String& identifier)
    {
      if (!CheckIdentifierUnique (identifier))
      {
        // TODO: Error handling
        return semantics::NamePtr();
      }
      semantics::NamePtr newName (new CommonName (identifier, semantics::Name::TypeAlias, aliasedType));
      identifiers[identifier] = newName;
      return newName;
    }
      
    semantics::FunctionPtr
    CommonSemanticsHandler::CommonScope::AddFunction (TypePtr returnType,
                                                      const uc::String& identifier,
                                                      const FunctionFormalParameters& params)
    {
      if (level >= Function)
      {
        // TODO: Error handling
        return semantics::FunctionPtr();
      }
      if (!CheckIdentifierUnique (identifier))
      {
        // TODO: Error handling
        return semantics::FunctionPtr();
      }
      semantics::NamePtr newName (new CommonName (identifier, semantics::Name::Function, returnType));
      identifiers[identifier] = newName;
      ScopePtr funcScope;
      funcScope = handler->CreateScope (shared_from_this(), Function);
      BlockPtr newBlock (handler->CreateBlock (funcScope));
      funcScope = ScopePtr();
      semantics::FunctionPtr newFunction (new CommonFunction (newBlock));
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
      return Error::IdentifierUndeclared;
    }
    
    CommonSemanticsHandler::ScopePtr CommonSemanticsHandler::CreateScope (ScopePtr parentScope,
                                                                          ScopeLevel scopeLevel)
    {
      return ScopePtr (new CommonScope (this,
        boost::static_pointer_cast<CommonScope> (parentScope),
        scopeLevel));
    }
    
  } // namespace parser
} // namespace s1
